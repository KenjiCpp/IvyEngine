#pragma once

namespace ivy {
	namespace math {

		template<typename T>
		struct rectangle {
			rectangle() noexcept;
			rectangle(T _x, T _y, T _w, T _h) noexcept;
			
			template<typename U>
			rectangle(const rectangle<U>& _other) noexcept;

			T x, y, w, h;
		};

		typedef rectangle<i32> rectangle_i;
		typedef rectangle<f32> rectangle_f;
		typedef rectangle<f64> rectangle_d;

	} // namespace math

	using math::rectangle_i;
	using math::rectangle_f;
	using math::rectangle_d;

} // namespace ivy

#include "../impl/math/rectangle.inl"
