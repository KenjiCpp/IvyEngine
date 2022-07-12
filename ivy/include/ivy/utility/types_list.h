#pragma once

namespace ivy {
	namespace utility {

		template<typename... Ts>
		struct types_list {};

		template<typename T, typename... Ts>
		struct types_list<T, Ts...> {
			typedef T                 first;
			typedef types_list<Ts...> rest;
		};

	} // namespace utility
} // namespace ivy
