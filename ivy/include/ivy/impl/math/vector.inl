namespace ivy {
	namespace math {

		template<typename T>
		vector<T, 2>::vector() noexcept :
			x(0),
			y(0) {
		}

		template<typename T>
		vector<T, 2>::vector(T _v) noexcept :
			x(_v),
			y(_v) {
		}

		template<typename T>
		vector<T, 2>::vector(T _x, T _y) noexcept :
			x(_x),
			y(_y) {
		}

		template<typename T>
		vector<T, 3>::vector() noexcept :
			x(0),
			y(0),
			z(0) {
		}

		template<typename T>
		vector<T, 3>::vector(T _v) noexcept :
			x(_v),
			y(_v),
			z(_v) {
		}

		template<typename T>
		vector<T, 3>::vector(T _x, T _y, T _z) noexcept :
			x(_x),
			y(_y),
			z(_z) {
		}

		template<typename T>
		vector<T, 4>::vector() noexcept :
			x(0),
			y(0),
			z(0),
			w(0) {
		}

		template<typename T>
		vector<T, 4>::vector(T _v) noexcept :
			x(_v),
			y(_v),
			z(_v),
			w(_v) {
		}

		template<typename T>
		vector<T, 4>::vector(T _x, T _y, T _z, T _w) noexcept :
			x(_x),
			y(_y),
			z(_z),
			w(_w) {
		}

		template<typename T, typename U>
		b8 operator==(const vector2<T>& _v0, const vector2<U>& _v1) noexcept {
			return (_v0.x == _v1.x && _v0.y == _v1.y);
		}

		template<typename T, typename U>
		b8 operator==(const vector3<T>& _v0, const vector3<U>& _v1) noexcept {
			return (_v0.x == _v1.x && _v0.y == _v1.y && _v0.z == _v1.z);
		}

		template<typename T, typename U>
		b8 operator==(const vector4<T>& _v0, const vector4<U>& _v1) noexcept {
			return (_v0.x == _v1.x && _v0.y == _v1.y && _v0.z == _v1.z && _v0.w == _v1.w);
		}

		template<typename T, typename U>
		b8 operator!=(const vector2<T>& _v0, const vector2<U>& _v1) noexcept {
			return (_v0.x != _v1.x || _v0.y != _v1.y);
		}

		template<typename T, typename U>
		b8 operator!=(const vector3<T>& _v0, const vector3<U>& _v1) noexcept {
			return (_v0.x != _v1.x || _v0.y != _v1.y || _v0.z != _v1.z);
		}

		template<typename T, typename U>
		b8 operator!=(const vector4<T>& _v0, const vector4<U>& _v1) noexcept {
			return (_v0.x != _v1.x || _v0.y != _v1.y || _v0.z != _v1.z || _v0.w != _v1.w);
		}

	} // namespace math
} // namespace ivy
