#ifndef CORE_DATETIME_TYPEDEF_H_
#define CORE_DATETIME_TYPEDEF_H_

#include <chrono>
namespace CppDateAndTime {
typedef std::chrono::system_clock ClockT;
typedef std::chrono::time_point<ClockT> TimePointT;
typedef std::chrono::milliseconds IntervalT; // ʱ��������
}

#endif /* CORE_DATETIME_TYPEDEF_H_ */
