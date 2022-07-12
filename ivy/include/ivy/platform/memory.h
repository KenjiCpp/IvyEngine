#pragma once

namespace ivy {
	namespace platform {
		
		namespace details {

			class memory_impl {
			public:
				~memory_impl() noexcept;

				static ptr allocate(u64 _size) noexcept;
				static b8 deallocate(ptr _memory) noexcept;
				static u64 total() noexcept;

				static b8 copy(cptr _source, ptr _dest, u64 _size) noexcept;

			private:
				memory_impl() noexcept;

			private:
				u64 m_total = 0;

				static memory_impl s_instance;
			};

		} // namespace details

		class IVY_API memory {
		public:
			~memory() noexcept = default;

			static ptr allocate(u64 _size) noexcept;
			static b8 deallocate(ptr _memory) noexcept;
			static u64 total() noexcept;

			static b8 copy(cptr _source, ptr _dest, u64 _size) noexcept;

		private:
			memory() noexcept = default;
		};

	} // namespace platform
} // namespace ivy
