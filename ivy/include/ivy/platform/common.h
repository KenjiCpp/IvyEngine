#pragma once

namespace ivy {
	namespace platform {

		namespace details {

			class common_impl {
			public:
				~common_impl() noexcept;

				static const c8* platform_name() noexcept;
				static u32 hardware_concurrency() noexcept;

			private:
				common_impl() noexcept;

			private:
				static common_impl s_instance;
			};

		} // namespace details

		class IVY_API common {
		public:
			~common() noexcept = default;

			static const c8* platform_name() noexcept;
			static u32 hardware_concurrency() noexcept;

		private:
			common() noexcept = default;
		};

	} // namespace platform

	using platform::common;

} // namespace ivy
