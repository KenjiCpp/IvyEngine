#pragma once

#include "formatter.h"

namespace ivy {
	namespace utility {

		template<typename... Args>
		string_type format(const template_type& _template, Args&&... _args) noexcept;

	} // namespace utility

	using utility::format;

} // namespace ivy

#include "../impl/utility/format.inl"
