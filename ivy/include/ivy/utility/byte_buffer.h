#pragma once

#include "platform/memory.h"
#include "formatter.h"

namespace ivy {
	namespace utility {

		class IVY_API byte_buffer {
		private:
			static constexpr u32 EXPAND_SIZE = 1024;

		public:
			byte_buffer() noexcept;
			explicit byte_buffer(u32 _size) noexcept;

			byte_buffer(byte_buffer&& _source) noexcept;
			byte_buffer(const byte_buffer& _other) noexcept;

			~byte_buffer() noexcept;

			byte_buffer& operator=(byte_buffer&& _source) noexcept;
			byte_buffer& operator=(const byte_buffer& _other) noexcept;

			u32 size() const noexcept;
		
			u8* data() noexcept;
			const u8* data() const noexcept;

			b8 append(cptr _data, u32 _size) noexcept;
			b8 clear() noexcept;
			b8 reserve(u32 _size) noexcept;

		private:
			b8 expand_to_fit(u32 _new_size) noexcept;

		private:
			u8* m_data;
			u32 m_size;
			u32 m_capacity;
		};

		struct byte_buffer_formatter : unsigned_integer_formatter {

			IVY_API b8 parse(const format_context& _ctx) noexcept;

			IVY_API string_type format(const byte_buffer& _value) const noexcept;
		};

		template<> struct formatter<byte_buffer> : byte_buffer_formatter { };

	} // namespace utility

	using utility::byte_buffer;

} // namespace ivy
