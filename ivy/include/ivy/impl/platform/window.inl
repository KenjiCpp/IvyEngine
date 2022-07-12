namespace ivy {
	namespace platform {

		template<typename EventT>
		b8 window::post_event(EventT& _event) noexcept {
			if (m_dispatcher && m_is_open) {
				return m_dispatcher->post(_event);
			}
			return false;
		}

	} // namespace platform
} // namespace ivy
