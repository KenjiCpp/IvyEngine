#include "platform/common.h"
#include "platform/console.h"

#if defined(IVY_PLATFORM_WINDOWS)

#include <windows.h>

namespace ivy {
	namespace platform {

		namespace details {

			static u32 get_hardware_concurrency() noexcept {
				console::write(">>> PLATFORM: Obtaining hardware concurrency.\n", console::log_level::Trace);
				size_t concurrency = 0;
				DWORD length = 0;
				if (GetLogicalProcessorInformationEx(RelationAll, nullptr, &length) != FALSE) {
					return concurrency;
				}
				if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
					return concurrency;
				}
				std::unique_ptr<void, void(*)(void*)>buffer(std::malloc(length), std::free);
				if (!buffer) {
					return concurrency;
				}
				unsigned char* mem = reinterpret_cast<unsigned char*>(buffer.get());
				if (GetLogicalProcessorInformationEx(RelationAll, reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(mem), &length) == false) {
					return concurrency;
				}
				for (DWORD i = 0; i < length;) {
					auto* proc = reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(mem + i);
					if (proc->Relationship == RelationProcessorCore) {
						for (WORD group = 0; group < proc->Processor.GroupCount; ++group) {
							for (KAFFINITY mask = proc->Processor.GroupMask[group].Mask; mask != 0; mask >>= 1) {
								concurrency += mask & 1;
							}
						}
					}
					i += proc->Size;
				}
				return concurrency;
			}

			common_impl::common_impl() noexcept {
				console::write(">>> PLATFORM: Common initialized\n", console::log_level::Info);
			}

			common_impl::~common_impl() noexcept {
				console::write(">>> PLATFORM: Common terminated\n", console::log_level::Info);
			}

			const c8* common_impl::platform_name() noexcept {
				static const c8* os_name = "Windows";
				return os_name;
			}
			u32 common_impl::hardware_concurrency() noexcept {
				static const u32 concurrency = get_hardware_concurrency();
				return concurrency;
			}

		} // namespace details

		const c8* common::platform_name() noexcept {
			return details::common_impl::platform_name();
		}

		u32 common::hardware_concurrency() noexcept {
			return details::common_impl::hardware_concurrency();
		}
	
	} // namespace platform
} // namespace ivy

#endif