namespace ivy {
	namespace utility {

		template<typename T>
		set<T>::set() noexcept :
			m_size(0) {
		}

		template<typename T>
		set<T>::set(set&& _source) noexcept :
			m_size(_source.m_size),
			m_tree(std::move(_source.m_tree)) {
			_source.m_size = 0;
		}

		template<typename T>
		set<T>::set(const set& _other) noexcept :
			m_size(_other.m_size),
			m_tree(_other.m_tree) {
		}

		template<typename T>
		set<T>::set(init_list<value_type> _init) noexcept :
			m_size(0) {
			for (auto it = _init.begin(); it != _init.end(); ++it) {
				emplace(*it);
			}
		}

		template<typename T>
		set<T>::~set() noexcept {
		}

		template<typename T>
		set<T>& set<T>::operator=(set&& _source) noexcept {
			if (this != &_source) {
				clear();

				m_size = _source.m_size;
				m_tree = std::move(_source.m_tree);

				_source.m_size = 0;
			}

			return *this;
		}

		template<typename T>
		set<T>& set<T>::operator=(const set& _other) noexcept {
			if (this != &_other) {
				clear();

				m_size = _other.m_size;
				m_tree = _other.m_tree;
			}

			return *this;
		}

		template<typename T>
		set<T>& set<T>::operator=(init_list<value_type> _init) noexcept {
			clear();

			for (auto it = _init.begin(); it != _init.end(); ++it) {
				emplace(*it);
			}

			return *this;
		}

		template<typename T>
		u32 set<T>::size() const noexcept {
			return m_size;
		}

		template<typename T>
		b8 set<T>::empty() const noexcept {
			return m_size == 0;
		}

		template<typename T>
		typename set<T>::iterator set<T>::begin() noexcept {
			return m_tree.begin();
		}

		template<typename T>
		typename set<T>::const_iterator set<T>::begin() const noexcept {
			return m_tree.begin();
		}

		template<typename T>
		typename set<T>::iterator set<T>::end() noexcept {
			return m_tree.end();
		}

		template<typename T>
		typename set<T>::const_iterator set<T>::end() const noexcept {
			return m_tree.end();
		}

		template<typename T>
		b8 set<T>::clear() noexcept {
			m_tree.clear();
			m_size = 0;
			return true;
		}

		template<typename T>
		b8 set<T>::contains(const value_type& _value) const noexcept {
			return m_tree.contains(_value);
		}

		template<typename T>
		template<typename... Args>
		const T& set<T>::emplace(Args&&... _args) noexcept {
			value_type value(std::forward<Args>(_args)...);
			if (!contains(value)) {
				++m_size;
			}
			return m_tree.emplace(value);
		}

		template<typename T>
		const T& set<T>::insert(value_type&& _value) noexcept {
			return emplace(std::move(_value));
		}

		template<typename T>
		const T& set<T>::insert(const value_type& _value) noexcept {
			return emplace(_value);
		}

		template<typename T>
		b8 set<T>::remove(const value_type& _value) noexcept {
			if (!contains(_value)) {
				return false;
			}
			m_tree.remove(_value);
			--m_size;
			return true;
		}

	} // namespace utility
} // namespace ivy
