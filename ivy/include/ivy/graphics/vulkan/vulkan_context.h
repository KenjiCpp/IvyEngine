#pragma once
#pragma once

#include <vulkan/vulkan.h>

#include "vulkan_device.h"
#include "vulkan_swapchain.h"
#include "vulkan_renderpass.h"
#include "vulkan_fence.h"
#include "vulkan_command_buffer.h"

#include "utility/array.h"

namespace ivy {
	namespace graphics {

		struct vulkan_context {
			i32 find_memory_index(u32 _type_filter, u32 _property_flags) noexcept;

			VkInstance                   instance;
			VkAllocationCallbacks*       allocator;
			vulkan_device                device;
			VkSurfaceKHR                 surface;
#if defined(DEBUG)
			VkDebugUtilsMessengerEXT     debug_messenger;
#endif
			u32                          framebuffer_width;
			u32                          framebuffer_height;
			u32                          current_frame;

			vulkan_swapchain             swapchain;
			vulkan_renderpass            main_renderpass;

			array<vulkan_command_buffer> graphics_command_buffers;

			array<VkSemaphore>           image_available_semaphores;
			array<VkSemaphore>           queue_complete_semaphores;

			u32                          in_flight_fence_count;
			vulkan_fence*                in_flight_fences;
			vulkan_fence**               images_in_flights;
		};

	} // namespace graphics
} // namespace ivy

