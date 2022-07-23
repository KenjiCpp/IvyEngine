#include "graphics/vulkan/vulkan_renderpass.h"
#include "graphics/vulkan/vulkan_context.h"

namespace ivy {
	namespace graphics {

		b8 vulkan_renderpass::initialize(vulkan_context& _ctx, const rectangle_f& _area, const vector4_f& _color, f32 _depth, u32 _stencil) noexcept {
			color   = _color;
			depth   = _depth;
			stencil = _stencil;
			state   = vulkan_renderpass_state::Ready;

			VkSubpassDescription subpass = { };
			subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

			// TODO: make this configurable
			constexpr u32 attachment_descrition_count = 2;
			VkAttachmentDescription attachment_descriptions[attachment_descrition_count] = { };

			VkAttachmentDescription color_attachment = { };
			color_attachment.format         = _ctx.swapchain.image_format.format; // TODO: make this configurable
			color_attachment.samples        = VK_SAMPLE_COUNT_1_BIT;
			color_attachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
			color_attachment.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
			color_attachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			color_attachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
			color_attachment.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			color_attachment.flags          = 0;

			attachment_descriptions[0] = color_attachment;

			VkAttachmentReference color_attachment_reference = { };
			color_attachment_reference.attachment = 0;
			color_attachment_reference.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			subpass.colorAttachmentCount = 1;
			subpass.pColorAttachments    = &color_attachment_reference;

			VkAttachmentDescription depth_attachment = { };
			depth_attachment.format         = _ctx.device.depth_format;
			depth_attachment.samples        = VK_SAMPLE_COUNT_1_BIT;
			depth_attachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
			depth_attachment.storeOp        = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			depth_attachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			depth_attachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
			depth_attachment.finalLayout    = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			depth_attachment.flags          = 0;

			attachment_descriptions[1] = depth_attachment;

			VkAttachmentReference depth_attachment_reference;
			depth_attachment_reference.attachment = 1;
			depth_attachment_reference.layout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

			subpass.pDepthStencilAttachment = &depth_attachment_reference;

			// TODO: other attachments
			subpass.inputAttachmentCount = 0;
			subpass.pInputAttachments    = 0;

			subpass.pResolveAttachments = 0;

			subpass.preserveAttachmentCount = 0;
			subpass.pPreserveAttachments    = 0;

			VkSubpassDependency dependency = { };
			dependency.srcSubpass      = VK_SUBPASS_EXTERNAL;
			dependency.dstSubpass      = 0;
			dependency.srcStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependency.dstStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependency.srcAccessMask   = 0;
			dependency.dstAccessMask   = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			dependency.dependencyFlags = 0;

			VkRenderPassCreateInfo create_info = { VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO };
			create_info.attachmentCount = attachment_descrition_count;
			create_info.pAttachments    = attachment_descriptions;
			create_info.subpassCount    = 1;
			create_info.pSubpasses      = &subpass;
			create_info.dependencyCount = 1;
			create_info.pDependencies   = &dependency;
			create_info.pNext           = 0;
			create_info.flags           = 0;

			if (vkCreateRenderPass(_ctx.device.logical_device, &create_info, _ctx.allocator, &handle) != VK_SUCCESS) {
				return false;
			}

			return true;
		}
		
		b8 vulkan_renderpass::terminate(vulkan_context& _ctx) noexcept {
			if (handle) {
				vkDestroyRenderPass(_ctx.device.logical_device, handle, _ctx.allocator);
				handle = 0;
			}

			return true;
		}

		b8 vulkan_renderpass::begin(vulkan_command_buffer& _command_buffer, VkFramebuffer _frame_buffer) noexcept {
			VkClearValue clear_values[2] = { };
			clear_values[0].color.float32[0]     = color.x;
			clear_values[0].color.float32[1]     = color.y;
			clear_values[0].color.float32[2]     = color.z;
			clear_values[0].color.float32[3]     = color.w;
			clear_values[1].depthStencil.depth   = depth;
			clear_values[1].depthStencil.stencil = stencil;

			VkRenderPassBeginInfo begin_info = { VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };
			begin_info.renderPass               = handle;
			begin_info.framebuffer              = _frame_buffer;
			begin_info.renderArea.offset.x      = area.x;
			begin_info.renderArea.offset.y      = area.y;
			begin_info.renderArea.extent.width  = area.w;
			begin_info.renderArea.extent.height = area.h;
			begin_info.clearValueCount          = 2;
			begin_info.pClearValues             = clear_values;

			vkCmdBeginRenderPass(_command_buffer.handle, &begin_info, VK_SUBPASS_CONTENTS_INLINE);
			_command_buffer.state = vulkan_command_buffer_state::InRenderpass;

			return true;
		}
		
		b8 vulkan_renderpass::end(vulkan_command_buffer& _command_buffer) noexcept {
			vkCmdEndRenderPass(_command_buffer.handle);
			_command_buffer.state = vulkan_command_buffer_state::Recording;

			return true;
		}

	} // namespace graphics
} // namespace ivy
