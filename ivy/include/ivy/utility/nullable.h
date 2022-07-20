#pragma once

namespace ivy {
	namespace utility {
		
		struct null_type { };
		constexpr null_type null;

		template<typename T>
		class nullable {
		public:
			typedef T value_type;

		public:
			nullable(null_type _null) noexcept;
			nullable(nullable&& _source) noexcept;
			nullable(const nullable& _other) noexcept;
			nullable(value_type&& _source) noexcept;
			nullable(const value_type& _other) noexcept;

			~nullable() noexcept;

			nullable& operator=(null_type _null) noexcept;
			nullable& operator=(nullable&& _source) noexcept;
			nullable& operator=(const nullable& _other) noexcept;
			nullable& operator=(value_type&& _source) noexcept;
			nullable& operator=(const value_type& _other) noexcept;

			operator value_type&() noexcept;
			operator const value_type&() const noexcept;

			b8 is_null() const noexcept;

			value_type& get() noexcept;
			const value_type& get() const noexcept;

		private:
			u8 m_data[sizeof(value_type)];
			b8 m_is_null;
		};

	} // namespace utility

	using utility::null;

	using utility::null_type;
	using utility::nullable;

} // namespace ivy

#include "../impl/utility/nullable.inl"
