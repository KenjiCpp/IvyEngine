namespace ivy {
	namespace utility {

		template<typename T>
		nullable<T>::nullable(null_type _null) noexcept :
			m_is_null(true) {
		}

		template<typename T>
		nullable<T>::nullable(nullable&& _source) noexcept :
			m_is_null(_source.m_is_null) {
			_source.m_is_null = true;
			if (!m_is_null) {
				new (m_data) value_type(std::move(_source.m_data));
			}
		}

		template<typename T>
		nullable<T>::nullable(const nullable& _other) noexcept :
			m_is_null(_other.m_is_null) {
			if (!m_is_null) {
				new (m_data) value_type(_other.m_data);
			}
		}

		template<typename T>
		nullable<T>::nullable(value_type&& _source) noexcept :
			m_is_null(false) {
			new (m_data) value_type(std::move(_source));
		}

		template<typename T>
		nullable<T>::nullable(const value_type& _other) noexcept :
			m_is_null(false) {
			new (m_data) value_type(_other);
		}

		template<typename T>
		nullable<T>::~nullable() noexcept {
			if (!m_is_null) {
				((value_type*)m_data)->~value_type();
			}
		}

		template<typename T>
		nullable<T>& nullable<T>::operator=(null_type _null) noexcept {
			if (!m_is_null) {
				((value_type*)m_data)->~value_type();
			}
			m_is_null = true;

			return *this;
		}

		template<typename T>
		nullable<T>& nullable<T>::operator=(nullable&& _source) noexcept {
			if (_source.m_is_null) {
				return *this = null;
			}
			if (!m_is_null) {
				get() = std::move(_source.get());
			}
			else {
				new (m_data) value_type(std::move(_source.get()));
				m_is_null = false;
			}
			_source.m_is_null = true;

			return *this;
		}

		template<typename T>
		nullable<T>& nullable<T>::operator=(const nullable& _other) noexcept {
			if (_other.m_is_null) {
				return *this = null;
			}
			if (!m_is_null) {
				get() = _other.get();
			}
			else {
				new (m_data) value_type(_other.get());
				m_is_null = false;
			}

			return *this;
		}

		template<typename T>
		nullable<T>& nullable<T>::operator=(value_type&& _source) noexcept {
			if (!m_is_null) {
				get() = std::move(_source);
			}
			else {
				new (m_data) value_type(std::move(_source));
				m_is_null = false;
			}

			return *this;
		}

		template<typename T>
		nullable<T>& nullable<T>::operator=(const value_type& _other) noexcept {
			if (!m_is_null) {
				get() = _other;
			}
			else {
				new (m_data) value_type(_other);
				m_is_null = false;
			}

			return *this;
		}

		template<typename T>
		nullable<T>::operator value_type& () noexcept {
			assert(!m_is_null);
			return *(value_type*)m_data;
		}

		template<typename T>
		nullable<T>::operator const value_type& () const noexcept {
			assert(!m_is_null);
			return *(const value_type*)m_data;
		}

		template<typename T>
		b8 nullable<T>::is_null() const noexcept {
			return m_is_null;
		}

		template<typename T>
		T& nullable<T>::get() noexcept {
			assert(!m_is_null);
			return *(value_type*)m_data;
		}

		template<typename T>
		const T& nullable<T>::get() const noexcept {
			assert(!m_is_null);
			return *(const value_type*)m_data;
		}

	} // namespace utility
} // namespace ivy
