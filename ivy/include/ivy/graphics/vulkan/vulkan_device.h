#pragma once

#include <vulkan/vulkan.h>

#include "vulkan_swapchain.h"

namespace ivy {
	namespace graphics {

		struct vulkan_context;

		struct vulkan_device {
			b8 detect_depth_format() noexcept;

			VkPhysicalDevice                 physical_device;
			VkDevice                         logical_device;
			vulkan_swapchain_support_info    swapchain_support;
			i32                              graphics_queue_index;
			i32                              present_queue_index;
			i32                              transfer_queue_index;
			b8                               supports_device_local_host_visible;
			VkPhysicalDeviceProperties       properties;
			VkPhysicalDeviceFeatures         features;
			VkPhysicalDeviceMemoryProperties memory;
			VkQueue							 graphics_queue;
			VkQueue							 present_queue;
			VkQueue							 transfer_queue;
			VkCommandPool                    graphics_command_pool;
			VkFormat                         depth_format;
			u8                               depth_channel_count;
		};
		
		b8 vulkan_device_initialize(vulkan_context& _ctx) noexcept;
		b8 vulkan_device_terminate(vulkan_context& _ctx) noexcept;

		b8 vulkan_device_query_swapchain_support(VkPhysicalDevice _device, VkSurfaceKHR _surface, vulkan_swapchain_support_info& _swapchain_support) noexcept;

	} // namespace graphics
} // namespace ivy

