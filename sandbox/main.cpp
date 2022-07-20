#include "ivy/ivy.h"

ivy::console_logger logger(4);

class application {
public:
	application(const ivy::string8& _title) noexcept :
		m_app_title(_title),
		m_clock() {
		m_valid = initialize();
		if (!m_valid) {
			logger.log(ivy::log_level::Fatal, "Failed to initialize application ({})", m_app_title);
		}
	}

	~application() noexcept {
		if (m_valid) {
			m_event_context->remove_listener(*m_event_listener);

			m_event_context->~event_context();
			m_graphics->~graphics_frontend();
			m_window->~window();

			ivy::memory::deallocate(m_event_context);
			ivy::memory::deallocate(m_window);
			ivy::memory::deallocate(m_graphics);

			m_event_context = nullptr;
			m_window        = nullptr;
			m_graphics      = nullptr;
		}
	}

	b8 run() noexcept {
		if (m_valid) {
			while (m_window->is_open()) {
				m_window->pump_messages();

				ivy::render_packet rp;
				rp.dt = m_clock.restart();

				m_graphics->draw_frame(rp);
			}

			return true;
		}

		return false;
	}

private:
	b8 initialize() noexcept {
		// Event context
		m_event_context = (ivy::event_context*)ivy::memory::allocate(sizeof(ivy::event_context));
		if (!m_event_context) {
			return false;
		}
		new (m_event_context) ivy::event_context();

		// Event listener
		m_event_listener = &m_event_context->get_listener();

		// Window
		m_window = (ivy::window*)ivy::memory::allocate(sizeof(ivy::window));
		if (!m_window) {
			m_event_context->~event_context();

			ivy::memory::deallocate(m_event_context);

			m_event_context = nullptr;

			return false;
		}
		new (m_window) ivy::window(*m_event_context, m_app_title, 100, 100, 600, 360);
		if (!m_window->is_valid()) {
			m_event_context->~event_context();
			m_window->~window();

			ivy::memory::deallocate(m_event_context);
			ivy::memory::deallocate(m_window);

			m_event_context = nullptr;
			m_window        = nullptr;

			return false;
		}
		m_event_listener->subscribe<ivy::window_close_event>([](ivy::window_close_event& _event) {
			if (_event.source_window) {
				_event.source_window->close();
			}
		});

		// Graphics
		m_graphics = (ivy::graphics_frontend*)ivy::memory::allocate(sizeof(ivy::graphics_frontend));
		if (!m_graphics) {
			m_event_context->~event_context();
			m_window->~window();
			
			ivy::memory::deallocate(m_event_context);
			ivy::memory::deallocate(m_window);

			m_event_context = nullptr;
			m_window        = nullptr;

			return false;
		}
		new (m_graphics) ivy::graphics_frontend(*m_window);
		if (!m_graphics->is_valid()) {
			m_event_context->~event_context();
			m_window->~window();
			m_graphics->~graphics_frontend();

			ivy::memory::deallocate(m_event_context);
			ivy::memory::deallocate(m_window);
			ivy::memory::deallocate(m_graphics);

			m_event_context = nullptr;
			m_window        = nullptr;
			m_graphics      = nullptr;

			return false;
		}

		return true;
	}

private:
	b8                      m_valid;

	ivy::string8            m_app_title;
	ivy::clock              m_clock;

	ivy::event_context*     m_event_context  = nullptr;
	ivy::event_listener*    m_event_listener = nullptr;
	ivy::window*            m_window         = nullptr;
	ivy::graphics_frontend* m_graphics       = nullptr;
};

int main() {

	application app("Test application");
	app.run();

	return 0;
}
