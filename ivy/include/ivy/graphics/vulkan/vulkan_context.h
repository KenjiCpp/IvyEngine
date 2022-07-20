#pragma once
#pragma once

#include <vulkan/vulkan.h>

#include "vulkan_device.h"

namespace ivy {
	namespace graphics {

		struct vulkan_context {
			VkInstance               instance;
			VkAllocationCallbacks*   allocator;
			vulkan_device            device;

#if defined(DEBUG)
			VkDebugUtilsMessengerEXT debug_messenger;
#endif

			VkSurfaceKHR surface;
		};

	} // namespace graphics
} // namespace ivy

