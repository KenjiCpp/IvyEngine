#pragma once

#include <ctime>

namespace ivy {
	namespace platform {

		class IVY_API datetime {
		public:
			static constexpr i32 Sunday    = 0;
			static constexpr i32 Monday    = 1;
			static constexpr i32 Tuesday   = 2;
			static constexpr i32 Wednesday = 3;
			static constexpr i32 Thursday  = 4;
			static constexpr i32 Friday    = 5;
			static constexpr i32 Saturday  = 6;

		public:
			datetime() noexcept;

			datetime(i32 _year, i32 _month, i32 _day) noexcept;
			datetime(i32 _year, i32 _month, i32 _day, i32 _hour, i32 _minute, i32 _second) noexcept;

			datetime(datetime&& _source) noexcept;
			datetime(const datetime& _other) noexcept;

			~datetime();

			datetime& operator=(datetime&& _source) noexcept;
			datetime& operator=(const datetime& _other) noexcept;

			i32 year() const noexcept;
			i32 month() const noexcept;
			i32 day() const noexcept;
			i32 hour() const noexcept;
			i32 minute() const noexcept;
			i32 second() const noexcept;

			i32 weekday() const noexcept;

			b8 is_leap_year() const noexcept;
			static b8 is_leap_year(i32 _year) noexcept;

		private:
			tm* m_time_info;

		};

	} // namespace platform

	using platform::datetime;

} // namespace ivy
