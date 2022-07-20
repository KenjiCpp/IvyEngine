#pragma once

#include "utility/format.h"

namespace ivy {
	namespace utility {

		namespace details {
			template<>
			b8 _format(const template_type& _template, string_type& _res, u32 _offset) noexcept {
				utility::format_context ctx;
				b8  err = false;
				u32 start = 0;
				u32 end = 0;

				while (utility::find_format_context(_template, _offset, ctx, start, end, err)) {
					_res.append(_template.subview(_offset, start - _offset));
					_res.append("<missing argument>");
					_offset = end + 1;
				}
				_res.append(_template.subview(_offset));

				return (!err);
			}
		} // namespace details

	} // namespace utility
} // namespace ivy
