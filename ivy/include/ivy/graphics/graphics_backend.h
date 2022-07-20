#pragma once

#include "platform/window.h"

namespace ivy{
	namespace graphics {

		enum class graphics_backend_framework {
			OpenGL, Vulkan, DirectX
		};

		class graphics_backend {
		protected:
			graphics_backend(graphics_backend_framework _framework) noexcept;

		public:
			graphics_backend_framework framework() const noexcept;

			virtual b8 initialize(window& _window) noexcept = 0;
			virtual b8 terminate(window& _window) noexcept = 0;

			virtual b8 begin_frame(f32 _dt) noexcept = 0;
			virtual b8 end_frame(f32 _dt) noexcept = 0;

		private:
			graphics_backend_framework m_framework;
		};

		graphics_backend* create_graphics_backend(graphics_backend_framework _framework, ptr _config) noexcept;
		
		b8 destroy_graphics_backend(graphics_backend* _backend) noexcept;

	} // namespace graphics

	using graphics::graphics_backend_framework;
	using graphics::graphics_backend;

	using graphics::create_graphics_backend;
	using graphics::destroy_graphics_backend;

} // namespace ivy
