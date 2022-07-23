#include "graphics/vulkan/vulkan_command_buffer.h" 
#include "graphics/vulkan/vulkan_context.h" 

namespace ivy {
	namespace graphics {
		b8 vulkan_command_buffer::allocate(vulkan_context& _ctx, VkCommandPool _command_pool, b8 _is_primary) noexcept {
			state = vulkan_command_buffer_state::NotAllocated;

			VkCommandBufferAllocateInfo allocate_info = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
			allocate_info.commandPool        = _command_pool;
			allocate_info.level              = _is_primary ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY;
			allocate_info.commandBufferCount = 1;
			allocate_info.pNext              = 0;

			if (vkAllocateCommandBuffers(_ctx.device.logical_device, &allocate_info, &handle) != VK_SUCCESS) {
				return false;
			}
			state  = vulkan_command_buffer_state::Ready;

			return true;
		}
		
		b8 vulkan_command_buffer::free(vulkan_context& _ctx, VkCommandPool _command_pool) noexcept {
			vkFreeCommandBuffers(_ctx.device.logical_device, _command_pool, 1, &handle);

			handle = 0;
			state  = vulkan_command_buffer_state::NotAllocated;

			return true;
		}

		b8 vulkan_command_buffer::begin(b8 _is_single_use, b8 _is_renderpass_continue, b8 _is_simultaneous_use) noexcept {
			VkCommandBufferBeginInfo begin_info = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
			begin_info.flags = 0;
			if (_is_single_use) {
				begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
			}
			if (_is_renderpass_continue) {
				begin_info.flags |= VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
			}
			if (_is_simultaneous_use) {
				begin_info.flags |= VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
			}

			if (vkBeginCommandBuffer(handle, &begin_info) != VK_SUCCESS) {
				return false;
			}
			state = vulkan_command_buffer_state::Recording;

			return true;
		}
		
		b8 vulkan_command_buffer::end() noexcept {
			if (vkEndCommandBuffer(handle) != VK_SUCCESS) {
				return false;
			}
			state = vulkan_command_buffer_state::RecordingEnded;

			return true;
		}
		
		b8 vulkan_command_buffer::update_submitted() noexcept {
			state = vulkan_command_buffer_state::Submitted;

			return true;
		}
		
		b8 vulkan_command_buffer::reset() noexcept {
			state = vulkan_command_buffer_state::Ready;

			return true;
		}

		b8 vulkan_command_buffer::begin_single_use(vulkan_context& _ctx, VkCommandPool _command_pool) noexcept {
			if (!allocate(_ctx, _command_pool, true)) {
				return false;
			}
			return begin(true, false, false);
		}
		
		b8 vulkan_command_buffer::end_single_use(vulkan_context& _ctx, VkCommandPool _command_pool, VkQueue _queue) noexcept {
			if (!end()) {
				return false;
			}

			VkSubmitInfo submit_info = { VK_STRUCTURE_TYPE_SUBMIT_INFO };
			submit_info.commandBufferCount = 1;
			submit_info.pCommandBuffers    = &handle;
			
			if (vkQueueSubmit(_queue, 1, &submit_info, 0) != VK_SUCCESS) {
				return false;
			}

			if (vkQueueWaitIdle(_queue) != VK_SUCCESS) {
				return false;
			}

			return free(_ctx, _command_pool);
		}

	} // namespace graphics
} // namespace ivy
