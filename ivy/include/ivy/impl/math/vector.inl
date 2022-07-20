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
		template<typename U>
		vector<T, 2>::vector(const vector<U, 2>& _other) noexcept :
			x(_other.x),
			y(_other.y) {
		}

		template<typename T>
		template<typename U>
		vector<T, 2>& vector<T, 2>::operator=(const vector<U, 2>& _other) noexcept {
			if (this != &_other) {
				x = _other.x;
				y = _other.y;
			}

			return *this;
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
		template<typename U>
		vector<T, 3>::vector(const vector<U, 3>& _other) noexcept :
			x(_other.x),
			y(_other.y),
			z(_other.z) {
		}

		template<typename T>
		template<typename U>
		vector<T, 3>& vector<T, 3>::operator=(const vector<U, 3>& _other) noexcept {
			if (this != &_other) {
				x = _other.x;
				y = _other.y;
				z = _other.z;
			}

			return *this;
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

		template<typename T>
		template<typename U>
		vector<T, 4>::vector(const vector<U, 4>& _other) noexcept :
			x(_other.x),
			y(_other.y),
			z(_other.z),
			w(_other.w) {
		}

		template<typename T>
		template<typename U>
		vector<T, 4>& vector<T, 4>::operator=(const vector<U, 4>& _other) noexcept {
			if (this != &_other) {
				x = _other.x;
				y = _other.y;
				z = _other.z;
				w = _other.w;
			}

			return *this;
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

		template<typename T, typename U>
		b8 operator<(const vector2<T>& _v0, const vector2<U>& _v1) noexcept {
			return (
				_v0.x == _v1.x 
					? _v0.y < _v1.y 
					: _v0.x < _v1.x
				);
		}

		template<typename T, typename U>
		b8 operator<(const vector3<T>& _v0, const vector3<U>& _v1) noexcept {
			return (
				_v0.x == _v1.x
					? _v0.y == _v1.y 
						? _v0.z < _v1.z 
						: _v0.y < _v1.y
					: _v0.x < _v1.x
				);
		}

		template<typename T, typename U>
		b8 operator<(const vector4<T>& _v0, const vector4<U>& _v1) noexcept {
			return (
				_v0.x == _v1.x
					? _v0.y == _v1.y
						? _v0.z == _v1.z 
							? _v0.w < _v1.w 
							: _v0.z < _v1.z
						: _v0.y < _v1.y
					: _v0.x < _v1.x
				);
		}

		template<typename T, typename U>
		b8 operator>(const vector2<T>& _v0, const vector2<U>& _v1) noexcept {
			return (
				_v0.x == _v1.x
					? _v0.y > _v1.y
					: _v0.x > _v1.x
				);
		}

		template<typename T, typename U>
		b8 operator>(const vector3<T>& _v0, const vector3<U>& _v1) noexcept {
			return (
				_v0.x == _v1.x
					? _v0.y == _v1.y
						? _v0.z > _v1.z
						: _v0.y > _v1.y
					: _v0.x > _v1.x
				);
		}

		template<typename T, typename U>
		b8 operator>(const vector4<T>& _v0, const vector4<U>& _v1) noexcept {
			return (
				_v0.x == _v1.x
					? _v0.y == _v1.y
						? _v0.z == _v1.z
							? _v0.w > _v1.w
							: _v0.z > _v1.z
						: _v0.y > _v1.y
					: _v0.x > _v1.x
				);
		}

		template<typename T, typename U>
		b8 operator<=(const vector2<T>& _v0, const vector2<U>& _v1) noexcept {
			return (
				_v0.x == _v1.x
					? _v0.y <= _v1.y
					: _v0.x <= _v1.x
				);
		}

		template<typename T, typename U>
		b8 operator<=(const vector3<T>& _v0, const vector3<U>& _v1) noexcept {
			return (
				_v0.x == _v1.x
					? _v0.y == _v1.y
						? _v0.z <= _v1.z
						: _v0.y <= _v1.y
					: _v0.x <= _v1.x
				);
		}

		template<typename T, typename U>
		b8 operator<=(const vector4<T>& _v0, const vector4<U>& _v1) noexcept {
			return (
				_v0.x == _v1.x
					? _v0.y == _v1.y
						? _v0.z == _v1.z
							? _v0.w <= _v1.w
							: _v0.z <= _v1.z
						: _v0.y <= _v1.y
					: _v0.x <= _v1.x
				);
		}

		template<typename T, typename U>
		b8 operator>=(const vector2<T>& _v0, const vector2<U>& _v1) noexcept {
			return (
				_v0.x == _v1.x
					? _v0.y >= _v1.y
					: _v0.x >= _v1.x
				);
		}

		template<typename T, typename U>
		b8 operator>=(const vector3<T>& _v0, const vector3<U>& _v1) noexcept {
			return (
				_v0.x == _v1.x
					? _v0.y == _v1.y
						? _v0.z >= _v1.z
						: _v0.y >= _v1.y
					: _v0.x >= _v1.x
				);
		}

		template<typename T, typename U>
		b8 operator>=(const vector4<T>& _v0, const vector4<U>& _v1) noexcept {
			return (
				_v0.x == _v1.x
					? _v0.y == _v1.y
						? _v0.z == _v1.z
							? _v0.w >= _v1.w
							: _v0.z >= _v1.z
						: _v0.y >= _v1.y
					: _v0.x >= _v1.x
				);
		}

		template<typename T>
		vector2<result_type<negate, T>> operator-(const vector2<T>& _v0) noexcept {
				return vector2<result_type<negate, T>>(-_v0.x, -_v0.y);
		}

		template<typename T>
		vector3<result_type<negate, T>> operator-(const vector3<T>& _v0) noexcept {
			return vector3<result_type<negate, T>>(-_v0.x, -_v0.y, _v0.z);
		}

		template<typename T>
		vector4<result_type<negate, T>> operator-(const vector4<T>& _v0) noexcept {
			return vector4<result_type<negate, T>>(-_v0.x, -_v0.y, _v0.z, _v0.w);
		}

		template<typename T, typename U>
		vector2<result_type<plus, T, U>> operator+(T _v0, const vector2<U>& _v1) noexcept {
			return vector2<result_type<plus, T, U>>(_v0 + _v1.x, _v0 + _v1.y);
		}

		template<typename T, typename U>
		vector3<result_type<plus, T, U>> operator+(T _v0, const vector3<U>& _v1) noexcept {
			return vector3<result_type<plus, T, U>>(_v0 + _v1.x, _v0 + _v1.y, _v0 + _v1.z);
		}

		template<typename T, typename U>
		vector4<result_type<plus, T, U>> operator+(T _v0, const vector4<U>& _v1) noexcept {
			return vector4<result_type<plus, T, U>>(_v0 + _v1.x, _v0 + _v1.y, _v0 + _v1.z, _v0 + _v1.w);
		}

		template<typename T, typename U>
		vector2<result_type<plus, T, U>> operator+(const vector2<T>& _v0, U _v1) noexcept {
			return vector2<result_type<plus, T, U>>(_v0.x + _v1, _v0.y + _v1);
		}

		template<typename T, typename U>
		vector3<result_type<plus, T, U>> operator+(const vector3<T>& _v0, U _v1) noexcept {
			return vector3<result_type<plus, T, U>>(_v0.x + _v1, _v0.y + _v1, _v0.z + _v1);
		}

		template<typename T, typename U>
		vector4<result_type<plus, T, U>> operator+(const vector4<T>& _v0, U _v1) noexcept {
			return vector4<result_type<plus, T, U>>(_v0.x + _v1, _v0.y + _v1, _v0.z + _v1, _v0.w + _v1);
		}

		template<typename T, typename U>
		vector2<result_type<plus, T, U>> operator+(const vector2<T>& _v0, const vector2<U>& _v1) noexcept {
			return vector2<result_type<plus, T, U>>(_v0.x + _v1.x, _v0.y + _v1.y);
		}

		template<typename T, typename U>
		vector3<result_type<plus, T, U>> operator+(const vector3<T>& _v0, const vector3<U>& _v1) noexcept {
			return vector3<result_type<plus, T, U>>(_v0.x + _v1.x, _v0.y + _v1.y, _v0.z + _v1.z);
		}

		template<typename T, typename U>
		vector4<result_type<plus, T, U>> operator+(const vector4<T>& _v0, const vector4<U>& _v1) noexcept {
			return vector4<result_type<plus, T, U>>(_v0.x + _v1.x, _v0.y + _v1.y, _v0.z + _v1.z, _v0.w + _v1.w);
		}

		template<typename T, typename U>
		vector2<result_type<minus, T, U>> operator-(T _v0, const vector2<U>& _v1) noexcept {
			return vector2<result_type<minus, T, U>>(_v0 - _v1.x, _v0 - _v1.y);
		}

		template<typename T, typename U>
		vector3<result_type<minus, T, U>> operator-(T _v0, const vector3<U>& _v1) noexcept {
			return vector3<result_type<minus, T, U>>(_v0 - _v1.x, _v0 - _v1.y, _v0 - _v1.z);
		}

		template<typename T, typename U>
		vector4<result_type<minus, T, U>> operator-(T _v0, const vector4<U>& _v1) noexcept {
			return vector4<result_type<minus, T, U>>(_v0 - _v1.x, _v0 - _v1.y, _v0 - _v1.z, _v0 - _v1.w);
		}

		template<typename T, typename U>
		vector2<result_type<minus, T, U>> operator-(const vector2<T>& _v0, U _v1) noexcept {
			return vector2<result_type<minus, T, U>>(_v0.x - _v1, _v0.y - _v1);
		}

		template<typename T, typename U>
		vector3<result_type<minus, T, U>> operator-(const vector3<T>& _v0, U _v1) noexcept {
			return vector3<result_type<minus, T, U>>(_v0.x - _v1, _v0.y - _v1, _v0.z - _v1);
		}

		template<typename T, typename U>
		vector4<result_type<minus, T, U>> operator-(const vector4<T>& _v0, U _v1) noexcept {
			return vector4<result_type<minus, T, U>>(_v0.x - _v1, _v0.y - _v1, _v0.z - _v1, _v0.w - _v1);
		}

		template<typename T, typename U>
		vector2<result_type<minus, T, U>> operator-(const vector2<T>& _v0, const vector2<U>& _v1) noexcept {
			return vector2<result_type<minus, T, U>>(_v0.x - _v1.x, _v0.y - _v1.y);
		}

		template<typename T, typename U>
		vector3<result_type<minus, T, U>> operator-(const vector3<T>& _v0, const vector3<U>& _v1) noexcept {
			return vector3<result_type<minus, T, U>>(_v0.x - _v1.x, _v0.y - _v1.y, _v0.z - _v1.z);
		}

		template<typename T, typename U>
		vector4<result_type<minus, T, U>> operator-(const vector4<T>& _v0, const vector4<U>& _v1) noexcept {
			return vector4<result_type<minus, T, U>>(_v0.x - _v1.x, _v0.y - _v1.y, _v0.z - _v1.z, _v0.w - _v1.w);
		}

		template<typename T, typename U>
		vector2<result_type<multiplies, T, U>> operator*(T _v0, const vector2<U>& _v1) noexcept {
			return vector2<result_type<multiplies, T, U>>(_v0 * _v1.x, _v0 * _v1.y);
		}

		template<typename T, typename U>
		vector3<result_type<multiplies, T, U>> operator*(T _v0, const vector3<U>& _v1) noexcept {
			return vector3<result_type<multiplies, T, U>>(_v0 * _v1.x, _v0 * _v1.y, _v0 * _v1.z);
		}

		template<typename T, typename U>
		vector4<result_type<multiplies, T, U>> operator*(T _v0, const vector4<U>& _v1) noexcept {
			return vector4<result_type<multiplies, T, U>>(_v0 * _v1.x, _v0 * _v1.y, _v0 * _v1.z, _v0 * _v1.w);
		}

		template<typename T, typename U>
		vector2<result_type<multiplies, T, U>> operator*(const vector2<T>& _v0, U _v1) noexcept {
			return vector2<result_type<multiplies, T, U>>(_v0.x * _v1, _v0.y * _v1);
		}

		template<typename T, typename U>
		vector3<result_type<multiplies, T, U>> operator*(const vector3<T>& _v0, U _v1) noexcept {
			return vector3<result_type<multiplies, T, U>>(_v0.x * _v1, _v0.y * _v1, _v0.z * _v1);
		}

		template<typename T, typename U>
		vector4<result_type<multiplies, T, U>> operator*(const vector4<T>& _v0, U _v1) noexcept {
			return vector4<result_type<multiplies, T, U>>(_v0.x * _v1, _v0.y * _v1, _v0.z * _v1, _v0.w * _v1);
		}

		template<typename T, typename U>
		vector2<result_type<multiplies, T, U>> operator*(const vector2<T>& _v0, const vector2<U>& _v1) noexcept {
			return vector2<result_type<multiplies, T, U>>(_v0.x * _v1.x, _v0.y * _v1.y);
		}

		template<typename T, typename U>
		vector3<result_type<multiplies, T, U>> operator*(const vector3<T>& _v0, const vector3<U>& _v1) noexcept {
			return vector3<result_type<multiplies, T, U>>(_v0.x * _v1.x, _v0.y * _v1.y, _v0.z * _v1.z);
		}

		template<typename T, typename U>
		vector4<result_type<multiplies, T, U>> operator*(const vector4<T>& _v0, const vector4<U>& _v1) noexcept {
			return vector4<result_type<multiplies, T, U>>(_v0.x * _v1.x, _v0.y * _v1.y, _v0.z * _v1.z, _v0.w * _v1.w);
		}

		template<typename T, typename U>
		vector2<result_type<divides, T, U>> operator/(T _v0, const vector2<U>& _v1) noexcept {
			return vector2<result_type<divides, T, U>>(_v0 / _v1.x, _v0 / _v1.y);
		}

		template<typename T, typename U>
		vector3<result_type<divides, T, U>> operator/(T _v0, const vector3<U>& _v1) noexcept {
			return vector3<result_type<divides, T, U>>(_v0 / _v1.x, _v0 / _v1.y, _v0 / _v1.z);
		}

		template<typename T, typename U>
		vector4<result_type<divides, T, U>> operator/(T _v0, const vector4<U>& _v1) noexcept {
			return vector4<result_type<divides, T, U>>(_v0 / _v1.x, _v0 / _v1.y, _v0 / _v1.z, _v0 / _v1.w);
		}

		template<typename T, typename U>
		vector2<result_type<divides, T, U>> operator/(const vector2<T>& _v0, U _v1) noexcept {
			return vector2<result_type<divides, T, U>>(_v0.x / _v1, _v0.y / _v1);
		}

		template<typename T, typename U>
		vector3<result_type<divides, T, U>> operator/(const vector3<T>& _v0, U _v1) noexcept {
			return vector3<result_type<divides, T, U>>(_v0.x / _v1, _v0.y / _v1, _v0.z / _v1);
		}

		template<typename T, typename U>
		vector4<result_type<divides, T, U>> operator/(const vector4<T>& _v0, U _v1) noexcept {
			return vector4<result_type<divides, T, U>>(_v0.x / _v1, _v0.y / _v1, _v0.z / _v1, _v0.w / _v1);
		}

		template<typename T, typename U>
		vector2<result_type<divides, T, U>> operator/(const vector2<T>& _v0, const vector2<U>& _v1) noexcept {
			return vector2<result_type<divides, T, U>>(_v0.x / _v1.x, _v0.y / _v1.y);
		}

		template<typename T, typename U>
		vector3<result_type<divides, T, U>> operator/(const vector3<T>& _v0, const vector3<U>& _v1) noexcept {
			return vector3<result_type<divides, T, U>>(_v0.x / _v1.x, _v0.y / _v1.y, _v0.z / _v1.z);
		}

		template<typename T, typename U>
		vector4<result_type<divides, T, U>> operator/(const vector4<T>& _v0, const vector4<U>& _v1) noexcept {
			return vector4<result_type<divides, T, U>>(_v0.x / _v1.x, _v0.y / _v1.y, _v0.z / _v1.z, _v0.w / _v1.w);
		}

	} // namespace math
} // namespace ivy
