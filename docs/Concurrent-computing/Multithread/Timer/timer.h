#ifndef CORE_DATETIME_TIMER_H_
#define CORE_DATETIME_TIMER_H_

/// timer的实现参考的是下面这篇文章：
/// https://codereview.stackexchange.com/questions/40473/portable-periodic-one-shot-timer-implementation
/// 在它的基础上进行了一定的扩展

/// 线程相关头文件
/// #include <thread>
/// #include <condition_variable>

#include <thread>
#include <condition_variable>
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <functional>
#include <iostream>
#include <list>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <string>
#include <vector>

#include "datetime_typedef.h"
#include "trade_datetime.h"

namespace CppDateAndTime {

typedef std::function<void(TimePointT&)> HandlerT;

/// \brief timed task
/// if the time expire,then the object will be invoke
class TimedTask {
public:
	typedef uint64_t IdT;
	/// 构建在一段时间内的periodic执行的task
	/// @param when_begin
	/// @param when_end
	/// @param interval
	/// @param handler
	template<typename FunctionT>
	TimedTask(const TimePointT& when_begin, const TimePointT& when_end,
			const IntervalT& interval, FunctionT&& handler,
			const std::string& name = "")
			noexcept :
			when_begin_(when_begin), when_end_(when_end), interval_(interval), handler_(
					std::forward<FunctionT>(handler)), name_(name) {
		init_time_flag();
		gen_id();
	}
	TimedTask() {
		name_ = "";
		handler_ = nullptr;
		when_begin_ = ClockT::now();
		init_time_flag();
		gen_id();
	}
	/// 构建一个periodic执行的task
	template<typename FunctionT>
	TimedTask(const TimePointT& when_begin, const IntervalT& interval,
			FunctionT&& handler, const std::string& name = "")
			noexcept :
			when_begin_(when_begin), interval_(interval), handler_(
					std::forward<FunctionT>(handler)), name_(name) {
		init_time_flag();
		gen_id();
	}
	/// 构建一个只执行一次的timed task
	template<typename FunctionT>
	TimedTask(const TimePointT& when_begin, FunctionT&& handler,
			const std::string& name = "")
			noexcept :
			when_begin_(when_begin), handler_(std::forward<FunctionT>(handler)), name_(
					name), interval_(0) {
		init_time_flag();
		gen_id();
	}
	/// CopyConstructible
	TimedTask(TimedTask const& e) noexcept :
			id_(e.id_), when_begin_(e.when_begin_), when_end_(e.when_end_), interval_(
					e.interval_), handler_(e.handler_), has_when_begin_(
					e.has_when_begin_), has_when_end_(e.has_when_end_), has_interval_(
					e.has_interval_), name_(e.name_) {
	}
	/// MoveConstructible
	TimedTask(TimedTask&& e) noexcept :
			id_(e.id_), when_begin_(e.when_begin_), when_end_(e.when_end_), interval_(
					e.interval_), handler_(std::move(e.handler_)), has_when_begin_(
					e.has_when_begin_), has_when_end_(e.has_when_end_), has_interval_(
					e.has_interval_), name_(e.name_) {
	}
	/// CopyAssignable
	TimedTask& operator=(TimedTask const& e) {
		if (this != &e) {
			id_ = e.id_;
			when_begin_ = e.when_begin_;
			when_end_ = e.when_end_;
			interval_ = e.interval_;
			handler_ = e.handler_;
			has_when_begin_ = e.has_when_begin_;
			has_when_end_ = e.has_when_end_;
			has_interval_ = e.has_interval_;
			name_ = e.name_;
		}
		return *this;
	}
	/// MoveAssignable
	TimedTask& operator=(TimedTask&& e) {
		if (this != &e) {
			id_ = e.id_;
			when_begin_ = e.when_begin_;
			when_end_ = e.when_end_;
			interval_ = e.interval_;
			handler_ = std::move(e.handler_);
			has_when_begin_ = e.has_when_begin_;
			has_when_end_ = e.has_when_end_;
			has_interval_ = e.has_interval_;
			name_ = e.name_;
		}
		return *this;
	}
	///
	/// 执行handler
	void operator()() {
		/// https://en.cppreference.com/w/cpp/utility/functional/function/operator_bool
		if (not handler_) {
			throw std::runtime_error("task的handler为空，不能够被调用");
		}
		// 调用handler的时候，传入由用户设置的期望该handler被执行的时间，我们称这个时间为，理想执行时间
		handler_(when_begin_);
	}
	/// std output
	friend std::ostream& operator<<(std::ostream &out, TimedTask* p_obj) {
		out << "定时任务[" << p_obj->id_ << "][" << p_obj->name_ << "]";
		return out;
	}
	/// std output
	friend std::ostream& operator<<(std::ostream &out, TimedTask& obj) {
		out << "定时任务[" << obj.id_ << "][" << obj.name_ << "]";
		return out;
	}

	/// 判断当前时间是否处于用户设置的开始时间，结束时间的时间段内
	/// @param now
	/// @return 如果用户既没有设置when_begin_，也没有设置when_end_，而仅仅设置了interval_，则此函数总是返回true
	bool between_begin_end(const TimePointT& now) {
		bool in = true;
		if (has_when_begin_)
			in = in and now >= when_begin_;
		if (has_when_end_)
			in = in and now <= when_end_;

		return in;
	}
	/// 判断当前时间是否已经超过了用户设置的结束时间
	/// 如果用户没有设置结束时间，则此函数返回false
	/// @param now
	/// @return
	bool end(const TimePointT& now) const {
		if (has_when_end_) {
			return now >= when_end_;
		} else
			return false;
	}
	///
	/// @return
	bool begin(const TimePointT& now) const {
		if (has_when_begin_) {
			return now >= when_begin_;
		} else
			return true;
	}
	/// 判断是否是periodic
	/// @return
	bool is_periodic() const {
		return has_interval_;
	}
	/// 调整下次执行时间，需要注意的是：如果下次执行时间依然晚于（小于）当前时间，则它必然会超时，则它必然会立刻被再次执行，为防止大量重复调用带来的风险，将其下次执行时间调整为当前时间加上执行间隔时间
	/// @param now 当前时间
	void once_more(const TimePointT& now) {
		if (now >= when_begin_ + interval_) {
			LogWarning(
					this<<"的下次执行时间晚于（小于）当前时间，则它必然会超时，为防止大量重复调用带来的风险，将其下次执行时间调整为当前时间加上执行间隔时间");
			when_begin_ = now;
		}
		when_begin_ += interval_;
	}
	void once_more() {
		TimePointT now = ClockT::now();
		once_more(now);
	}
	IntervalT get_interval() const {
		return interval_;
	}
	TimePointT get_when_begin() const {
		return when_begin_;
	}
private:
	void gen_id() {
		id_ = ID++;
	}
	void init_time_flag() {
		has_when_begin_ =
				when_begin_.time_since_epoch().count() > 0 ? true : false;
		has_when_end_ = when_end_.time_since_epoch().count() > 0 ? true : false;
		has_interval_ = interval_.count() > 0 ? true : false;
	}
public:

	std::string name_;
	HandlerT handler_; //
private:
	static IdT ID;
	IdT id_;

	// time_point that indicates when it needs to fire，这是由用户设置的期望该task被执行的时间，称这个时间为理想执行时间
	TimePointT when_begin_;
	TimePointT when_end_; // time_point that indicate when it needs to end

	/// 添加这三个has flag是出于性能考虑的，在用户没有设置的对应time_point的情况下， 无需去取系统时间而仅仅基于对应的flag就可以做出判断
	bool has_when_begin_; // whether user set when_begin_
	bool has_when_end_; // whether user set when_end_
	bool has_interval_; // whether user set interval_
	IntervalT interval_; // to imitate setInterval in JavaScripte

};

struct TimedTaskLess: public std::less<TimedTask> {
	bool operator()(const TimedTask &e1, const TimedTask &e2) const {
		return (e2.get_when_begin() < e1.get_when_begin());
	}
};

/// 根据时间来调度向其注册的task
/// 计时和任务的执行在同一个thread完成，所以任务的执行可能会导致计时不准确
class SyncTimer {
public:
	typedef std::unique_lock<std::mutex> ScopedLockT; // RAII
public:
	/// 刚刚启动的时候，默认是暂停的，需要用户显式调用resume()来启动定时器
	SyncTimer(bool pause_timer = false);

	SyncTimer(SyncTimer const& t) = delete; // not CopyConstructible
	SyncTimer& operator=(SyncTimer const& t) = delete; // not CopyAssignable
	/// TODO Timer是moveConstructable and moveAssignable
	virtual ~SyncTimer();

	void add_task(const TimePointT& when_begin, const TimePointT& when_end,
			const IntervalT& interval, HandlerT&& handler,
			const std::string& name = "");
	void add_task(const TimePointT& when_begin, const TimePointT& when_end,
			const IntervalT& interval, const HandlerT& handler,
			const std::string& name = "");

	void add_task(const TimePointT& when_begin, const IntervalT& interval,
			HandlerT&& handler, const std::string& name = "");
	void add_task(const TimePointT& when_begin, const IntervalT& interval,
			const HandlerT& handler, const std::string& name = "");

	void add_task(const TimePointT& when_begin, HandlerT&& handler,
			const std::string& name = "");
	void add_task(const TimePointT& when_begin, const HandlerT& handler,
			const std::string& name = "");
	/// 暂停timer
	void pause();
	/// 重启timer
	void resume();
	/// std output
	friend std::ostream& operator<<(std::ostream &out, SyncTimer* p_obj) {
		p_obj->print(out);
		return out;
	}
	/// 清空任务队列
	/// https://stackoverflow.com/a/2852183
	void clear_task_queue() {
		task_queue_ = std::priority_queue<TimedTask, std::vector<TimedTask>,
				TimedTaskLess>();
	}
	/// 判断任务队列是否为空
	bool task_queue_empty() {
		return task_queue_.empty();
	}
protected:
	/// 向Timer中添加Timertask
	virtual void add_task(TimedTask && task);

	/// timer_thread_的线程执行函数
	void timer_thread_start_rtn();
	/// 最终执行的函数，子类override
	virtual void timer(); // time_thread_ execute this function to time
	///
	virtual void print(std::ostream &out) const {
		out << "SyncTimer";
	}
protected:
	std::thread timer_thread_; // thread to time

	std::priority_queue<TimedTask, std::vector<TimedTask>, TimedTaskLess> task_queue_; // the TimedTask queue

	std::mutex timer_mutex_;

	std::condition_variable timer_cv_;

	bool stop_timer_; // 停止time标志，表示要停止线程的执行，即退出线程执行函数
	bool pause_timer_; // 暂停time标志
};

/// 根据时间来调度向其注册的task
/// Separate the timing and the execution of task,
/// so that the execution of task will not block the thread of timing
/// 非阻塞且异步的计数器
class AsyncTimer: public SyncTimer {
public:
	/// 刚刚启动的时候，默认是暂停的，需要用户显式调用resume()来启动定时器
	AsyncTimer(bool pause_timer = false);
	AsyncTimer(AsyncTimer const& t) = delete; // not CopyConstructible
	AsyncTimer& operator=(AsyncTimer const& t) = delete; // not CopyAssignable
	/// TODO Timer是moveConstructable and moveAssignable
	virtual ~AsyncTimer();

	/// std output
	virtual void print(std::ostream &out) const {
		out << "AsyncTimer";
	}
protected:
	/// 覆盖基类函数
	virtual void timer() override; // time_thread_ execute this function to time
	/// expire_task_thread_的线程执行函数
	void expire_task_thread_start_rtn();
	/// 添加超时的task
	void add_expire_task(TimedTask & task); // 添加 一个回调
protected:
	std::thread expire_task_thread_; // thread to execute expiration task

	std::queue<TimedTask> expire_task_queue_; // 到期的TimedTask

	std::mutex expire_task_mutex_;

	std::condition_variable expire_task_cv_;

	bool stop_expire_task_; // 停止expire task的执行
};

/// 添加一个thread来监控当前时间是否在交易时间段，只在交易时间段内运行TimedTask
/// 这主要用于金融领域的应用
/// trade_timer_thread就相当于redis中的sentinel，它控制这运行时间范围
class TradeTimer: public AsyncTimer {
public:
	TradeTimer(bool pause_trade_timer = false);
	virtual ~TradeTimer();
	/// TODO TradeTimer是moveConstructable and moveAssignable

	/// 添加日终处理
	/// @param when 执行时间，必须大于下午交易日结束时间
	/// @param handler
	template<typename FunctionT>
	bool add_day_end_handler(TradeTime& when, FunctionT&& handler) {
		LogFlow(this<<"添加day_end_handler，day_end_handler的执行时间是"<<when);
		TradeTime close_time =
				trade_datetime_reader_.get_trade_time_reader().PM_close_time();
		if (when <= close_time) {
			LogError(
					"day_end_handler的执行时间必须必晚于闭市时间"<<close_time<<"传入值"<<when<<"不是一个合法值，");
			return false;
		}
		when_day_end_ = when;
		day_end_handler_ = std::forward<FunctionT>(handler);
		return true;
	}
	/// 虽然添加了此函数，但是此函数的实现逻辑有待商榷，因为暂停trade timer的逻辑是比较复杂的，比如算是否暂停子线程等；所以现在最好不要使用此函数
	/// 暂停trade timer
	void pause_trade_timer();
	/// 重启一个暂停trade timer
	void resume_trade_timer();

//	void add_task(const IntervalT& interval, HandlerT&& handler,
//			const std::string& name = "");
	/// 对于TradeTimer，它的task只能够在交易时间段内运行，所以它的task的开始执行时间和结束执行时间必须要由timer来进行设置，而不能够由用户来进行设置
	/// TradeTimer会根据当前时间，交易日，交易时间 来计算出task的开始执行时间，结束执行时间
	/// @param interval
	/// @param handler
	/// @param name
	void add_task(const IntervalT& interval, HandlerT&& handler,
			const std::string& name = "");
protected:
	void trade_timer_thread_start_rtn(); // trade_timer_thread_ 执行此函数
	/// std output
	virtual void print(std::ostream &out) const {
		out << "TradeTimer";
	}
private:
	bool in_trade_period(); // 判断当前时间是否在交易时间段内
	/// 获得下个交易日的开始执行时间
	TimePointT get_next_trady_day_begin_time_point();
	///
	void readd_task();
	/// 该函数构建TimedTask，并且将其加入到task queue中，它供add_task和readd_task调用，执行实际的程序
	/// @param interval
	/// @param handler
	/// @param name
	/// @param back_up 是否将task备份到task_back_up_中
	void do_add_task(const IntervalT& interval, const HandlerT& handler,
			const std::string& name, bool back_up);
private:
	std::list<TimedTask> task_back_up_; // 备份的task，为了支持重新添加task
	std::thread trade_timer_thread_; // 监控当前时间是否是交易时间，如果不是，则一直睡到下一个交易时间
	CppDateAndTime::TradeDateTimeReader trade_datetime_reader_;

	bool stop_trade_timer_; // 停止trade timer
	bool pause_trade_timer_; // 暂停trade timer标志
	std::condition_variable trade_timer_cv_;
	std::mutex trade_timer_mutex_;

	/// 日初 和 日终 的执行都不在trade timer中，而是由另外一个线程来进行
	SyncTimer non_trade_period_timer_; // 交易时间段外的task由这个timer来进行调度
	TradeTime when_day_end_; // 日终处理函数执行时间
	HandlerT day_end_handler_; // 日终处理函数
	TradeTime when_day_begin_; // 日初处理函数执行时间
	HandlerT day_begin_handler_; // 日初处理函数
};

} /* namespace TimeWithThreadPool */

#endif /* CORE_DATETIME_TIMER_H_ */
