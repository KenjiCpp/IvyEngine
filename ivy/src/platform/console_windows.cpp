#include "platform/console.h"

#if defined(IVY_PLATFORM_WINDOWS)

#include <windows.h>

namespace ivy {
	namespace platform {

		namespace details {

			HANDLE write_handle = GetStdHandle(STD_OUTPUT_HANDLE);
			HANDLE error_handle = GetStdHandle(STD_ERROR_HANDLE);

			CONSOLE_SCREEN_BUFFER_INFO console_info;

			u8 console_text_color_codes[6] = { 64, 4, 6, 2, 1, 8 };

			console_impl::console_impl() noexcept {
				write(">>> PLATFORM: Console initialized\n", 3);
			}

			console_impl::~console_impl() noexcept {
				write(">>> PLATFORM: Console terminated\n", 3);
			}

			i32 console_impl::write(const c8* _message, u8 _level) noexcept {

				// Save the current text attribute
				WORD console_text_attribute = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
				if (GetConsoleScreenBufferInfo(write_handle, &console_info))
					console_text_attribute = console_info.wAttributes;

				// Set color and output message
				SetConsoleTextAttribute(write_handle, console_text_color_codes[_level]);
				OutputDebugStringA(_message);
				u64 length = strlen(_message);
				DWORD number_written = 0;

				// If failed to write, revert text attribute and return console::failed
				if (!WriteConsoleA(write_handle, _message, (DWORD)length, &number_written, 0)) {
					SetConsoleTextAttribute(write_handle, console_text_attribute);
					return -1;
				}

				// If succeeded to write, revert text attribute and return the number of characters wrote
				SetConsoleTextAttribute(write_handle, console_text_attribute);
				return number_written;
			}

			i32 console_impl::Error(const c8* _message, u8 _level) noexcept {

				// Save the current text attribute
				WORD console_text_attribute = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
				if (GetConsoleScreenBufferInfo(error_handle, &console_info))
					console_text_attribute = console_info.wAttributes;

				// Set color and output message
				SetConsoleTextAttribute(error_handle, console_text_color_codes[_level]);
				OutputDebugStringA(_message);
				u64 length = strlen(_message);
				DWORD number_written = 0;

				// If failed to write, revert text attribute and return console::failed
				if (!WriteConsoleA(error_handle, _message, (DWORD)length, &number_written, 0)) {
					SetConsoleTextAttribute(error_handle, console_text_attribute);
					return -1;
				}

				// If succeeded to write, revert text attribute and return the number of characters wrote
				SetConsoleTextAttribute(error_handle, console_text_attribute);
				return number_written;
			}

		} // namespace details

		i32 console::write(const c8* _message, log_level _level) noexcept {
			return details::console_impl::write(_message, (u8)_level);
		}

		i32 console::error(const c8* _message, log_level _level) noexcept {
			return details::console_impl::Error(_message, (u8)_level);
		}

	} // namespace platform
} // namespace ivy

#endif