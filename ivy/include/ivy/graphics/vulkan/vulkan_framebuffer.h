#pragma once

#include <vulkan/vulkan.h>

#include "vulkan_renderpass.h"

namespace ivy {
	namespace graphics {

		struct vulkan_context;

		struct vulkan_framebuffer {
			b8 initialize(vulkan_context& _ctx, vulkan_renderpass& _renderpass, u32 _width, u32 _height, u32 _attachment_count, VkImageView* _attachments) noexcept;
			b8 terminate(vulkan_context& _ctx) noexcept;

			VkFramebuffer      handle;
			vulkan_renderpass* renderpass;
			u32                attachment_count;
			VkImageView*       attachments;
		};

	} // namespace graphics
} // namespace ivy
