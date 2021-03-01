#ifndef CORE_DATETIME_TIMER_H_
#define CORE_DATETIME_TIMER_H_

/// timer��ʵ�ֲο�����������ƪ���£�
/// https://codereview.stackexchange.com/questions/40473/portable-periodic-one-shot-timer-implementation
/// �����Ļ����Ͻ�����һ������չ

/// �߳����ͷ�ļ�
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
	/// ������һ��ʱ���ڵ�periodicִ�е�task
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
	/// ����һ��periodicִ�е�task
	template<typename FunctionT>
	TimedTask(const TimePointT& when_begin, const IntervalT& interval,
			FunctionT&& handler, const std::string& name = "")
			noexcept :
			when_begin_(when_begin), interval_(interval), handler_(
					std::forward<FunctionT>(handler)), name_(name) {
		init_time_flag();
		gen_id();
	}
	/// ����һ��ִֻ��һ�ε�timed task
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
	/// ִ��handler
	void operator()() {
		/// https://en.cppreference.com/w/cpp/utility/functional/function/operator_bool
		if (not handler_) {
			throw std::runtime_error("task��handlerΪ�գ����ܹ�������");
		}
		// ����handler��ʱ�򣬴������û����õ�������handler��ִ�е�ʱ�䣬���ǳ����ʱ��Ϊ������ִ��ʱ��
		handler_(when_begin_);
	}
	/// std output
	friend std::ostream& operator<<(std::ostream &out, TimedTask* p_obj) {
		out << "��ʱ����[" << p_obj->id_ << "][" << p_obj->name_ << "]";
		return out;
	}
	/// std output
	friend std::ostream& operator<<(std::ostream &out, TimedTask& obj) {
		out << "��ʱ����[" << obj.id_ << "][" << obj.name_ << "]";
		return out;
	}

	/// �жϵ�ǰʱ���Ƿ����û����õĿ�ʼʱ�䣬����ʱ���ʱ�����
	/// @param now
	/// @return ����û���û������when_begin_��Ҳû������when_end_��������������interval_����˺������Ƿ���true
	bool between_begin_end(const TimePointT& now) {
		bool in = true;
		if (has_when_begin_)
			in = in and now >= when_begin_;
		if (has_when_end_)
			in = in and now <= when_end_;

		return in;
	}
	/// �жϵ�ǰʱ���Ƿ��Ѿ��������û����õĽ���ʱ��
	/// ����û�û�����ý���ʱ�䣬��˺�������false
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
	/// �ж��Ƿ���periodic
	/// @return
	bool is_periodic() const {
		return has_interval_;
	}
	/// �����´�ִ��ʱ�䣬��Ҫע����ǣ�����´�ִ��ʱ����Ȼ���ڣ�С�ڣ���ǰʱ�䣬������Ȼ�ᳬʱ��������Ȼ�����̱��ٴ�ִ�У�Ϊ��ֹ�����ظ����ô����ķ��գ������´�ִ��ʱ�����Ϊ��ǰʱ�����ִ�м��ʱ��
	/// @param now ��ǰʱ��
	void once_more(const TimePointT& now) {
		if (now >= when_begin_ + interval_) {
			LogWarning(
					this<<"���´�ִ��ʱ�����ڣ�С�ڣ���ǰʱ�䣬������Ȼ�ᳬʱ��Ϊ��ֹ�����ظ����ô����ķ��գ������´�ִ��ʱ�����Ϊ��ǰʱ�����ִ�м��ʱ��");
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

	// time_point that indicates when it needs to fire���������û����õ�������task��ִ�е�ʱ�䣬�����ʱ��Ϊ����ִ��ʱ��
	TimePointT when_begin_;
	TimePointT when_end_; // time_point that indicate when it needs to end

	/// ���������has flag�ǳ������ܿ��ǵģ����û�û�����õĶ�Ӧtime_point������£� ����ȥȡϵͳʱ����������ڶ�Ӧ��flag�Ϳ��������ж�
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

/// ����ʱ������������ע���task
/// ��ʱ�������ִ����ͬһ��thread��ɣ����������ִ�п��ܻᵼ�¼�ʱ��׼ȷ
class SyncTimer {
public:
	typedef std::unique_lock<std::mutex> ScopedLockT; // RAII
public:
	/// �ո�������ʱ��Ĭ������ͣ�ģ���Ҫ�û���ʽ����resume()��������ʱ��
	SyncTimer(bool pause_timer = false);

	SyncTimer(SyncTimer const& t) = delete; // not CopyConstructible
	SyncTimer& operator=(SyncTimer const& t) = delete; // not CopyAssignable
	/// TODO Timer��moveConstructable and moveAssignable
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
	/// ��ͣtimer
	void pause();
	/// ����timer
	void resume();
	/// std output
	friend std::ostream& operator<<(std::ostream &out, SyncTimer* p_obj) {
		p_obj->print(out);
		return out;
	}
	/// ����������
	/// https://stackoverflow.com/a/2852183
	void clear_task_queue() {
		task_queue_ = std::priority_queue<TimedTask, std::vector<TimedTask>,
				TimedTaskLess>();
	}
	/// �ж���������Ƿ�Ϊ��
	bool task_queue_empty() {
		return task_queue_.empty();
	}
protected:
	/// ��Timer�����Timertask
	virtual void add_task(TimedTask && task);

	/// timer_thread_���߳�ִ�к���
	void timer_thread_start_rtn();
	/// ����ִ�еĺ���������override
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

	bool stop_timer_; // ֹͣtime��־����ʾҪֹͣ�̵߳�ִ�У����˳��߳�ִ�к���
	bool pause_timer_; // ��ͣtime��־
};

/// ����ʱ������������ע���task
/// Separate the timing and the execution of task,
/// so that the execution of task will not block the thread of timing
/// ���������첽�ļ�����
class AsyncTimer: public SyncTimer {
public:
	/// �ո�������ʱ��Ĭ������ͣ�ģ���Ҫ�û���ʽ����resume()��������ʱ��
	AsyncTimer(bool pause_timer = false);
	AsyncTimer(AsyncTimer const& t) = delete; // not CopyConstructible
	AsyncTimer& operator=(AsyncTimer const& t) = delete; // not CopyAssignable
	/// TODO Timer��moveConstructable and moveAssignable
	virtual ~AsyncTimer();

	/// std output
	virtual void print(std::ostream &out) const {
		out << "AsyncTimer";
	}
protected:
	/// ���ǻ��ຯ��
	virtual void timer() override; // time_thread_ execute this function to time
	/// expire_task_thread_���߳�ִ�к���
	void expire_task_thread_start_rtn();
	/// ��ӳ�ʱ��task
	void add_expire_task(TimedTask & task); // ��� һ���ص�
protected:
	std::thread expire_task_thread_; // thread to execute expiration task

	std::queue<TimedTask> expire_task_queue_; // ���ڵ�TimedTask

	std::mutex expire_task_mutex_;

	std::condition_variable expire_task_cv_;

	bool stop_expire_task_; // ֹͣexpire task��ִ��
};

/// ���һ��thread����ص�ǰʱ���Ƿ��ڽ���ʱ��Σ�ֻ�ڽ���ʱ���������TimedTask
/// ����Ҫ���ڽ��������Ӧ��
/// trade_timer_thread���൱��redis�е�sentinel��������������ʱ�䷶Χ
class TradeTimer: public AsyncTimer {
public:
	TradeTimer(bool pause_trade_timer = false);
	virtual ~TradeTimer();
	/// TODO TradeTimer��moveConstructable and moveAssignable

	/// ������մ���
	/// @param when ִ��ʱ�䣬����������罻���ս���ʱ��
	/// @param handler
	template<typename FunctionT>
	bool add_day_end_handler(TradeTime& when, FunctionT&& handler) {
		LogFlow(this<<"���day_end_handler��day_end_handler��ִ��ʱ����"<<when);
		TradeTime close_time =
				trade_datetime_reader_.get_trade_time_reader().PM_close_time();
		if (when <= close_time) {
			LogError(
					"day_end_handler��ִ��ʱ���������ڱ���ʱ��"<<close_time<<"����ֵ"<<when<<"����һ���Ϸ�ֵ��");
			return false;
		}
		when_day_end_ = when;
		day_end_handler_ = std::forward<FunctionT>(handler);
		return true;
	}
	/// ��Ȼ����˴˺��������Ǵ˺�����ʵ���߼��д���ȶ����Ϊ��ͣtrade timer���߼��ǱȽϸ��ӵģ��������Ƿ���ͣ���̵߳ȣ�����������ò�Ҫʹ�ô˺���
	/// ��ͣtrade timer
	void pause_trade_timer();
	/// ����һ����ͣtrade timer
	void resume_trade_timer();

//	void add_task(const IntervalT& interval, HandlerT&& handler,
//			const std::string& name = "");
	/// ����TradeTimer������taskֻ�ܹ��ڽ���ʱ��������У���������task�Ŀ�ʼִ��ʱ��ͽ���ִ��ʱ�����Ҫ��timer���������ã������ܹ����û�����������
	/// TradeTimer����ݵ�ǰʱ�䣬�����գ�����ʱ�� �������task�Ŀ�ʼִ��ʱ�䣬����ִ��ʱ��
	/// @param interval
	/// @param handler
	/// @param name
	void add_task(const IntervalT& interval, HandlerT&& handler,
			const std::string& name = "");
protected:
	void trade_timer_thread_start_rtn(); // trade_timer_thread_ ִ�д˺���
	/// std output
	virtual void print(std::ostream &out) const {
		out << "TradeTimer";
	}
private:
	bool in_trade_period(); // �жϵ�ǰʱ���Ƿ��ڽ���ʱ�����
	/// ����¸������յĿ�ʼִ��ʱ��
	TimePointT get_next_trady_day_begin_time_point();
	///
	void readd_task();
	/// �ú�������TimedTask�����ҽ�����뵽task queue�У�����add_task��readd_task���ã�ִ��ʵ�ʵĳ���
	/// @param interval
	/// @param handler
	/// @param name
	/// @param back_up �Ƿ�task���ݵ�task_back_up_��
	void do_add_task(const IntervalT& interval, const HandlerT& handler,
			const std::string& name, bool back_up);
private:
	std::list<TimedTask> task_back_up_; // ���ݵ�task��Ϊ��֧���������task
	std::thread trade_timer_thread_; // ��ص�ǰʱ���Ƿ��ǽ���ʱ�䣬������ǣ���һֱ˯����һ������ʱ��
	CppDateAndTime::TradeDateTimeReader trade_datetime_reader_;

	bool stop_trade_timer_; // ֹͣtrade timer
	bool pause_trade_timer_; // ��ͣtrade timer��־
	std::condition_variable trade_timer_cv_;
	std::mutex trade_timer_mutex_;

	/// �ճ� �� ���� ��ִ�ж�����trade timer�У�����������һ���߳�������
	SyncTimer non_trade_period_timer_; // ����ʱ������task�����timer�����е���
	TradeTime when_day_end_; // ���մ�����ִ��ʱ��
	HandlerT day_end_handler_; // ���մ�����
	TradeTime when_day_begin_; // �ճ�������ִ��ʱ��
	HandlerT day_begin_handler_; // �ճ�������
};

} /* namespace TimeWithThreadPool */

#endif /* CORE_DATETIME_TIMER_H_ */
