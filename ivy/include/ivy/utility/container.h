#pragma once

namespace ivy {
	namespace utility {

		// For each

		template<typename F, typename... ArgCtns>
		F for_each(F _f, ArgCtns&&... _arg_ctns) noexcept;

		// Container traits

		template<typename T>
		struct container_traits {
			using value_type    = decltype(*(std::declval<T>().begin()));
			using iterator_type = decltype(std::declval<T>().begin());
		};

	} // namespace utility
} // namespace ivy

#include "../impl/utility/container.inl"
