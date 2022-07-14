#pragma once

#include "string.h"
#include "nullable.h"
#include "array.h"
#include "deque.h"
#include "set.h"
#include "map.h"

#include "platform/datetime.h"

namespace ivy {
	namespace utility {

		typedef string8       string_type;
		typedef string8::view template_type;
		typedef string8::view format_context;

		IVY_API b8 find_format_context(const template_type& _template, u32 _offset, format_context& _ctx, u32& _start, u32& _end, b8& _Error) noexcept;

		template<typename T>
		struct formatter {
			b8 parse(const format_context& _ctx) noexcept;

			string_type format(const T& _value) const noexcept;
		};

		struct digit_formatter {
			i32 base        = 10;
			b8  capitalized = false;

			static constexpr const c8* digits_lower = "0123456789abcdef";
			static constexpr const c8* digits_upper = "0123456789ABCDEF";

			IVY_API c8 digit(u32 _num) const noexcept;
		};

		struct string_formatter {
			i32 width = -1;
			i8  align = -1;

			IVY_API b8 parse(const format_context& _ctx) noexcept;

			IVY_API string_type format(c8                   _value) const noexcept;
			IVY_API string_type format(const c8*            _value) const noexcept;
			IVY_API string_type format(const string8&       _value) const noexcept;
			IVY_API string_type format(const string8::view& _value) const noexcept;

			IVY_API static u32 to_uint(const c8* _cstr, u32& _num) noexcept;
		};

		struct bool_formatter : string_formatter {
			b8 capitalized = false;
			b8 as_number   = false;

			IVY_API b8 parse(const format_context& _ctx) noexcept;

			IVY_API string_type format(b8 _value) const noexcept;
		};

		struct pointer_formatter : string_formatter, digit_formatter {
			IVY_API b8 parse(const format_context& _ctx) noexcept;

			IVY_API string_type format(ptr _value) const noexcept;
		};

		struct signed_integer_formatter : string_formatter, digit_formatter {
			b8 zero  = false;
			b8 plus  = false;
			b8 space = false;

			IVY_API b8 parse(const format_context& _ctx) noexcept;

			IVY_API string_type format(i8  _value) const noexcept;
			IVY_API string_type format(i16 _value) const noexcept;
			IVY_API string_type format(i32 _value) const noexcept;
			IVY_API string_type format(i64 _value) const noexcept;
		};

		struct unsigned_integer_formatter : string_formatter, digit_formatter {
			b8 zero = false;

			IVY_API b8 parse(const format_context& _ctx) noexcept;

			IVY_API string_type format(u8  _value) const noexcept;
			IVY_API string_type format(u16 _value) const noexcept;
			IVY_API string_type format(u32 _value) const noexcept;
			IVY_API string_type format(u64 _value) const noexcept;
		};

		struct floating_point_formatter : string_formatter, digit_formatter {
			i32 precision = 6;
			b8  zero      = false;
			b8  plus      = false;
			b8  space     = false;
			b8  exponent  = false;

			static constexpr f32 EPSILON_F = 0.00000000000001f;
			static constexpr f64 EPSILON_D = 0.00000000000001;

			IVY_API b8 parse(const format_context& _ctx) noexcept;

			IVY_API string_type format(f32 _value) const noexcept;
			IVY_API string_type format(f64 _value) const noexcept;
		};

		template<typename T>
		struct container_formatter : string_formatter, formatter<T> {
			b8 parse(const format_context& _ctx) noexcept;

			template<template<typename> typename C>
			string_type format(const C<T>& _value) const noexcept;
		};

		template<typename T>
		struct set_formatter : string_formatter, formatter<T> {
			b8 parse(const format_context& _ctx) noexcept;

			template<template<typename> typename C>
			string_type format(const C<T>& _value) const noexcept;
		};

		template<typename KeyT, typename T>
		struct map_formatter : string_formatter {
			formatter<KeyT> key_formatter;
			formatter<T> value_formatter;

			b8 parse(const format_context& _ctx) noexcept;

			template<template<typename, typename> typename M>
			string_type format(const M<KeyT, T>& _value) const noexcept;
		};

		struct null_formatter : string_formatter {
			b8 null_capitalized = false;

			IVY_API b8 parse(const format_context& _ctx) noexcept;

			IVY_API string_type format(null_type _value) const noexcept;
		};

		template<> struct formatter<null_type> : null_formatter { };

		template<typename T>
		struct nullable_formatter : null_formatter, formatter<T> {
			b8 parse(const format_context& _ctx) noexcept;

			string_type format(const nullable<T>& _value) const noexcept;
		};

		struct datetime_formatter {
			signed_integer_formatter formatter_2;
			signed_integer_formatter formatter_4;

			IVY_API b8 parse(const format_context& _ctx) noexcept;

			IVY_API string_type format(const datetime& _value) const noexcept;
		};

		template<> struct formatter<datetime> : datetime_formatter { };

		template<typename T> struct formatter<nullable<T>> : nullable_formatter<T> { };

		template<> struct formatter<b8> : bool_formatter { };

		template<typename T> struct formatter<T*> : pointer_formatter { };

		template<>         struct formatter<c8>                     : string_formatter { };
		template<>         struct formatter<string8>                : string_formatter { };
		template<>         struct formatter<typename string8::view> : string_formatter { };
		template<>         struct formatter<c8*>                    : string_formatter { };
		template<>         struct formatter<c8[]>                   : string_formatter { };
		template<size_t N> struct formatter<c8[N]>                  : string_formatter { };

		template<> struct formatter<i8>  : signed_integer_formatter { };
		template<> struct formatter<i16> : signed_integer_formatter { };
		template<> struct formatter<i32> : signed_integer_formatter { };
		template<> struct formatter<i64> : signed_integer_formatter { };

		template<> struct formatter<u8>  : unsigned_integer_formatter { };
		template<> struct formatter<u16> : unsigned_integer_formatter { };
		template<> struct formatter<u32> : unsigned_integer_formatter { };
		template<> struct formatter<u64> : unsigned_integer_formatter { };

		template<> struct formatter<f32> : floating_point_formatter { };
		template<> struct formatter<f64> : floating_point_formatter { };

		template<typename T> struct formatter<array<T>> : container_formatter<T> { };
		template<typename T> struct formatter<deque<T>> : container_formatter<T> { };

		template<typename T> struct formatter<set<T>> : set_formatter<T> { };

		template<typename KeyT, typename T> struct formatter<map<KeyT, T>> : map_formatter<KeyT, T> { };

	} // namespace utility
} // namespace ivy

#include "../impl/utility/formatter.inl"
