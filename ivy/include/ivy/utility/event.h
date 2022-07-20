#pragma once

#include "types_list.h"
#include "set.h"
#include "map.h"

#define DEFINE_BASE_EVENTS(...) typedef ivy::utility::types_list<__VA_ARGS__> bases

namespace ivy {
	namespace utility {

		struct event {
			DEFINE_BASE_EVENTS();

			b8 handled = false;
		};

		class event_context;

		class IVY_API event_dispatcher {
		public:
			friend class event_context;

		public:
			event_dispatcher(event_dispatcher&& _source) noexcept = delete;
			event_dispatcher(const event_dispatcher& _other) noexcept = delete;

			~event_dispatcher() noexcept;

			event_dispatcher& operator=(event_dispatcher&& _source) noexcept = delete;
			event_dispatcher& operator=(const event_dispatcher& _other) noexcept = delete;

			b8 is_valid() const noexcept;

			template<typename EventT>
			b8 post(EventT& _ev) noexcept;

		private:
			event_dispatcher(event_context* _ctx) noexcept;

		private:
			event_context* m_ctx;
		};

		class IVY_API event_listener {
		public:
			friend class event_context;

		public:
			event_listener(event_listener&& _source) noexcept = delete;
			event_listener(const event_listener& _other) noexcept = delete;

			~event_listener() noexcept;

			event_listener& operator=(event_listener&& _source) noexcept = delete;
			event_listener& operator=(const event_listener& _other) noexcept = delete;

			b8 is_valid() const noexcept;

			template<typename EventT>
			b8 subscribe(function<void, EventT&> _callback) noexcept;

			template<typename EventT>
			b8 unsubscribe() noexcept;

		private:
			event_listener(event_context* _ctx) noexcept;

		private:
			event_context* m_ctx;
		};

		class IVY_API event_context {
		public:
			friend class event_dispatcher;
			friend class event_listener;

		public:
			event_context() noexcept;

			event_context(event_context&& _source) noexcept = delete;
			event_context(const event_context& _other) noexcept = delete;

			~event_context() noexcept;

			event_context& operator=(event_context&& _source) noexcept = delete;
			event_context& operator=(const event_context& _other) noexcept = delete;

			event_dispatcher& get_dispatcher() noexcept;
			b8 remove_dispatcher(event_dispatcher& _dispatcher) noexcept;

			event_listener& get_listener() noexcept;
			b8 remove_listener(event_listener& _listener) noexcept;

		private:
			template<typename EventT>
			b8 dispatch(EventT& _event) noexcept;

			template<typename EventT, typename... BasesT>
			b8 dispatch_recursive(EventT& _event, types_list<BasesT...> _bases, set<type_index>& _types) noexcept;

			template<typename EventT>
			b8 dispatch_recursive(EventT& _event, types_list<> _bases, set<type_index>& _types) noexcept;

			template<typename EventT, typename BaseT, typename... BasesT>
			b8 dispatch_recursive(EventT& _event, types_list<BaseT, BasesT...> _bases, set<type_index>& _types) noexcept;

		private:
			set<event_dispatcher*>                                     m_dispatchers;
			set<event_listener*>                                       m_listeners;
			map<type_index, map<event_listener*, function<void, ptr>>> m_subscription_functions;
		};

	} // namespace utility

	using utility::event;

	using utility::event_context;
	using utility::event_dispatcher;
	using utility::event_listener;

} // namespace ivy

#include "../impl/utility/event.inl"
