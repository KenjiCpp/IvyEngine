#pragma once

#include <vulkan/vulkan.h>

namespace ivy {
	namespace graphics {

		struct vulkan_context;

		struct vulkan_fence {
			VkFence handle;
			b8 is_signaled;
		};

	} // namespace graphics
} // namespace ivy
