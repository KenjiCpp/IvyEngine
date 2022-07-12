#pragma once

#include "../platform/memory.h"

namespace ivy {
	namespace utility {

		template<typename T>
		class deque {
		public:
			typedef T value_type;

			class iterator {
			private:
				friend class deque;
				friend class const_iterator;

			public:
				iterator() noexcept;

				iterator(iterator&& _source) noexcept = default;
				iterator(const iterator & _other) noexcept = default;

				~iterator() noexcept = default;

				iterator& operator=(iterator && _source) noexcept = default;
				iterator& operator=(const iterator & _other) noexcept = default;

				b8 operator==(const iterator & _other) const noexcept;
				b8 operator!=(const iterator & _other) const noexcept;

				iterator& operator++() noexcept;
				iterator operator++(int) noexcept;

				value_type& operator*() const noexcept;
				value_type* operator->() const noexcept;

			private:
				iterator(deque& _deque, u32 _idx) noexcept;

			private:
				deque* m_deque;
				u32	   m_idx;
			};

			class const_iterator {
			private:
				friend class deque;

			public:
				const_iterator() noexcept;

				const_iterator(const_iterator&& _source) noexcept = default;
				const_iterator(const const_iterator& _other) noexcept = default;
				const_iterator(iterator&& _source) noexcept;
				const_iterator(const iterator & _other) noexcept;

				~const_iterator() noexcept = default;

				const_iterator& operator=(const_iterator&& _source) noexcept = default;
				const_iterator& operator=(const const_iterator& _other) noexcept = default;

				b8 operator==(const const_iterator& _other) const noexcept;
				b8 operator!=(const const_iterator& _other) const noexcept;

				const_iterator& operator++() noexcept;
				const_iterator operator++(int) noexcept;

				const value_type& operator*() const noexcept;
				const value_type* operator->() const noexcept;

			private:
				const_iterator(const deque& _deque, u32 _idx) noexcept;

			private:
				const deque* m_deque;
				u32			 m_idx;
			};

		private:
			static constexpr u32 EXPAND_FACTOR = 2;
			
		public:
			deque() noexcept;

			deque(deque<value_type>&& _source) noexcept;
			deque(const deque<value_type>& _other) noexcept;

			deque(init_list<value_type> _init) noexcept;

			~deque() noexcept;

			deque<value_type>& operator=(deque<value_type>&& _source) noexcept;
			deque<value_type>& operator=(const deque<value_type>& _other) noexcept;

			deque<value_type>& operator=(init_list<value_type> _init) noexcept;

			u32 size() const noexcept;
			u32 capacity() const noexcept;
			b8 empty() const noexcept;

			iterator begin() noexcept;
			iterator end() noexcept;

			const_iterator begin() const noexcept;
			const_iterator end() const noexcept;

			value_type& front() noexcept;
			const value_type& front() const noexcept;
			
			value_type& back() noexcept;
			const value_type& back() const noexcept;

			value_type& operator[](u32 _idx) noexcept;
			const value_type& operator[](u32 _idx) const noexcept;

			// TODO: Reserve and shrink to fit
			b8 clear() noexcept;

			template<typename... Args>
			value_type& emplace_front(Args&&... _args) noexcept;
			template<typename... Args>
			value_type& emplace_back(Args&&... _args) noexcept;

			value_type& push_front(const value_type& _value) noexcept;
			value_type& push_front(value_type&& _value) noexcept;
			value_type& push_back(const value_type& _value) noexcept;
			value_type& push_back(value_type&& _value) noexcept;

			b8 pop_front() noexcept;
			b8 pop_back() noexcept;

		private:
			b8 expand() noexcept;

		private:
			value_type* m_data;			// Raw pointer to data buffer
			u32			m_capacity;		// Maximum number of elements the current buffer can hold 
			i32			m_front_idx;	// Index of the first element
			i32			m_back_idx;		// Index of the last element
			i32			m_left_idx;		// Index of the left-most element
			i32			m_right_idx;	// Index of the right-most element
		};

	} // namespace utility
} // namespace ivy

#include "../impl/utility/deque.inl"
