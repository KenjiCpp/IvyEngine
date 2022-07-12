namespace ivy {
	namespace utility {

		template<typename KeyT, typename T>
		template<typename... Args>
		map<KeyT, T>::pair::pair(const key_type& _key, Args&&... _args) noexcept :
			m_key(_key),
			m_value(value_type(std::forward<Args>(_args)...)) {
		}

		template<typename KeyT, typename T>
		map<KeyT, T>::pair::pair(pair&& _source) noexcept :
			m_key(std::move(_source.m_key)),
			m_value(std::move(_source.m_value)) {
		}

		template<typename KeyT, typename T>
		map<KeyT, T>::pair::pair(const pair& _other) noexcept :
			m_key(_other.m_key),
			m_value(_other.m_value) {
		}

		template<typename KeyT, typename T>
		map<KeyT, T>::pair::pair(null_type _null, const key_type& _key) noexcept :
			m_key(_key),
			m_value(_null) {
		}

		template<typename KeyT, typename T>
		map<KeyT, T>::pair::~pair() noexcept {
		}

		template<typename KeyT, typename T>
		typename map<KeyT, T>::pair& map<KeyT, T>::pair::operator=(pair&& _source) noexcept {
			if (this != &_source) {
				m_key   = std::move(_source.m_key);
				m_value = std::move(_source.m_value);
			}

			return *this;
		}

		template<typename KeyT, typename T>
		typename map<KeyT, T>::pair& map<KeyT, T>::pair::operator=(const pair& _other) noexcept {
			if (this != &_other) {
				m_key   = _other.m_key;
				m_value = _other.m_value;
			}

			return *this;
		}

		template<typename KeyT, typename T>
		b8 map<KeyT, T>::pair::operator==(const pair& _other) const noexcept {
			return m_key == _other.m_key;
		}

		template<typename KeyT, typename T>
		b8 map<KeyT, T>::pair::operator!=(const pair& _other) const noexcept {
			return m_key != _other.m_key;
		}

		template<typename KeyT, typename T>
		b8 map<KeyT, T>::pair::operator>(const pair& _other) const noexcept {
			return m_key > _other.m_key;
		}

		template<typename KeyT, typename T>
		b8 map<KeyT, T>::pair::operator<(const pair& _other) const noexcept {
			return m_key < _other.m_key;
		}

		template<typename KeyT, typename T>
		b8 map<KeyT, T>::pair::operator>=(const pair& _other) const noexcept {
			return m_key >= _other.m_key;
		}

		template<typename KeyT, typename T>
		b8 map<KeyT, T>::pair::operator<=(const pair& _other) const noexcept {
			return m_key <= _other.m_key;
		}

		template<typename KeyT, typename T>
		const KeyT& map<KeyT, T>::pair::key() const noexcept {
			return m_key;
		}

		template<typename KeyT, typename T>
		T& map<KeyT, T>::pair::value() noexcept {
			return m_value.get();
		}

		template<typename KeyT, typename T>
		const T& map<KeyT, T>::pair::value() const noexcept {
			return m_value.get();
		}

		template<typename KeyT, typename T>
		map<KeyT, T>::map() noexcept {
		}

		template<typename KeyT, typename T>
		map<KeyT, T>::map(map&& _source) noexcept :
			m_tree(std::move(_source.m_tree)) {
		}

		template<typename KeyT, typename T>
		map<KeyT, T>::map(const map& _other) noexcept :
			m_tree(_other.m_tree) {
		}

		template<typename KeyT, typename T>
		map<KeyT, T>::~map() noexcept {
		}

		template<typename KeyT, typename T>
		map<KeyT, T>& map<KeyT, T>::operator=(map&& _source) noexcept {
			if (this != &_source) {
				m_tree = std::move(_source.m_tree);
			}

			return *this;
		}

		template<typename KeyT, typename T>
		map<KeyT, T>& map<KeyT, T>::operator=(const map& _other) noexcept {
			if (this != &_other) {
				m_tree = _other.m_tree;
			}

			return *this;
		}

		template<typename KeyT, typename T>
		b8 map<KeyT, T>::empty() const noexcept {
			return m_tree.empty();
		}

		template<typename KeyT, typename T>
		typename map<KeyT, T>::iterator map<KeyT, T>::begin() noexcept {
			return m_tree.begin();
		}

		template<typename KeyT, typename T>
		typename map<KeyT, T>::const_iterator map<KeyT, T>::begin() const noexcept {
			return m_tree.begin();
		}

		template<typename KeyT, typename T>
		typename map<KeyT, T>::iterator map<KeyT, T>::end() noexcept {
			return m_tree.end();
		}

		template<typename KeyT, typename T>
		typename map<KeyT, T>::const_iterator map<KeyT, T>::end() const noexcept {
			return m_tree.end();
		}

		template<typename KeyT, typename T>
		T& map<KeyT, T>::operator[](const key_type& _key) noexcept {
			return m_tree.emplace(_key).value();
		}

		template<typename KeyT, typename T>
		const T& map<KeyT, T>::operator[](const key_type& _key) const noexcept {
			return m_tree.emplace(_key).value();
		}

		template<typename KeyT, typename T>
		b8 map<KeyT, T>::clear() noexcept {
			m_tree.clear();

			return false;
		}

		template<typename KeyT, typename T>
		b8 map<KeyT, T>::contains(const key_type& _key) const noexcept {
			return m_tree.contains(pair(null, _key));
		}

		template<typename KeyT, typename T>
		template<typename... Args>
		const T& map<KeyT, T>::emplace(const key_type& _key, Args&&... _args) noexcept {
			return m_tree.emplace(_key, std::forward<Args>(_args)...).value();
		}

		template<typename KeyT, typename T>
		b8 map<KeyT, T>::remove(const key_type& _key) noexcept {
			return m_tree.remove(pair(null, _key));
		}

	 } // namespace utility
} // namespace ivy
