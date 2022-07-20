#pragma once

#include "scalar.h"

namespace ivy {
	namespace math {

		template<typename T, u8 N>
		struct vector;

		template<typename T>
		struct vector<T, 2> {
			vector() noexcept;
			vector(T _v) noexcept;
			vector(T _x, T _y) noexcept;

			template<typename U>
			vector(const vector<U, 2>& _other) noexcept;
			
			template<typename U>
			vector& operator=(const vector<U, 2>& _other) noexcept;

			T x, y;
		};

		template<typename T>
		using vector2 = vector<T, 2>;

		typedef vector2<b8>  vector2_b;
		typedef vector2<i32> vector2_i;
		typedef vector2<u32> vector2_u;
		typedef vector2<f32> vector2_f;
		typedef vector2<f64> vector2_d;

		template<typename T>
		struct vector<T, 3> {
			vector() noexcept;
			vector(T _v) noexcept;
			vector(T _x, T _y, T _z) noexcept;

			template<typename U>
			vector(const vector<U, 3>& _other) noexcept;

			template<typename U>
			vector& operator=(const vector<U, 3>& _other) noexcept;

			T x, y, z;
		};

		template<typename T>
		using vector3 = vector<T, 3>;

		typedef vector3<b8>  vector3_b;
		typedef vector3<i32> vector3_i;
		typedef vector3<u32> vector3_u;
		typedef vector3<f32> vector3_f;
		typedef vector3<f64> vector3_d;

		template<typename T>
		struct vector<T, 4> {
			vector() noexcept;
			vector(T _v) noexcept;
			vector(T _x, T _y, T _z, T _w) noexcept;

			template<typename U>
			vector(const vector<U, 4>& _other) noexcept;

			template<typename U>
			vector& operator=(const vector<U, 4>& _other) noexcept;

			T x, y, z, w;
		};
		
		template<typename T>
		using vector4 = vector<T, 4>;

		typedef vector4<b8>  vector4_b;
		typedef vector4<i32> vector4_i;
		typedef vector4<u32> vector4_u;
		typedef vector4<f32> vector4_f;
		typedef vector4<f64> vector4_d;

		template<typename T, typename U, u8 N>
		b8 operator==(const vector<T, N>& _v0, const vector<U, N>& _v1) noexcept;
		template<typename T, typename U, u8 N>
		b8 operator!=(const vector<T, N>& _v0, const vector<U, N>& _v1) noexcept;
		template<typename T, typename U, u8 N>
		b8 operator>(const vector<T, N>& _v0, const vector<U, N>& _v1) noexcept;
		template<typename T, typename U, u8 N>
		b8 operator<(const vector<T, N>& _v0, const vector<U, N>& _v1) noexcept;
		template<typename T, typename U, u8 N>
		b8 operator>=(const vector<T, N>& _v0, const vector<U, N>& _v1) noexcept;
		template<typename T, typename U, u8 N>
		b8 operator<=(const vector<T, N>& _v0, const vector<U, N>& _v1) noexcept;

		template<typename T, u8 N>
		vector<result_type<negate, T>, N> operator-(const vector<T, N>& _v0) noexcept;

		template<typename T, typename U, u8 N>
		vector<result_type<plus, T, U>, N> operator+(T _v0, const vector<U, N>& _v1) noexcept;
		template<typename T, typename U, u8 N>
		vector<result_type<plus, T, U>, N> operator+(const vector<T, N>& _v0, U _v1) noexcept;
		template<typename T, typename U, u8 N>
		vector<result_type<plus, T, U>, N> operator+(const vector<T, N>& _v0, const vector<U, N>& _v1) noexcept;

		template<typename T, typename U, u8 N>
		vector<result_type<minus, T, U>, N> operator-(T _v0, const vector<U, N>& _v1) noexcept;
		template<typename T, typename U, u8 N>
		vector<result_type<minus, T, U>, N> operator-(const vector<T, N>& _v0, U _v1) noexcept;
		template<typename T, typename U, u8 N>
		vector<result_type<minus, T, U>, N> operator-(const vector<T, N>& _v0, const vector<U, N>& _v1) noexcept;

		template<typename T, typename U, u8 N>
		vector<result_type<multiplies, T, U>, N> operator*(T _v0, const vector<U, N>& _v1) noexcept;
		template<typename T, typename U, u8 N>
		vector<result_type<multiplies, T, U>, N> operator*(const vector<T, N>& _v0, U _v1) noexcept;
		template<typename T, typename U, u8 N>
		vector<result_type<multiplies, T, U>, N> operator*(const vector<T, N>& _v0, const vector<U, N>& _v1) noexcept;

		template<typename T, typename U, u8 N>
		vector<result_type<divides, T, U>, N> operator/(T _v0, const vector<U, N>& _v1) noexcept;
		template<typename T, typename U, u8 N>
		vector<result_type<divides, T, U>, N> operator/(const vector<T, N>& _v0, U _v1) noexcept;
		template<typename T, typename U, u8 N>
		vector<result_type<divides, T, U>, N> operator/(const vector<T, N>& _v0, const vector<U, N>& _v1) noexcept;

	} // namespace math

	using math::vector2_b;
	using math::vector2_i;
	using math::vector2_u;
	using math::vector2_f;
	using math::vector2_d;

	using math::vector3_b;
	using math::vector3_i;
	using math::vector3_u;
	using math::vector3_f;
	using math::vector3_d;

	using math::vector4_b;
	using math::vector4_i;
	using math::vector4_u;
	using math::vector4_f;
	using math::vector4_d;

} // namespace ivy

#include "../impl/math/vector.inl"
