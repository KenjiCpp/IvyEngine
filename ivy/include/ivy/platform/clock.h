#pragma once

namespace ivy {
	namespace platform {

		class IVY_API clock {
		public:
			clock() noexcept;

			~clock() noexcept;

			f32 elasped() const noexcept;
			f32 restart() noexcept;

		private:
			ptr m_state;
		};

	} // namespace platform
} // namespace ivy
