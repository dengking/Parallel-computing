/*
 * trade_datetime.h
 *
 *  Created on: 2019��6��27��
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

/// `struct tm` (broken-down time) �Ƿǳ��ʺ�����Ϊ��ʾ�����յģ�����ʱ���ֶ�ȫ������Ϊ0��
/// ����`struct tm`�ǿ���ת��ΪUnix time��ת��Ϊstd::chrono:system_clock:time_point��
/// ���Ϊʱ������ṩ�ǳ���ı���
/// ����ʱ��Ľṹ��ʹ��:
/// std::chrono::hours // ʱ
/// std::chrono::minutes // ��
/// std::chrono::seconds // ��

namespace CppDateAndTime {
/// ʹ��`struct tm`����ʾ�����գ���`struct tm`��ʱ���ֶ�ȫ������Ϊ0
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
		std::tm tm_ = { }; // initialization�ǳ���Ҫ������ʱ�佫�ǳ���׼ȷ
		tm_.tm_year = tm.tm_year;
		tm_.tm_mon = tm.tm_mon;
		tm_.tm_mday = tm.tm_mday;
		time_point_ = CppDateAndTime::ClockT::from_time_t(mktime(&tm_));
	}
	/// ��Ҫ�������local timeת��Ϊstruct tm�ĸ�ʽ
	/// @param year
	/// @param month
	/// @param day
	void from_time_t(const unsigned int year, const unsigned int month,
			const unsigned int day) {
		std::tm tm_ = { }; // initialization�ǳ���Ҫ������ʱ�佫�ǳ���׼ȷ
		tm_.tm_year = year - 1900;
		tm_.tm_mon = month - 1;
		tm_.tm_mday = day;
		time_point_ = CppDateAndTime::ClockT::from_time_t(mktime(&tm_));
	}
	/// std output
	friend std::ostream& operator<<(std::ostream &out, TradeDate* p_obj) {
		out << "��������(" << p_obj->to_string() << ")";
		return out;
	}
	/// std output
	friend std::ostream& operator<<(std::ostream &out, TradeDate& obj) {

		out << "��������(" << obj.to_string() << ")";
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

/// ��ʾʱ��
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
			// TODO ����һ���쳣����Ϊ������޷������ģ���ʱӦ�ý��쳣�׳�������ʹ��usable_�ֶ�����ʾ�Ƿ����
			throw std::runtime_error("�޷�����:" + t);
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
	std::chrono::hours hour_; //ʱ
	std::chrono::minutes miniute_; //��
	std::chrono::seconds second_; //��
};
/// ��ʾ����ʱ�䣬ͨ�����TradeDate��TradeTime����
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
		std::time_t t = s.count(); // ����
		struct tm tm_info;
		localtime_r(&t, &tm_info); // ���ػ�
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

/// \brief ����ʱ���
class TradeTimePeriod {
public:
	typedef TradeTime TimeT; //ʱ������
	TradeTimePeriod(const TimeT & begin, const TimeT& end) :
			begin_(begin), end_(end) {

	}
	bool include(TimeT & t) {
		return begin_ <= t and t < end_; // ��Ҫע�⣬���ܹ��Ǳ����䣬��Ҫ���ұգ�����Ǳ�����Ļ����������߼�����
	}
public:

	TimeT begin_; //��ʼʱ��
	TimeT end_; //����ʱ��
};

/// \brief ��`~/workspace/configuration/non_trade_date.csv`�ж�ȡ**�ǽ�����**��Ϣ���ڴ���
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
	/// \brief ִ�ж�ȡ
	/// @return ��ȡ�Ľ����յĸ���
	virtual int read() {
		strtk::token_grid::options options;
		options.column_delimiters = ",";
		options.support_dquotes = true;
		strtk::token_grid grid(file_, options);
		if (grid.row_count() == 0) {
			/// ����������ļ������ڣ����н����գ�����������£�Ҳ��Ϊ��ȡ�ɹ�
			LogWarning("�ļ�["<<file_<<"]�����ڻ��ļ�����Ϊ�գ�ϵͳ�����жϵ�ǰ�����Ƿ��ǽ�����");
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

	/// ��ӡtrade_days_�����ڲ���
	void print() {
		for (TradeDate day : trade_days_) {
			LogFlow(day);
		}
	}
	/// �����һ��������
	TradeDate next_trade_date() {
		TradeDate today = TradeDate::now();
		int future_days = 1;
		while (1) {
			today.future(future_days);
			if (include(today)) { // next���ǽ�����
				++future_days;
			} else {
				return today;
			}
		}
	}
	/// �����һ��������
	TradeDate next_trade_date(TradeDate date) {
		int future_days = 1;
		while (1) {
			date.future(future_days);
			if (include(date)) { // next���ǽ�����
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

/// \brief ��ȡ����ʱ��
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
			LogError("�ļ�[" << file_ << "]�����ڻ��ļ�����Ϊ��");
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
	/// \brief �ж�ʱ��t�Ƿ��ڽ���ʱ��
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
	/// \brief �жϵ�ǰʱ���Ƿ��ڽ���ʱ��
	/// \brief �жϵ�ǰʱ���Ƿ��ڽ���ʱ���
	bool include(TradeDateTime& datetime) {
		return include(datetime.time_);
	}

	void print() {
		for (auto const& period : periods_) {
			LogFlow("["<<period.begin_<<","<<period.end_<<"]");
		}
	}
	/// ���罻���տ���ʱ��
	TradeTime AM_open_time() {
		return periods_[0].begin_;
	}
	/// ���罻���ձ���ʱ��
	TradeTime AM_close_time() {
		return periods_[0].end_;
	}
	/// ���罻���տ���ʱ��
	TradeTime PM_open_time() {
		return periods_[1].begin_;
	}
	/// ���罻���ձ���ʱ��
	TradeTime PM_close_time() {
		return periods_[1].end_;
	}
private:
	std::vector<TradeTimePeriod> periods_;
	std::string file_; //�ļ�����
};

/// ʹ�ñ���������ʱ�䴦���ĸ�����ʱ���
enum class DateTimeInTradePeriodEnum {
	NonTradeDate,  // �ǽ�����
	/// �����ǽ�����
	BeforeAMTradePeriod, // ���罻����֮ǰ
	InAMTradePeriod, // �������罻��ʱ���
	InNoonClose, // ������������
	InPMTradePeriod, // �������罻��ʱ���
	AfterPMTradePeriod // �������罻��ʱ���֮��
};

/// ����ʱ�����Ϣ
struct TradePeriodInfo {
	DateTimeInTradePeriodEnum in_which_trade_period_; //��ǰʱ�䴦���ĸ�����ʱ���
	TradeDateTime now_;
	TradeDateTime AM_open_date_time_; // ���翪��ʱ��
	TradeDateTime AM_close_date_time_;
	TradeDateTime PM_open_date_time_; // ���翪��ʱ��
	TradeDateTime PM_close_date_time_;
};

/// \brief �����գ�����ʱ��
/// ����ʱ��ľ������루S������������൱��CPU�����Ƿǳ�����׼��
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

	/// ���δ������Ŀ�������ʱ��
	TradeDateTime get_next_open_datetime() {
		TradeDateTime datetime;
		TradeTime AM_open_time = trade_time_reader_.AM_open_time();
		TradeTime PM_open_time = trade_time_reader_.PM_open_time();
		/// �����жϽ����Ƿ��ǽ�����
		if (non_trade_date_reader_.not_include(datetime)) {
			if (datetime.time_ < AM_open_time) { // ��ǰʱ�������翪��ʱ��֮ǰ
				return TradeDateTime(datetime.date_, AM_open_time);
			} else if (datetime.time_ < PM_open_time) { // ��ǰʱ�������翪��ʱ��֮ǰ
				return TradeDateTime(datetime.date_, PM_open_time);
			} else {
				return TradeDateTime(
						non_trade_date_reader_.next_trade_date(datetime.date_),
						AM_open_time); // ��һ�������յ����翪��ʱ��
			}
		} else {
			return TradeDateTime(
					non_trade_date_reader_.next_trade_date(datetime.date_),
					AM_open_time); // ��һ�������յ����翪��ʱ��
		}
	}

	/// ���δ������ı���ʱ��
	TradeDateTime get_next_close_datetime() {
		TradeDateTime datetime;
		TradeTime AM_close_time = trade_time_reader_.AM_close_time();
		TradeTime PM_close_time = trade_time_reader_.PM_close_time();
		/// �����жϽ����Ƿ��ǽ�����
		if (non_trade_date_reader_.not_include(datetime)) {
			if (datetime.time_ < AM_close_time) {
				return TradeDateTime(datetime.date_, AM_close_time);
			} else if (datetime.time_ < PM_close_time) {
				return TradeDateTime(datetime.date_, PM_close_time);
			} else {
				return TradeDateTime(
						non_trade_date_reader_.next_trade_date(datetime.date_),
						AM_close_time); // ��һ�������յ����翪��ʱ��
			}
		} else {
			return TradeDateTime(
					non_trade_date_reader_.next_trade_date(datetime.date_),
					AM_close_time); // ��һ�������յ����翪��ʱ��
		}
	}

	/// ��ý���ʱ�����Ϣ
	void get_trade_period_info(TradePeriodInfo& trade_period_info) {
		TradeDateTime datetime;
		trade_period_info.now_ = datetime;

		/// �����ж��Ƿ��ǽ�����
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
			/// ��Ҫע����ǣ�����ʱ��ľ������루S���������ڻ���ʱ�������ʱ��һ��Ҫע��ֽ�㴦�Ĵ��������ѭ��ͬ�Ŀ���ԭ��
			/// ����Ļ��ֲ�ȡ����ҿ���ԭ��
			if (datetime.time_ < AM_open_time) { // ��ǰʱ�������翪��ʱ��֮ǰ
				trade_period_info.in_which_trade_period_ =
						DateTimeInTradePeriodEnum::BeforeAMTradePeriod;
			} else if (datetime.time_ < AM_close_time) { // ��ǰʱ�������罻��ʱ�����
				trade_period_info.in_which_trade_period_ =
						DateTimeInTradePeriodEnum::InAMTradePeriod;
			} else if (datetime.time_ < PM_open_time) { // ��ǰʱ�䴦����������ʱ�����
				trade_period_info.in_which_trade_period_ =
						DateTimeInTradePeriodEnum::InNoonClose;
			} else if (datetime.time_ < PM_close_time) { // ��ǰʱ�䴦�����罻��ʱ�����
				trade_period_info.in_which_trade_period_ =
						DateTimeInTradePeriodEnum::InPMTradePeriod;
			} else { // ��ǰʱ�䴦�ڱ���
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
