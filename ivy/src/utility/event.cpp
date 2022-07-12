#include "utility/event.h"

namespace ivy {
	namespace utility {

		event_dispatcher::event_dispatcher(event_context* _ctx) noexcept :
			m_ctx(_ctx) {
		}

		event_dispatcher::~event_dispatcher() noexcept {
			m_ctx = nullptr;
		}

		b8 event_dispatcher::is_valid() const noexcept {
			return m_ctx;
		}

		event_listener::event_listener(event_context* _ctx) noexcept :
			m_ctx(_ctx) {
		}

		event_listener::~event_listener() noexcept {
			m_ctx = nullptr;
		}

		b8 event_listener::is_valid() const noexcept {
			return m_ctx;
		}

		event_context::event_context() noexcept {
		}

		event_context::~event_context() noexcept {
			for (auto dispatcher : m_dispatchers) {
				dispatcher->~event_dispatcher();
				platform::memory::deallocate(dispatcher);
			}
			for (auto listener : m_listeners) {
				listener->~event_listener();
				platform::memory::deallocate(listener);
			}
		}

		event_dispatcher& event_context::get_dispatcher() noexcept {
			event_dispatcher* dispatcher = (event_dispatcher*)platform::memory::allocate(sizeof(event_dispatcher));
			assert(dispatcher);
			
			new (dispatcher) event_dispatcher(this);
			m_dispatchers.insert(dispatcher);

			return *dispatcher;
		}

		b8 event_context::remove_dispatcher(event_dispatcher& _dispatcher) noexcept {
			if (_dispatcher.m_ctx == this) {
				m_dispatchers.remove(&_dispatcher);
				_dispatcher.~event_dispatcher();
				platform::memory::deallocate(&_dispatcher);
				return true;
			}

			return false;
		}

		event_listener& event_context::get_listener() noexcept {
			event_listener* listener = (event_listener*)platform::memory::allocate(sizeof(event_listener));
			assert(listener);

			new (listener) event_listener(this);
			m_listeners.insert(listener);

			return *listener;
		}

		b8 event_context::remove_listener(event_listener& _listener) noexcept {
			if (_listener.m_ctx == this) {
				m_listeners.remove(&_listener);

				for (auto& p : m_subscription_functions) {
					p.value().remove(&_listener);
				}

				_listener.~event_listener();
				platform::memory::deallocate(&_listener);

				return true;
			}

			return false;
		}

	} // namespace utility
} // namespace ivy
