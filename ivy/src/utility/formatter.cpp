#include "utility/formatter.h"

#include <cmath>

namespace ivy {
	namespace utility {

		b8 find_format_context(const template_type& _template, u32 _offset, format_context& _ctx, u32& _start, u32& _end, b8& _Error) noexcept {
			u32 first = 0;
			i32 count = 0;
			_Error = false;

			while (_offset < _template.length()) {
				if (_template[_offset] == '{') {
					if (!count) {
						first = _offset;
					}
					++count;
				}
				else if (_template[_offset] == '}') {
					--count;
					if (!count) {
						_ctx = _template.subview(first + 1, _offset - 1 - first);
						_start = first;
						_end = _offset;
						return true;
					}
				}
				++_offset;
			}

			_Error = (b8)count;
			return false;
		}

		c8 digit_formatter::digit(u32 _num) const noexcept {
			if (capitalized) {
				return digits_upper[_num];
			}
			return digits_lower[_num];
		}

		b8 string_formatter::parse(const format_context& _ctx) noexcept {
			/*
			*	Format: {[width[align]]}
			*		width : <uint>
			*		align : { '<', '=', '>' }^1
			*/

			auto it = _ctx.begin();

			if (it != _ctx.end()) {
				u32 num = 0;
				u32 len = to_uint(it, num);
				if (len) {
					width = num;
					it += len;

					if (it == _ctx.end()) {
						return true;
					}

					if (*it == '<') {
						align = -1;
						++it;
					}
					else if (*it == '=') {
						align = 0;
						++it;
					}
					else if (*it == '>') {
						align = 1;
						++it;
					}
				}
			}
			
			return (it == _ctx.end());
		}

		string_type string_formatter::format(c8 _value) const noexcept {
			string_type res;
			res.append(_value);

			if ((i32)res.length() < width) {
				if (align == 1) {
					res.prepend(string_type(' ', width - (i32)res.length()));
				}
				else if (align == -1) {
					res.append(string_type(' ', width - (i32)res.length()));
				}
				else {
					u32 dw = width - (i32)res.length();
					res.prepend(string_type(' ', dw / 2));
					res.append(string_type(' ', dw - (dw / 2)));
				}
			}

			return res;
		}

		string_type string_formatter::format(const c8* _value) const noexcept {
			string_type res = _value;
			
			if ((i32)res.length() < width) {
				if (align == 1) {
					res.prepend(string_type(' ', width - (i32)res.length()));
				}
				else if (align == -1) {
					res.append(string_type(' ', width - (i32)res.length()));
				}
				else {
					i32 dw = width - (i32)res.length();
					res.prepend(string_type(' ', dw / 2));
					res.append(string_type(' ', dw - (dw / 2)));
				}
			}

			return res;
		}

		string_type string_formatter::format(const string8& _value) const noexcept {
			string_type res = _value;

			if ((i32)res.length() < width) {
				if (align == 1) {
					res.prepend(string_type(' ', width - (i32)res.length()));
				}
				else if (align == -1) {
					res.append(string_type(' ', width - (i32)res.length()));
				}
				else {
					i32 dw = width - (i32)res.length();
					res.prepend(string_type(' ', dw / 2));
					res.append(string_type(' ', dw - (dw / 2)));
				}
			}

			return res;
		}

		string_type string_formatter::format(const string8::view& _value) const noexcept {
			string_type res = _value;

			if ((i32)res.length() < width) {
				if (align == 1) {
					res.prepend(string_type(' ', width - (i32)res.length()));
				}
				else if (align == -1) {
					res.append(string_type(' ', width - (i32)res.length()));
				}
				else {
					u32 dw = width - (i32)res.length();
					res.prepend(string_type(' ', dw / 2));
					res.append(string_type(' ', dw - (dw / 2)));
				}
			}

			return res;
		}

		u32 string_formatter::to_uint(const c8* _cstr, u32& _num) noexcept {
			const c8* s = _cstr;
			_num = 0;
			while (*s) {
				if (*s < '0' || *s > '9') {
					break;
				}
				_num = 10 * _num + (*s - '0');
				++s;
			}
			return s - _cstr;
		}

		b8 bool_formatter::parse(const format_context& _ctx) noexcept {
			/*
			*	Format: {[width[align]][display][cap]}
			*		width   : <uint>
			*		align   : { '<', '=', '>' }^1
			*		display : { 's', 'd' }^1
			*		cap     : { '^' }^1
			*/

			auto it = _ctx.begin();

			if (it != _ctx.end()) {
				u32 num = 0;
				u32 len = to_uint(it, num);
				if (len) {
					width = num;
					it += len;

					if (it == _ctx.end()) {
						return true;
					}

					if (*it == '<') {
						align = -1;
						++it;
					}
					else if (*it == '=') {
						align = 0;
						++it;
					}
					else if (*it == '>') {
						align = 1;
						++it;
					}
				}
			}

			if (it != _ctx.end()) {
				if (*it == 's') {
					as_number = false;
					++it;
				}
				else if (*it == 'd') {
					as_number = true;
					++it;
				}
			}

			if (it != _ctx.end()) {
				if (*it == '^') {
					capitalized = true;
					++it;
				}
			}

			return (it == _ctx.end());
		}

		string_type bool_formatter::format(b8 _value) const noexcept {
			string_type res;

			if (as_number) {
				res = _value ? "1" : "0";
			}
			else if (capitalized) {
				res = _value ? "TRUE" : "FALSE";
			}
			else {
				res = _value ? "true" : "false";
			}

			return string_formatter::format(res);
		}


		b8 pointer_formatter::parse(const format_context& _ctx) noexcept {
			/*
			*	Format: {[width[align]][cap]}
			*		width   : <uint>
			*		align   : { '<', '=', '>' }^1
			*		cap     : { '^' }^1
			*/

			auto it = _ctx.begin();

			if (it != _ctx.end()) {
				u32 num = 0;
				u32 len = to_uint(it, num);
				if (len) {
					width = num;
					it += len;

					if (it == _ctx.end()) {
						return true;
					}

					if (*it == '<') {
						align = -1;
						++it;
					}
					else if (*it == '=') {
						align = 0;
						++it;
					}
					else if (*it == '>') {
						align = 1;
						++it;
					}
				}
			}

			if (it != _ctx.end()) {
				if (*it == '^') {
					capitalized = true;
					++it;
				}
			}

			return (it == _ctx.end());
		}

		string_type pointer_formatter::format(cptr _value) const noexcept {
			string_type res;
			
			u32 len = 2 * sizeof(cptr);
			u32 val = *(u32*)&_value;

			while (len--) {
				res.prepend(digit(val % 16));
				val /= 16;
			}
			
			return string_formatter::format(res);
		}

		b8 signed_integer_formatter::parse(const format_context& _ctx) noexcept {
			/* 
			*	Format: {[prefix][zero][width[align]][base][cap]}
			*		zero  : { '0' }^1
			*		width : <uint>
			*		align : { '<', '=', '>' }^1
			*		prefix: { '+', ' ' }^1
			*		base  : { 'b', 'd', 'x' }^1
			*		cap   : { '^' }^1
			*/
			
			auto it = _ctx.begin();

			if (it != _ctx.end()) {
				if (*it == '+') {
					plus = true;
					++it;
				}
				else if (*it == ' ') {
					space = true;
					++it;
				}
			}

			if (it != _ctx.end()) {
				if (*it == '0') {
					zero = true;
					++it;
				}
			}

			if (it != _ctx.end()) {
				u32 num = 0;
				u32 len = to_uint(it, num);
				if (len) {
					width = num;
					it += len;

					if (it == _ctx.end()) {
						return true;
					}

					if (*it == '<') {
						align = -1;
						++it;
					}
					else if (*it == '=') {
						align = 0;
						++it;
					}
					else if (*it == '>') {
						align = 1;
						++it;
					}
				}
			}

			if (it != _ctx.end()) {
				if (*it == 'b') {
					base = 2;
					++it;
				}
				else if (*it == 'd') {
					base = 10;
					++it;
				}
				else if (*it == 'x') {
					base = 16;
					++it;
				}
			}

			if (it != _ctx.end()) {
				if (*it == '^') {
					capitalized = true;
					++it;
				}
			}

			return (it == _ctx.end());
		}

		string_type signed_integer_formatter::format(i8 _value) const noexcept {
			string_type res(4);

			u8 v = _value < 0 ? -_value : _value;

			do {
				res.prepend(digit(v % base));
				v /= base;
			} while (v);

			if (width > -1) {
				if (zero) {
					b8 inserted = (i32)res.length() < width;
					if ((i32)res.length() < width) {
						res.prepend(string_type('0', width - (i32)res.length()));
					}
					if (_value >= 0) {
						if (inserted) {
							if (plus) {
								res[0] = '+';
							}
							else if (space) {
								res[0] = ' ';
							}
						}
						else {
							if (plus) {
								res.prepend('+');
							}
							else if (space) {
								res.prepend(' ');
							}
						}
					}
					else {
						if (inserted) {
							res[0] = '-';
						}
						else {
							res.prepend('-');
						}
					}
				}
				else {
					if (_value >= 0) {
						if (plus) {
							res.prepend('+');
						}
						else if (space) {
							res.prepend(' ');
						}
					}
					else {
						res.prepend('-');
					}

					res = string_formatter::format(res);
				}
			}
			else {
				if (_value >= 0) {
					if (plus) {
						res.prepend('+');
					}
					else if (space) {
						res.prepend(' ');
					}
				}
				else {
					res.prepend('-');
				}
			}

			return res;
		}

		string_type signed_integer_formatter::format(i16 _value) const noexcept {
			string_type res(6);

			u16 v = _value < 0 ? -_value : _value;

			do {
				res.prepend(digit(v % base));
				v /= base;
			} while (v);

			if (width > -1) {
				if (zero) {
					b8 inserted = (i32)res.length() < width;
					if ((i32)res.length() < width) {
						res.prepend(string_type('0', width - (i32)res.length()));
					}
					if (_value >= 0) {
						if (inserted) {
							if (plus) {
								res[0] = '+';
							}
							else if (space) {
								res[0] = ' ';
							}
						}
						else {
							if (plus) {
								res.prepend('+');
							}
							else if (space) {
								res.prepend(' ');
							}
						}
					}
					else {
						if (inserted) {
							res[0] = '-';
						}
						else {
							res.prepend('-');
						}
					}
				}
				else {
					if (_value >= 0) {
						if (plus) {
							res.prepend('+');
						}
						else if (space) {
							res.prepend(' ');
						}
					}
					else {
						res.prepend('-');
					}

					res = string_formatter::format(res);
				}
			}
			else {
				if (_value >= 0) {
					if (plus) {
						res.prepend('+');
					}
					else if (space) {
						res.prepend(' ');
					}
				}
				else {
					res.prepend('-');
				}
			}

			return res;
		}

		string_type signed_integer_formatter::format(i32 _value) const noexcept {
			string_type res(11);

			u32 v = _value < 0 ? -_value : _value;

			do {
				res.prepend(digit(v % base));
				v /= base;
			} while (v);

			if (width > -1) {
				if (zero) {
					b8 inserted = (i32)res.length() < width;
					if ((i32)res.length() < width) {
						res.prepend(string_type('0', width - (i32)res.length()));
					}
					if (_value >= 0) {
						if (inserted) {
							if (plus) {
								res[0] = '+';
							}
							else if (space) {
								res[0] = ' ';
							}
						}
						else {
							if (plus) {
								res.prepend('+');
							}
							else if (space) {
								res.prepend(' ');
							}
						}
					}
					else {
						if (inserted) {
							res[0] = '-';
						}
						else {
							res.prepend('-');
						}
					}
				}
				else {
					if (_value >= 0) {
						if (plus) {
							res.prepend('+');
						}
						else if (space) {
							res.prepend(' ');
						}
					}
					else {
						res.prepend('-');
					}

					res = string_formatter::format(res);
				}
			}
			else {
				if (_value >= 0) {
					if (plus) {
						res.prepend('+');
					}
					else if (space) {
						res.prepend(' ');
					}
				}
				else {
					res.prepend('-');
				}
			}

			return res;
		}

		string_type signed_integer_formatter::format(i64 _value) const noexcept {
			string_type res(21);

			u64 v = _value < 0 ? -_value : _value;

			do {
				res.prepend(digit(v % base));
				v /= base;
			} while (v);

			if (width > -1) {
				if (zero) {
					b8 inserted = (i32)res.length() < width;
					if ((i32)res.length() < width) {
						res.prepend(string_type('0', width - (i32)res.length()));
					}
					if (_value >= 0) {
						if (inserted) {
							if (plus) {
								res[0] = '+';
							}
							else if (space) {
								res[0] = ' ';
							}
						}
						else {
							if (plus) {
								res.prepend('+');
							}
							else if (space) {
								res.prepend(' ');
							}
						}
					}
					else {
						if (inserted) {
							res[0] = '-';
						}
						else {
							res.prepend('-');
						}
					}
				}
				else {
					if (_value >= 0) {
						if (plus) {
							res.prepend('+');
						}
						else if (space) {
							res.prepend(' ');
						}
					}
					else {
						res.prepend('-');
					}

					res = string_formatter::format(res);
				}
			}
			else {
				if (_value >= 0) {
					if (plus) {
						res.prepend('+');
					}
					else if (space) {
						res.prepend(' ');
					}
				}
				else {
					res.prepend('-');
				}
			}

			return res;
		}

		b8 unsigned_integer_formatter::parse(const format_context& _ctx) noexcept {
			/*
			*	Format: {[zero][width[align]][base][cap]}
			*		zero  : { '0' }^1
			*		width : <uint>
			*		align : { '<', '=', '>' }^1
			*		base  : { 'b', 'd', 'x' }^1
			*		cap   : { '^' }^1
			*/

			auto it = _ctx.begin();

			if (it != _ctx.end()) {
				if (*it == '0') {
					zero = true;
					++it;
				}
			}

			if (it != _ctx.end()) {
				u32 num = 0;
				u32 len = to_uint(it, num);
				if (len) {
					width = num;
					it += len;

					if (it == _ctx.end()) {
						return true;
					}

					if (*it == '<') {
						align = -1;
						++it;
					}
					else if (*it == '=') {
						align = 0;
						++it;
					}
					else if (*it == '>') {
						align = 1;
						++it;
					}
				}
			}

			if (it != _ctx.end()) {
				if (*it == 'b') {
					base = 2;
					++it;
				}
				else if (*it == 'd') {
					base = 10;
					++it;
				}
				else if (*it == 'x') {
					base = 16;
					++it;
				}
			}

			if (it != _ctx.end()) {
				if (*it == '^') {
					capitalized = true;
					++it;
				}
			}

			return (it == _ctx.end());
		}

		string_type unsigned_integer_formatter::format(u8  _value) const noexcept {
			string_type res(3);

			u8 v = _value;

			do {
				res.prepend(digit(v % base));
				v /= base;
			} while (v);

			if (width > -1) {
				if (zero) {
					if ((i32)res.length() < width) {
						res.prepend(string_type('0', width - (i32)res.length()));
					}
				}
				else {
					res = string_formatter::format(res);
				}
			}

			return res;
		}

		string_type unsigned_integer_formatter::format(u16 _value) const noexcept {
			string_type res(5);

			u16 v = _value;

			do {
				res.prepend(digit(v % base));
				v /= base;
			} while (v);

			if (width > -1) {
				if (zero) {
					if ((i32)res.length() < width) {
						res.prepend(string_type('0', width - (i32)res.length()));
					}
				}
				else {
					res = string_formatter::format(res);
				}
			}

			return res;
		}

		string_type unsigned_integer_formatter::format(u32 _value) const noexcept {
			string_type res(10);

			u32 v = _value;

			do {
				res.prepend(digit(v % base));
				v /= base;
			} while (v);

			if (width > -1) {
				if (zero) {
					if ((i32)res.length() < width) {
						res.prepend(string_type('0', width - (i32)res.length()));
					}
				}
				else {
					res = string_formatter::format(res);
				}
			}

			return res;
		}

		string_type unsigned_integer_formatter::format(u64 _value) const noexcept {
			string_type res(20);

			u64 v = _value;

			do {
				res.prepend(digit(v % base));
				v /= base;
			} while (v);

			if (width > -1) {
				if (zero) {
					if ((i32)res.length() < width) {
						res.prepend(string_type('0', width - (i32)res.length()));
					}
				}
				else {
					res = string_formatter::format(res);
				}
			}

			return res;
		}

		b8 floating_point_formatter::parse(const format_context& _ctx) noexcept {
			/*
			*	Format: {[prefix][zero][width[align]][.precision[display]][cap]}
			*/

			auto it = _ctx.begin();

			if (it != _ctx.end()) {
				if (*it == '+') {
					plus = true;
					++it;
				}
				else if (*it == ' ') {
					space = true;
					++it;
				}
			}

			if (it != _ctx.end()) {
				if (*it == '0') {
					zero = true;
					++it;
				}
			}

			if (it != _ctx.end()) {
				u32 num = 0;
				u32 len = to_uint(it, num);
				if (len) {
					width = num;
					it += len;
					
					if (it == _ctx.end()) {
						return true;
					}

					if (*it == '<') {
						align = -1;
						++it;
					}
					else if (*it == '=') {
						align = 0;
						++it;
					}
					else if (*it == '>') {
						align = 1;
						++it;
					}
				}
			}

			if (it != _ctx.end()) {
				if (*it == '.') {
					++it;
					u32 num;
					u32 len = to_uint(it, num);
					if (len) {
						precision = num;
						it += len;
					}
				}
			}

			if (it != _ctx.end()) {
				if (*it == 'f') {
					exponent = false;
					++it;
				}
				else if (*it == 'e') {
					exponent = true;
					++it;
				}
			}

			if (it != _ctx.end()) {
				if (*it == '^') {
					capitalized = true;
					++it;
				}
			}

			return (it == _ctx.end());
		}

		string_type floating_point_formatter::format(f32 _value) const noexcept {
			string_type res;
			if (std::isnan(_value)) {
				res = capitalized ? "NAN" : "nan";
			}
			else if (std::isinf(_value)) {
				res = capitalized ? "INF" : "inf";
				if (_value >= 0.0f) {
					if (plus) {
						res.prepend('+');
					}
					else if (space) {
						res.prepend(" ");
					}
				}
				else {
					res.prepend('-');
				}
			}
			else if (_value == 0.0f) {
				b8 neg = _value < 0.0f;

				res.append('0');
				i32 p = precision;
				if (p) {
					res.append('.');
				}
				while (p--) {
					res.append('0');
				}

				if (zero) {
					b8 inserted = (i32)res.length() < width;
					if (inserted) {
						res.prepend(string_type('0', width - (i32)res.length()));
					}
					if (inserted) {
						if (neg) {
							res[0] = '-';
						}
						else if (plus) {
							res[0] = '+';
						}
						else if (space) {
							res[0] = ' ';
						}
					}
					else {
						if (neg) {
							res.prepend('-');
						}
						else if (plus) {
							res.prepend('+');
						}
						else if (space) {
							res.prepend(' ');
						}
					}
				}
			}
			else {
				b8 neg = _value < 0.0f;
				f32 num = neg ? -_value : _value;
				i32 m = std::log10f(num), m1, d;
				b8 exp = (m >= 14 || (neg && m >= 9) || m <= -9 || exponent);

				if (exp) {
					if (m < 0) {
						m -= 1;
					}
					num /= std::powf(10.0f, m);
					m1 = m;
					m = 0;
				}
				if (m < 1) {
					m = 0;
				}

				i32 p = precision;
				while ((num > EPSILON_F || m >= -precision) && p > 0) {
					f32 weight = powf(10.0f, m);
					if (weight > 0.0f && !isinf(weight)) {
						d = std::floorf(num / weight);
						num -= (d * weight);
						res.append(digit(d));
					}
					if (m == 0) {
						res.append('.');
					}
					if (m < 0) {
						--p;
					}
					--m;
				}

				if (exp) {
					i32 i, j;
					res.append(capitalized ? 'E' : 'e');
					if (m1 >= 0) {
						res.append('+');
					}
					else {
						res.append('-');
						m1 = -m1;
					}
					m = 0;
					string_type exp_str;
					do {
						exp_str.prepend(digit(m1 % 10));
						m1 /= 10;
					} while (m1 > 0);
					res.append(exp_str);
				}

				if (zero) {
					b8 inserted = (i32)res.length() < width;
					if (inserted) {
						res.prepend(string_type('0', width - (i32)res.length()));
					}
					if (inserted) {
						if (neg) {
							res[0] = '-';
						}
						else if (plus) {
							res[0] = '+';
						}
						else if (space) {
							res[0] = ' ';
						}
					}
					else {
						if (neg) {
							res.prepend('-');
						}
						else if (plus) {
							res.prepend('+');
						}
						else if (space) {
							res.prepend(' ');
						}
					}
				}
			}

			return string_formatter::format(res);
		}

		string_type floating_point_formatter::format(f64 _value) const noexcept {
			string_type res;
			if (std::isnan(_value)) {
				res = capitalized ? "NAN" : "nan";
			}
			else if (std::isinf(_value)) {
				res = capitalized ? "INF" : "inf";
				if (_value >= 0.0) {
					if (plus) {
						res.prepend('+');
					}
					else if (space) {
						res.prepend(" ");
					}
				}
				else {
					res.prepend('-');
				}
			}
			else if (_value == 0.0) {
				b8 neg = _value < 0.0;

				res.append('0');
				i32 p = precision;
				if (p) {
					res.append('.');
				}
				while (p--) {
					res.append('0');
				}

				if (zero) {
					b8 inserted = (i32)res.length() < width;
					if (inserted) {
						res.prepend(string_type('0', width - (i32)res.length()));
					}
					if (inserted) {
						if (neg) {
							res[0] = '-';
						}
						else if (plus) {
							res[0] = '+';
						}
						else if (space) {
							res[0] = ' ';
						}
					}
					else {
						if (neg) {
							res.prepend('-');
						}
						else if (plus) {
							res.prepend('+');
						}
						else if (space) {
							res.prepend(' ');
						}
					}
				}
			}
			else {
				b8 neg = _value < 0.0f;
				f64 num = neg ? -_value : _value;
				i32 m = std::log10(num), m1, d;
				b8 exp = (m >= 14 || (neg && m >= 9) || m <= -9 || exponent);

				if (exp) {
					if (m < 0) {
						m -= 1;
					}
					num /= std::powf(10.0f, m);
					m1 = m;
					m = 0;
				}
				if (m < 1) {
					m = 0;
				}

				i32 p = precision;
				while ((num > EPSILON_F || m >= -precision) && p > 0) {
					f64 weight = pow(10.0, m);
					if (weight > 0.0 && !isinf(weight)) {
						d = std::floor(num / weight);
						num -= (d * weight);
						res.append(digit(d));
					}
					if (m == 0) {
						res.append('.');
					}
					if (m < 0) {
						--p;
					}
					--m;
				}

				if (exp) {
					i32 i, j;
					res.append(capitalized ? 'E' : 'e');
					if (m1 >= 0) {
						res.append('+');
					}
					else {
						res.append('-');
						m1 = -m1;
					}
					m = 0;
					string_type exp_str;
					do {
						exp_str.prepend(digit(m1 % 10));
						m1 /= 10;
					} while (m1 > 0);
					res.append(exp_str);
				}

				if (zero) {
					b8 inserted = (i32)res.length() < width;
					if (inserted) {
						res.prepend(string_type('0', width - (i32)res.length()));
					}
					if (inserted) {
						if (neg) {
							res[0] = '-';
						}
						else if (plus) {
							res[0] = '+';
						}
						else if (space) {
							res[0] = ' ';
						}
					}
					else {
						if (neg) {
							res.prepend('-');
						}
						else if (plus) {
							res.prepend('+');
						}
						else if (space) {
							res.prepend(' ');
						}
					}
				}
			}

			return string_formatter::format(res);
		}

		b8 null_formatter::parse(const format_context& _ctx) noexcept {
			/*
			*	Format: {[width[align]][cap]}
			*		width : <uint>
			*		align : { '<', '=', '>' }^1
			*		cap	  : { '^' }^1
			*/

			auto it = _ctx.begin();

			if (it != _ctx.end()) {
				u32 num = 0;
				u32 len = to_uint(it, num);
				if (len) {
					width = num;
					it += len;

					if (it == _ctx.end()) {
						return true;
					}

					if (*it == '<') {
						align = -1;
						++it;
					}
					else if (*it == '=') {
						align = 0;
						++it;
					}
					else if (*it == '>') {
						align = 1;
						++it;
					}
				}
			}

			if (it != _ctx.end()) {
				if (*it == '^') {
					null_capitalized = true;
					++it;
				}
			}

			return (it == _ctx.end());
		}

		string_type null_formatter::format(null_type _value) const noexcept {
			return string_formatter::format(null_capitalized ? "NULL" : "null");
		}

		b8 datetime_formatter::parse(const format_context& _ctx) noexcept {
			formatter_2.zero  = true;
			formatter_2.width = 2;
			formatter_4.zero  = true;
			formatter_4.width = 4;
			return _ctx.begin() == _ctx.end();
		}

		string_type datetime_formatter::format(const datetime& _value) const noexcept {
			string_type res = "";

			res.append(formatter_4.format(_value.year()));
			res.append('/');
			res.append(formatter_2.format(_value.month()));
			res.append('/');
			res.append(formatter_2.format(_value.day()));
			res.append('-');
			res.append(formatter_2.format(_value.hour()));
			res.append(':');
			res.append(formatter_2.format(_value.minute()));
			res.append(':');
			res.append(formatter_2.format(_value.second()));

			return res;
		}

	} // namespace utility
} // namespace ivy
