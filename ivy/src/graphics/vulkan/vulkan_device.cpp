#include "graphics/vulkan/vulkan_device.h"
#include "graphics/vulkan/vulkan_context.h"
#include "graphics/vulkan/vulkan_swapchain.h"

#include "platform/memory.h"

#include "utility/array.h"
#include "utility/logger.h"

namespace ivy {
	namespace graphics {

		struct vulkan_physical_device_requirements {
			b8               graphics;
			b8               present;
			b8               compute;
			b8               transfer;
			b8               sampler_anisotropy;
			b8               discrete_gpu;
			array<const c8*> device_extension_names;
		};

		struct vulkan_physical_device_queue_family_info {
			u32 graphics_family_index;
			u32 present_family_index;
			u32 compute_family_index;
			u32 transfer_family_index;
		};

		b8 physical_device_meets_requirements(VkPhysicalDevice _device, VkSurfaceKHR _surface, const VkPhysicalDeviceProperties& _properties, const VkPhysicalDeviceFeatures _features, const vulkan_physical_device_requirements& _requirements, vulkan_physical_device_queue_family_info& _queue_info, vulkan_swapchain_support_info& _swapchain_support) noexcept {
			_queue_info.graphics_family_index = -1;
			_queue_info.present_family_index  = -1;
			_queue_info.compute_family_index  = -1;
			_queue_info.transfer_family_index = -1;

			if (_requirements.discrete_gpu) {
				if (_properties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
					return false;
				}
			}

			u32 queue_family_count = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(_device, &queue_family_count, 0);
			VkQueueFamilyProperties queue_families[32];
			vkGetPhysicalDeviceQueueFamilyProperties(_device, &queue_family_count, queue_families);
			
			IVY_DEBUG("| Graphics | Present  | Compute  | Transfer | Name");
			u8 min_transfer_score = 255;
			for (u32 i = 0; i < queue_family_count; ++i) {
				u8 current_transfer_score = 0;

				if (_queue_info.graphics_family_index == -1 && queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
					_queue_info.graphics_family_index = i;
					++current_transfer_score;

					VkBool32 supports_present = VK_FALSE;
					if (vkGetPhysicalDeviceSurfaceSupportKHR(_device, i, _surface, &supports_present) != VK_SUCCESS) {
						return false;
					}
					if (supports_present) {
						_queue_info.present_family_index = i;
						++current_transfer_score;
					}
				}

				if (queue_families[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
					_queue_info.compute_family_index = i;
					++current_transfer_score;
				}

				if (queue_families[i].queueFlags & VK_QUEUE_TRANSFER_BIT) {
					if (current_transfer_score <= min_transfer_score) {
						min_transfer_score = current_transfer_score;
						_queue_info.transfer_family_index = i;
					}
				}
			}

			if (_queue_info.present_family_index == -1) {
				for (u32 i = 0; i < queue_family_count; ++i) {
					VkBool32 supports_present = VK_FALSE;
					if (vkGetPhysicalDeviceSurfaceSupportKHR(_device, i, _surface, &supports_present) != VK_SUCCESS) {
						return false;
					}
					if (supports_present) {
						_queue_info.present_family_index = i;
						if (_queue_info.present_family_index != _queue_info.graphics_family_index) {
							IVY_WARN("Different queue index used for present vs graphics: {}", i);
						}
						break;
					}
				}
			}

			IVY_DEBUG("| {8=} | {8=} | {8=} | {8=} | {}", _queue_info.graphics_family_index != -1, _queue_info.present_family_index != -1, _queue_info.compute_family_index != -1, _queue_info.transfer_family_index != -1, _properties.deviceName);

			vulkan_device_query_swapchain_support(_device, _surface, _swapchain_support);
		
			if (_swapchain_support.format_count < 1 || _swapchain_support.present_mode_count < 1) {
				if (_swapchain_support.formats) {
					memory::deallocate(_swapchain_support.formats);
					_swapchain_support.formats = nullptr;
				}
				if (_swapchain_support.present_modes) {
					memory::deallocate(_swapchain_support.present_modes);
					_swapchain_support.present_modes = nullptr;
				}
				return false;
			}

			if (!_requirements.device_extension_names.empty()) {
				u32                    available_extension_count = 0;
				VkExtensionProperties* available_extensions      = 0;
				if (vkEnumerateDeviceExtensionProperties(_device, 0, &available_extension_count, 0) != VK_SUCCESS) { 
					return false;
				}
				if (available_extension_count != 0) {
					available_extensions = (VkExtensionProperties*)memory::allocate(sizeof(VkExtensionProperties) * available_extension_count);
					if (vkEnumerateDeviceExtensionProperties(_device, 0, &available_extension_count, available_extensions)) {
						return false;
					}

					u32 required_extension_count = _requirements.device_extension_names.size();
					for (u32 i = 0; i < required_extension_count; ++i) {
						b8 found = false;
						for (u32 j = 0; j < available_extension_count; ++j) {
							if (string8(_requirements.device_extension_names[i]) == available_extensions[j].extensionName) {
								found = true;
								break;
							}
						}

						if (!found) {
							memory::deallocate(available_extensions);
							return false;
						}
					}
				}
				memory::deallocate(available_extensions);
			}

			if (_requirements.sampler_anisotropy && !_features.samplerAnisotropy) {
				return false;
			}

			return true;
		}

		b8 select_physical_device(vulkan_context& _ctx) noexcept {
			u32 physical_devices_count = 0;
			if (vkEnumeratePhysicalDevices(_ctx.instance, &physical_devices_count, 0) != VK_SUCCESS) {
				return false;
			}
			if (physical_devices_count) {
				VkPhysicalDevice* physical_devices = (VkPhysicalDevice*)memory::allocate(sizeof(VkPhysicalDevice) * physical_devices_count);
				if (!physical_devices) {
					return false;
				}

				if (vkEnumeratePhysicalDevices(_ctx.instance, &physical_devices_count, physical_devices) != VK_SUCCESS) {
					memory::deallocate(physical_devices);
					return false;
				}

				// TODO: make this configurable
				vulkan_physical_device_requirements requirements = { };
				requirements.graphics           = true;
				requirements.present            = true;
				requirements.compute            = true;
				requirements.transfer           = true;
				requirements.sampler_anisotropy = true;
				requirements.discrete_gpu       = true;
				requirements.device_extension_names.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

				for (u32 i = 0; i < physical_devices_count; ++i) {
					VkPhysicalDeviceProperties properties;
					vkGetPhysicalDeviceProperties(physical_devices[i], &properties);

					VkPhysicalDeviceFeatures features;
					vkGetPhysicalDeviceFeatures(physical_devices[i], &features);

					VkPhysicalDeviceMemoryProperties memory;
					vkGetPhysicalDeviceMemoryProperties(physical_devices[i], &memory);
				
					b8 supports_device_local_host_visible = false;
					for (u32 i = 0; i < memory.memoryTypeCount; ++i) {
						if (((memory.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0) && ((memory.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) != 0)) {
							supports_device_local_host_visible = true;
							break;
						}
					}

					vulkan_physical_device_queue_family_info queue_info = { };
					if (physical_device_meets_requirements(physical_devices[i], _ctx.surface, properties, features, requirements, queue_info, _ctx.device.swapchain_support)) {
						IVY_DEBUG("Selected device: {}", properties.deviceName);
						switch (properties.deviceType) {
						default:
						case VK_PHYSICAL_DEVICE_TYPE_OTHER:
							IVY_DEBUG(" - GPU type: Unknown.");
							break;
						case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
							IVY_DEBUG(" - GPU type: Integrated.");
							break;
						case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
							IVY_DEBUG(" - GPU type: Descrete.");
							break;
						case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
							IVY_DEBUG(" - GPU type: Virtual.");
							break;
						case VK_PHYSICAL_DEVICE_TYPE_CPU:
							IVY_DEBUG(" - GPU type: CPU.");
							break;
						}

						IVY_DEBUG(" - GPU Driver version: {}.{}.{}", VK_VERSION_MAJOR(properties.driverVersion), VK_VERSION_MINOR(properties.driverVersion), VK_VERSION_PATCH(properties.driverVersion));
						IVY_DEBUG(" - Vulkan API version: {}.{}.{}", VK_VERSION_MAJOR(properties.apiVersion), VK_VERSION_MINOR(properties.apiVersion), VK_VERSION_PATCH(properties.apiVersion));

						for (u32 j = 0; j < memory.memoryHeapCount; ++j) {
							f32 memory_size_gib = (((f32)memory.memoryHeaps[j].size) / 1024.0f / 1024.0f / 1024.0f);
							if (memory.memoryHeaps[j].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
								IVY_DEBUG(" - Local GPU memory: {.2} GiB", memory_size_gib);
							}
							else {
								IVY_DEBUG(" - Shared System memory: {.2} GiB", memory_size_gib);
							}
						}

						_ctx.device.physical_device                    = physical_devices[i];
						_ctx.device.graphics_queue_index               = queue_info.graphics_family_index;
						_ctx.device.present_queue_index                = queue_info.present_family_index;
						_ctx.device.transfer_queue_index               = queue_info.transfer_family_index;
						_ctx.device.properties                         = properties;
						_ctx.device.features                           = features;
						_ctx.device.memory                             = memory;
						_ctx.device.supports_device_local_host_visible = supports_device_local_host_visible;
						break;
					}
				}

				if (!_ctx.device.physical_device) {
					IVY_ERROR("No physical devices were found which meet the requirements");
					return false;
				}

				memory::deallocate(physical_devices);
				return true;
			}

			return false;
		}

		b8 vulkan_device_initialize(vulkan_context& _ctx) noexcept {
			if (!select_physical_device(_ctx)) {
				return false;
			}

			b8 present_shares_graphics_queue  = _ctx.device.graphics_queue_index == _ctx.device.present_queue_index;
			b8 transfer_shares_graphics_queue = _ctx.device.graphics_queue_index == _ctx.device.transfer_queue_index;
			u32 index_count = 1;
			if (!present_shares_graphics_queue) {
				index_count++;
			}
			if (!transfer_shares_graphics_queue) {
				index_count++;
			}
			u32 indices[32];
			u8 index = 0;
			indices[index++] = _ctx.device.graphics_queue_index;
			if (!present_shares_graphics_queue) {
				indices[index++] = _ctx.device.present_queue_index;
			}
			if (!transfer_shares_graphics_queue) {
				indices[index++] = _ctx.device.transfer_queue_index;
			}

			VkDeviceQueueCreateInfo queue_create_infos[32];
			for (u32 i = 0; i < index_count; ++i) {
				queue_create_infos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queue_create_infos[i].queueFamilyIndex = indices[i];
				queue_create_infos[i].queueCount = 1;

				// TODO: Enable this for a future enhancement.
				// if (indices[i] == _ctx.device.graphics_queue_index) {
				//     queue_create_infos[i].queueCount = 2;
				// }
				queue_create_infos[i].flags = 0;
				queue_create_infos[i].pNext = 0;
				f32 queue_priority = 1.0f;
				queue_create_infos[i].pQueuePriorities = &queue_priority;
			}

			// TODO: make this configurable
			VkPhysicalDeviceFeatures device_features = {};
			device_features.samplerAnisotropy = VK_TRUE;

			b8 portability_required = false;
			u32 available_extension_count = 0;
			VkExtensionProperties* available_extensions = 0;
			if (vkEnumerateDeviceExtensionProperties(_ctx.device.physical_device, 0, &available_extension_count, 0) != VK_SUCCESS) {
				return false;
			}
			if (available_extension_count != 0) {
				available_extensions = (VkExtensionProperties*)memory::allocate(sizeof(VkExtensionProperties) * available_extension_count);
				if (vkEnumerateDeviceExtensionProperties(_ctx.device.physical_device, 0, &available_extension_count, available_extensions) != VK_SUCCESS) {
					return false;
				}
				for (u32 i = 0; i < available_extension_count; ++i) {
					if (string8(available_extensions[i].extensionName) == "VK_KHR_portability_subset") {
						IVY_DEBUG("Adding required extension 'VK_KHR_portability_subset'");
						portability_required = true;
						break;
					}
				}
			}
			memory::deallocate(available_extensions);

			array<const c8*> extension_names;
			if (portability_required) {
				extension_names = { VK_KHR_SWAPCHAIN_EXTENSION_NAME, "VK_KHR_portability_subset" };
			}
			else {
				extension_names = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
			}
			VkDeviceCreateInfo device_create_info = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
			device_create_info.queueCreateInfoCount    = index_count;
			device_create_info.pQueueCreateInfos       = queue_create_infos;
			device_create_info.pEnabledFeatures        = &device_features;
			device_create_info.enabledExtensionCount   = extension_names.size();
			device_create_info.ppEnabledExtensionNames = extension_names.data();
			device_create_info.enabledLayerCount       = 0;
			device_create_info.ppEnabledLayerNames     = 0;

			if (vkCreateDevice(_ctx.device.physical_device, &device_create_info, _ctx.allocator, &_ctx.device.logical_device) != VK_SUCCESS) {
				return false;
			}
			IVY_DEBUG("Logical device created");

			vkGetDeviceQueue(_ctx.device.logical_device, _ctx.device.graphics_queue_index, 0, &_ctx.device.graphics_queue);
			vkGetDeviceQueue(_ctx.device.logical_device, _ctx.device.present_queue_index, 0, &_ctx.device.present_queue);
			vkGetDeviceQueue(_ctx.device.logical_device, _ctx.device.transfer_queue_index, 0, &_ctx.device.transfer_queue);

			VkCommandPoolCreateInfo pool_create_info = { VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
			pool_create_info.queueFamilyIndex = _ctx.device.graphics_queue_index;
			pool_create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
			if (vkCreateCommandPool(_ctx.device.logical_device, &pool_create_info, _ctx.allocator, &_ctx.device.graphics_command_pool) != VK_SUCCESS) {
				return false;
			}

			return true;
		}

		b8 vulkan_device_terminate(vulkan_context& _ctx) noexcept {
			_ctx.device.graphics_queue = 0;
			_ctx.device.present_queue = 0;
			_ctx.device.transfer_queue = 0;

			vkDestroyCommandPool(_ctx.device.logical_device, _ctx.device.graphics_command_pool, _ctx.allocator);

			if (_ctx.device.logical_device) {
				vkDestroyDevice(_ctx.device.logical_device, _ctx.allocator);
				_ctx.device.logical_device = 0;
			}

			_ctx.device.physical_device = 0;

			if (_ctx.device.swapchain_support.formats) {
				memory::deallocate(_ctx.device.swapchain_support.formats);
				_ctx.device.swapchain_support.formats = nullptr;
				_ctx.device.swapchain_support.format_count = 0;
			}
			if (_ctx.device.swapchain_support.present_modes) {
				memory::deallocate(_ctx.device.swapchain_support.present_modes);
				_ctx.device.swapchain_support.present_modes = nullptr;
				_ctx.device.swapchain_support.present_mode_count = 0;
			}

			_ctx.device.graphics_queue_index = -1;
			_ctx.device.present_queue_index  = -1;
			_ctx.device.transfer_queue_index = -1;

			return true;
		}

		b8 vulkan_device_query_swapchain_support(VkPhysicalDevice _device, VkSurfaceKHR _surface, vulkan_swapchain_support_info& _swapchain_support) noexcept {
			if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_device, _surface, &_swapchain_support.capabilities) != VK_SUCCESS) {
				return false;
			}

			if (vkGetPhysicalDeviceSurfaceFormatsKHR(_device, _surface, &_swapchain_support.format_count, 0) != VK_SUCCESS) {
				return false;
			}

			if (_swapchain_support.format_count != 0) {
				if (!_swapchain_support.formats) {
					_swapchain_support.formats = (VkSurfaceFormatKHR*)memory::allocate(sizeof(VkSurfaceFormatKHR) * _swapchain_support.format_count);
				}
				if (vkGetPhysicalDeviceSurfaceFormatsKHR(_device, _surface, &_swapchain_support.format_count, _swapchain_support.formats) != VK_SUCCESS) {
					return false;
				}
			}

			if (vkGetPhysicalDeviceSurfacePresentModesKHR(_device, _surface, &_swapchain_support.present_mode_count, 0) != VK_SUCCESS) {
				return false;
			}
			if (_swapchain_support.present_mode_count != 0) {
				if (!_swapchain_support.present_modes) {
					_swapchain_support.present_modes = (VkPresentModeKHR*)memory::allocate(sizeof(VkPresentModeKHR) * _swapchain_support.present_mode_count);
				}
				if (vkGetPhysicalDeviceSurfacePresentModesKHR(_device, _surface, &_swapchain_support.present_mode_count, _swapchain_support.present_modes) != VK_SUCCESS) {
					return false;
				}
			}

			return true;
		}

	} // namespace graphics
} // namespace ivy
