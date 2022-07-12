namespace ivy {
	namespace utility {

		namespace details {

			// Compare two pointer/iterator tuples,
			// If there's atleast one equal pair of elements, return true

			template<typename... Args>
			b8 one_equal(const tuple<Args...>& _left, const tuple<Args...>& _right) noexcept {
				if (_left._Myfirst._Val == _right._Myfirst._Val) {
					return true;
				}
				return one_equal(_left._Get_rest(), _right._Get_rest());
			}

			template<typename T>
			b8 one_equal(const tuple<T>& _left, const tuple<T>& _right) noexcept {
				return _left._Myfirst._Val == _right._Myfirst._Val;
			}

			// Increment a(n) pointer/iterator tuple by incrementing each element

			template<std::size_t I = 0, typename... Args>
			typename std::enable_if<I == sizeof...(Args), void>::type increment(tuple<Args...>& _tup) {
			}

			template<std::size_t I = 0, typename... Args>
			typename std::enable_if < I < sizeof...(Args), void>::type increment(tuple<Args...>& _tup) {
				++std::get<I>(_tup);
				increment<I + 1, Args...>(_tup);
			}

			// Generate index sequence

			template<int... Is>
			struct seq {
			};

			template<int N, int... Is>
			struct gen_seq : gen_seq<N - 1, N - 1, Is...> {
			};

			template<int... Is>
			struct gen_seq<0, Is...> : seq<Is...> {
			};

			// Dereferencing a(n) pointer/iterator tuple

			template<typename T>
			struct dereference {
				using type = decltype(*std::declval<T>());
			};

			template<typename... Args, int... Is>
			tuple<typename dereference<Args>::type...> dereference_pointer_tuple(tuple<Args...>& t, seq<Is...>) {
				return std::tie(*std::get<Is>(t)...);
			}

			template<typename... Args>
			tuple<typename dereference<Args>::type...> dereference_pointer_tuple(tuple<Args...>& t) {
				return dereference_pointer_tuple(t, gen_seq<sizeof...(Args)>());
			}

			// Unpack a tuple and execute a function using its elements

			template<typename F, typename... Args, int... Is>
			result_type<F, Args...> execute_on_tuple(F& _f, const tuple<Args...>& t, seq<Is...>) {
				return _f(std::get<Is>(t)...);
			}

			template<typename F, typename... Args>
			result_type<F, Args...> execute_on_tuple(F& _f, const std::tuple<Args...>& t) {
				return execute_on_tuple(_f, t, gen_seq<sizeof...(Args)>());
			}

		} // namespace details

		template<typename F, typename... ArgCtns>
		F for_each(F _f, ArgCtns&&... _arg_ctns) noexcept {
			auto b = std::make_tuple(_arg_ctns.begin()...);
			auto e = std::make_tuple(_arg_ctns.end()...);

			while (!details::one_equal(b, e)) {
				details::execute_on_tuple(_f, details::dereference_pointer_tuple(b));
				details::increment(b);
			}

			return _f;
		}

	} // namespace utility
} // namespace ivy
