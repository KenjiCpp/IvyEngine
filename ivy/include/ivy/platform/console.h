#pragma once

namespace ivy {
	namespace platform {

		namespace details {

			class console_impl {
			public:
				~console_impl() noexcept;

				static i32 write(const c8* _message, u8 _level) noexcept;
				static i32 Error(const c8* _message, u8 _level) noexcept;

			private:
				console_impl() noexcept;

			private:
				static console_impl s_instance;
			};

		} // namespace details

		class IVY_API console {
		public:
			enum class log_level : u8 {
				Fatal, Error, Warn,
				Info,  Debug, Trace
			};

			static constexpr i32 failed = -1;

		public:
			~console() noexcept = default;

			static i32 write(const c8* _message, log_level _level = log_level::Info) noexcept;
			static i32 error(const c8* _message, log_level _level = log_level::Info) noexcept;

		private:
			console() noexcept = default;

		};

	} // namespace platform
} // namespace ivy
