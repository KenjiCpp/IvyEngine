#pragma once

#include <vulkan/vulkan.h>

namespace ivy {
	namespace graphics {

		struct vulkan_swapchain_support_info {
            VkSurfaceCapabilitiesKHR capabilities;
            u32                      format_count;
            VkSurfaceFormatKHR*      formats;
            u32                      present_mode_count;
            VkPresentModeKHR*        present_modes;
		};

	} // namespace graphics
} //namespace ivy
