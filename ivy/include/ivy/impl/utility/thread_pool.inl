namespace ivy {
	namespace utility {

		template<typename F, typename... Args>
		future<result_type<F, Args...>> thread_pool::enqueue(F _f, Args&&... _args) noexcept {
			using return_type = result_type<F, Args...>;

			auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(_f), std::forward<Args>(_args)...));

			future<return_type> res = task->get_future();
			{
				unique_lock<mutex> lock(m_mtx);
				assert(!m_stop);
				m_jobs.emplace_back([task] { (*task)(); });
			}
			m_cv.notify_one();
			return res;
		}

		template<template<typename> typename Ctn, typename F, typename... ArgCtns>
		Ctn<result_type<F, typename container_traits<ArgCtns>::value_type...>> thread_pool::map(F _f, ArgCtns&&... _arg_ctns) noexcept {
			using return_type = result_type<F, typename container_traits<ArgCtns>::value_type...>;

			if constexpr (sizeof...(ArgCtns) == 0) {
				return Ctn<return_type>();
			}

			Ctn<future<return_type>> futures;

			for_each([this, &futures, _f](typename container_traits<ArgCtns>::value_type... _args) {
				futures.emplace_back(this->enqueue(_f, _args...));
				}, std::forward<ArgCtns>(_arg_ctns)...);

			Ctn<return_type> res;
			for (auto it = futures.begin(); it != futures.end(); ++it)
				res.emplace_back(it->get());
			return res;
		}

	} // namespace utility
} // namespace ivy
