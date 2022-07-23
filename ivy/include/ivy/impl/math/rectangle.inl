namespace ivy {
	namespace math {

		template<typename T>
		rectangle<T>::rectangle() noexcept :
			x(0),
			y(0),
			w(0),
			h(0) {
		}

		template<typename T>
		rectangle<T>::rectangle(T _x, T _y, T _w, T _h) noexcept :
			x(_x),
			y(_y),
			w(_w),
			h(_h) {
		}

		template<typename T>
		template<typename U>
		rectangle<T>::rectangle(const rectangle<U>& _other) noexcept :
			x(_other.x),
			y(_other.y),
			w(_other.w),
			h(_other.h) {
		}

	} // namespace math
} // namespace ivy
