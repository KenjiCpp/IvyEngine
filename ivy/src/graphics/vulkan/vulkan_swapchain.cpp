#include "graphics/vulkan/vulkan_swapchain.h"
#include "graphics/vulkan/vulkan_context.h"
#include "graphics/vulkan/vulkan_device.h"

#include "utility/logger.h"

#include "platform/memory.h"

namespace ivy {
	namespace graphics {

		b8 swapchain_create(vulkan_context& _ctx, u32 _width, u32 _height, vulkan_swapchain& _swapchain) noexcept {
			VkExtent2D swapchain_extent = { _width, _height };
			_swapchain.max_frames_in_flight = 2;

			b8 found = false;
			for (u32 i = 0; i < _ctx.device.swapchain_support.format_count; ++i) {
				VkSurfaceFormatKHR format = _ctx.device.swapchain_support.formats[i];
				if (format.format == VK_FORMAT_B8G8R8A8_UNORM &&
					format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
					_swapchain.image_format = format;
					found = true;
					break;
				}
			}
			if (!found) {
				_swapchain.image_format = _ctx.device.swapchain_support.formats[0];
			}

			VkPresentModeKHR present_mode = VK_PRESENT_MODE_FIFO_KHR;
			for (u32 i = 0; i < _ctx.device.swapchain_support.present_mode_count; ++i) {
				VkPresentModeKHR mode = _ctx.device.swapchain_support.present_modes[i];
				if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
					present_mode = mode;
					break;
				}
			}

			vulkan_device_query_swapchain_support(_ctx.device.physical_device, _ctx.surface, _ctx.device.swapchain_support);
			
			if (_ctx.device.swapchain_support.capabilities.currentExtent.width != UINT32_MAX) {
				swapchain_extent = _ctx.device.swapchain_support.capabilities.currentExtent;
			}
		
			VkExtent2D min = _ctx.device.swapchain_support.capabilities.minImageExtent;
			VkExtent2D max = _ctx.device.swapchain_support.capabilities.maxImageExtent;
			swapchain_extent.width  = swapchain_extent.width  < min.width  ? min.width  : swapchain_extent.width  > max.width  ? max.width  : swapchain_extent.width;
			swapchain_extent.height = swapchain_extent.height < min.height ? min.height : swapchain_extent.height > max.height ? max.height : swapchain_extent.height;

			u32 image_count = _ctx.device.swapchain_support.capabilities.minImageCount + 1;
			if (_ctx.device.swapchain_support.capabilities.maxImageCount > 0 && image_count > _ctx.device.swapchain_support.capabilities.maxImageCount) {
				image_count = _ctx.device.swapchain_support.capabilities.maxImageCount;
			}

			_swapchain.max_frames_in_flight = image_count - 1;

			VkSwapchainCreateInfoKHR swapchain_create_info = { VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
			swapchain_create_info.surface          = _ctx.surface;
			swapchain_create_info.minImageCount    = image_count;
			swapchain_create_info.imageFormat      = _swapchain.image_format.format;
			swapchain_create_info.imageColorSpace  = _swapchain.image_format.colorSpace;
			swapchain_create_info.imageExtent      = swapchain_extent;
			swapchain_create_info.imageArrayLayers = 1;
			swapchain_create_info.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

			if (_ctx.device.graphics_queue_index != _ctx.device.present_queue_index) {
				u32 queueFamilyIndices[] = { (u32)_ctx.device.graphics_queue_index, (u32)_ctx.device.present_queue_index };
				swapchain_create_info.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
				swapchain_create_info.queueFamilyIndexCount = 2;
				swapchain_create_info.pQueueFamilyIndices   = queueFamilyIndices;
			}
			else {
				swapchain_create_info.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
				swapchain_create_info.queueFamilyIndexCount = 0;
				swapchain_create_info.pQueueFamilyIndices   = 0;
			}

			swapchain_create_info.preTransform   = _ctx.device.swapchain_support.capabilities.currentTransform;
			swapchain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			swapchain_create_info.presentMode    = present_mode;
			swapchain_create_info.clipped        = VK_TRUE;
			swapchain_create_info.oldSwapchain   = 0;

			if (vkCreateSwapchainKHR(_ctx.device.logical_device, &swapchain_create_info, _ctx.allocator, &_swapchain.handle) != VK_SUCCESS) {
				return false;
			}
			_ctx.current_frame     = 0;
			_swapchain.image_count = 0;
			if (vkGetSwapchainImagesKHR(_ctx.device.logical_device, _swapchain.handle, &_swapchain.image_count, 0) != VK_SUCCESS) {
				return false;
			}
			if (!_swapchain.images) {
				_swapchain.images = (VkImage*)memory::allocate(sizeof(VkImage) * _swapchain.image_count);
			}
			if (!_swapchain.views) {
				_swapchain.views = (VkImageView*)memory::allocate(sizeof(VkImageView) * _swapchain.image_count);
			}
			if (vkGetSwapchainImagesKHR(_ctx.device.logical_device, _swapchain.handle, &_swapchain.image_count, _swapchain.images) != VK_SUCCESS) {
				memory::deallocate(_swapchain.images);
				memory::deallocate(_swapchain.views);
				return false;
			}

			for (u32 i = 0; i < _swapchain.image_count; ++i) {
				VkImageViewCreateInfo view_info = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
				view_info.image                           = _swapchain.images[i];
				view_info.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
				view_info.format                          = _swapchain.image_format.format;
				view_info.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
				view_info.subresourceRange.baseMipLevel   = 0;
				view_info.subresourceRange.levelCount     = 1;
				view_info.subresourceRange.baseArrayLayer = 0;
				view_info.subresourceRange.layerCount     = 1;

				if (vkCreateImageView(_ctx.device.logical_device, &view_info, _ctx.allocator, &_swapchain.views[i])) {
					return false;
				}
			}

			if (!_ctx.device.detect_depth_format()) {
				_ctx.device.depth_format = VK_FORMAT_UNDEFINED;
			}

			if (!_swapchain.depth_attachment.initialize(_ctx, swapchain_extent.width, swapchain_extent.height, true, _ctx.device.depth_format, VK_IMAGE_TYPE_2D, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_DEPTH_BIT)) {
				IVY_WARN("Failed to create depthattachment");
			}

			return true;
		}

		b8 swapchain_destroy(vulkan_context& _ctx, vulkan_swapchain& _swapchain) noexcept {
			_swapchain.depth_attachment.terminate(_ctx);

			for (u32 i = 0; i < _swapchain.image_count; ++i) {
				vkDestroyImageView(_ctx.device.logical_device, _swapchain.views[i], _ctx.allocator);
			}

			vkDestroySwapchainKHR(_ctx.device.logical_device, _swapchain.handle, _ctx.allocator);
			
			if (_swapchain.images) {
				memory::deallocate(_swapchain.images);
				_swapchain.images = nullptr;
			}
			if (_swapchain.views) {
				memory::deallocate(_swapchain.views);
				_swapchain.views = nullptr;
			}

			return true;
		}

		b8 vulkan_swapchain::initialize(vulkan_context& _ctx, u32 _width, u32 _height, b8 _recreate) noexcept {
			if (_recreate) {
				if (!swapchain_destroy(_ctx, *this)) {
					return false;
				}
			}
			return swapchain_create(_ctx, _width, _height, *this);
		}

		b8 vulkan_swapchain::terminate(vulkan_context& _ctx) noexcept {
			return swapchain_destroy(_ctx, *this);
		}

		b8 vulkan_swapchain::aqquire_next_image_index(vulkan_context& _ctx, u64 _timeout_ns, VkSemaphore _image_available_semaphore, VkFence _fence, u32& _image_index) noexcept {
			VkResult result = vkAcquireNextImageKHR(_ctx.device.logical_device, handle, _timeout_ns, _image_available_semaphore, _fence, &_image_index);
			
			if (result == VK_ERROR_OUT_OF_DATE_KHR) {
				initialize(_ctx, _ctx.framebuffer_width, _ctx.framebuffer_height, true);
				return false;
			}
			if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
				return false;
			}

			return true;
		}

		b8 vulkan_swapchain::present(vulkan_context& _ctx, VkQueue _graphics_queue, VkQueue _present_queue, VkSemaphore _render_complete_semaphore, u32 _present_image_index) noexcept {
			VkPresentInfoKHR present_info = { VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
			present_info.waitSemaphoreCount = 1;
			present_info.pWaitSemaphores    = &_render_complete_semaphore;
			present_info.swapchainCount     = 1;
			present_info.pSwapchains        = &handle;
			present_info.pImageIndices      = &_present_image_index;
			present_info.pResults           = 0;
			
			VkResult result = vkQueuePresentKHR(_present_queue, &present_info);
			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
				initialize(_ctx, _ctx.framebuffer_width, _ctx.framebuffer_height, true);
				IVY_DEBUG("Swapchain recreated because swapchain returned out of date or suboptimal.");
			}
			else if (result != VK_SUCCESS) {
				IVY_DEBUG("Failed to present swap chain image!");
			}
			_ctx.current_frame = (_ctx.current_frame + 1) % max_frames_in_flight;

			return true;
		}

	} // namespace graphics
} // namespace ivy
