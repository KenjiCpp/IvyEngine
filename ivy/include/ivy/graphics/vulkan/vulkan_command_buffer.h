#pragma once

#include <vulkan/vulkan.h>

namespace ivy {
	namespace graphics {

		enum class vulkan_command_buffer_state : u8 {
			Ready, Recording, InRenderpass, RecordingEnded, Submitted, NotAllocated,
			Count
		};

		struct vulkan_context;

		struct vulkan_command_buffer {
			b8 allocate(vulkan_context& _ctx, VkCommandPool _command_pool, b8 _is_primary) noexcept;
			b8 free(vulkan_context& _ctx, VkCommandPool _command_pool) noexcept;

			b8 begin(b8 _is_single_use, b8 _is_renderpass_continue, b8 _is_simultaneous_use) noexcept;
			b8 end() noexcept;
			b8 update_submitted() noexcept;
			b8 reset() noexcept;

			b8 begin_single_use(vulkan_context& _ctx, VkCommandPool _command_pool) noexcept;
			b8 end_single_use(vulkan_context& _ctx, VkCommandPool _command_pool, VkQueue _queue) noexcept;

			VkCommandBuffer             handle;
			vulkan_command_buffer_state state;
		};

	} // namespace graphics
} // namespace ivy
