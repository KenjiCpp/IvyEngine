#include "ivy/ivy.h"

using namespace ivy;

utility::console_logger logger(4);

class application {
public:
	application(const utility::string8& _title) noexcept :
		m_event_context(),
		m_window(m_event_context, _title, 100, 100, 600, 360),
		m_event_listener(&m_event_context.get_listener()) {
		m_event_listener->subscribe<window_close_event>([](window_close_event& _event) {
			if (_event.source_window) {
				_event.source_window->close();
			}
		});
	}

	void run() noexcept {
		while (m_window.is_open()) {
			m_window.pump_messages();
		}
	}

private:
	utility::event_context   m_event_context;
	utility::event_listener* m_event_listener;

	platform::window         m_window;
};

int main() {

	application app("Test application");
	app.run();

	return 0;
}
