#pragma once

#include "thread_pool.h"
#include "format.h"
#include "formatter.h"
#include "platform/console.h"
#include "platform/filesystem.h"

namespace ivy {
	namespace utility {

		class logger {
		public:
			typedef platform::console::log_level log_level;

		public:
			IVY_API logger(u32 _threads_count) noexcept;

			IVY_API virtual ~logger() noexcept;

		public:
			template<typename... Args>
			b8 log(log_level _level, const template_type& _template, Args&&... _args) noexcept;

			IVY_API virtual b8 write(log_level _level, const string_type& _string) noexcept;

		private:
			mutable thread_pool m_format_pool;
			mutable thread_pool m_output_pool;
		};

		struct log_level_formatter {
			IVY_API b8 parse(const format_context& _ctx) noexcept;

			IVY_API string_type format(typename logger::log_level _value) const noexcept;
		};

		template<> struct formatter<typename logger::log_level> : log_level_formatter { };

		class console_logger : public logger {
		public:
			IVY_API console_logger(u32 _threads_count) noexcept;

			IVY_API ~console_logger() noexcept;

			IVY_API b8 write(log_level _level, const string_type& _string) noexcept;
		};

		class file_logger : public logger {
		public:
			IVY_API file_logger(u32 _threads_count, const string_type& _filepath) noexcept;

			IVY_API ~file_logger() noexcept;

			IVY_API b8 write(log_level _level, const string_type& _string) noexcept;

		private:
			platform::file m_file;
		};

	} // namespace utility

	typedef utility::logger::log_level log_level;

} // namespace ivy

#include "../impl/utility/logger.inl"
