#include "graphics/graphics_frontend.h"

namespace ivy {
	namespace graphics {

		graphics_frontend::graphics_frontend(window& _window) noexcept :
			m_backend(nullptr),
			m_window(&_window) {

			// TODO: Make this configurable
			struct {
				const c8* app_name = "Test application";
			} config;
			graphics_backend* new_backend = create_graphics_backend(graphics_backend_framework::Vulkan, &config);

			if (new_backend) {
				if (new_backend->initialize(*m_window)) {
					m_backend = new_backend;
				}
			}
		}

		graphics_frontend::~graphics_frontend() noexcept {
			if (m_backend) {
				m_backend->terminate(*m_window);
				destroy_graphics_backend(m_backend);
			}
		}

		b8 graphics_frontend::is_valid() const noexcept {
			return m_backend != nullptr;
		}

		b8 graphics_frontend::begin_frame(f32 _dt) noexcept {
			if (m_backend) {
				return m_backend->begin_frame(_dt);
			}

			return false;
		}

		b8 graphics_frontend::end_frame(f32 _dt) noexcept {
			if (m_backend) {
				return m_backend->end_frame(_dt);
			}

			return false;
		}

		b8 graphics_frontend::draw_frame(const render_packet& _packet) noexcept {
			if (begin_frame(_packet.dt)) {

				// TODO: render packet

				if (!end_frame(_packet.dt)) {
					return false;
				}
			}

			return true;
		}


	} // namespace graphics
} // namespace ivy
