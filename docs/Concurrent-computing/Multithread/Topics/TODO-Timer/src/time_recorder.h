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

/// \brief Ĭ�ϵļ�¼����ʵ����һ���ռ�¼���������غ�ʱ
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
/// \brief ��¼�ĵ���־�ļ���
struct FileRecorder {
	FileRecorder() {

	}
	/// \brief ��¼��ʱ
	template<typename TimeResolution = std::chrono::microseconds>
	unsigned long operator()(const std::chrono::steady_clock::time_point& begin,
			const std::chrono::steady_clock::time_point& end) {
		int duration =
				std::chrono::duration_cast<TimeResolution>(end - begin).count();
		LogFlow(duration);
		return duration;
	}
	/// \brief ��¼ʱ��
	template<typename TimeResolution = std::chrono::milliseconds>
	void operator()(const std::string& key, const int& turn) {
		std::string t = quote_predict_utility::Timer<TimeResolution>::now();
		LogFlow(key<<"��["<<turn<<"]��ʱ��Ϊ"<<t);
	}
};

/// \brief ��¼��redis��
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
	/// ��¼��ʱ
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
	/// ��¼ʱ��
	/// \param turn �ڼ���
	/// \param key ��ֵ
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

/// \brief ��¼ʱ��
/// ����`now()`����ǰʱ�̼�¼����
template<typename Recorder>
class TimeRecorder {
public:
	TimeRecorder(Recorder&& recorder) :
			recorder_(recorder) {
	}
	virtual ~TimeRecorder() {

	}
	/// ��¼��ǰʱ��ֵ
	/// \param turn �ڼ���
	/// \param key ��ֵ
	void now(const std::string& key, const int& turn) {
		recorder_(key, turn);
	}
private:
	Recorder recorder_;
};

/// \brief ��¼��ʱ
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
	/// \brief ��ʼ��ʱ
	void begin() {
		begin_ = std::chrono::steady_clock::now();
	}
	/// \brief ֹͣ��ʱ��������ʱ��������
	/// Ĭ�ϵ�ʱ��ֱ���Ϊ΢��
	unsigned long end() {
		end_ = std::chrono::steady_clock::now();
		return recorder_.template operator()<TimeResolution>(begin_, end_);
	}
private:
	/// ��¼��ʱ
	std::chrono::steady_clock::time_point begin_;
	std::chrono::steady_clock::time_point end_;
	Recorder recorder_;
};

#endif /* CORE_ELAPSED_TIME_RECORDER_H_ */
