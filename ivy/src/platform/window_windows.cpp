#include "platform/window.h"
#include "platform/console.h"
#include "platform/memory.h"

#if defined(IVY_PLATFORM_WINDOWS)

#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>

namespace ivy {
	namespace platform {

		struct window_state {
			HWND handle;
			// VkSurfaceKHR surface;
		};

		struct window_user_data {
			HWND            handle;
			window*         source_window;
			b8              closed;
			b8              maximized;
			b8              mouse_leaved;
			math::vector2_i old_position;
			math::vector2_i new_position;
			math::vector2_i old_size;
			math::vector2_i new_size;
			math::vector2_i mouse_position;

		};

		WPARAM map_vk(WPARAM vk, LPARAM lParam)
		{
			WPARAM new_vk = vk;
			UINT scancode = (lParam & 0x00ff0000) >> 16;
			int extended = (lParam & 0x01000000) != 0;

			switch (vk) {
			case VK_SHIFT:
				new_vk = MapVirtualKey(scancode, MAPVK_VSC_TO_VK_EX);
				break;
			case VK_CONTROL:
				new_vk = extended ? VK_RCONTROL : VK_LCONTROL;
				break;
			case VK_MENU:
				new_vk = extended ? VK_RMENU : VK_LMENU;
				break;
			default:
				new_vk = vk;
				break;
			}

			return new_vk;
		}

		LRESULT CALLBACK win32_process_message(HWND hwnd, u32 msg, WPARAM w_param, LPARAM l_param) {
			switch (msg) {
			case WM_ERASEBKGND: {
				return 1;
			}
			case WM_CLOSE: {
				window_user_data* user_data = (window_user_data*)GetWindowLongPtrA(hwnd, GWLP_USERDATA);

				window_close_event ev;
				ev.source_window = user_data->source_window;

				user_data->source_window->post_event(ev);
				return 0;
			}
			case WM_MOVE: {
				window_user_data* user_data = (window_user_data*)GetWindowLongPtrA(hwnd, GWLP_USERDATA);
				user_data->new_position = math::vector2_i(LOWORD(l_param), HIWORD(l_param));

				return 0;
			}
			case WM_SIZE: {
				window_user_data* user_data = (window_user_data*)GetWindowLongPtrA(hwnd, GWLP_USERDATA);
				
				if (w_param || user_data->maximized) {
					RECT client_rect;
					if (GetClientRect(hwnd, &client_rect)) {
						user_data->new_size = math::vector2_i(client_rect.right - client_rect.left, client_rect.bottom - client_rect.top);
						
						window_size_event ev;

						ev.source_window = user_data->source_window;
						ev.old_size      = user_data->old_size;
						ev.new_size      = user_data->new_size;
						ev.maximized     = w_param == SIZE_MAXIMIZED;
						ev.minimized     = w_param == SIZE_MINIMIZED;

						user_data->source_window->post_event(ev);

						user_data->old_size  = user_data->new_size;
						user_data->maximized = (w_param == SIZE_MAXIMIZED);
					}
				}
				else {
					user_data->new_size  = math::vector2_i(LOWORD(l_param), HIWORD(l_param));
					user_data->maximized = false;
				}

				return 0;
			}
			case WM_EXITSIZEMOVE: {
				window_user_data* user_data = (window_user_data*)GetWindowLongPtrA(hwnd, GWLP_USERDATA);

				if (user_data->old_position != user_data->new_position) {
					window_move_event ev;

					ev.source_window = user_data->source_window;
					ev.old_position  = user_data->old_position;
					ev.new_position  = user_data->new_position;

					user_data->source_window->post_event(ev);

					user_data->old_position = user_data->new_position;
				}

				if (user_data->old_size != user_data->new_size) {
					window_size_event ev;

					ev.source_window = user_data->source_window;
					ev.old_size      = user_data->old_size;
					ev.new_size      = user_data->new_size;
					ev.maximized     = false;
					ev.minimized     = false;

					user_data->source_window->post_event(ev);

					user_data->old_size = user_data->new_size;
				}

				return 0;
			}
			case WM_SETFOCUS: {
				window_user_data* user_data = (window_user_data*)GetWindowLongPtrA(hwnd, GWLP_USERDATA);
				
				window_focus_event ev0;
				ev0.source_window = user_data->source_window;
				ev0.gain_focus    = true;

				user_data->source_window->post_event(ev0);

				if (user_data->old_size != user_data->new_size) {
					window_size_event ev1;

					ev1.source_window = user_data->source_window;
					ev1.old_size = user_data->old_size;
					ev1.new_size = user_data->new_size;

					user_data->source_window->post_event(ev1);

					user_data->old_size = user_data->new_size;
				}

				return 0;
			}
			case WM_KILLFOCUS: {
				window_user_data* user_data = (window_user_data*)GetWindowLongPtrA(hwnd, GWLP_USERDATA);

				if (!user_data->closed) {
					window_focus_event ev0;
					ev0.source_window = user_data->source_window;
					ev0.gain_focus = false;

					user_data->source_window->post_event(ev0);

					if (user_data->old_size != user_data->new_size) {
						window_size_event ev1;

						ev1.source_window = user_data->source_window;
						ev1.old_size = user_data->old_size;
						ev1.new_size = user_data->new_size;

						user_data->source_window->post_event(ev1);

						user_data->old_size = user_data->new_size;
					}
				}

				return 0;
			}
			case WM_DESTROY: {
				window_user_data* user_data = (window_user_data*)GetWindowLongPtrA(hwnd, GWLP_USERDATA);
				user_data->closed = true;

				PostQuitMessage(0);
				return 0;
			}
			case WM_KEYDOWN: {
				w_param = map_vk(w_param, l_param);

				window_user_data* user_data = (window_user_data*)GetWindowLongPtrA(hwnd, GWLP_USERDATA);

				if ((HIWORD(l_param) & KF_REPEAT) == KF_REPEAT) {
					key_repeat_event ev;
					ev.key = w_param;

					user_data->source_window->post_event(ev);
				}
				else {
					key_down_event ev;
					ev.key = w_param;

					user_data->source_window->post_event(ev);
				}

				return 0;
			}
			case WM_KEYUP: {
				w_param = map_vk(w_param, l_param);

				window_user_data* user_data = (window_user_data*)GetWindowLongPtrA(hwnd, GWLP_USERDATA);

				key_up_event ev;
				ev.key = w_param;

				user_data->source_window->post_event(ev);

				return 0;
			}
			case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_XBUTTONDOWN: {
				window_user_data* user_data = (window_user_data*)GetWindowLongPtrA(hwnd, GWLP_USERDATA);

				mouse_button_down_event ev;
				ev.position = math::vector2_i(GET_X_LPARAM(l_param), GET_Y_LPARAM(l_param));

				if (msg == WM_LBUTTONDOWN) {
					ev.button = mouse_button::Left;
				}
				else if (msg == WM_RBUTTONDOWN) {
					ev.button = mouse_button::Right;
				}
				else if (msg == WM_MBUTTONDOWN) {
					ev.button = mouse_button::Middle;
				}
				else if (msg == WM_XBUTTONDOWN) {
					if (HIWORD(w_param) == XBUTTON1) {
						ev.button = mouse_button::X1;
					}
					else {
						ev.button = mouse_button::X2;
					}
				}

				user_data->source_window->post_event(ev);
				return 0;
			}
			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
			case WM_MBUTTONUP:
			case WM_XBUTTONUP: {
				window_user_data* user_data = (window_user_data*)GetWindowLongPtrA(hwnd, GWLP_USERDATA);

				mouse_button_up_event ev;
				ev.position = math::vector2_i(GET_X_LPARAM(l_param), GET_Y_LPARAM(l_param));

				if (msg == WM_LBUTTONUP) {
					ev.button = mouse_button::Left;
				}
				else if (msg == WM_RBUTTONUP) {
					ev.button = mouse_button::Right;
				}
				else if (msg == WM_MBUTTONUP) {
					ev.button = mouse_button::Middle;
				}
				else if (msg == WM_XBUTTONUP) {
					if (HIWORD(w_param) == XBUTTON1) {
						ev.button = mouse_button::X1;
					}
					else {
						ev.button = mouse_button::X2;
					}
				}

				user_data->source_window->post_event(ev);
				return 0;
			}
			case WM_LBUTTONDBLCLK:
			case WM_RBUTTONDBLCLK:
			case WM_MBUTTONDBLCLK: 
			case WM_XBUTTONDBLCLK: {
				window_user_data* user_data = (window_user_data*)GetWindowLongPtrA(hwnd, GWLP_USERDATA);

				mouse_button_double_click_event ev;
				ev.position = math::vector2_i(GET_X_LPARAM(l_param), GET_Y_LPARAM(l_param));

				if (msg == WM_LBUTTONDBLCLK) {
					ev.button = mouse_button::Left;
				}
				else if (msg == WM_RBUTTONDBLCLK) {
					ev.button = mouse_button::Right;
				}
				else if (msg == WM_MBUTTONDBLCLK) {
					ev.button = mouse_button::Middle;
				}
				else if (msg == WM_XBUTTONDBLCLK) {
					if (HIWORD(w_param) == XBUTTON1) {
						ev.button = mouse_button::X1;
					}
					else {
						ev.button = mouse_button::X2;
					}
				}

				user_data->source_window->post_event(ev);
				return 0;
			}
			case WM_MOUSEWHEEL:
			case WM_MOUSEHWHEEL: {
				window_user_data* user_data = (window_user_data*)GetWindowLongPtrA(hwnd, GWLP_USERDATA);

				mouse_wheel_event ev;
				if (msg == WM_MOUSEHWHEEL) {
					ev.delta.x = (i16)HIWORD(w_param) / WHEEL_DELTA;
				}
				else {
					ev.delta.y = (i16)HIWORD(w_param) / WHEEL_DELTA;
				}

				user_data->source_window->post_event(ev);
				return 0;
			}
			case WM_MOUSEMOVE: {
				window_user_data* user_data = (window_user_data*)GetWindowLongPtrA(hwnd, GWLP_USERDATA);

				if (user_data->mouse_leaved) {
					POINT p;
					GetCursorPos(&p);
					ScreenToClient(user_data->handle, &p);

					user_data->mouse_leaved   = false;
					user_data->mouse_position = math::vector2_i(p.x, p.y);

					mouse_enter_event ev;

					user_data->source_window->post_event(ev);
				}
				else {
					TRACKMOUSEEVENT tme{};
					tme.cbSize    = sizeof(TRACKMOUSEEVENT);
					tme.dwFlags   = TME_LEAVE;
					tme.hwndTrack = user_data->handle;
					TrackMouseEvent(&tme);

					mouse_move_event ev;
					ev.old_position = user_data->mouse_position;
					ev.new_position = math::vector2_i(GET_X_LPARAM(l_param), GET_Y_LPARAM(l_param));

					user_data->mouse_position = ev.new_position;

					user_data->source_window->post_event(ev);
				}

				return 0;
			}
			case WM_MOUSELEAVE: {
				window_user_data* user_data = (window_user_data*)GetWindowLongPtrA(hwnd, GWLP_USERDATA);

				mouse_leave_event ev;

				user_data->mouse_leaved = true;

				user_data->source_window->post_event(ev);
				return 0;
			}
			default: {
				break;
			}
			}

			return DefWindowProcA(hwnd, msg, w_param, l_param);
		}

		static b8                  window_class_registered = false;
		static HINSTANCE           window_class_instance;
		static constexpr const c8* window_class_name = "IVY_ENGINE_WINDOW_CLASS";

		static b8 register_window_class() {
			if (!window_class_registered) {
				window_class_registered = true;

				window_class_instance = GetModuleHandleA(0);
				HICON icon = LoadIcon(window_class_instance, IDI_APPLICATION);

				WNDCLASSA wc;
				memset(&wc, 0, sizeof(wc));
				wc.style         = CS_DBLCLKS;
				wc.lpfnWndProc   = win32_process_message;
				wc.cbClsExtra    = 0;
				wc.cbWndExtra    = 0;
				wc.hInstance     = window_class_instance;
				wc.hIcon         = icon;
				wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
				wc.hbrBackground = NULL;
				wc.lpszClassName = window_class_name;

				if (!RegisterClassA(&wc)) {
					console::error(">>> PLATFORM: Failed to register window class\n", console::log_level::Fatal);
					return false;
				}
			}

			return true;
		}

		window::window(event_context& _event_ctx, const title_type& _title, i32 _x, i32 _y, i32 _w, i32 _h) noexcept :
			m_state(nullptr),
			m_title(_title),
			m_x(_x), m_y(_y), m_w(_w), m_h(_h),
			m_dispatcher(&_event_ctx.get_dispatcher()) {

			m_is_open = false;

			if (register_window_class()) {
				m_state = memory::allocate(sizeof(window_state));
				if (!m_state) {
					console::error(">>> PLATFORM: Failed to allocate memory for window state\n", console::log_level::Error);
					return;
				}

				u32 client_x = m_x;
				u32 client_y = m_y;
				u32 client_w = m_w;
				u32 client_h = m_h;

				u32 window_x = client_x;
				u32 window_y = client_y;
				u32 window_w = client_w;
				u32 window_h = client_h;

				u32 window_style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME;
				u32 window_ex_style = WS_EX_APPWINDOW;

				RECT border_rect = { 0, 0, 0, 0 };
				AdjustWindowRectEx(&border_rect, window_style, 0, window_ex_style);

				window_x += border_rect.left;
				window_y += border_rect.top;

				window_w += border_rect.right - border_rect.left;
				window_h += border_rect.bottom - border_rect.top;

				((window_state*)m_state)->handle = CreateWindowExA(window_ex_style, window_class_name, _title.cstr(), window_style, window_x, window_y, window_w, window_h, 0, 0, window_class_instance, 0);
				if (!((window_state*)m_state)->handle) {
					console::error(">>> PLATFORM: Failed to create window\n", console::log_level::Error);

					memory::deallocate(m_state);
					m_state = nullptr;

					return;
				}

				POINT p;
				GetCursorPos(&p);
				ScreenToClient(((window_state*)m_state)->handle, &p);

				window_user_data* user_data = (window_user_data*)memory::allocate(sizeof(window_user_data));
				user_data->handle         = ((window_state*)m_state)->handle;
				user_data->source_window  = this;
				user_data->closed         = false;
				user_data->maximized      = false;
				user_data->old_position   = math::vector2_i(client_x, client_y);
				user_data->new_position   = user_data->old_position;
				user_data->old_size       = math::vector2_i(client_w, client_h);
				user_data->new_size       = user_data->old_size;
				user_data->mouse_position = math::vector2_i(p.x, p.y);
				user_data->mouse_leaved   = p.x < client_x || p.x > client_x + client_w || p.y < client_y || p.y > client_y + client_h;

				SetWindowLongPtrA(((window_state*)m_state)->handle, GWLP_USERDATA, (LONG_PTR)user_data);

				i32 should_activate = 1;
				i32 show_window_command_flags = should_activate ? SW_SHOW : SW_SHOWNOACTIVATE;

				m_is_open = true;
				ShowWindow(((window_state*)m_state)->handle, show_window_command_flags);
			}	
		}

		window::window(window&& _source) noexcept :
			m_state(_source.m_state),
			m_title(std::move(_source.m_title)),
			m_x(_source.m_x), m_y(_source.m_y), m_w(_source.m_w), m_h(_source.m_h),
			m_is_open(_source.m_is_open),
			m_dispatcher(_source.m_dispatcher) {
			_source.m_state   = nullptr;
			_source.m_is_open = false;
			window_user_data* user_data = (window_user_data*)GetWindowLongPtrA(((window_state*)m_state)->handle, GWLP_USERDATA);
			user_data->source_window = this;
		}

		window::~window() noexcept {
			if (m_state) {
				window_user_data* user_data = (window_user_data*)GetWindowLongPtrA(((window_state*)m_state)->handle, GWLP_USERDATA);
				memory::deallocate(user_data);

				DestroyWindow(((window_state*)m_state)->handle);

				memory::deallocate(m_state);
				m_state = nullptr;
			}
		}

		window& window::operator=(window&& _source) noexcept {
			if (this != &_source) {
				m_state      = _source.m_state;
				m_title      = std::move(_source.m_title);
				m_x          = _source.m_x;
				m_y          = _source.m_y;
				m_w          = _source.m_w;
				m_h          = _source.m_h;
				m_is_open    = _source.m_is_open;
				m_dispatcher = _source.m_dispatcher;

				_source.m_state   = nullptr;
				_source.m_is_open = false;
				window_user_data* user_data = (window_user_data*)GetWindowLongPtrA(((window_state*)m_state)->handle, GWLP_USERDATA);
				user_data->source_window = this;
			}

			return *this;
		}

		b8 window::is_valid() const noexcept {
			return m_state;
		}

		b8 window::is_open() const noexcept {
			return m_is_open;
		}

		b8 window::close() noexcept {
			m_is_open = false;
			return true;
		}

		b8 window::pump_messages() const noexcept {
			if (m_state) {
				MSG message;
				while (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) {
					TranslateMessage(&message);
					DispatchMessageA(&message);
				}

				return true;
			}

			return false;
		}

	} // namespace platform
} // namespace ivy

#endif