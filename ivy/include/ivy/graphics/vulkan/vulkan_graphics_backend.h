#pragma once

#include "graphics/graphics_backend.h"

#include "utility/array.h"
#include "utility/string.h"

#include <vulkan/vulkan.h>

namespace ivy {
	namespace graphics {

		struct vulkan_graphics_backend_configuration {
			const c8* application_name;
		};

		class vulkan_graphics_backend : public graphics_backend {
		public:
			vulkan_graphics_backend(const vulkan_graphics_backend_configuration& _config) noexcept;

			b8 initialize(window& _window) noexcept;
			b8 terminate(window& _window) noexcept;
			
			b8 begin_frame(f32 _dt) noexcept;
			b8 end_frame(f32 _dt) noexcept;

		private:
			vulkan_graphics_backend_configuration m_config;
		};

	} // namespace graphics
} // namespace ivy
