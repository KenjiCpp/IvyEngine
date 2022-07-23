#include "graphics/vulkan/vulkan_context.h"

namespace ivy {
	namespace graphics {

		i32 vulkan_context::find_memory_index(u32 _type_filter, u32 _property_flags) noexcept {
            VkPhysicalDeviceMemoryProperties memory_properties;
            vkGetPhysicalDeviceMemoryProperties(device.physical_device, &memory_properties);

            for (u32 i = 0; i < memory_properties.memoryTypeCount; ++i) {
                if (_type_filter & (1 << i) && (memory_properties.memoryTypes[i].propertyFlags & _property_flags) == _property_flags) {
                    return i;
                }
            }

            return -1;
		}

	} // namespace graphics
} // namespace ivy