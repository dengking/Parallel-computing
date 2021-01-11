/*
 * trade_datetime.h
 *
 *  Created on: 2019年6月27日
 *      Author: dengkai17334
 */

#ifndef CORE_DATETIME_TRADE_DATETIME_H_
#define CORE_DATETIME_TRADE_DATETIME_H_

#include <sys/types.h>
#include <chrono>
#include <cstddef>
#include <ctime>
#include "time.h"
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "../../core/datetime/datetime_typedef.h"
#include "../../core/datetime/datetime_utility.h"
#include "../../third_party/strtk/strtk.hpp"

/// `struct tm` (broken-down time) 是非常适合于作为表示交易日的（将其时间字段全部设置为0）
/// 并且`struct tm`是可以转换为Unix time，转换为std::chrono:system_clock:time_point的
/// 这就为时间操作提供非常大的便利
/// 交易时间的结构的使用:
/// std::chrono::hours // 时
/// std::chrono::minutes // 分
/// std::chrono::seconds // 秒

namespace CppDateAndTime {
/// 使用`struct tm`来表示交易日，将`struct tm`的时间字段全部设置为0
class TradeDate {
public:
	TradeDate() {
	}
	TradeDate(CppDateAndTime::TimePointT& time_point) :
			time_point_(time_point) {
	}
	TradeDate(const std::string& date) {
		int year = std::stoi(date.substr(0, 4));
		int month = std::stoi(date.substr(4, 2));
		int day = std::stoi(date.substr(6, 2));
		from_time_t(year, month, day);
	}
	TradeDate(const unsigned int year, const unsigned int month,
			const unsigned int day) {
		from_time_t(year, month, day);
	}
	TradeDate(std::tm& tm) {
		std::tm tm_ = { }; // initialization非常重要，否则时间将非常不准确
		tm_.tm_year = tm.tm_year;
		tm_.tm_mon = tm.tm_mon;
		tm_.tm_mday = tm.tm_mday;
		time_point_ = CppDateAndTime::ClockT::from_time_t(mktime(&tm_));
	}
	/// 需要将传入的local time转换为struct tm的格式
	/// @param year
	/// @param month
	/// @param day
	void from_time_t(const unsigned int year, const unsigned int month,
			const unsigned int day) {
		std::tm tm_ = { }; // initialization非常重要，否则时间将非常不准确
		tm_.tm_year = year - 1900;
		tm_.tm_mon = month - 1;
		tm_.tm_mday = day;
		time_point_ = CppDateAndTime::ClockT::from_time_t(mktime(&tm_));
	}
	/// std output
	friend std::ostream& operator<<(std::ostream &out, TradeDate* p_obj) {
		out << "交易日期(" << p_obj->to_string() << ")";
		return out;
	}
	/// std output
	friend std::ostream& operator<<(std::ostream &out, TradeDate& obj) {

		out << "交易日期(" << obj.to_string() << ")";
		return out;
	}
	std::string to_string() {
		return CppDateAndTime::Clock<std::chrono::milliseconds>::to_string(
				time_point_);
	}

	void future(int day) {
		time_point_ += std::chrono::hours(24 * day);
	}

	static TradeDate now() {
		/// Number of sec since January 1,1970
		time_t now_sec = time(0);
		struct tm now_tm;
		localtime_r(&now_sec, &now_tm);
		return TradeDate(now_tm.tm_year + 1900, now_tm.tm_mon + 1,
				now_tm.tm_mday);
	}

	bool operator<(const TradeDate& other) const {
		return time_point_ < other.time_point_;
	}
public:
	CppDateAndTime::TimePointT time_point_;
};

/// 表示时间
class TradeTime {
public:
	TradeTime(const unsigned int &hour, const unsigned int &miniute,
			const unsigned int &second) :
			hour_(hour), miniute_(miniute), second_(second) {
	}
	TradeTime() :
			hour_(0), miniute_(0), second_(0) {
	}
	TradeTime(std::tm& tm) :
			hour_(tm.tm_hour), miniute_(tm.tm_min), second_(tm.tm_sec) {
	}
	std::string to_string() const {
		return std::to_string(hour_.count()) + ":"
				+ std::to_string(miniute_.count()) + ":"
				+ std::to_string(second_.count());
	}
	/// std output
	friend std::ostream& operator<<(std::ostream &out, const TradeTime* p_obj) {
		out << "[" << p_obj->to_string() << "]";
		return out;
	}
	/// std output
	friend std::ostream& operator<<(std::ostream &out, const TradeTime& obj) {
		out << "[" << obj.to_string() << "]";
		return out;
	}
	static TradeTime now() {
		/// Number of sec since January 1,1970
		time_t now_sec = time(0);
		struct tm now_tm;
		localtime_r(&now_sec, &now_tm);
		return TradeTime(now_tm.tm_hour, now_tm.tm_min, now_tm.tm_sec);
	}

	void parse(const std::string &t, TradeTime& trade_time) {
		trade_time = parse(t);
	}

	static TradeTime parse(const std::string &t) {
		if (t.size() == 5) {
			return TradeTime(std::stoi(t.substr(0, 1)),
					std::stoi(t.substr(1, 2)), std::stoi(t.substr(3, 2)));
		} else if (t.size() == 6) {
			return TradeTime(std::stoi(t.substr(0, 2)),
					std::stoi(t.substr(2, 2)), std::stoi(t.substr(4, 2)));
		} else {
			// TODO 这是一种异常，因为入参是无法解析的，此时应该将异常抛出，现在使用usable_字段来表示是否可用
			throw std::runtime_error("无法解析:" + t);
		}
	}
	bool operator>(const TradeTime& other) {
		if (hour_ > other.hour_) {
			return true;
		}
		if (hour_ == other.hour_ and miniute_ > other.miniute_) {
			return true;
		}
		return (hour_ == other.hour_ and miniute_ == other.miniute_
				and second_ > other.second_);
	}

	bool operator<(const TradeTime& other) {
		return not (*this > other) and not (*this == other);
	}
	bool operator==(const TradeTime& other) {
		return hour_ == other.hour_ and miniute_ == other.miniute_
				and second_ == other.second_;
	}
	bool operator >=(const TradeTime& other) {
		return (*this > other) or (*this == other);
	}
	bool operator <=(const TradeTime& other) {
		return not (*this > other);
	}
public:
	std::chrono::hours hour_; //时
	std::chrono::minutes miniute_; //分
	std::chrono::seconds second_; //秒
};
/// 表示日期时间，通过组合TradeDate和TradeTime而成
///
class TradeDateTime {
public:
	TradeDateTime(TradeDate date, TradeTime time) {
		time_point_ = date.time_point_;
		time_point_ += time.hour_;
		time_point_ += time.miniute_;
		time_point_ += time.second_;
		set_date_time();
	}
	TradeDateTime() {
		time_point_ = CppDateAndTime::ClockT::now();
		set_date_time();
	}
	/// https://codereview.stackexchange.com/questions/156695/converting-stdchronotime-point-to-from-stdstring
	void set_date_time() {
		std::chrono::milliseconds ms = std::chrono::duration_cast<
				std::chrono::milliseconds>(time_point_.time_since_epoch());
		std::chrono::seconds s =
				std::chrono::duration_cast<std::chrono::seconds>(ms);
		std::time_t t = s.count(); // 秒数
		struct tm tm_info;
		localtime_r(&t, &tm_info); // 本地化
		date_ = TradeDate(tm_info);
		time_ = TradeTime(tm_info);
	}
	/// std output
	friend std::ostream& operator<<(std::ostream &out, TradeDateTime& p_obj) {
		out << p_obj.date_;
		out << p_obj.time_;
		return out;
	}
	long long to_timestamp() {
		std::chrono::seconds s =
				std::chrono::duration_cast<std::chrono::seconds>(
						time_point_.time_since_epoch());
		return s.count();
	}
public:
	CppDateAndTime::TimePointT time_point_;

	TradeDate date_;
	TradeTime time_;
};

/// \brief 交易时间段
class TradeTimePeriod {
public:
	typedef TradeTime TimeT; //时间类型
	TradeTimePeriod(const TimeT & begin, const TimeT& end) :
			begin_(begin), end_(end) {

	}
	bool include(TimeT & t) {
		return begin_ <= t and t < end_; // 需要注意，不能够是闭区间，需要左开右闭；如果是闭区间的话，则会存在逻辑问题
	}
public:

	TimeT begin_; //开始时间
	TimeT end_; //结束时间
};

/// \brief 从`~/workspace/configuration/non_trade_date.csv`中读取**非交易日**信息到内存中
class NonTradeDateReader {
	typedef std::set<TradeDate> TradeDays_t;
public:
	NonTradeDateReader() {

	}

	NonTradeDateReader(const std::string& file) :
			file_(file) {
	}
	virtual ~NonTradeDateReader() {
	}
	/// \brief 执行读取
	/// @return 读取的交易日的个数
	virtual int read() {
		strtk::token_grid::options options;
		options.column_delimiters = ",";
		options.support_dquotes = true;
		strtk::token_grid grid(file_, options);
		if (grid.row_count() == 0) {
			/// 如果交易日文件不存在，则不判交易日，在这种情况下，也认为读取成功
			LogWarning("文件["<<file_<<"]不存在或文件内容为空，系统将不判断当前日期是否是交易日");
			return true;
		}
		/// for each row r, for each column c
		for (std::size_t r = 0; r < grid.row_count(); ++r) {
			strtk::token_grid::row_type row = grid.row(r);
			std::string date = row.get<std::string>(0);
			trade_days_.emplace(date);
		}
		return true;
	}

	const TradeDays_t& get_trade_days() const {
		return trade_days_;
	}

	bool not_include(TradeDateTime& datetime) {
		return not_include(datetime.date_);
	}

	bool not_include(const TradeDate& t) {
		return not include(t);
	}

	bool include(TradeDateTime& datetime) {
		return include(datetime.date_);
	}
	bool include(const TradeDate& t) {
		if (trade_days_.size() > 0) {
			return trade_days_.find(t) != trade_days_.end();
		} else {
			return true;
		}
	}

	/// 打印trade_days_，便于测试
	void print() {
		for (TradeDate day : trade_days_) {
			LogFlow(day);
		}
	}
	/// 获得下一个交易日
	TradeDate next_trade_date() {
		TradeDate today = TradeDate::now();
		int future_days = 1;
		while (1) {
			today.future(future_days);
			if (include(today)) { // next不是交易日
				++future_days;
			} else {
				return today;
			}
		}
	}
	/// 获得下一个交易日
	TradeDate next_trade_date(TradeDate date) {
		int future_days = 1;
		while (1) {
			date.future(future_days);
			if (include(date)) { // next不是交易日
				++future_days;
			} else {
				return date;
			}
		}
	}
private:
	TradeDays_t trade_days_;
	std::string file_;
};

/// \brief 读取交易时间
class TradeTimeReader {
public:
	TradeTimeReader() {

	}
	TradeTimeReader(const std::string& file) :
			file_(file) {

	}
	virtual ~TradeTimeReader() {
	}
	virtual bool read() {
		strtk::token_grid::options options;
		options.column_delimiters = ",";
		options.support_dquotes = true;
		strtk::token_grid grid(file_, options);
		if (grid.row_count() == 0) {
			LogError("文件[" << file_ << "]不存在或文件内容为空");
			return false;
		}

		/// for each row r, for each column c
		for (std::size_t r = 0; r < grid.row_count(); ++r) {
			strtk::token_grid::row_type row = grid.row(r);
			auto col_0 = row.get<std::string>(0);
			auto col_1 = row.get<std::string>(1);
			auto start = TradeTime::parse(col_0);
			auto end = TradeTime::parse(col_1);
			periods_.emplace_back(start, end);
		}
		return periods_.size() > 0;
	}
	/// \brief 判断时间t是否处于交易时间
	bool include(TradeTimePeriod::TimeT & t) {
		for (auto& period : periods_) {
			if (period.include(t)) {
				return true;
			} else {
				continue;
			}
		}
		return false;
	}
	/// \brief 判断当前时间是否处于交易时间
	/// \brief 判断当前时间是否处于交易时间段
	bool include(TradeDateTime& datetime) {
		return include(datetime.time_);
	}

	void print() {
		for (auto const& period : periods_) {
			LogFlow("["<<period.begin_<<","<<period.end_<<"]");
		}
	}
	/// 上午交易日开市时间
	TradeTime AM_open_time() {
		return periods_[0].begin_;
	}
	/// 上午交易日闭市时间
	TradeTime AM_close_time() {
		return periods_[0].end_;
	}
	/// 下午交易日开市时间
	TradeTime PM_open_time() {
		return periods_[1].begin_;
	}
	/// 下午交易日闭市时间
	TradeTime PM_close_time() {
		return periods_[1].end_;
	}
private:
	std::vector<TradeTimePeriod> periods_;
	std::string file_; //文件名称
};

/// 使用本类来描述时间处于哪个交易时间段
enum class DateTimeInTradePeriodEnum {
	NonTradeDate,  // 非交易日
	/// 以下是交易日
	BeforeAMTradePeriod, // 上午交易日之前
	InAMTradePeriod, // 处于上午交易时间段
	InNoonClose, // 处于中午休市
	InPMTradePeriod, // 处于下午交易时间段
	AfterPMTradePeriod // 处于下午交易时间段之后
};

/// 交易时间段信息
struct TradePeriodInfo {
	DateTimeInTradePeriodEnum in_which_trade_period_; //当前时间处于哪个交易时间段
	TradeDateTime now_;
	TradeDateTime AM_open_date_time_; // 上午开市时间
	TradeDateTime AM_close_date_time_;
	TradeDateTime PM_open_date_time_; // 下午开市时间
	TradeDateTime PM_close_date_time_;
};

/// \brief 交易日，交易时间
/// 交易时间的精度是秒（S），这个精度相当于CPU而言是非常不精准的
class TradeDateTimeReader {
public:
	TradeDateTimeReader() {

	}
	TradeDateTimeReader(const std::string& nontrade_date_file,
			const std::string& trade_time_file) :
			trade_date_file_(nontrade_date_file), trade_time_file_(
					trade_time_file), non_trade_date_reader_(
					NonTradeDateReader(trade_date_file_)), trade_time_reader_(
					TradeTimeReader(trade_time_file_)) {

	}

	bool read() {
		return trade_time_reader_.read() and non_trade_date_reader_.read();
	}

	bool in_trade_time() {
		TradeDateTime datetime;
		return non_trade_date_reader_.not_include(datetime)
				and trade_time_reader_.include(datetime);
	}

	NonTradeDateReader& get_non_trade_date_reader() {
		return non_trade_date_reader_;
	}

	TradeTimeReader& get_trade_time_reader() {
		return trade_time_reader_;
	}

	/// 获得未来最近的开市日期时间
	TradeDateTime get_next_open_datetime() {
		TradeDateTime datetime;
		TradeTime AM_open_time = trade_time_reader_.AM_open_time();
		TradeTime PM_open_time = trade_time_reader_.PM_open_time();
		/// 首先判断今天是否是交易日
		if (non_trade_date_reader_.not_include(datetime)) {
			if (datetime.time_ < AM_open_time) { // 当前时间在上午开市时间之前
				return TradeDateTime(datetime.date_, AM_open_time);
			} else if (datetime.time_ < PM_open_time) { // 当前时间在下午开市时间之前
				return TradeDateTime(datetime.date_, PM_open_time);
			} else {
				return TradeDateTime(
						non_trade_date_reader_.next_trade_date(datetime.date_),
						AM_open_time); // 下一个交易日的上午开市时间
			}
		} else {
			return TradeDateTime(
					non_trade_date_reader_.next_trade_date(datetime.date_),
					AM_open_time); // 下一个交易日的上午开市时间
		}
	}

	/// 获得未来最近的闭市时间
	TradeDateTime get_next_close_datetime() {
		TradeDateTime datetime;
		TradeTime AM_close_time = trade_time_reader_.AM_close_time();
		TradeTime PM_close_time = trade_time_reader_.PM_close_time();
		/// 首先判断今天是否是交易日
		if (non_trade_date_reader_.not_include(datetime)) {
			if (datetime.time_ < AM_close_time) {
				return TradeDateTime(datetime.date_, AM_close_time);
			} else if (datetime.time_ < PM_close_time) {
				return TradeDateTime(datetime.date_, PM_close_time);
			} else {
				return TradeDateTime(
						non_trade_date_reader_.next_trade_date(datetime.date_),
						AM_close_time); // 下一个交易日的上午开市时间
			}
		} else {
			return TradeDateTime(
					non_trade_date_reader_.next_trade_date(datetime.date_),
					AM_close_time); // 下一个交易日的上午开市时间
		}
	}

	/// 获得交易时间段信息
	void get_trade_period_info(TradePeriodInfo& trade_period_info) {
		TradeDateTime datetime;
		trade_period_info.now_ = datetime;

		/// 首先判断是否是交易日
		if (non_trade_date_reader_.not_include(datetime)) {
			TradeTime AM_open_time = trade_time_reader_.AM_open_time();
			trade_period_info.AM_open_date_time_ = TradeDateTime(datetime.date_,
					AM_open_time);

			TradeTime AM_close_time = trade_time_reader_.AM_close_time();
			trade_period_info.AM_close_date_time_ = TradeDateTime(
					datetime.date_, AM_close_time);

			TradeTime PM_open_time = trade_time_reader_.PM_open_time();
			trade_period_info.PM_open_date_time_ = TradeDateTime(datetime.date_,
					PM_open_time);

			TradeTime PM_close_time = trade_time_reader_.PM_close_time();
			trade_period_info.PM_close_date_time_ = TradeDateTime(
					datetime.date_, PM_close_time);
			/// 需要注意的是，由于时间的精度是秒（S），所以在划分时间区间的时候一定要注意分界点处的处理，最好遵循相同的开闭原则
			/// 区间的划分采取左闭右开的原则
			if (datetime.time_ < AM_open_time) { // 当前时间在上午开市时间之前
				trade_period_info.in_which_trade_period_ =
						DateTimeInTradePeriodEnum::BeforeAMTradePeriod;
			} else if (datetime.time_ < AM_close_time) { // 当前时间在上午交易时间段内
				trade_period_info.in_which_trade_period_ =
						DateTimeInTradePeriodEnum::InAMTradePeriod;
			} else if (datetime.time_ < PM_open_time) { // 当前时间处于中午休市时间段内
				trade_period_info.in_which_trade_period_ =
						DateTimeInTradePeriodEnum::InNoonClose;
			} else if (datetime.time_ < PM_close_time) { // 当前时间处于下午交易时间段内
				trade_period_info.in_which_trade_period_ =
						DateTimeInTradePeriodEnum::InPMTradePeriod;
			} else { // 当前时间处于闭市
				trade_period_info.in_which_trade_period_ =
						DateTimeInTradePeriodEnum::AfterPMTradePeriod;
			}
		} else {
			trade_period_info.in_which_trade_period_ =
					DateTimeInTradePeriodEnum::NonTradeDate;
		}

	}

private:
	std::string trade_date_file_;
	std::string trade_time_file_;
	CppDateAndTime::TradeTimeReader trade_time_reader_;
	CppDateAndTime::NonTradeDateReader non_trade_date_reader_;
};
}
#endif /* CORE_DATETIME_TRADE_DATETIME_H_ */
