namespace ivy {
	namespace utility {

		template<typename EventT>
		b8 event_dispatcher::post(EventT& _ev) noexcept {
			if (m_ctx) {
				return m_ctx->dispatch(_ev);
			}

			return false;
		}

		template<typename EventT>
		b8 event_listener::subscribe(function<void, EventT&> _callback) noexcept {
			if (m_ctx) {
				type_index tid(typeid(EventT));
				m_ctx->m_subscription_functions[tid][this] = [_callback](ptr _arg) {
					_callback(*(EventT*)_arg);
				};
				return true;
			}

			return false;
		}

		template<typename EventT>
		b8 event_listener::unsubscribe() noexcept {
			if (m_ctx) {
				type_index tid(typeid(EventT));
				m_ctx->m_subscription_functions[tid].remove(this);
				return true;
			}

			return false;
		}

		template<typename EventT>
		b8 event_context::dispatch(EventT& _event) noexcept {
			set<type_index> types;
			return dispatch_recursive(_event, typename EventT::bases(), types);
		}

		template<typename EventT>
		b8 event_context::dispatch_recursive(EventT& _event, types_list<> _bases, set<type_index>& _types) noexcept {
			if (_event.handled) {
				return true;
			}
			if (_types.contains(type_index(typeid(EventT)))) {
				return false;
			}
			for (auto& p : m_subscription_functions[type_index(typeid(EventT))]) {
				p.value()(&_event);
			}
			_types.insert(type_index(typeid(EventT)));
			return true;
		}

		template<typename EventT, typename BaseT, typename... BasesT>
		b8 event_context::dispatch_recursive(EventT& _event, types_list<BaseT, BasesT...> _bases, set<type_index>& _types) noexcept {
			dispatch_recursive(_event, types_list<BasesT...>(), _types);
			dispatch_recursive((BaseT&)_event, typename BaseT::bases(), _types);
			return true;
		}

	} // namespace utility
} // namespace ivy
