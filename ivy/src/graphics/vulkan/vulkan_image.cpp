#include "graphics/vulkan/vulkan_image.h"
#include "graphics/vulkan/vulkan_context.h"
#include "graphics/vulkan/vulkan_device.h"

#include "utility/logger.h"

#include "platform/memory.h"

namespace ivy {
	namespace graphics {

		b8 vulkan_image::initialize(vulkan_context& _ctx, u32 _width, u32 _height, b8 _create_view, VkFormat _format, VkImageType _image_type, VkImageTiling _tiling, VkImageUsageFlags _usage, VkMemoryPropertyFlags _memory, VkImageAspectFlags _view_aspect) noexcept {
			width = _width;
			height = _height;

			VkImageCreateInfo create_info = { VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };
			create_info.imageType     = VK_IMAGE_TYPE_2D;                           // TODO: Support cubemap, 3d, ...
			create_info.extent.width  = width;
			create_info.extent.height = height;
			create_info.extent.depth  = 1;											// TODO: Support configurable depth.
			create_info.mipLevels     = 4;											// TODO: Support mip mapping
			create_info.arrayLayers   = 1; //_type == TEXTURE_TYPE_CUBE ? 6 : 1;	// TODO: Support number of layers in the image.
			create_info.format        = _format;
			create_info.tiling        = _tiling;
			create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			create_info.usage         = _usage;
			create_info.samples       = VK_SAMPLE_COUNT_1_BIT;						// TODO: Configurable sample count.
			create_info.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;					// TODO: Configurable sharing mode.

			if (vkCreateImage(_ctx.device.logical_device, &create_info, _ctx.allocator, &handle) != VK_SUCCESS) {
				return false;
			}
		
			VkMemoryRequirements memory_requirements;
			vkGetImageMemoryRequirements(_ctx.device.logical_device, handle, &memory_requirements);

			i32 memory_type = _ctx.find_memory_index(memory_requirements.memoryTypeBits, _memory);
			if (memory_type == -1) {
				IVY_ERROR("Required memory type not found, image not valid");
				return false;
			}

			VkMemoryAllocateInfo memory_allocate_info = { VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO };
			memory_allocate_info.allocationSize  = memory_requirements.size;
			memory_allocate_info.memoryTypeIndex = memory_type;

			if (vkAllocateMemory(_ctx.device.logical_device, &memory_allocate_info, _ctx.allocator, &memory) != VK_SUCCESS) {
				return false;
			}
			if (vkBindImageMemory(_ctx.device.logical_device, handle, memory, 0) != VK_SUCCESS) {
				return false;
			}

			if (_create_view) {
				view = 0;
				if (!create_view(_ctx, _format, _view_aspect)) {
					return false;
				}
			}

			return true;
		}
		
		b8 vulkan_image::terminate(vulkan_context& _ctx) {
			if (view) {
				vkDestroyImageView(_ctx.device.logical_device, view, _ctx.allocator);
				view = 0;
			}
			if (memory) {
				vkFreeMemory(_ctx.device.logical_device, memory, _ctx.allocator);
				memory = 0;
			}
			if (handle) {
				vkDestroyImage(_ctx.device.logical_device, handle, _ctx.allocator);
				handle = 0;
			}

			return true;
		}

		b8 vulkan_image::create_view(vulkan_context& _ctx, VkFormat _format, VkImageAspectFlags _view_aspect) noexcept {
			VkImageViewCreateInfo create_info = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
			create_info.image                           = handle;
			create_info.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
			create_info.format                          = _format;
			create_info.subresourceRange.aspectMask     = _view_aspect;
			create_info.subresourceRange.baseMipLevel   = 0;
			create_info.subresourceRange.levelCount     = 1;
			create_info.subresourceRange.baseArrayLayer = 0;
			create_info.subresourceRange.layerCount     = 1;

			if (vkCreateImageView(_ctx.device.logical_device, &create_info, _ctx.allocator, &view) != VK_SUCCESS) {
				return false;
			}

			return true;
		}

	} // namespace graphics
} // namespace ivy
