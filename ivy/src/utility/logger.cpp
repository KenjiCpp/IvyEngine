#include "utility/logger.h"

namespace ivy {
	namespace utility {

		logger::logger(u32 _threads_count) noexcept :
			m_format_pool(_threads_count),
			m_output_pool(1) {
		}

		logger::~logger() noexcept {
		}

		b8 logger::write(log_level _level, const string_type& _string) noexcept {
			return true;
		}

		b8 log_level_formatter::parse(const format_context& _ctx) noexcept {
			return _ctx.empty();
		}

		string_type log_level_formatter::format(typename logger::log_level _value) const noexcept {
			switch (_value)
			{
			case logger::log_level::Fatal : return "FATAL";
			case logger::log_level::Error : return "ERROR";
			case logger::log_level::Warn  : return "WARN";
			case logger::log_level::Info  : return "INFO";
			case logger::log_level::Debug : return "DEBUG";
			case logger::log_level::Trace : return "TRACE";
			default                       : return "UNKNOWN";
			}
		}

		console_logger::console_logger(u32 _threads_count) noexcept :
			logger(_threads_count) {
		}

		console_logger::~console_logger() noexcept {
		}

		b8 console_logger::write(log_level _level, const string_type& _string) noexcept {
			string_type str = format(">>> [{}] {}\n", _level, _string);
			platform::console::write(str.cstr(), _level);
			return true;
		}

		file_logger::file_logger(u32 _threads_count, const string_type& _filepath) noexcept :
			logger(_threads_count),
			m_file(_filepath, platform::file::access_mode::Write) {
		}

		file_logger::~file_logger() noexcept {
		}

		b8 file_logger::write(log_level _level, const string_type& _string) noexcept {
			if (m_file.is_valid()) {
				string_type str = format(">>> [{}] {}\n", _level, _string);
				m_file.write(str.cstr(), str.length());
				return true;
			}

			return false;
		}

	} // namespace utility
} // namespace ivy
