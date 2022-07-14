#include "platform/datetime.h"

#include "platform/memory.h"

namespace ivy {
	namespace platform {

		datetime::datetime() noexcept :
			m_time_info(nullptr) {
			tm* new_time_info = (tm*)memory::allocate(sizeof(tm));
			assert(new_time_info);

			m_time_info = new_time_info;

			time_t rawtime;
			time(&rawtime);
			tm* tm_now = localtime(&rawtime);
			m_time_info->tm_year  = tm_now->tm_year;
			m_time_info->tm_mon   = tm_now->tm_mon;
			m_time_info->tm_mday  = tm_now->tm_mday;
			m_time_info->tm_hour  = tm_now->tm_hour;
			m_time_info->tm_min   = tm_now->tm_min;
			m_time_info->tm_sec   = tm_now->tm_sec;
			m_time_info->tm_isdst = tm_now->tm_isdst;
			m_time_info->tm_wday  = tm_now->tm_wday;
			m_time_info->tm_yday  = tm_now->tm_yday;
		}

		datetime::datetime(i32 _year, i32 _month, i32 _day) noexcept :
			m_time_info(nullptr) {
			tm* new_time_info = (tm*)memory::allocate(sizeof(tm));
			assert(new_time_info);

			m_time_info = new_time_info;

			assert(_month >= 1 && _month <= 12);
			assert(_day >= 1);
			if (_month == 1 || _month == 3 || _month == 5 || _month == 7 || _month == 8 || _month == 10 || _month == 12) {
				assert(_day <= 31);
			}
			else if (_month != 2) {
				assert(_day <= 30);
			}
			else if (is_leap_year(_year)) {
				assert(_day <= 29);
			}
			else {
				assert(_day <= 28);
			}

			m_time_info->tm_year  = _year - 1900;
			m_time_info->tm_mon   = _month - 1;
			m_time_info->tm_mday  = _day;
			m_time_info->tm_hour  = 0;
			m_time_info->tm_min   = 0;
			m_time_info->tm_sec   = 0;
			m_time_info->tm_isdst = -1;
		}

		datetime::datetime(i32 _year, i32 _month, i32 _day, i32 _hour, i32 _minute, i32 _second) noexcept :
			m_time_info(nullptr) {
			tm* new_time_info = (tm*)memory::allocate(sizeof(tm));
			assert(new_time_info);

			m_time_info = new_time_info;

			assert(_month >= 1 && _month <= 12);
			assert(_day >= 1);
			if (_month == 1 || _month == 3 || _month == 5 || _month == 7 || _month == 8 || _month == 10 || _month == 12) {
				assert(_day <= 31);
			}
			else if (_month != 2) {
				assert(_day <= 30);
			}
			else if (is_leap_year(_year)) {
				assert(_day <= 29);
			}
			else {
				assert(_day <= 28);
			}
			assert(_hour   >= 0 && _hour   <= 23);
			assert(_minute >= 0 && _minute <= 59);
			assert(_second >= 0 && _second <= 59);

			m_time_info->tm_year  = _year - 1900;
			m_time_info->tm_mon   = _month - 1;
			m_time_info->tm_mday  = _day;
			m_time_info->tm_hour  = _hour;
			m_time_info->tm_min   = _minute;
			m_time_info->tm_sec   = _second;
			m_time_info->tm_isdst = -1;
		}

		datetime::datetime(datetime&& _source) noexcept :
			m_time_info(_source.m_time_info) {
			_source.m_time_info = nullptr;
		}

		datetime::datetime(const datetime& _other) noexcept :
			m_time_info(nullptr) {
			tm* new_time_info = (tm*)memory::allocate(sizeof(tm));
			assert(new_time_info);

			m_time_info = new_time_info;

			m_time_info->tm_year  = _other.m_time_info->tm_year;
			m_time_info->tm_mon   = _other.m_time_info->tm_mon;
			m_time_info->tm_mday  = _other.m_time_info->tm_mday;
			m_time_info->tm_hour  = _other.m_time_info->tm_hour;
			m_time_info->tm_min   = _other.m_time_info->tm_min;
			m_time_info->tm_sec   = _other.m_time_info->tm_sec;
			m_time_info->tm_isdst = _other.m_time_info->tm_isdst;
			m_time_info->tm_wday  = _other.m_time_info->tm_wday;
			m_time_info->tm_yday  = _other.m_time_info->tm_yday;
		}

		datetime::~datetime() {
			if (m_time_info) {
				memory::deallocate(m_time_info);
			}
		}

		datetime& datetime::operator=(datetime&& _source) noexcept {
			if (this != &_source) {
				if (m_time_info) {
					memory::deallocate(m_time_info);
				}

				m_time_info = _source.m_time_info;

				_source.m_time_info = nullptr;
			}

			return *this;
		}

		datetime& datetime::operator=(const datetime& _other) noexcept {
			if (this != &_other) {
				if (!m_time_info) {
					tm* new_time_info = (tm*)memory::allocate(sizeof(tm));
					assert(new_time_info);

					m_time_info = new_time_info;
				}

				if (_other.m_time_info) {
					m_time_info->tm_year  = _other.m_time_info->tm_year;
					m_time_info->tm_mon   = _other.m_time_info->tm_mon;
					m_time_info->tm_mday  = _other.m_time_info->tm_mday;
					m_time_info->tm_hour  = _other.m_time_info->tm_hour;
					m_time_info->tm_min   = _other.m_time_info->tm_min;
					m_time_info->tm_sec   = _other.m_time_info->tm_sec;
					m_time_info->tm_isdst = _other.m_time_info->tm_isdst;
					m_time_info->tm_wday  = _other.m_time_info->tm_wday;
					m_time_info->tm_yday  = _other.m_time_info->tm_yday;
				}
			}

			return *this;
		}

		i32 datetime::year() const noexcept {
			return m_time_info->tm_year + 1900;
		}

		i32 datetime::month() const noexcept {
			return m_time_info->tm_mon + 1;
		}

		i32 datetime::day() const noexcept {
			return m_time_info->tm_mday;
		}

		i32 datetime::hour() const noexcept {
			return m_time_info->tm_hour;
		}

		i32 datetime::minute() const noexcept {
			return m_time_info->tm_min;
		}

		i32 datetime::second() const noexcept {
			return m_time_info->tm_sec;
		}

		i32 datetime::weekday() const noexcept {
			return m_time_info->tm_wday;
		}

		b8 datetime::is_leap_year() const noexcept {
			return is_leap_year(year());
		}

		b8 datetime::is_leap_year(i32 _year) noexcept {
			return ((_year % 4 == 0 && _year % 100 != 0) || (_year % 400 == 0));
		}

	} // namespace platform
} // namespace ivy
