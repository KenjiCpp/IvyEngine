#include "graphics/graphics_backend.h"

#include "platform/memory.h"

#include "graphics/vulkan/vulkan_graphics_backend.h"

namespace ivy {
	namespace graphics {

		graphics_backend::graphics_backend(graphics_backend_framework _framework) noexcept : 
			m_framework(_framework) {
		}

		graphics_backend_framework graphics_backend::framework() const noexcept {
			return m_framework;
		}

		graphics_backend* create_graphics_backend(graphics_backend_framework _framework, ptr _config) noexcept {
			switch (_framework)
			{
			case graphics_backend_framework::OpenGL: {

				return nullptr;
			}
			case graphics_backend_framework::Vulkan: {
				graphics_backend* res = (vulkan_graphics_backend*)memory::allocate(sizeof(vulkan_graphics_backend));
				new (res) vulkan_graphics_backend(*(vulkan_graphics_backend_configuration*)_config);
				return res;
			}
			case graphics_backend_framework::DirectX: {

				return nullptr;
			}
			default:
				return nullptr;
			}
		}

		b8 destroy_graphics_backend(graphics_backend* _backend) noexcept {
			memory::deallocate(_backend);
			return true;
		}

	} // namespace graphics
} // namespace ivy
