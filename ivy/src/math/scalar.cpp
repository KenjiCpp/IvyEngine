#include "math/scalar.h"

#include <cmath>

namespace ivy {
	namespace math {

		f32 sqrt_f(f32 _value) noexcept {
			return std::sqrtf(_value);
		}

		f64 sqrt_d(f64 _value) noexcept {
			return std::sqrt(_value);
		}

	} // namespace math
} // namespace ivy
