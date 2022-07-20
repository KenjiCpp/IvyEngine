#pragma once

#include "array.h"
#include "deque.h"
#include "container.h"

namespace ivy {
	namespace utility {

		class IVY_API thread_pool {
		private:
			typedef function<void> job;

		public:
			thread_pool() noexcept;
			thread_pool(u32 _threads_count) noexcept;

			~thread_pool();

			u32 threads_count() const noexcept;

			template<typename F, typename... Args>
			future<result_type<F, Args...>> enqueue(F _f, Args&&... _args) noexcept;

			template<template<typename> typename Ctn, typename F, typename... ArgCtns>
			Ctn<result_type<F, typename container_traits<ArgCtns>::value_type...>> map(F _f, ArgCtns&&... _arg_ctns) noexcept;

		private:
			b8 init() noexcept;

		private:
			u32				   m_threads_count;
			thread*			   m_threads;
			mutex			   m_mtx;
			condition_variable m_cv;
			b8				   m_stop;
			deque<job>		   m_jobs;
		};

	} // namespace utility

	using utility::thread_pool;

} // namespace ivy

#include "../impl/utility/thread_pool.inl"
