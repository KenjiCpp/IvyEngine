#pragma once

#include <vulkan/vulkan.h>

#include "vulkan_image.h"
#include "vulkan_framebuffer.h"

#include "utility/array.h"

namespace ivy {
	namespace graphics {

        struct vulkan_context;

		struct vulkan_swapchain_support_info {
            VkSurfaceCapabilitiesKHR capabilities;
            u32                      format_count;
            VkSurfaceFormatKHR*      formats;
            u32                      present_mode_count;
            VkPresentModeKHR*        present_modes;
		};

        struct vulkan_swapchain {
            b8 initialize(vulkan_context& _ctx, u32 _width, u32 _height, b8 _recreate) noexcept;
            b8 terminate(vulkan_context& _ctx) noexcept;

            b8 aqquire_next_image_index(vulkan_context& _ctx, u64 _timeout_ns, VkSemaphore _image_available_semaphore, VkFence _fence, u32& _image_index) noexcept;

            b8 present(vulkan_context& _ctx, VkQueue _graphics_queue, VkQueue _present_queue, VkSemaphore _render_complete_semaphore, u32 _present_image_index) noexcept;

            VkSurfaceFormatKHR        image_format;
            u8                        max_frames_in_flight;
            VkSwapchainKHR            handle;
            u32                       image_count;
            VkImage*                  images;
            VkImageView*              views;

            vulkan_image              depth_attachment;

            array<vulkan_framebuffer> framebuffers;
        };

	} // namespace graphics
} //namespace ivy
