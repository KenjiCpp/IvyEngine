#include "platform/filesystem.h"
#include "platform/memory.h"

namespace ivy {
	namespace platform {

		file::file(const path_type& _path, access_mode _mode) noexcept :
			m_handle(nullptr),
			m_writable(false),
			m_unsaved(false) {
			switch (_mode)
			{
			case access_mode::Read: {
				m_handle = fopen(_path.cstr(), "rb");
				if (m_handle) {
					fseek(m_handle, 0, SEEK_END);
					u32 file_size = ftell(m_handle);
					rewind(m_handle);

					u8* read_data = (u8*)memory::allocate(file_size);
					fread(read_data, 1, file_size, m_handle);

					m_bytes.reserve(file_size);
					m_bytes.append(read_data, file_size);

					memory::deallocate(read_data);
				}
				break;
			}
			case access_mode::Write: {
				m_handle = fopen(_path.cstr(), "wb");
				if (m_handle) {
					m_writable = true;
				}
				break;
			}
			default:
				break;
			}
		}

		file::file(file&& _source) noexcept :
			m_handle(_source.m_handle),
			m_bytes(std::move(_source.m_bytes)),
			m_writable(_source.m_writable),
			m_unsaved(_source.m_unsaved) {
			_source.m_handle   = nullptr;
			_source.m_writable = false;
			_source.m_unsaved  = false;
		}

		file::~file() noexcept {
			save();
			if (m_handle) {
				fclose(m_handle);
			}
		}

		file& file::operator=(file&& _source) noexcept {
			if (&_source != this) {
				if (m_handle) {
					fclose(m_handle);
				}
				m_handle   = _source.m_handle;
				m_bytes    = std::move(_source.m_bytes);
				m_writable = _source.m_writable;

				_source.m_handle   = nullptr;
				_source.m_writable = false;
			}

			return *this;
		}

		b8 file::is_valid() const noexcept {
			return m_handle;
		}

		u32 file::size() const noexcept {
			return m_bytes.size();
		}

		const utility::byte_buffer& file::bytes() const noexcept {
			return m_bytes;
		}

		b8 file::write(cptr _data, u32 _size) noexcept {
			if (m_writable) {
				m_unsaved = true;
				return m_bytes.append(_data, _size);
			}
			return false;
		}

		b8 file::save() noexcept {
			if (m_handle) {
				if (m_unsaved) {
					if (fwrite(m_bytes.data(), 1, m_bytes.size(), m_handle) != m_bytes.size()) {
						return false;
					}
					fflush(m_handle);
					m_unsaved = false;
				}

				return true;
			}

			return false;
		}

	} // namespace platform
} // namespace ivy
