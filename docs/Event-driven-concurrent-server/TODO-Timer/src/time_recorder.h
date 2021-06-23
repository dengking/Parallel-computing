#ifndef CORE_ELAPSED_TIME_RECORDER_H_
#define CORE_ELAPSED_TIME_RECORDER_H_

#include <algorithm>
#include <array>
#include <chrono>
#include <fstream>
#include <sstream>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

/// \brief 默认的记录器，实际是一个空记录，仅仅返回耗时
struct DefaultRecorder {
	DefaultRecorder() {

	}
	template<typename TimeResolution = std::chrono::microseconds>
	unsigned long operator()(const std::chrono::steady_clock::time_point& begin,
			const std::chrono::steady_clock::time_point& end) {
		int duration =
				std::chrono::duration_cast<TimeResolution>(end - begin).count();
		return duration;
	}
};
/// \brief 记录的到日志文件中
struct FileRecorder {
	FileRecorder() {

	}
	/// \brief 记录耗时
	template<typename TimeResolution = std::chrono::microseconds>
	unsigned long operator()(const std::chrono::steady_clock::time_point& begin,
			const std::chrono::steady_clock::time_point& end) {
		int duration =
				std::chrono::duration_cast<TimeResolution>(end - begin).count();
		LogFlow(duration);
		return duration;
	}
	/// \brief 记录时刻
	template<typename TimeResolution = std::chrono::milliseconds>
	void operator()(const std::string& key, const int& turn) {
		std::string t = quote_predict_utility::Timer<TimeResolution>::now();
		LogFlow(key<<"第["<<turn<<"]轮时刻为"<<t);
	}
};

/// \brief 记录到redis中
struct RedisRecorder {
public:
	RedisRecorder() {
	}
	/// Ctor
	RedisRecorder(const std::string key) :
			key_(key) {
	}
	/// Dtor
	~RedisRecorder() {
	}
	/// copy Ctor
	RedisRecorder(RedisRecorder& other) {
		key_ = other.key_;
	}
	/// move Ctor
	RedisRecorder(RedisRecorder&& other) {
		key_ = other.key_;
	}
	/// copy assignment operator
	RedisRecorder & operator=(RedisRecorder other) {
		other.swap(*this);
		return *this;
	}

	void swap(RedisRecorder & other) {
		std::swap(this->key_, other.key_);
	}
	/// 记录耗时
	template<typename RedisT,
			typename TimeResolution = std::chrono::microseconds>
	unsigned long operator()(RedisT& redis,
			const std::chrono::steady_clock::time_point& begin,
			const std::chrono::steady_clock::time_point& end) {
		int duration =
				std::chrono::duration_cast<TimeResolution>(end - begin).count();
		redis.lpush_list(key_, duration);
		return duration;
	}
	/// 记录时刻
	/// \param turn 第几轮
	/// \param key 键值
	template<typename RedisT,
			typename TimeResolution = std::chrono::milliseconds>
	void operator()(RedisT& redis, const std::string& key, const int& turn) {
		std::string value = std::to_string(turn) + "_"
				+ quote_predict_utility::Timer<TimeResolution>::now();
		redis.lpush(key, value);
	}
private:
	std::string key_;
};

/// \brief 记录时刻
/// 调用`now()`将当前时刻记录下来
template<typename Recorder>
class TimeRecorder {
public:
	TimeRecorder(Recorder&& recorder) :
			recorder_(recorder) {
	}
	virtual ~TimeRecorder() {

	}
	/// 记录当前时刻值
	/// \param turn 第几轮
	/// \param key 键值
	void now(const std::string& key, const int& turn) {
		recorder_(key, turn);
	}
private:
	Recorder recorder_;
};

/// \brief 记录耗时
template<typename Recorder = DefaultRecorder,
		typename TimeResolution = std::chrono::microseconds>
class ElapsedTimeRecorder {
public:
	ElapsedTimeRecorder(Recorder&& recorder) :
			recorder_(recorder) {
	}
	ElapsedTimeRecorder() {
		recorder_ = Recorder();
	}
	virtual ~ElapsedTimeRecorder() {
	}
	/// \brief 开始计时
	void begin() {
		begin_ = std::chrono::steady_clock::now();
	}
	/// \brief 停止计时，并将耗时保存起来
	/// 默认的时间分辨率为微秒
	unsigned long end() {
		end_ = std::chrono::steady_clock::now();
		return recorder_.template operator()<TimeResolution>(begin_, end_);
	}
private:
	/// 记录耗时
	std::chrono::steady_clock::time_point begin_;
	std::chrono::steady_clock::time_point end_;
	Recorder recorder_;
};

#endif /* CORE_ELAPSED_TIME_RECORDER_H_ */
