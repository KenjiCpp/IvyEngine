#pragma once

namespace ivy {
	namespace utility {

		class IVY_API clock {
		public:
			clock() noexcept;

			f32 elasped() const noexcept;
			b8 restart() noexcept;

		private:
			f32 m_start;
		};

	} // namespace utility
} // namespace ivy
