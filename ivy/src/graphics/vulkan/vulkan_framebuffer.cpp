#include "graphics/vulkan/vulkan_framebuffer.h"
#include "graphics/vulkan/vulkan_context.h"

#include "platform/memory.h"

namespace ivy {
	namespace graphics {

		b8 vulkan_framebuffer::initialize(vulkan_context& _ctx, vulkan_renderpass& _renderpass, u32 _width, u32 _height, u32 _attachment_count, VkImageView* _attachments) noexcept {
			attachment_count = _attachment_count;
			renderpass       = &_renderpass;
			
			VkImageView* new_attachments = (VkImageView*)memory::allocate(sizeof(VkImageView) * attachment_count);
			if (!new_attachments) {
				return false;
			}
			for (u32 i = 0; i < attachment_count; ++i) {
				new_attachments[i] = _attachments[i];
			}
			attachments = new_attachments;

			VkFramebufferCreateInfo create_info = { VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };
			create_info.renderPass      = renderpass->handle;
			create_info.attachmentCount = attachment_count;
			create_info.pAttachments    = attachments;
			create_info.width           = _width;
			create_info.height          = _height;
			create_info.layers          = 1;

			if (vkCreateFramebuffer(_ctx.device.logical_device, &create_info, _ctx.allocator, &handle) != VK_SUCCESS) {
				memory::deallocate(attachments);
				attachments = nullptr;
				return false;
			}
		}
		
		b8 vulkan_framebuffer::terminate(vulkan_context& _ctx) noexcept {
			vkDestroyFramebuffer(_ctx.device.logical_device, handle, _ctx.allocator);
			if (attachments) {
				memory::deallocate(attachments);
				attachments = nullptr;
			}
			handle           = 0;
			attachment_count = 0;
			renderpass       = 0;

			return true;
		}

	} // namespace graphics
} // namespace ivy
