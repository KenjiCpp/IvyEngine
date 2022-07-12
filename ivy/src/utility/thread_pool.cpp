#include "utility/thread_pool.h"

#include "platform/common.h"
#include "platform/memory.h"

namespace ivy {
	namespace utility {

		thread_pool::thread_pool() noexcept :
			m_threads_count(platform::common::hardware_concurrency()),
			m_threads(nullptr),
			m_stop(false) {
			assert(init());
		}

		thread_pool::thread_pool(u32 _threads_count) noexcept :
			m_threads_count(_threads_count),
			m_threads(nullptr),
			m_stop(false) {
			assert(init());
		}

		thread_pool::~thread_pool() {
			if (m_threads) {
				{
					unique_lock<mutex> lock(m_mtx);
					m_stop = true;
				}
				m_cv.notify_all();
				for (i32 i = 0; i < m_threads_count; ++i) {
					m_threads[i].join();
					(m_threads + i)->~thread();
				}
				platform::memory::deallocate(m_threads);
			}
		}

		u32 thread_pool::threads_count() const noexcept {
			return m_threads_count;
		}

		b8 thread_pool::init() noexcept {
			thread* threads = (thread*)platform::memory::allocate(m_threads_count * sizeof(thread));
			if (!threads)
				return false;

			m_threads = threads;
			for (i32 i = 0; i < m_threads_count; ++i) {
				new (m_threads + i) thread([this] {
					for (;;) {
						job task;
						{
							unique_lock<mutex> lock(this->m_mtx);
							this->m_cv.wait(lock, [this] { 
								return m_stop || !m_jobs.empty(); 
							});
							if (m_stop && m_jobs.empty()) {
								return;
							}
							
							task = std::move(m_jobs.front());
							m_jobs.pop_front();
						}
						task();
					}
				});
			}
			return true;
		}

	} // namespace utility
} // namespace ivy
