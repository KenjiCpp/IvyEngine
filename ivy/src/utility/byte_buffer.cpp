#include "utility/byte_buffer.h"

#include "platform/memory.h"

namespace ivy {
	namespace utility {

		byte_buffer::byte_buffer() noexcept :
			m_data(nullptr),
			m_size(0),
			m_capacity(0) {
		}

		byte_buffer::byte_buffer(u32 _size) noexcept :
			m_data(nullptr),
			m_size(_size),
			m_capacity(0) {
			assert(expand_to_fit(m_size));
		}

		byte_buffer::byte_buffer(byte_buffer&& _source) noexcept :
			m_data(_source.m_data),
			m_size(_source.m_size),
			m_capacity(_source.m_capacity) {
			_source.m_data     = nullptr;
			_source.m_size     = 0;
			_source.m_capacity = 0;
		}

		byte_buffer::byte_buffer(const byte_buffer& _other) noexcept :
			m_data(nullptr),
			m_size(_other.m_size),
			m_capacity(0) {
			assert(expand_to_fit(m_size));
			platform::memory::copy(_other.m_data, m_data, m_size);
		}

		byte_buffer::~byte_buffer() noexcept {
			platform::memory::deallocate(m_data);
		}

		byte_buffer& byte_buffer::operator=(byte_buffer&& _source) noexcept {
			if (&_source != this) {
				platform::memory::deallocate(m_data);

				m_data     = _source.m_data;
				m_size     = _source.m_size;
				m_capacity = _source.m_capacity;

				_source.m_data     = nullptr;
				_source.m_size     = 0;
				_source.m_capacity = 0;
			}
	
			return *this;
		}

		byte_buffer& byte_buffer::operator=(const byte_buffer& _other) noexcept {
			if (&_other != this) {
				platform::memory::deallocate(m_data);
				m_data     = nullptr;
				m_capacity = 0;
				
				m_size = _other.m_size;

				assert(expand_to_fit(m_size));
				platform::memory::copy(_other.m_data, m_data, m_size);
			}

			return *this;
		}

		u32 byte_buffer::size() const noexcept {
			return m_size;
		}

		u8* byte_buffer::data() noexcept {
			return m_data;
		}

		const u8* byte_buffer::data() const noexcept {
			return m_data;
		}

		b8 byte_buffer::append(cptr _data, u32 _size) noexcept {
			u32 new_size = m_size + _size;
			if (!expand_to_fit(new_size)) {
				return false;
			}

			platform::memory::copy(_data, m_data + m_size, _size);
			m_size += _size;

			return true;
		}

		b8 byte_buffer::clear() noexcept {
			m_size = 0;
			return true;
		}

		b8 byte_buffer::reserve(u32 _size) noexcept {
			if (_size > m_capacity) {
				ptr new_data = platform::memory::allocate(_size);

				if (!new_data) {
					return false;
				}

				platform::memory::copy(m_data, new_data, m_size);
				platform::memory::deallocate(m_data);
				m_data = (u8*)new_data;
				m_capacity = _size;
			}

			return true;
		}

		b8 byte_buffer::expand_to_fit(u32 _new_size) noexcept {
			if (_new_size > m_capacity) {
				u32 new_capacity = ((_new_size - 1) / EXPAND_SIZE + 1) * EXPAND_SIZE;
				ptr new_data = platform::memory::allocate(new_capacity);

				if (!new_data) {
					return false;
				}

				platform::memory::copy(m_data, new_data, m_size);
				platform::memory::deallocate(m_data);
				m_data     = (u8*)new_data;
				m_capacity = new_capacity;
			}
			
			return true;
		}

		b8 byte_buffer_formatter::parse(const format_context& _ctx) noexcept {
			/*
			*	Format: [cap]
			*		cap : { '^' }^1
			*/

			auto it = _ctx.begin();

			if (it != _ctx.end()) {
				if (*it == '^') {
					capitalized = true;
					++it;
				}
			}

			base  = 16;
			zero  = true;
			width = 8;

			return it == _ctx.end();
		}

		string_type byte_buffer_formatter::format(const byte_buffer& _value) const noexcept {
			string_type res = capitalized 
				? "\n            0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F" 
				: "\n            0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f";

			u32 size = _value.size();
			u32 line_offset = 0;
			const u8* line_data = _value.data();

			while (size >= 16) {
				res.append("\n ");
				res.append(unsigned_integer_formatter::format(line_offset));
				res.append("  ");

				for (i32 i = 0; i < 16; ++i) {
					res.append(digit(line_data[i] / 16));
					res.append(digit(line_data[i] % 16));
					res.append(' ');
				}
				
				res.append(' ');
				for (i32 i = 0; i < 16; ++i) {
					if (iscntrl(line_data[i])) {
						res.append('.');
					}
					else {
						res.append(line_data[i]);
					}
				}

				size -= 16;
				line_data += 16;
				line_offset += 16;
			}

			if (size) {
				res.append("\n ");
				res.append(unsigned_integer_formatter::format(line_offset));
				res.append("  ");

				for (i32 i = 0; i < 16; ++i) {
					if (i < size) {
						res.append(digit(line_data[i] / 16));
						res.append(digit(line_data[i] % 16));
						res.append(' ');
					}
					else {
						res.append("   ");
					}
				}

				res.append(' ');
				for (i32 i = 0; i < size; ++i) {
					if (iscntrl(line_data[i])) {
						res.append('.');
					}
					else {
						res.append(line_data[i]);
					}
				}
			}

			res.append('\n');
			return res;
		}

	} // namespace utility
} // namespace ivy
