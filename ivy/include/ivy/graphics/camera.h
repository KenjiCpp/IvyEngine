#pragma once

#include "math/vector.h"

namespace ivy {
	namespace graphics {

		class IVY_API camera {
		public:
			camera() noexcept;

			b8 set_position(const vector3_f& _position) noexcept;
			b8 set_rotation(const vector3_f& _rotation) noexcept;

			const vector3_f& position() const noexcept;
			const vector3_f& rotation() const noexcept;

			// const matrix4x4_f& view_matrix() const noexcept;

			vector3_f forward() const noexcept;
			vector3_f backward() const noexcept;
			vector3_f left() const noexcept;
			vector3_f right() const noexcept;
			vector3_f up() const noexcept;
			vector3_f down() const noexcept;

		private:
			b8 update_view() const noexcept;

		private:
			b8 m_is_dirty;

			vector3_f m_position;
			vector3_f m_rotation;

			// mutable matrix4x4_f m_view_matrix;
		};

	} // namespace graphics

	using graphics::camera;

} // namespace ivy
