#pragma once

#include <vulkan/vulkan.h>

#include "math/rectangle.h"
#include "math/vector.h"

#include "vulkan_command_buffer.h"

namespace ivy {
	namespace graphics {

		struct vulkan_context;

		enum class vulkan_renderpass_state : u8 {
			Ready, Recording, InRenderpass, RecordingEnded, Submitted, NotAllocated,
			Count
		};

		struct vulkan_renderpass {
			b8 initialize(vulkan_context& _ctx, const rectangle_f& _area, const vector4_f& _color, f32 _depth, u32 _stencil) noexcept;
			b8 terminate(vulkan_context& _ctx) noexcept;

			b8 begin(vulkan_command_buffer& _command_buffer, VkFramebuffer _frame_buffer) noexcept;
			b8 end(vulkan_command_buffer& _command_buffer) noexcept;

			VkRenderPass            handle;
			rectangle_f             area;
			vector4_f               color;
			f32                     depth;
			u32                     stencil;
			vulkan_renderpass_state state;
		};

	} // namespace graphics
} // namespace ivy
