namespace ivy {
	namespace utility {

		template<typename T>
		array<T>::array() noexcept :
			m_data(nullptr),
			m_size(0),
			m_offset(0),
			m_capacity(0) {
		}

		template<typename T>
		array<T>::array(array&& _source) noexcept :
			m_data(_source.m_data),
			m_size(_source.m_size),
			m_offset(_source.m_offset),
			m_capacity(_source.m_capacity) {
			_source.m_data     = nullptr;
			_source.m_size     = 0;
			_source.m_offset   = 0;
			_source.m_capacity = 0;
		}

		template<typename T>
		array<T>::array(const array& _other) noexcept :
			m_data(nullptr),
			m_size(0),
			m_offset(0),
			m_capacity(0) {
			value_type* dst = (value_type*)platform::memory::allocate(_other.m_size * sizeof(value_type));
			assert(dst);
			const value_type* src = _other.m_data + _other.m_offset;

			m_data     = dst;
			m_size     = _other.m_size;
			m_capacity = _other.m_size;

			u32 cnt = _other.m_size;
			while (cnt--) {
				new (dst++) value_type(*(src++));
			}
		}

		template<typename T>
		array<T>::array(u32 _reserved_size) noexcept :
			m_data(nullptr),
			m_size(0),
			m_offset(0),
			m_capacity(0) {
			value_type* dst = (value_type*)platform::memory::allocate(_reserved_size * 2 * sizeof(value_type));
			assert(dst);

			m_data     = dst;
			m_capacity = _reserved_size * 2;
			m_offset   = _reserved_size;
		}

		template<typename T>
		array<T>::array(const value_type& _value, u32 _size) noexcept :
			m_data(nullptr),
			m_size(0),
			m_offset(0),
			m_capacity(0) {
			value_type* dst = (value_type*)platform::memory::allocate(_size * sizeof(value_type));
			assert(dst);

			m_data     = dst;
			m_size	   = _size;
			m_capacity = _size;

			while (_size--) {
				new (dst++) value_type(_value);
			}
		}

		template<typename T>
		array<T>::array(init_list<value_type> _init) noexcept :
			m_data(nullptr),
			m_size(0),
			m_offset(0),
			m_capacity(0) {
			value_type* dst = (value_type*)platform::memory::allocate(_init.size() * sizeof(value_type));
			assert(dst);
			const value_type* src = _init.begin();

			m_data     = dst;
			m_size     = _init.size();
			m_capacity = _init.size();

			while (src != _init.end()) {
				new (dst++) value_type(*(src++));
			}
		}

		template<typename T>
		array<T>::~array() noexcept {
			if (m_data) {
				value_type* ptr = m_data + m_offset;
				while (m_size--) {
					(ptr++)->~value_type();
				}
				platform::memory::deallocate(m_data);
			}
		}

		template<typename T>
		array<T>& array<T>::operator=(array&& _source) noexcept {
			if (this != &_source) {
				if (m_data) {
					value_type* ptr = m_data + m_offset;
					while (m_size--) {
						(ptr++)->~value_type();
					}
					platform::memory::deallocate(m_data);
				}

				m_data     = _source.m_data;
				m_size     = _source.m_size;
				m_offset   = _source.m_offset;
				m_capacity = _source.m_capacity;

				_source.m_data     = nullptr;
				_source.m_size     = 0;
				_source.m_offset   = 0;
				_source.m_capacity = 0;
			}

			return *this;
		}

		template<typename T>
		array<T>& array<T>::operator=(const array& _other) noexcept {
			if (this != &_other) {
				value_type* ptr = m_data + m_offset;
				while (m_size--) {
					(ptr++)->~value_type();
				}

				if (_other.m_size <= m_capacity) {
					m_offset = (m_capacity - _other.m_size) / 2;
					m_size   = _other.m_size;

					value_type*       dst = m_data + m_offset;
					const value_type* src = _other.m_data + _other.m_offset;

					u32 size = m_size;
					while (size--) {
						new (dst++) value_type(std::move(*(src++)));
					}
				}
				else {
					platform::memory::deallocate(m_data);
					m_data     = nullptr;
					m_size     = 0;
					m_offset   = 0;
					m_capacity = 0;

					if (_other.m_size) {
						value_type* dst = (value_type*)platform::memory::allocate(_other.m_size * sizeof(value_type));
						assert(dst);
						const value_type* src = _other.m_data + _other.m_offset;

						m_data     = dst;
						m_size     = _other.m_size;
						m_capacity = _other.m_size;

						u32 size = m_size;
						while (size--) {
							new (dst++) value_type(std::move(*(src++)));
						}
					}
				}
			}

			return *this;
		}

		template<typename T>
		array<T>& array<T>::operator=(init_list<value_type> _init) noexcept {
			value_type* ptr = m_data + m_offset;
			while (m_size--) {
				(ptr++)->~value_type();
			}
			
			if (_init.size() <= m_capacity) {
				m_offset = (m_capacity - _init.size()) / 2;
				m_size   = _init.size();

				value_type*       dst = m_data + m_offset;
				const value_type* src = _init.begin();

				u32 size = m_size;
				while (size--) {
					new (dst++) value_type(std::move(*(src++)));
				}
			}
			else {
				platform::memory::deallocate(m_data);
				m_data     = nullptr;
				m_size     = 0;
				m_offset   = 0;
				m_capacity = 0;

				if (_init.size()) {
					value_type* dst = (value_type*)platform::memory::allocate(_init.size() * sizeof(value_type));
					assert(dst);
					const value_type* src = _init.begin();

					m_data     = dst;
					m_size     = _init.size();
					m_capacity = _init.size();

					u32 size = m_size;
					while (size--) {
						new (dst++) value_type(std::move(*(src++)));
					}
				}
			}
			
			return *this;
		}

		template<typename T>
		u32 array<T>::size() const noexcept {
			return m_size;
		}

		template<typename T>
		u32 array<T>::capacity() const noexcept {
			return m_capacity;
		}

		template<typename T>
		b8 array<T>::empty() const noexcept {
			return (m_size == 0);
		}

		template<typename T>
		T* array<T>::data() noexcept {
			return m_data + m_offset;
		}

		template<typename T>
		const T* array<T>::data() const noexcept {
			return m_data + m_offset;
		}

		template<typename T>
		T* array<T>::begin() noexcept {
			return m_data + m_offset;
		}

		template<typename T>
		const T* array<T>::begin() const noexcept {
			return m_data + m_offset;
		}

		template<typename T>
		T* array<T>::end() noexcept {
			return m_data + m_offset + m_size;
		}

		template<typename T>
		const T* array<T>::end() const noexcept {
			return m_data + m_offset + m_size;
		}

		template<typename T>
		T& array<T>::front() noexcept {
			assert(m_size > 0);
			return m_data[m_offset];
		}

		template<typename T>
		const T& array<T>::front() const noexcept {
			assert(m_size > 0);
			return m_data[m_offset];
		}

		template<typename T>
		T& array<T>::back() noexcept {
			assert(m_size > 0);
			return m_data[m_offset + m_size - 1];
		}

		template<typename T>
		const T& array<T>::back() const noexcept {
			assert(m_size > 0);
			return m_data[m_offset + m_size - 1];
		}

		template<typename T>
		T& array<T>::operator[](u32 _idx) noexcept {
			assert(m_size > _idx);
			return m_data[m_offset + _idx];
		}
		
		template<typename T>
		const T& array<T>::operator[](u32 _idx) const noexcept {
			assert(m_size > _idx);
			return m_data[m_offset + _idx];
		}

		template<typename T>
		b8 array<T>::clear() noexcept {
			value_type* ptr = m_data + m_offset;
			while (m_size--) {
				(ptr++)->~value_type();
			}
			m_offset = m_capacity / 2;
			return true;
		}

		template<typename T>
		b8 array<T>::reserve(u32 _reserved_size) noexcept {
			if (_reserved_size * 2 > m_capacity) {
				value_type* dst = (value_type*)platform::memory::allocate(_reserved_size * 2 * sizeof(value_type));
				assert(dst);

				u32 old_offset = m_offset;

				m_capacity = _reserved_size * 2;
				m_offset   = (m_capacity - m_size) / 2;

				value_type* new_data = dst;
				if (m_data) {
					value_type* src = m_data + old_offset;
					u32 size = m_size;
					dst += m_offset;
					while (size--) {
						new (dst++) value_type(std::move(*(src++)));
					}
					platform::memory::deallocate(m_data);
				}
				m_data = new_data;

				return true;
			}

			return false;
		}

		template<typename T>
		b8 array<T>::shrink_to_fit() noexcept {
			if (m_size < m_capacity) {
				if (m_size) {
					value_type* new_data = (value_type*)platform::memory::allocate(m_size * sizeof(value_type));
					assert(new_data);

					value_type* dst = new_data;
					value_type* src = m_data + m_offset;

					u32 size = m_size;
					while (size--) {
						new (dst++) value_type(std::move(*(src++)));
					}

					platform::memory::deallocate(m_data);
					m_data     = new_data;
					m_offset   = 0;
					m_capacity = m_size;
				}
				else {
					platform::memory::deallocate(m_data);
					m_data     = nullptr;
					m_size     = 0;
					m_offset   = 0;
					m_capacity = 0;
				}

				return true;
			}

			return false;
		}

		template<typename T>
		template<typename... Args>
		T& array<T>::emplace_front(Args&&... _args) noexcept {
			if (m_offset <= 0) {
				expand_front();
			}

			new (m_data + --m_offset) value_type(std::forward<Args>(_args)...);
			++m_size;

			return front();
		}

		template<typename T>
		template<typename... Args>
		T& array<T>::emplace_back(Args&&... _args) noexcept {
			if (m_offset + m_size >= m_capacity) {
				expand_back();
			}

			new (m_data + m_offset + m_size++) value_type(std::forward<Args>(_args)...);

			return back();
		}

		template<typename T>
		T& array<T>::push_front(value_type&& _value) noexcept {
			return emplace_front(std::move(_value));
		}

		template<typename T>
		T& array<T>::push_back(value_type&& _value) noexcept {
			return emplace_back(std::move(_value));
		}

		template<typename T>
		T& array<T>::push_front(const value_type& _value) noexcept {
			return emplace_front(_value);
		}

		template<typename T>
		T& array<T>::push_back(const value_type& _value) noexcept {
			return emplace_back(_value);
		}

		template<typename T>
		T array<T>::pop_front() noexcept {
			assert(m_size > 0);
			value_type res = std::move(*(m_data + m_offset++));
			--m_size;

			return res;
		}

		template<typename T>
		T array<T>::pop_back() noexcept {
			assert(m_size > 0);
			value_type res = std::move(*(m_data + m_offset + --m_size));

			return res;
		}

		template<typename T>
		b8 array<T>::expand_front() noexcept {
			u32 new_capacity = m_capacity * EXPAND_FACTOR + 1;
			value_type* new_data = (value_type*)platform::memory::allocate(new_capacity * sizeof(value_type));
			assert(new_data);

			u32 new_offset = m_offset + (new_capacity - m_capacity);

			if (m_data) {
				value_type* src = m_data + m_offset;
				value_type* dst = new_data + new_offset;
				u32 size = m_size;

				while (size--) {
					new (dst++) value_type(std::move(*(src++)));
				}

				platform::memory::deallocate(m_data);
			}

			m_data     = new_data;
			m_offset   = new_offset;
			m_capacity = new_capacity;
			
			return true;
		}

		template<typename T>
		b8 array<T>::expand_back() noexcept {
			u32 new_capacity = m_capacity * EXPAND_FACTOR + 1;
			value_type* new_data = (value_type*)platform::memory::allocate(new_capacity * sizeof(value_type));
			assert(new_data);

			if (m_data) {
				value_type* src = m_data + m_offset;
				value_type* dst = new_data + m_offset;
				u32 size = m_size;

				while (size--) {
					new (dst++) value_type(std::move(*(src++)));
				}

				platform::memory::deallocate(m_data);
			}

			m_data     = new_data;
			m_capacity = new_capacity;

			return true;
		}

	} // namespace utility
} // namespace ivy
