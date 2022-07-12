#pragma once

#include "../utility/string.h"
#include "../utility/byte_buffer.h"

namespace ivy {
	namespace platform {

		class IVY_API file {
		public:
			typedef utility::string8     path_type;
			typedef utility::byte_buffer bytes_type;

			enum class access_mode {
				Read, Write
			};

		public:
			file(const path_type& _path, access_mode _mode = access_mode::Write) noexcept;

			file(file&& _source) noexcept;
			file(const file& _other) noexcept = delete;

			~file() noexcept;

			file& operator=(file&& _source) noexcept;
			file& operator=(const file& _other) noexcept = delete;

			b8 is_valid() const noexcept;

			u32 size() const noexcept;

			const bytes_type& bytes() const noexcept;

			b8 write(cptr _data, u32 _size) noexcept;
			b8 save() noexcept;

		private:
			FILE*      m_handle;
			bytes_type m_bytes;
			b8		   m_writable;
			b8		   m_unsaved;
		};

	} // namespace platform
} // namespace ivy