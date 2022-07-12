#include "platform/memory.h"
#include "platform/console.h"

#include <cstdlib>

namespace ivy {
	namespace platform {

		namespace details {

			struct memory_block_header {
				ptr mark;
				u64 size;
			};

			memory_impl::memory_impl() noexcept {
				console::write(">>> PLATFORM: Memory initialized\n", console::log_level::Info);
			}

			memory_impl::~memory_impl() noexcept {
				if (s_instance.m_total) {
					console::write(">>> PLATFORM: Memory leaks detected\n", console::log_level::Warn);
				}
				console::write(">>> PLATFORM: Memory terminated\n", console::log_level::Info);
			}

			ptr memory_impl::allocate(u64 _size) noexcept {
				// If required size is 0, return nullptr
				if (!_size) {
					return nullptr;
				}

				// Allocate enough for the required amount and header
				ptr res = std::malloc(_size + sizeof(memory_block_header));

				// If success, construct header and return the actual usable block
				if (res) {
					s_instance.m_total += _size + sizeof(memory_block_header);
					((memory_block_header*)res)->mark = res;
					((memory_block_header*)res)->size = _size;
					return (u8*)res + sizeof(memory_block_header);
				}

				// If failed
				return nullptr;
			}

			b8 memory_impl::deallocate(ptr _memory) noexcept {
				if (_memory > (u8*)nullptr + sizeof(memory_block_header)) {

					// Get the actual pointer to the block
					ptr actual_block = (u8*)_memory - sizeof(memory_block_header);

					// Check if header is marked as valid
					if (((memory_block_header*)actual_block)->mark != actual_block) {
						return false;
					}

					// Unmark
					((memory_block_header*)actual_block)->mark = nullptr;
					s_instance.m_total -= ((memory_block_header*)actual_block)->size + sizeof(memory_block_header);

					// Deallocate
					std::free(actual_block);

					return true;
				}

				// Return false if pointer is invalid
				return false;
			}

			u64 memory_impl::total() noexcept {
				return s_instance.m_total;
			}

			b8 memory_impl::copy(cptr _source, ptr _dest, u64 _size) noexcept {
				std::memcpy(_dest, _source, _size);
				return true;
			}

		} // namespace details

		ptr memory::allocate(u64 _size) noexcept {
			return details::memory_impl::allocate(_size);
		}

		b8 memory::deallocate(ptr _memory) noexcept {
			return details::memory_impl::deallocate(_memory);
		}

		u64 memory::total() noexcept {
			return details::memory_impl::total();
		}

		b8 memory::copy(cptr _source, ptr _dest, u64 _size) noexcept {
			return details::memory_impl::copy(_source, _dest, _size);
		}

	} // namespace platform
} // namespace ivy
