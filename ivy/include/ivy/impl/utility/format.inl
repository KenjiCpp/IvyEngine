namespace ivy {
	namespace utility {

		namespace details {

			template<typename... Args>
			b8 format(const template_type& _template, string_type& _res, u32 _offset, Args&&... _args) noexcept;

			template<typename T, typename... Args>
			b8 format(const template_type& _template, string_type& _res, u32 _offset, T&& _t, Args&&... _args) noexcept {
				utility::format_context ctx;
				b8  err   = false;
				u32 start = 0;
				u32 end   = 0;

				if (utility::find_format_context(_template, _offset, ctx, start, end, err)) {
					_res.append(_template.subview(_offset, start - _offset));
					formatter<remove_const_reference_type<T>> fmt;
					if (fmt.parse(ctx)) {
						_res.append(fmt.format(std::forward<T>(_t)));
					}
					else {
						_res.append("<invalid format>");
					}
					return format(_template, _res, end + 1, std::forward<Args>(_args)...);
				}
				else {
					_res.append(_template.subview(_offset));
				}

				return (!err);
			}

			template<>
			b8 format(const template_type& _template, string_type& _res, u32 _offset) noexcept {
				utility::format_context ctx;
				b8  err   = false;
				u32 start = 0;
				u32 end   = 0;

				while (utility::find_format_context(_template, _offset, ctx, start, end, err)) {
					_res.append(_template.subview(_offset, start - _offset));
					_res.append("<missing argument>");
					_offset = end + 1;
				}
				_res.append(_template.subview(_offset));

				return (!err);
			}

		} // namespace details

		

		template<typename... Args>
		string_type format(const template_type& _template, Args&&... _args) noexcept {
			string_type res(_template.length());
			if (!details::format(_template, res, 0, std::forward<Args>(_args)...)) {
				return "<invalid template>";
			}
			res.shrink_to_fit();
			return res;
		}

	} // namespace utility
} // namespace ivy
