#pragma once

#include "../platform/memory.h"

namespace ivy {
	namespace utility {

		template<typename T>
		class array {
		public:
			typedef T value_type;
			
			typedef value_type*       iterator;
			typedef const value_type* const_iterator;

		private:
			static constexpr u32 EXPAND_FACTOR = 2;

		public:
			array() noexcept;

			array(array&& _source) noexcept;
			array(const array& _other) noexcept;

			explicit array(u32 _reserved_size) noexcept;

			array(const value_type& _value, u32 _size) noexcept;
			array(init_list<value_type> _init) noexcept;

			~array() noexcept;

			array& operator=(array&& _source) noexcept;
			array& operator=(const array& _other) noexcept;

			array& operator=(init_list<value_type> _init) noexcept;

			u32 size() const noexcept;
			u32 capacity() const noexcept;
			b8 empty() const noexcept;

			value_type* data() noexcept;
			const value_type* data() const noexcept;

			iterator begin() noexcept;
			const_iterator begin() const noexcept;

			iterator end() noexcept;
			const_iterator end() const noexcept;

			value_type& front() noexcept;
			const value_type& front() const noexcept;

			value_type& back() noexcept;
			const value_type& back() const noexcept;

			value_type& operator[](u32 _idx) noexcept;
			const value_type& operator[](u32 _idx) const noexcept;

			b8 clear() noexcept;
			b8 reserve(u32 _reserved_size) noexcept;
			b8 shrink_to_fit() noexcept;

			template<typename... Args>
			value_type& emplace_front(Args&&... _args) noexcept;
			template<typename... Args>
			value_type& emplace_back(Args&&... _args) noexcept;

			value_type& push_front(value_type&& _value) noexcept;
			value_type& push_back(value_type&& _value) noexcept;

			value_type& push_front(const value_type& _value) noexcept;
			value_type& push_back(const value_type& _value) noexcept;

			value_type pop_front() noexcept;
			value_type pop_back() noexcept;

		private:
			b8 expand_front() noexcept;
			b8 expand_back() noexcept;

		private:
			value_type* m_data;
			u32         m_size;
			u32         m_offset;
			u32         m_capacity;
		};

	} // namespace utility
} // namespace ivy

#include "../impl/utility/array.inl"
