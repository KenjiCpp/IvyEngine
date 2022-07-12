namespace ivy {
	namespace utility {

		template<typename T>
		string_type type_name() noexcept {
			static string_type res = typeid(T).name();
			return res;
		}

		template<typename T>
		b8 formatter<T>::parse(const format_context& _ctx) noexcept {
			return true;
		}

		template<typename T>
		string_type formatter<T>::format(const T& _value) const noexcept {
			string_type res = "<undefined formatter - ";
			res.append(type_name<T>());
			res.append(">");
			return res;
		}

		template<typename T>
		b8 container_formatter<T>::parse(const format_context& _ctx) noexcept {
			/*
			*	Format: {[width[align]][value_format]}
			*		width        : <uint>
			*		align        : { '<', '=', '>' }^1
			*		value_format : <format for value type>
			*/
			auto it = _ctx.begin();


			if (it != _ctx.end()) {
				u32 num = 0;
				u32 len = to_uint(it, num);
				if (len) {
					width = num;
					it += len;
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
				u32 offset = it - _ctx.begin();
				format_context val_ctx;
				u32 start, end;
				b8 error = false;

				if (find_format_context(_ctx, offset, val_ctx, start, end, error)) {
					if (start == offset) {
						it += end - start;
						if (!formatter<T>::parse(val_ctx)) {
							return false;
						}
					}
					else {
						return false;
					}
				}
				else if (error) {
					return false;
				}
				++it;
			}

			return (it == _ctx.end());
		}

		template<typename T>
		template<template<typename> typename C>
		string_type container_formatter<T>::format(const C<T>& _value) const noexcept {
			string_type res;
			res.append("[ ");
			if (_value.empty()) {
				res.append("]");
			}
			else {
				auto it = _value.begin();
				res.append(formatter<T>::format(*(it++)));
				while (it != _value.end()) {
					res.append(", ");
					res.append(formatter<T>::format(*(it++)));
				}
				res.append(" ]");
			}

			return string_formatter::format(res);
		}

		template<typename T>
		b8 set_formatter<T>::parse(const format_context& _ctx) noexcept {
			/*
			*	Format: {[width[align]][value_format]}
			*		width        : <uint>
			*		align        : { '<', '=', '>' }^1
			*		value_format : <format for value type>
			*/
			auto it = _ctx.begin();


			if (it != _ctx.end()) {
				u32 num = 0;
				u32 len = to_uint(it, num);
				if (len) {
					width = num;
					it += len;
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
				u32 offset = it - _ctx.begin();
				format_context val_ctx;
				u32 start, end;
				b8 error = false;

				if (find_format_context(_ctx, offset, val_ctx, start, end, error)) {
					if (start == offset) {
						it += end - start;
						if (!formatter<T>::parse(val_ctx)) {
							return false;
						}
					}
					else {
						return false;
					}
				}
				else if (error) {
					return false;
				}
				++it;
			}

			return (it == _ctx.end());
		}

		template<typename T>
		template<template<typename> typename C>
		string_type set_formatter<T>::format(const C<T>& _value) const noexcept {
			string_type res;
			res.append("{ ");
			if (_value.empty()) {
				res.append("}");
			}
			else {
				auto it = _value.begin();
				res.append(formatter<T>::format(*(it++)));
				while (it != _value.end()) {
					res.append(", ");
					res.append(formatter<T>::format(*(it++)));
				}
				res.append(" }");
			}

			return string_formatter::format(res);
		}

		template<typename KeyT, typename T>
		b8 map_formatter<KeyT, T>::parse(const format_context& _ctx) noexcept {
			/*
			*	Format: {[width[align]][key_format][:value_format]}
			*		width        : <uint>
			*		align        : { '<', '=', '>' }^1
			*		value_format : <format for value type>
			*/
			auto it = _ctx.begin();


			if (it != _ctx.end()) {
				u32 num = 0;
				u32 len = to_uint(it, num);
				if (len) {
					width = num;
					it += len;
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
				if (*it != ':') {
					if (it != _ctx.end()) {
						u32 offset = it - _ctx.begin();
						format_context val_ctx;
						u32 start, end;
						b8 error = false;

						if (find_format_context(_ctx, offset, val_ctx, start, end, error)) {
							if (start == offset) {
								it += end - start;
								if (!key_formatter.parse(val_ctx)) {
									return false;
								}
							}
							else {
								return false;
							}
						}
						else if (error) {
							return false;
						}
						++it;
					}

					if (it != _ctx.end()) {
						if (*it == ':') {
							++it;
							if (it != _ctx.end()) {
								u32 offset = it - _ctx.begin();
								format_context val_ctx;
								u32 start, end;
								b8 error = false;

								if (find_format_context(_ctx, offset, val_ctx, start, end, error)) {
									if (start == offset) {
										it += end - start;
										if (!value_formatter.parse(val_ctx)) {
											return false;
										}
									}
									else {
										return false;
									}
								}
								else if (error) {
									return false;
								}
								++it;
							}
							else {
								return false;
							}
						}
					}
				}
				else {
					++it;
					if (it != _ctx.end()) {
						u32 offset = it - _ctx.begin();
						format_context val_ctx;
						u32 start, end;
						b8 error = false;

						if (find_format_context(_ctx, offset, val_ctx, start, end, error)) {
							if (start == offset) {
								it += end - start;
								if (!value_formatter.parse(val_ctx)) {
									return false;
								}
							}
							else {
								return false;
							}
						}
						else if (error) {
							return false;
						}
						++it;
					}
					else {
						return false;
					}
				}
			}

			return (it == _ctx.end());
		}

		template<typename KeyT, typename T>
		template<template<typename, typename> typename M>
		string_type map_formatter<KeyT, T>::format(const M<KeyT, T>& _value) const noexcept {
			string_type res;
			res.append("{ ");
			if (_value.empty()) {
				res.append("}");
			}
			else {
				auto it = _value.begin();
				res.append(key_formatter.format(it->key()));
				res.append(": ");
				res.append(value_formatter.format(it->value()));
				++it;
				while (it != _value.end()) {
					res.append(", ");
					res.append(key_formatter.format(it->key()));
					res.append(": ");
					res.append(value_formatter.format(it->value()));
					++it;
				}
				res.append(" }");
			}

			return string_formatter::format(res);
		}

		template<typename T>
		b8 nullable_formatter<T>::parse(const format_context& _ctx) noexcept {
			/*
			*	Format: {[width[align]][cap][value_format]}
			*		width  : <uint>
			*		align  : { '<', '=', '>' }^1
			*		cap	   : { '^' }^1
			*		format : <format for value type>
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

			if (it != _ctx.end()) {
				u32 offset = it - _ctx.begin();
				format_context val_ctx;
				u32 start, end;
				b8 error = false;

				if (find_format_context(_ctx, offset, val_ctx, start, end, error)) {
					if (start == offset) {
						it += end - start;
						if (!formatter<T>::parse(val_ctx)) {
							return false;
						}
					}
					else {
						return false;
					}
				}
				else if (error) {
					return false;
				}
				++it;
			}

			return (it == _ctx.end());
		}

		template<typename T>
		string_type nullable_formatter<T>::format(const nullable<T>& _value) const noexcept {
			if (_value.is_null()) {
				return null_formatter::format(null);
			}
			return string_formatter::format(formatter<T>::format(_value.get()));
		}

	} // namespace utility
} // namespace ivy
