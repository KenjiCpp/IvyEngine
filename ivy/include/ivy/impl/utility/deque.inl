namespace ivy {
	namespace utility {

		template<typename T>
		deque<T>::deque() noexcept : 
			m_data(nullptr), 
			m_capacity(0),
			m_front_idx(0),
			m_back_idx(-1),
			m_left_idx(0),
			m_right_idx(-1) {
		}

		template<typename T>
		deque<T>::deque(deque<value_type>&& _source) noexcept :
			m_data(_source.m_data),
			m_capacity(_source.m_capacity),
			m_front_idx(_source.m_front_idx),
			m_back_idx(_source.m_back_idx),
			m_left_idx(_source.m_left_idx),
			m_right_idx(_source.m_right_idx) {
			_source.m_data      = nullptr;
			_source.m_capacity  = 0;
			_source.m_front_idx = 0;
			_source.m_back_idx  = -1;
			_source.m_left_idx  = 0;
			_source.m_right_idx = -1;
		}

		template<typename T>
		deque<T>::deque(const deque<value_type>& _other) noexcept :
			m_data(nullptr),
			m_capacity(_other.m_capacity),
			m_front_idx(_other.m_front_idx),
			m_back_idx(_other.m_back_idx),
			m_left_idx(_other.m_left_idx),
			m_right_idx(_other.m_right_idx) {
			ptr new_data = platform::memory::allocate(m_capacity * sizeof(value_type));
			if (!new_data) {
				m_capacity  = 0;
				m_front_idx = 0;
				m_back_idx  = -1;
				m_left_idx  = 0;
				m_right_idx = -1;
				return;
			}

			m_data = (value_type*)new_data;

			value_type* data_front = m_data + m_front_idx;
			const value_type* src_front = _other.m_data + m_front_idx;
			while (data_front != m_data + m_capacity) {
				new (data_front++) value_type(*(src_front++));
			}

			value_type* data_back = m_data;
			const value_type* src_back = _other.m_data;
			while (data_back != m_data + m_back_idx + 1) {
				new (data_back++) value_type(*(src_back++));
			}
		}

		template<typename T>
		deque<T>::deque(init_list<value_type> _init) noexcept :
			m_data(nullptr),
			m_capacity(_init.size()),
			m_front_idx(0),
			m_back_idx(-1),
			m_left_idx(0),
			m_right_idx(_init.size() - 1) {
			ptr new_data = platform::memory::allocate(m_capacity * sizeof(value_type));
			if (!new_data) {
				m_capacity  = 0;
				m_front_idx = 0;
				m_back_idx  = -1;
				m_left_idx  = 0;
				m_right_idx = -1;
				return;
			}

			m_data = (value_type*)new_data;

			value_type* data = m_data;
			const value_type* src = _init.begin();
			while (data != m_data + m_capacity) {
				new (data++) value_type(*(src++));
			}
		}

		template<typename T>
		deque<T>::~deque() noexcept {
			value_type* data_front = m_data + m_front_idx;
			while (data_front != m_data + m_capacity) {
				data_front++->~value_type();
			}

			value_type* data_back = m_data;
			while (data_back != m_data + m_back_idx + 1) {
				data_back++->~value_type();
			}

			platform::memory::deallocate(m_data);
		}

		template<typename T>
		deque<T>& deque<T>::operator=(deque<value_type>&& _source) noexcept {
			if (this != &_source) {
				value_type* data_front = m_data + m_front_idx;
				while (data_front != m_data + m_capacity) {
					data_front++->~value_type();
				}

				value_type* data_back = m_data;
				while (data_back != m_data + m_back_idx + 1) {
					data_back++->~value_type();
				}

				platform::memory::deallocate(m_data);

				m_data      = _source.m_data;
				m_capacity  = _source.m_capacity;
				m_front_idx = _source.m_front_idx;
				m_back_idx  = _source.m_back_idx;
				m_left_idx  = _source.m_left_idx;
				m_right_idx = _source.m_right_idx;

				_source.m_data      = nullptr;
				_source.m_capacity  = 0;
				_source.m_front_idx = 0;
				_source.m_back_idx  = -1;
				_source.m_left_idx  = 0;
				_source.m_right_idx = -1;
			}

			return *this;
		}

		template<typename T>
		deque<T>& deque<T>::operator=(const deque<value_type>& _other) noexcept {
			if (this != &_other) {
				value_type* data_front = m_data + m_front_idx;
				while (data_front != m_data + m_capacity) {
					data_front++->~value_type();
				}

				value_type* data_back = m_data;
				while (data_back != m_data + m_back_idx + 1) {
					data_back++->~value_type();
				}

				platform::memory::deallocate(m_data);

				m_data      = nullptr;
				m_capacity  = _other.m_capacity;
				m_front_idx = _other.m_front_idx;
				m_back_idx  = _other.m_back_idx;
				m_left_idx  = _other.m_left_idx;
				m_right_idx = _other.m_right_idx;

				ptr new_data = platform::memory::allocate(m_capacity * sizeof(value_type));
				if (!new_data) {
					m_capacity  = 0;
					m_front_idx = 0;
					m_back_idx  = -1;
					m_left_idx  = 0;
					m_right_idx = -1;
					return *this;
				}

				m_data = (value_type*)new_data;

				data_front = m_data + m_front_idx;
				const value_type* src_front = _other.m_data + m_front_idx;
				while (data_front != m_data + m_capacity) {
					new (data_front++) value_type(*(src_front++));
				}

				data_back = m_data;
				const value_type* src_back = _other.m_data;
				while (data_back != m_data + m_back_idx + 1) {
					new (data_back++) value_type(*(src_back++));
				}
			}

			return *this;
		}

		template<typename T>
		deque<T>& deque<T>::operator=(init_list<value_type> _init) noexcept {
			value_type* data_front = m_data + m_front_idx;
			while (data_front != m_data + m_capacity) {
				data_front++->~value_type();
			}

			value_type* data_back = m_data;
			while (data_back != m_data + m_back_idx + 1) {
				data_back++->~value_type();
			}

			platform::memory::deallocate(m_data);

			m_data      = nullptr;
			m_capacity  = _init.size();
			m_front_idx = 0;
			m_back_idx  = -1;
			m_left_idx  = 0;
			m_right_idx = _init.size();

			ptr new_data = platform::memory::allocate(m_capacity * sizeof(value_type));
			if (!new_data) {
				m_capacity  = 0;
				m_front_idx = 0;
				m_back_idx  = -1;
				m_left_idx  = 0;
				m_right_idx = -1;
				return *this;
			}

			m_data = (value_type*)new_data;

			value_type* data = m_data;
			const value_type* src = _init.begin();
			while (data != m_data + m_capacity) {
				new (data++) value_type(*(src++));
			}

			return *this;
		}
		
		template<typename T>
		u32 deque<T>::size() const noexcept {
			return (m_right_idx - m_front_idx + m_back_idx - m_left_idx + 2);
		}

		template<typename T>
		u32 deque<T>::capacity() const noexcept {
			return m_capacity;
		}

		template<typename T>
		b8 deque<T>::empty() const noexcept {
			return (m_front_idx > m_right_idx) && (m_back_idx < m_left_idx);
		}

		template<typename T>
		typename deque<T>::iterator deque<T>::begin() noexcept {
			return iterator(*this, 0);
		}

		template<typename T>
		typename deque<T>::iterator deque<T>::end() noexcept {
			return iterator(*this, size());
		}

		template<typename T>
		typename deque<T>::const_iterator deque<T>::begin() const noexcept {
			return const_iterator(*this, 0);
		}

		template<typename T>
		typename deque<T>::const_iterator deque<T>::end() const noexcept {
			return const_iterator(*this, size());
		}

		template<typename T>
		T& deque<T>::front() noexcept {
			assert(size() > 0);
			if (m_front_idx <= m_right_idx) {
				return m_data[m_front_idx];
			}
			return m_data[m_left_idx];
		}

		template<typename T>
		const T& deque<T>::front() const noexcept {
			assert(size() > 0);
			if (m_front_idx <= m_right_idx) {
				return m_data[m_front_idx];
			}
			return m_data[m_left_idx];
		}

		template<typename T>
		T& deque<T>::back() noexcept {
			assert(size() > 0);
			if (m_back_idx >= m_left_idx) {
				return m_data[m_back_idx];
			}
			return m_data[m_right_idx];
		}

		template<typename T>
		const T& deque<T>::back() const noexcept {
			assert(size() > 0);
			if (m_back_idx >= m_left_idx) {
				return m_data[m_back_idx];
			}
			return m_data[m_right_idx];
		}

		template<typename T>
		T& deque<T>::operator[](u32 _idx) noexcept {
			assert(size() > _idx);
			return m_data[(m_front_idx + _idx - m_left_idx) % (m_right_idx - m_left_idx + 1) + m_left_idx];
		}

		template<typename T>
		const T& deque<T>::operator[](u32 _idx) const noexcept {
			assert(size() > _idx);
			return m_data[(m_front_idx + _idx - m_left_idx) % (m_right_idx - m_left_idx + 1) + m_left_idx];
		}

		template<typename T>
		b8 deque<T>::clear() noexcept {
			value_type* data_front = m_data + m_front_idx;
			while (data_front != m_data + m_capacity) {
				data_front++->~value_type();
			}

			value_type* data_back = m_data;
			while (data_back != m_data + m_back_idx + 1) {
				data_back++->~value_type();
			}

			m_front_idx = m_capacity;
			m_back_idx  = -1;
			m_left_idx  = 0;
			m_right_idx = m_capacity - 1;

			return true;
		}

		template<typename T>
		b8 deque<T>::expand() noexcept {
			// Allocate new data buffer, return false if failed
			u32 new_capacity = m_capacity * EXPAND_FACTOR + 1;
			ptr new_data = platform::memory::allocate(new_capacity * sizeof(value_type));
			if (!new_data)
				return false;

			// Move data from the old buffer to the new one
			i32 new_front_idx = new_capacity - m_capacity + m_front_idx;

			value_type* new_data_front = (value_type*)new_data + new_front_idx;
			value_type* old_data_front = m_data + m_front_idx;
			while (old_data_front != m_data + m_capacity) {
				new (new_data_front++) value_type(std::move(*(old_data_front++)));
			}

			value_type* new_data_back = (value_type*)new_data;
			value_type* old_data_back = m_data;
			while (old_data_back != m_data + m_back_idx + 1) {
				new (new_data_back++) value_type(std::move(*(old_data_back++)));
			}

			// Update indices and capacity
			m_front_idx = new_front_idx;
			m_capacity = new_capacity;
			m_left_idx = 0;
			m_right_idx = new_capacity - 1;

			// Replace old data buffer
			platform::memory::deallocate(m_data);
			m_data = (value_type*)new_data;

			return true;
		}

		template<typename T>
		template<typename... Args>
		T& deque<T>::emplace_front(Args&&... _args) noexcept {
			// Check if there's room left. If not, expand the buffer
			if (size() >= m_capacity) {
				expand();
			}

			// Construct and return new element
			if (m_left_idx > 0) {
				--m_left_idx;
				new (m_data + m_left_idx) value_type(std::forward<Args>(_args)...);
				return *(m_data + m_left_idx);
			}
			else {
				// Handle wrapping around
				if (m_front_idx == 0) {
					m_front_idx = m_capacity;
					m_back_idx  = m_right_idx;
					m_left_idx  = 0;
					m_right_idx = m_capacity - 1;
				}

				--m_front_idx;
				new (m_data + m_front_idx) value_type(std::forward<Args>(_args)...);
				return *(m_data + m_front_idx);
			}
		}

		template<typename T>
		template<typename... Args>
		T& deque<T>::emplace_back(Args&&... _args) noexcept {
			// Check if there's room left. If not, expand the buffer
			if (size() >= m_capacity) {
				expand();
			}

			// Construct and return new element
			if (m_right_idx < m_capacity - 1) {
				++m_right_idx;
				new (m_data + m_right_idx) value_type(std::forward<Args>(_args)...);
				return *(m_data + m_right_idx);
			}
			else {
				// Handle wrapping around
				if (m_back_idx == m_capacity - 1) {
					m_front_idx = m_left_idx;
					m_back_idx  = -1;
					m_left_idx  = 0;
					m_right_idx = m_capacity - 1;
				}

				++m_back_idx;
				new (m_data + m_back_idx) value_type(std::forward<Args>(_args)...);
				return *(m_data + m_back_idx);
			}
		}

		template<typename T>
		T& deque<T>::push_front(const value_type& _value) noexcept {
			return emplace_front(_value);
		}

		template<typename T>
		T& deque<T>::push_front(value_type&& _value) noexcept {
			return emplace_front(std::move(_value));
		}

		template<typename T>
		T& deque<T>::push_back(const value_type& _value) noexcept {
			return emplace_back(_value);
		}

		template<typename T>
		T& deque<T>::push_back(value_type&& _value) noexcept {
			return emplace_back(std::move(_value));
		}

		template<typename T>
		b8 deque<T>::pop_front() noexcept {
			// If there're elements on the front side
			if (m_front_idx <= m_right_idx) {
				(m_data + m_front_idx)->~value_type();
				++m_front_idx;
				return true;
			}

			// If there's no element on the front side
			if (m_left_idx <= m_back_idx) {
				(m_data + m_left_idx)->~value_type();
				++m_left_idx;
				return true;
			}

			// If there's no element left
			return false;
		}

		template<typename T>
		b8 deque<T>::pop_back() noexcept {
			// If there're elements on the back side
			if (m_back_idx >= m_left_idx) {
				(m_data + m_back_idx)->~value_type();
				--m_back_idx;
				return true;
			}

			// If there's no element on the back side
			if (m_right_idx >= m_front_idx) {
				(m_data + m_right_idx)->~value_type();
				--m_right_idx;
				return true;
			}

			// If there's no element left
			return false;
		}

		template<typename T>
		deque<T>::iterator::iterator() noexcept :
			m_deque(nullptr),
			m_idx(0) {
		}

		template<typename T>
		deque<T>::iterator::iterator(deque& _deque, u32 _idx) noexcept :
			m_deque(&_deque),
			m_idx(_idx) {
		}

		template<typename T>
		b8 deque<T>::iterator::operator==(const iterator& _other) const noexcept {
			return (m_deque == _other.m_deque && m_idx == _other.m_idx);
		}

		template<typename T>
		b8 deque<T>::iterator::operator!=(const iterator& _other) const noexcept {
			return (m_deque != _other.m_deque || m_idx != _other.m_idx);
		}

		template<typename T>
		typename deque<T>::iterator& deque<T>::iterator::operator++() noexcept {
			++m_idx;
			return *this;
		}

		template<typename T>
		typename deque<T>::iterator deque<T>::iterator::operator++(int) noexcept {
			typename deque<T>::iterator res = *this;
			++(*this);
			return res;
		}

		template<typename T>
		typename deque<T>::value_type& deque<T>::iterator::operator*() const noexcept {
			return (*m_deque)[m_idx];
		}

		template<typename T>
		typename deque<T>::value_type* deque<T>::iterator::operator->() const noexcept {
			return &(*m_deque)[m_idx];
		}

		template<typename T>
		deque<T>::const_iterator::const_iterator() noexcept :
			m_deque(nullptr),
			m_idx(0) {
		}

		template<typename T>
		deque<T>::const_iterator::const_iterator(const deque& _deque, u32 _idx) noexcept :
			m_deque(&_deque),
			m_idx(_idx) {
		}

		template<typename T>
		deque<T>::const_iterator::const_iterator(iterator&& _other) noexcept :
			m_deque(_other.m_deque),
			m_idx(_other.m_idx) {
		}

		template<typename T>
		deque<T>::const_iterator::const_iterator(const iterator& _other) noexcept :
			m_deque(_other.m_deque),
			m_idx(_other.m_idx) {
		}

		template<typename T>
		b8 deque<T>::const_iterator::operator==(const const_iterator& _other) const noexcept {
			return (m_deque == _other.m_deque && m_idx == _other.m_idx);
		}

		template<typename T>
		b8 deque<T>::const_iterator::operator!=(const const_iterator& _other) const noexcept {
			return (m_deque != _other.m_deque || m_idx != _other.m_idx);
		}

		template<typename T>
		typename deque<T>::const_iterator& deque<T>::const_iterator::operator++() noexcept {
			++m_idx;
			return *this;
		}

		template<typename T>
		typename deque<T>::const_iterator deque<T>::const_iterator::operator++(int) noexcept {
			typename deque<T>::const_iterator res = *this;
			++(*this);
			return res;
		}

		template<typename T>
		const typename deque<T>::value_type& deque<T>::const_iterator::operator*() const noexcept {
			return (*m_deque)[m_idx];
		}

		template<typename T>
		const typename deque<T>::value_type* deque<T>::const_iterator::operator->() const noexcept {
			return &(*m_deque)[m_idx];
		}

	} // namespace utility
} // namespace ivy
