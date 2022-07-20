#pragma once

#include "utility/array.h"

#include "platform/window.h"

#include "vulkan_context.h"

namespace ivy {
	namespace graphics {

		array<const c8*> vulkan_platform_required_extensions() noexcept;

		b8 vulkan_platform_create_surface(vulkan_context& _ctx, window& _window) noexcept;
		b8 vulkan_platform_remove_surface(vulkan_context& _ctx, window& _window) noexcept;

	} // namespace graphics
} // namespace ivy
