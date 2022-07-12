#pragma once

#include "utility/string.h"

#include "input.h"

namespace ivy {
	namespace platform {

		class window;

		struct window_event : utility::event {
			DEFINE_BASE_EVENTS(utility::event);

			window* source_window = nullptr;
		};

		struct window_close_event : window_event {
			DEFINE_BASE_EVENTS(window_event);
		};

		struct window_move_event : window_event {
			DEFINE_BASE_EVENTS(window_event);

			math::vector2_i old_position;
			math::vector2_i new_position;
		};

		struct window_size_event : window_event {
			DEFINE_BASE_EVENTS(window_event);

			math::vector2_i old_size;
			math::vector2_i new_size;

			b8 maximized = false;
			b8 minimized = false;
		};

		struct window_focus_event : window_event {
			DEFINE_BASE_EVENTS(window_event);

			b8 gain_focus = false;
		};

		class IVY_API window {
		public:
			typedef utility::string8          title_type;
			typedef utility::event            event;
			typedef utility::event_context    event_context;
			typedef utility::event_dispatcher event_dispatcher;
			typedef utility::event_listener   event_listener;

		public:
			window(event_context& _event_ctx, const title_type& _title, i32 _x, i32 _y, i32 _w, i32 _h) noexcept;

			window(window&& _source) noexcept;
			window(const window& _other) noexcept = delete;

			~window() noexcept;

			window& operator=(window&& _source) noexcept;
			window& operator=(const window& _other) noexcept = delete;

			b8 is_valid() const noexcept;
			b8 is_open() const noexcept;

			b8 close() noexcept;

			b8 pump_messages() const noexcept;
			
			template<typename EventT>
			b8 post_event(EventT& _event) noexcept;

		private:
			ptr               m_state;
			title_type        m_title;
			i32               m_x, m_y, m_w, m_h;
			b8                m_is_open;
			event_dispatcher* m_dispatcher;
		};

	} // namespace platform

	using platform::window_event;
	using platform::window_close_event;
	using platform::window_move_event;
	using platform::window_size_event;
	using platform::window_focus_event;

} // namespace ivy

#include "../impl/platform/window.inl"
