#ifndef CORE2_Clock_DATETIME_UTILITY_H_
#define CORE2_Clock_DATETIME_UTILITY_H_

#include <sys/time.h>
#include <sys/types.h>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <ctime>
#include <string>
#include <time.h>
#include "datetime_typedef.h"

namespace CppDateAndTime {

template<typename Clockesolution>
struct Clock {
	static std::string now() {
		return "";
	}
};

template<>
struct Clock<std::chrono::seconds> {
	/// \brief href:https://stackoverflow.com/questions/10289017/how-to-format-date-and-time-string-in-c
	/// 本函数的功能是获得当前时间，精度为s，返回值是字符串表示
	static std::string now() {
		/// Number of sec since January 1,1970
		time_t now_sec = time(0);
		struct tm now_tm;
		localtime_r(&now_sec, &now_tm);
		char buffer[36];
		strftime(buffer, sizeof(buffer), "%Y%m%d %H:%M:%S", &now_tm);

		return std::string(buffer);
	}

};

template<>
struct Clock<std::chrono::milliseconds> {
	/// \brief https://stackoverflow.com/a/32983646
	/// 本函数的功能是获得当前时间，精度为ms，返回值是字符串表示
	static std::string now() {
		char buffer[26];
		int millisec; // ms
		struct tm tm_info;
		struct timeval tv;
		gettimeofday(&tv, NULL);
		millisec = lrint(tv.tv_usec / 1000.0); // Round to nearest millisec
		if (millisec >= 1000) { // Allow for rounding up to nearest second
			millisec -= 1000;
			tv.tv_sec++;
		}
		localtime_r(&tv.tv_sec, &tm_info);
		std::strftime(buffer, sizeof(buffer), "%Y%m%d %H:%M:%S", &tm_info);
		std::sprintf(buffer, "%s.%03d", buffer, millisec);
		return buffer;
	}
	/// https://stackoverflow.com/a/12836048
	/// 将time_point本地化后然后转换为字符串
	/// @param time_point
	/// @return
	static std::string to_string(CppDateAndTime::TimePointT &time_point) {
		std::chrono::milliseconds ms = std::chrono::duration_cast<
				std::chrono::milliseconds>(time_point.time_since_epoch());
		std::chrono::seconds s =
				std::chrono::duration_cast<std::chrono::seconds>(ms);
		std::time_t t = s.count(); // 秒数
		std::size_t fractional_seconds = ms.count() % 1000; // 毫秒数
		struct tm tm_info;
		/// localtime_r是线程安全的
		localtime_r(&t, &tm_info); // 本地化

		char buffer[26] = { 0 };
		std::strftime(buffer, sizeof(buffer), "%Y%m%d %H:%M:%S", &tm_info);
		std::sprintf(buffer, "%s.%03d", buffer, fractional_seconds);
		return buffer;

	}
};

}

#endif /* CORE2_Clock_DATETIME_UTILITY_H_ */
