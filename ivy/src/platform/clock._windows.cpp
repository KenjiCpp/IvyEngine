#include "platform/clock.h"

#if defined(IVY_PLATFORM_WINDOWS)

#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>

#include "platform/memory.h"

namespace ivy {
	namespace platform {

		static b8            clock_initialized = false;
		static LARGE_INTEGER frequency;

		b8 clock_init() noexcept {
			if (!clock_initialized) {
				QueryPerformanceFrequency(&frequency);
			}

			return true;
		}

		struct clock_state {
			LARGE_INTEGER start;
		};

		clock::clock() noexcept :
			m_state(nullptr) {
			clock_state* new_state = (clock_state*)memory::allocate(sizeof(clock_state));
			assert(new_state);

			QueryPerformanceCounter(&new_state->start);

			m_state = new_state;
		}

		clock::~clock() noexcept {
			if (m_state) {
				memory::deallocate(m_state);
				m_state = nullptr;
			}
		}

		f32 clock::elasped() const noexcept {
			if (clock_init()) {
				if (m_state) {
					LARGE_INTEGER now, elasped_microseconds;
					QueryPerformanceCounter(&now);
					elasped_microseconds.QuadPart = now.QuadPart - ((clock_state*)m_state)->start.QuadPart;
					return elasped_microseconds.QuadPart / (f32)frequency.QuadPart;
				}
			}

			return 0.0f;
		}

		f32 clock::restart() noexcept {
			if (clock_init()) {
				if (m_state) {
					LARGE_INTEGER now, elasped_microseconds;
					QueryPerformanceCounter(&now);
					elasped_microseconds.QuadPart = now.QuadPart - ((clock_state*)m_state)->start.QuadPart;
					((clock_state*)m_state)->start = now;
					return elasped_microseconds.QuadPart / (f32)frequency.QuadPart;
				}
			}

			return 0.0f;
		}

	} // namespace platform
} // namespace ivy


#endif