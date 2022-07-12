#pragma once

#include "utility/event.h"

#include "math/vector.h"

namespace ivy {
	namespace platform {

		enum class keyboard_key : u32 {
			// TODO: key codes
			Count, Undefined = -1
		};

		// TODO: function to map from win32 key codes to keyboard_key

		enum class mouse_button : u32 {
			Left, Right, Middle, X1, X2,
			Count, Undefined = -1
		};

		// TODO: function to map from win32 mouse button codes to mouse_button

		struct input_event : utility::event {
			DEFINE_BASE_EVENTS(utility::event);
		};

		struct key_event : input_event {
			DEFINE_BASE_EVENTS(input_event);

			// TODO: change this
			// keyboard_key key;
			int key;
		};

		struct key_down_event : key_event {
			DEFINE_BASE_EVENTS(key_event);
		};

		struct key_up_event : key_event {
			DEFINE_BASE_EVENTS(key_event);
		};

		struct key_repeat_event : key_event {
			DEFINE_BASE_EVENTS(key_event);
		};

		struct mouse_event : input_event {
			DEFINE_BASE_EVENTS(input_event);
		};

		struct mouse_button_event : mouse_event {
			DEFINE_BASE_EVENTS(mouse_event);

			mouse_button    button;
			math::vector2_i position;
		};

		struct mouse_button_down_event : mouse_button_event {
			DEFINE_BASE_EVENTS(mouse_button_event);
		};

		struct mouse_button_up_event : mouse_button_event {
			DEFINE_BASE_EVENTS(mouse_button_event);
		};

		struct mouse_button_double_click_event : mouse_button_event {
			DEFINE_BASE_EVENTS(mouse_button_event);
		};

		struct mouse_wheel_event : mouse_event {
			DEFINE_BASE_EVENTS(mouse_event);

			math::vector2_i delta;
		};

		struct mouse_move_event : mouse_event {
			DEFINE_BASE_EVENTS(mouse_event);

			math::vector2_i old_position;
			math::vector2_i new_position;
		};

		struct mouse_enter_event : mouse_event {
			DEFINE_BASE_EVENTS(mouse_event);
		};

		struct mouse_leave_event : mouse_event {
			DEFINE_BASE_EVENTS(mouse_event);
		};

	} // namespace platform

	using platform::keyboard_key;

	using platform::input_event;

	using platform::key_event;
	using platform::key_down_event;
	using platform::key_up_event;
	using platform::key_repeat_event;

	using platform::mouse_event;
	using platform::mouse_button_event;
	using platform::mouse_button_down_event;
	using platform::mouse_button_up_event;
	using platform::mouse_button_double_click_event;
	using platform::mouse_wheel_event;
	using platform::mouse_move_event;
	using platform::mouse_enter_event;
	using platform::mouse_leave_event;

} // namespace ivy
