namespace ivy {
	namespace pattern {

		template<typename BaseT, typename IdGenT>
		IdGenT unique_id<BaseT, IdGenT>::s_id_generator;

		template<typename BaseT, typename IdGenT>
		unique_id<BaseT, IdGenT>::unique_id() noexcept :
			m_id(s_id_generator.get()) {
		}

		template<typename BaseT, typename IdGenT>
		unique_id<BaseT, IdGenT>::unique_id(unique_id&& _source) noexcept :
			m_id(_source.m_id) {
			_source.m_id = 0;
		}

		template<typename BaseT, typename IdGenT>
		unique_id<BaseT, IdGenT>::~unique_id() noexcept {
		}

		template<typename BaseT, typename IdGenT>
		unique_id<BaseT, IdGenT>& unique_id<BaseT, IdGenT>::operator=(unique_id&& _source) noexcept {
			if (this != &_source) {
				m_id = _source.m_id;
				_source.m_id = 0;
			}

			return *this;
		}

		template<typename BaseT, typename IdGenT>
		typename unique_id<BaseT, IdGenT>::id_type unique_id<BaseT, IdGenT>::id() const noexcept {
			return m_id;
		}

	} // namespace pattern
} // namespace ivy
