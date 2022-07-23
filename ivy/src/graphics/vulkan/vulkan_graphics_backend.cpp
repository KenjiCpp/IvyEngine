#include "graphics/vulkan/vulkan_graphics_backend.h"
#include "graphics/vulkan/vulkan_platform.h"
#include "graphics/vulkan/vulkan_context.h"
#include "graphics/vulkan/vulkan_renderpass.h"
#include "graphics/vulkan/vulkan_command_buffer.h"
#include "graphics/vulkan/vulkan_framebuffer.h"

#include "utility/logger.h"

#include <vulkan/vulkan.h>

namespace ivy {
	namespace graphics {

		static vulkan_context context;
		
		VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT message_types, const VkDebugUtilsMessengerCallbackDataEXT* callback_data, void* user_data) {
			switch (message_severity) {
			default:
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
				IVY_TRACE("Vulkan error: {}", string8(callback_data->pMessage));
				break;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
				IVY_TRACE("Vulkan warning: {}", string8(callback_data->pMessage));
				break;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
				IVY_TRACE("Vulkan info: {}", string8(callback_data->pMessage));
				break;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
				IVY_TRACE("Vulkan debug: {}", string8(callback_data->pMessage));
				break;
			}
			return VK_FALSE;
		}

		b8 create_graphics_command_buffers() noexcept {
			if (context.graphics_command_buffers.empty()) {
				context.graphics_command_buffers.reserve(context.swapchain.image_count);
				vulkan_command_buffer default_buffer = { };
				for (u32 i = 0; i < context.swapchain.image_count; ++i) {
					context.graphics_command_buffers.push_back(default_buffer);
				}
			}

			for (u32 i = 0; i < context.swapchain.image_count; ++i) {
				if (context.graphics_command_buffers[i].handle) {
					context.graphics_command_buffers[i].free(context, context.device.graphics_command_pool);
					context.graphics_command_buffers[i] = { };
					context.graphics_command_buffers[i].allocate(context, context.device.graphics_command_pool, true);
				}
			}

			return true;
		}

		b8 regenerate_framebuffers(vulkan_swapchain& _swapchain, vulkan_renderpass& _renderpass) noexcept {
			if (context.swapchain.framebuffers.empty()) {
				vulkan_framebuffer default_buffer = { };
				for (u32 i = 0; i < _swapchain.image_count; ++i) {
					context.swapchain.framebuffers.push_back(default_buffer);
				}
			}

			for (u32 i = 0; i < _swapchain.image_count; ++i) {
				u32 attachment_count = 2;
				VkImageView attachments[] = { _swapchain.views[i], _swapchain.depth_attachment.view };

				context.swapchain.framebuffers[i].initialize(context, _renderpass, context.framebuffer_width, context.framebuffer_height, attachment_count, attachments);
			}

			return true;
		}

		vulkan_graphics_backend::vulkan_graphics_backend(const vulkan_graphics_backend_configuration& _config) noexcept :
			graphics_backend(graphics_backend_framework::Vulkan),
			m_config(_config) {
		}

		b8 vulkan_graphics_backend::initialize(window& _window) noexcept {
			// TODO: custom allocator
			context.allocator = 0;

			context.framebuffer_width  = _window.width();
			context.framebuffer_height = _window.height();

			VkApplicationInfo app_info = { VK_STRUCTURE_TYPE_APPLICATION_INFO };
			app_info.apiVersion         = VK_API_VERSION_1_3;
			app_info.pApplicationName   = m_config.application_name;
			app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			app_info.pEngineName        = "Ivy";
			app_info.engineVersion      = VK_MAKE_VERSION(1, 0, 0);

			array<const c8*> extensions = vulkan_platform_required_extensions();
			array<const c8*> layers;

			extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
#if defined(DEBUG)
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
			IVY_DEBUG("Required extension(s):");
			for (auto ext : extensions) {
				IVY_DEBUG(" - {}", string8(ext));
			}

			layers.push_back("VK_LAYER_KHRONOS_validation");
			IVY_DEBUG("Required validating layer(s):");

			u32 available_layer_count = 0;
			if (vkEnumerateInstanceLayerProperties(&available_layer_count, 0) != VK_SUCCESS) {
				return false;
			}
			VkLayerProperties* available_layers = (VkLayerProperties*)memory::allocate(sizeof(VkLayerProperties) * available_layer_count);
			if (vkEnumerateInstanceLayerProperties(&available_layer_count, available_layers) != VK_SUCCESS) {
				return false;
			}

			for (auto layer : layers) {
				b8 found = false;
				for (i32 i = 0; i < available_layer_count; ++i) {
					if (string8(layer) == available_layers[i].layerName) {
						found = true;
						break;
					}
				}

				IVY_DEBUG(" - {} ({})", string8(layer), string8(found ? "available" : "not available"));
				if (!found) {
					return false;
				}
			}
			memory::deallocate(available_layers);
#endif
			
			VkInstanceCreateInfo create_info = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
			create_info.pApplicationInfo        = &app_info;
			create_info.enabledExtensionCount   = extensions.size();
			create_info.ppEnabledExtensionNames = extensions.data();
			create_info.enabledLayerCount       = layers.size();
			create_info.ppEnabledLayerNames     = layers.data();

			VkResult result = vkCreateInstance(&create_info, context.allocator, &context.instance);
			if (result != VK_SUCCESS) {
				return false;
			}
			IVY_DEBUG("Vulkan instance created");

#if defined(DEBUG)
			VkDebugUtilsMessengerCreateInfoEXT debug_create_info = { VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT };
			debug_create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
			debug_create_info.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
			debug_create_info.pfnUserCallback = vk_debug_callback;

			PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(context.instance, "vkCreateDebugUtilsMessengerEXT");
			if (!func) {
				return false;
			}
			if (func(context.instance, &debug_create_info, context.allocator, &context.debug_messenger) != VK_SUCCESS) {
				return false;
			}
			IVY_DEBUG("Vulkan debug callback set");
#endif
			if (!vulkan_platform_create_surface(context, _window)) {
				return false;
			}
			IVY_DEBUG("Vulkan surface created");

			if (!vulkan_device_initialize(context)) {
				return false;
			}
			IVY_DEBUG("Vulkan device initialized");

			if (!context.swapchain.initialize(context, context.framebuffer_width, context.framebuffer_height, false)) {
				return false;
			}
			IVY_DEBUG("Vulkan swapchain initialized");

			if (!context.main_renderpass.initialize(context, rectangle_f(0.0f, 0.0f, context.framebuffer_width, context.framebuffer_height), vector4_f(0.0f, 0.0f, 0.2f, 1.0f), 1.0f, 0)) {
				return false;
			}
			IVY_DEBUG("Vulkan main renderpass initialized");

			if (!regenerate_framebuffers(context.swapchain, context.main_renderpass)) {
				return false;
			}
			IVY_DEBUG("Vulkan swapchain's framebuffers initialized");

			if (!create_graphics_command_buffers()) {
				return false;
			}
			IVY_DEBUG("Vulkan graphics command buffers initialized");

			return true;
		}

		b8 vulkan_graphics_backend::terminate(window& _window) noexcept {
			for (u32 i = 0; i < context.swapchain.image_count; ++i) {
				if (context.graphics_command_buffers[i].handle) {
					context.graphics_command_buffers[i].free(context, context.device.graphics_command_pool);
					context.graphics_command_buffers[i].handle = 0;
				}
			}
			context.graphics_command_buffers.~array();

			for (u32 i = 0; i < context.swapchain.image_count; ++i) {
				context.swapchain.framebuffers[i].terminate(context);
			}
			context.swapchain.framebuffers.~array();

			context.main_renderpass.terminate(context);
			context.swapchain.terminate(context);
			vulkan_device_terminate(context);
			vulkan_platform_remove_surface(context, _window);

#if defined(_DEBUG)
			if (context.debug_messenger) {
				PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(context.instance, "vkDestroyDebugUtilsMessengerEXT");
				func(context.instance, context.debug_messenger, context.allocator);
			}
#endif

			vkDestroyInstance(context.instance, context.allocator);

			return true;
		}

		b8 vulkan_graphics_backend::begin_frame(f32 _dt) noexcept {
			return true;
		}

		b8 vulkan_graphics_backend::end_frame(f32 _dt) noexcept {
			return true;
		}

	} // namespace graphics
} // namespace ivy
