#include "graphics/vulkan/vulkan_graphics_backend.h"
#include "graphics/vulkan/vulkan_platform.h"
#include "graphics/vulkan/vulkan_context.h"

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

		vulkan_graphics_backend::vulkan_graphics_backend(const vulkan_graphics_backend_configuration& _config) noexcept :
			graphics_backend(graphics_backend_framework::Vulkan),
			m_config(_config) {
		}

		b8 vulkan_graphics_backend::initialize(window& _window) noexcept {
			// TODO: custom allocator
			context.allocator = 0;

			// Setup Vulkan instance
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

			return true;
		}

		b8 vulkan_graphics_backend::terminate(window& _window) noexcept {
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
