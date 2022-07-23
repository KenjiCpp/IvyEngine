#pragma once

#include <vulkan/vulkan.h>

namespace ivy {
	namespace graphics {

        struct vulkan_context;

		struct vulkan_image {
            b8 initialize(vulkan_context& _ctx, u32 _width, u32 _height, b8 _create_view, VkFormat _format, VkImageType _image_type, VkImageTiling _tiling, VkImageUsageFlags _usage, VkMemoryPropertyFlags _memory, VkImageAspectFlags _view_aspect) noexcept;
            b8 terminate(vulkan_context& _ctx);

            b8 create_view(vulkan_context& _ctx, VkFormat _format, VkImageAspectFlags _view_aspect) noexcept;

            VkImage        handle;
            VkDeviceMemory memory;
            VkImageView    view;
            u32            width;
            u32            height;
		};

	} // namespace graphics
} // namespace ivy
