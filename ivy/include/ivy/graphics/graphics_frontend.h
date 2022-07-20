#pragma once

#include "graphics_backend.h"

namespace ivy {
	namespace graphics {

		struct render_packet {
			f32 dt;
		};

		class IVY_API graphics_frontend {
		public:
			graphics_frontend(window& _window) noexcept;

			~graphics_frontend() noexcept;

			b8 is_valid() const noexcept;

			b8 begin_frame(f32 _dt) noexcept;
			b8 end_frame(f32 _dt) noexcept;
			b8 draw_frame(const render_packet& _packet) noexcept;

		private:
			graphics_backend* m_backend;
			window*           m_window;
		};

	} // namespace graphics

	using graphics::render_packet;

	using graphics::graphics_frontend;

} // namespace ivy
