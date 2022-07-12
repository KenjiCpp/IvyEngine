namespace ivy {
	namespace utility {

		template<typename UintT>
		uint_id_generator<UintT>::uint_id_generator() noexcept :
			m_id(0) {
		}

		template<typename UintT>
		uint_id_generator<UintT>::uint_id_generator(uint_id_generator&& _source) noexcept :
			m_id(_source.m_id) {
			_source.m_id = 0;
		}

		template<typename UintT>
		uint_id_generator<UintT>::~uint_id_generator() noexcept {
		}

		template<typename UintT>
		uint_id_generator<UintT>& uint_id_generator<UintT>::operator=(uint_id_generator&& _source) noexcept {
			if (this != &_source) {
				m_id = _source.m_id;
				_source.m_id = 0;
			}

			return *this;
		}

		template<typename UintT>
		UintT uint_id_generator<UintT>::get() noexcept {
			return ++m_id;
		}

	} // namespace utility
} // namespace ivy