#include "graphics/vulkan/vulkan_platform.h"

#if defined(IVY_PLATFORM_WINDOWS)

#include <windows.h>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>

#include "utility/logger.h"

namespace ivy {
	namespace graphics {

		array<const c8*> vulkan_platform_required_extensions() noexcept {
			return { "VK_KHR_win32_surface" };
		}

		struct vulkan_graphics_data {
			VkSurfaceKHR surface;
		};

		struct window_state {
			HWND      handle;
			HINSTANCE instance;
			ptr       graphics;
		};

		b8 vulkan_platform_create_surface(vulkan_context& _ctx, window& _window) noexcept {
			vulkan_graphics_data* res = (vulkan_graphics_data*)platform::memory::allocate(sizeof(vulkan_graphics_data));
			if (res) {
				VkWin32SurfaceCreateInfoKHR create_info = { VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR };
				create_info.hinstance = ((window_state*)_window.state())->instance;
				create_info.hwnd      = ((window_state*)_window.state())->handle;

				if (vkCreateWin32SurfaceKHR(_ctx.instance, &create_info, _ctx.allocator, &(res->surface)) != VK_SUCCESS) {
					memory::deallocate(res);
					return false;
				}

				_ctx.surface = res->surface;
				_window.graphics_data() = res;

				return true;
			}

			_window.graphics_data() = nullptr;
			return false;
		}

		b8 vulkan_platform_remove_surface(vulkan_context& _ctx, window& _window) noexcept {
			if (_window.graphics_data()) {
				vkDestroySurfaceKHR(_ctx.instance, ((vulkan_graphics_data*)(_window.graphics_data()))->surface, _ctx.allocator);
				memory::deallocate(_window.graphics_data());
			}

			return true;
		}

	} // namespace graphics
} // namespace ivy

#endif