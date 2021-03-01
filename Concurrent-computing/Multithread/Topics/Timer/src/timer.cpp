#include "timer.h"

#include "../constant/constants.h"
#include "datetime_utility.h"

namespace CppDateAndTime {

TimedTask::IdT TimedTask::ID = 0;

SyncTimer::SyncTimer(bool pause_timer) :
		pause_timer_(pause_timer) {
	stop_timer_ = false;
	/// TODO std::thread是non-copyable的，为什么下面的写法是可以的？
	/// 应该是下面的写法默认是使用的move constructor
	try {
		timer_thread_ = std::thread(
				std::bind(&SyncTimer::timer_thread_start_rtn, this));
	} catch (...) {
		stop_timer_ = true;
		pause_timer_ = true;
		throw;
	}
}

SyncTimer::~SyncTimer() {
	LogFlow("开始析构"<<this);
	{
		ScopedLockT lock(timer_mutex_);
		stop_timer_ = true; // 会造成阻塞
	}
	if (timer_thread_.joinable()) {
		timer_thread_.join();
	}
}

/// timer_thread_的线程执行函数
void SyncTimer::timer_thread_start_rtn() {
	timer();
}

/// 造成thread被唤醒的condition有：
/// - 添加了新的TimedTask；需要注意的是：每次新添加了一个TimedTask，都需要将time_thread_唤醒，
///   因为新的TimedTask的执行时间可能比已有的超时时间更短，所以必须要将time_thread_唤醒，然后筛选出下一个最早超时的TimedTask
/// - TimedTask超时了，在这种情况下，必须要将thread唤醒来执行这个超时的TimedTask
void SyncTimer::timer() {
	LogFlow(this<<"进入线程执行函数");
	for (;;) {
		{
			ScopedLockT lock(timer_mutex_); // acquire the mutex and it is RAII, so it will release the mutex once out of the block
			/// 阻塞，则predicate需要返回false
			/// 不阻塞，则predicate需要返回true
			/// 在停止timer_thread的时候，需要及时响应，不能够进行阻塞；因为如果timer_thread处于阻塞状态，而此时又将要停止timer_thread，
			/// 则此时就无法使timer_thread被唤醒，则此时就无法处理停止的请求了，则就相当于进入了dead lock
			/// this->stop_timer_ 如果为true，则表示需要停止，
			/// this->stop_timer_ 如果为false，则表示不停止，需要运行，在这种 情况下是否停止由task_queue_是否为空而定
			timer_cv_.wait(lock,
					[this] {return this->stop_timer_ or ((not this->pause_timer_) and (not this->task_queue_.empty()));}); // 如果停止timer或者队列非空，则返回为true，则表示不再wait
			/// 此时thread持有了lock，在此期间，最好不要将lock给release掉，防止出现race condition
			if (stop_timer_) { // 一旦用户停止了timer，则需要立即停止timer
				LogFlow(this<<"被停止将退出");
				return;
			} else {
				TimePointT now = ClockT::now();
				if (task_queue_.top().begin(now)) { // expire
					if (task_queue_.top().end(now)) { // 超过了TimedTask的截止日期，直接将其删除
						LogDebug("任务队列的top任务已经过期了，将被删除");
						task_queue_.pop();
					} else {
						/// https://stackoverflow.com/a/20149745
						TimedTask task = std::move(
								const_cast<TimedTask&>(task_queue_.top()));
						task_queue_.pop();
						task(); // 执行task
						if (task.is_periodic()) { // 如果是periodic，则需要再次加入到队列中
							LogDebug(task<<"是周期性任务，需要再次被执行");
							task.once_more(now);
							task_queue_.push(std::move(task)); // 此时，time_thread_依然lock住了time_mutex_，所以此处是能够安全地添加task的
						}

					}
				} else {
					/// 此处不使用std::this_thread::sleep_for的原因是它无法使线程在指定地条件下被唤醒；
					/// 在很多情况下，我们是需要让一个sleeping的thread在指定条件下被唤醒的，比如：
					/// - 停止该线程
					/// - 唤醒一个sleeping的thread去处理一些其他的事情
					/// 所以在这些情况下就需要能够使thread被唤醒
					/// 这些需求就是wait_until能够提供的；
					/// 下列函数存在Spurious wakeup的风险，按照目前的运行流畅来看，即使发生了Spurious wakeup，也不会出现问题
					timer_cv_.wait_until(lock,
							task_queue_.top().get_when_begin()); // Wait until the timer is ready or a timer creation notifies
				}
			}
		}
	}
}

void SyncTimer::add_task(const TimePointT& when_begin,
		const TimePointT& when_end, const IntervalT& interval,
		HandlerT&& handler, const std::string& name) {
	return add_task(
			TimedTask(when_begin, when_end, interval, std::move(handler), name));
}

void SyncTimer::add_task(const TimePointT& when_begin,
		const TimePointT& when_end, const IntervalT& interval,
		const HandlerT& handler, const std::string& name) {
	return add_task(TimedTask(when_begin, when_end, interval, handler, name));
}

void SyncTimer::add_task(const TimePointT& when_begin,
		const IntervalT& interval, HandlerT&& handler,
		const std::string& name) {
	return add_task(TimedTask(when_begin, interval, std::move(handler), name));
}

void SyncTimer::add_task(const TimePointT& when_begin,
		const IntervalT& interval, const HandlerT& handler,
		const std::string& name) {
	return add_task(TimedTask(when_begin, interval, handler, name));
}

void SyncTimer::add_task(const TimePointT& when_begin, HandlerT&& handler,
		const std::string& name) {
	return add_task(TimedTask(when_begin, handler, name));
}

void SyncTimer::add_task(const TimePointT& when_begin, const HandlerT& handler,
		const std::string& name) {
	return add_task(TimedTask(when_begin, handler, name));
}

void SyncTimer::add_task(TimedTask&& task) {
	// don't allow enqueueing after stopping or pausing the timer_thread
	if (stop_timer_)
		throw std::runtime_error("SyncTimer已经被停止了，不要再添加task");
	ScopedLockT lock(timer_mutex_);
	LogDebug(this<<"添加"<<task);
	task_queue_.emplace(task);
	timer_cv_.notify_one();
}

void SyncTimer::pause() {
	/// 阻塞，则predicate需要返回false
	/// 不阻塞，则predicate需要返回true
	/// 当需要暂停timer_thread的时候，需要将其阻塞起来；暂停会设置pause = true,
	{
		ScopedLockT lock(timer_mutex_);
		pause_timer_ = true;
		LogFlow(this<<"将被暂停");
	}
}

void SyncTimer::resume() {
	{
		ScopedLockT lock(timer_mutex_);
		pause_timer_ = false;
		LogFlow(this<<"将被重启");
	}
	timer_cv_.notify_all();
}

AsyncTimer::AsyncTimer(bool pause_timer) :
		SyncTimer(pause_timer) {
	stop_expire_task_ = false;
	try {
		expire_task_thread_ = std::thread(
				std::bind(&AsyncTimer::expire_task_thread_start_rtn, this));
	} catch (...) {
		stop_expire_task_ = true;
		throw;
	}
}

AsyncTimer::~AsyncTimer() {
	LogFlow("开始析构"<<this);
	{
		ScopedLockT lock(expire_task_mutex_);
		stop_expire_task_ = true; // 会造成阻塞
	}
	expire_task_cv_.notify_all();

	if (expire_task_thread_.joinable()) {
		expire_task_thread_.join();
	}
}

/// 造成thread被唤醒的condition有：
/// - 添加了新的TimedTask；需要注意的是：每次新添加了一个TimedTask，都需要将time_thread_唤醒，
///   因为新的TimedTask的执行时间可能比已有的超时时间更短，所以必须要将time_thread_唤醒，然后筛选出下一个最早超时的TimedTask
/// - TimedTask超时了，在这种情况下，必须要将thread唤醒来执行这个超时的TimedTask
void AsyncTimer::timer() {
	LogFlow(this<<"进入线程执行函数");
	for (;;) {
		{
			ScopedLockT lock(timer_mutex_); // acquire the mutex and it is RAII, so it will release the mutex once out of the block
			/// 阻塞，则predicate需要返回false
			/// 不阻塞，则predicate需要返回true
			/// 在停止timer_thread的时候，需要及时响应，不能够进行阻塞；因为如果timer_thread处于阻塞状态，而此时又将要停止timer_thread，
			/// 则此时就无法使timer_thread被唤醒，则此时就无法处理停止的请求了，则就相当于进入了dead lock
			/// this->stop_timer_ 如果为true，则表示需要停止，
			/// this->stop_timer_ 如果为false，则表示不停止，需要运行，在这种 情况下是否停止由task_queue_是否为空而定
			timer_cv_.wait(lock,
					[this] {return this->stop_timer_ or ((not this->pause_timer_) and (not this->task_queue_.empty()));}); // 如果停止timer或者队列非空，则返回为true，则表示不再wait
			/// 此时thread持有了lock，在此期间，最好不要将lock给release掉，防止出现race condition
			if (stop_timer_) { // 一旦用户停止了timer，则需要立即停止timer
				LogFlow(this<<"被停止将退出");
				return;
			} else {
				TimePointT now = ClockT::now();
				if (task_queue_.top().begin(now)) { // expire
					if (task_queue_.top().end(now)) { // 超过了TimedTask的截止日期，直接将其删除
						LogDebug("队列的top已经到达了结束时间，"<<this<<"将其删除");
						task_queue_.pop();

					} else {
						/// https://stackoverflow.com/a/20149745
						TimedTask task = std::move(
								const_cast<TimedTask&>(task_queue_.top()));
						task_queue_.pop();
						add_expire_task(task);
						if (task.is_periodic()) { // 如果是periodic，则需要再次加入到队列中
							LogDebug(task<<"是周期性任务，需要再次被执行");
							task.once_more();
							task_queue_.push(std::move(task)); // 此时，time_thread_依然lock住了time_mutex_，所以此处是能够安全地添加task的
						}

					}
				} else {
					/// 此处不使用std::this_thread::sleep_for的原因是它无法使线程在指定地条件下被唤醒；
					/// 在很多情况下，我们是需要让一个sleeping的thread在指定条件下被唤醒的，比如：
					/// - 停止该线程
					/// - 唤醒一个sleeping的thread去处理一些其他的事情
					/// 所以在这些情况下就需要能够使thread被唤醒
					/// 这些需求就是wait_until能够提供的；
					/// 下列函数存在Spurious wakeup的风险，按照目前的运行流畅来看，即使发生了Spurious wakeup，也不会出现问题
					timer_cv_.wait_until(lock,
							task_queue_.top().get_when_begin()); // Wait until the timer is ready or a timer creation notifies
				}
			}
		}
	}
}

void AsyncTimer::expire_task_thread_start_rtn() {
	LogFlow(this<<"的超时任务线程进入线程执行函数");
	for (;;) {
		TimedTask task;
		{
			ScopedLockT lock(expire_task_mutex_); // acquire the mutex and it is RAII, so it will release the mutex once out of the block
			expire_task_cv_.wait(lock,
					[this] {return this->stop_timer_ or not this->expire_task_queue_.empty();});
			if (stop_expire_task_ and this->expire_task_queue_.empty()) {
				/// 当用户设置了stop_expire_task_来告知本线程要将其停止，在这种情况下采取的措施不应该和timer_thread_相同；
				/// 在这种情况下，需要将队列中的所有的task执行完成后才能够停止
				LogFlow("expire_task_thread被停止并且队列为空，将退出执行");
				return;
			}
			task = std::move(expire_task_queue_.front());
			expire_task_queue_.pop(); // 出队
		}
		task(); // 执行回调函数
	}
}

void AsyncTimer::add_expire_task(TimedTask & task) {
	if (stop_expire_task_)
		throw std::runtime_error(
				"AsyncTimer的expire_task_thread已经被停止了，不要再添加task");
	{
		ScopedLockT lock(expire_task_mutex_);
		expire_task_queue_.emplace(task);
	}
	expire_task_cv_.notify_one(); // notify callback_thread_ to task
}

// timer由trade_timer_thread_来进行控制，在trade_timer_thread_运行之前，先将timer给暂停下来，所以传给AsyncTimer的构造函数的入参是true
TradeTimer::TradeTimer(bool pause_trade_timer) :
		AsyncTimer(true), trade_datetime_reader_(
				SystemConstant::NON_TRADE_DATE_FILE,
				SystemConstant::TRADE_TIME_FILE), stop_trade_timer_(false) {
	if (not trade_datetime_reader_.read()) {
		LogError(this<<"读取交易日，交易时间信息失败，线程启动失败");
		throw std::runtime_error("读取交易日，交易时间信息失败");
	}
	pause_trade_timer_ = pause_trade_timer;
	trade_timer_thread_ = std::thread(
			std::bind(&TradeTimer::trade_timer_thread_start_rtn, this));
}

bool TradeTimer::in_trade_period() {
	return trade_datetime_reader_.in_trade_time();
}

void TradeTimer::pause_trade_timer() {
	/// 阻塞，则predicate需要返回false
	/// 不阻塞，则predicate需要返回true
	/// 当需要暂停timer_thread的时候，需要将其阻塞起来；暂停会设置pause = true,
	{
		ScopedLockT lock(timer_mutex_);
		pause_trade_timer_ = true;
		LogFlow(this<<"将被暂停");
	}
}

void TradeTimer::resume_trade_timer() {
	{
		ScopedLockT lock(timer_mutex_);
		pause_trade_timer_ = false;
		LogFlow(this<<"将被重启");
	}
	trade_timer_cv_.notify_all();
}

void TradeTimer::trade_timer_thread_start_rtn() {

	/// 每个交易日，需要保证，交易日的每一个时间段都只执行一次，决不允许同一天内一个时间段被执行多次（大于一次）；
	/// 由于日初handler和日终handler的执行是在非交易时间段的，但是通过add_task向TradeTimer添加的task的执行则一定是要交易时间段的
	/// 为了避免两者之间的冲突，需要添加一个专门来执行日初handler和日终handler的thread
	TimePointT sleep_to_time_point; //线程的睡至该时间点
	TradePeriodInfo trade_period_info; //交易时间段信息

	for (;;) {
		/// 为支持暂停trade timer而添加的如下代码
		/// 当pause_trade_timer_为true时，执行此函数的线程将阻塞
		{
			ScopedLockT lock(trade_timer_mutex_);

			trade_timer_cv_.wait(lock,
					[this] {return this->stop_trade_timer_ or (not this->pause_trade_timer_);}); // 一直睡到交易日结束
		}
		/// 要使线程执行下面的if-else语句，只存在两种情况：
		/// - pause_trade_timer_设置为true，即重启执行此函数的线程
		/// - stop_trade_timer_设置为true，即准备停止此线程
		if (stop_trade_timer_) {
			LogFlow(this<<"被停止，将退出执行");
			return;
		} else {
			trade_datetime_reader_.get_trade_period_info(trade_period_info);
			switch (trade_period_info.in_which_trade_period_) {
			case DateTimeInTradePeriodEnum::NonTradeDate: {
				/// 今天是非交易日，则睡到下一交易日的开始time_point
				LogFlow("今天不是交易日，"<<this<<"将暂停timer_thread");
				pause(); // 停止timer

				sleep_to_time_point = get_next_trady_day_begin_time_point();

				break;
			}
			case DateTimeInTradePeriodEnum::BeforeAMTradePeriod: {
				/// 当前处于上午交易开市之前
				/// - 添加日初handler
				/// - 定时线程睡至 AM_open_time
				/// 只有当如下情况才会进入到这个分支：
				/// - 用户设置了日初handler
				/// - 用户在这段时间启动了程序
				LogFlow("当前时间处于上午交易时间段之前，不可以进行交易，"<<this<<"将暂停timer_thread");
				pause(); // 停止timer

				if (day_begin_handler_) {
					TradeDateTime when_day_begin_date_time(
							trade_period_info.AM_open_date_time_.date_,
							when_day_begin_);
					non_trade_period_timer_.add_task(
							when_day_begin_date_time.time_point_,
							day_begin_handler_, "日初handler");
				}

				sleep_to_time_point =
						trade_period_info.AM_open_date_time_.time_point_;

				break;
			}
			case DateTimeInTradePeriodEnum::InAMTradePeriod: {
				LogFlow("当前时间处于上午交易时间段，可以进行交易，"<<this<<"将重启timer_thread");
				resume(); // 启动timer

				sleep_to_time_point =
						trade_period_info.AM_close_date_time_.time_point_;

				break;
			}
			case DateTimeInTradePeriodEnum::InNoonClose: {
				LogFlow("当前时间处于中午休市时间段，不可以进行交易，"<<this<<"将暂停timer_thread");
				pause(); // 停止timer
				/// 经过测试发现，存在这样的可能性： 如果在上一阶段（DateTimeInTradePeriodEnum::InAMTradePeriod）结束时，task_queue_中仍然有数据（待执行的task），
				/// 则进入到下一个阶段（DateTimeInTradePeriodEnum::InPMTradePeriod）时，这些task（它们的超时时间是[20190716 11:30:01.841]）肯定已经超时了，
				/// 所以一旦进入到下一阶段，这些task将会被执行，并且一直执行到不再超时为止，如果这些task的执行间隔比较小的话，则执行量将会非常大，并且这些执行都是没有意义的；
				/// 所以使用readd_task来将上一阶段的task_queue清空，然后将之前添加的task在添加一遍；
				///
				/// 	Line 264341: [tools.cpp:521:WriteToLogQueue]0716 112958.841876(140682914133760)[流程][quote_predict.h:229:timed_task]QuotePredict开始运行第[2505]轮，本轮time_index为[20190716 11:29:58.841]
				///     Line 270077: [tools.cpp:521:WriteToLogQueue]0716 125959.1000483(140682914133760)[流程][quote_predict.h:229:timed_task]QuotePredict开始运行第[2506]轮，本轮time_index为[20190716 11:30:01.841]
				///     Line 270125: [tools.cpp:521:WriteToLogQueue]0716 125959.1000790(140682914133760)[流程][quote_predict.h:229:timed_task]QuotePredict开始运行第[2507]轮，本轮time_index为[20190716 11:30:04.841]
				readd_task();
				sleep_to_time_point =
						trade_period_info.PM_open_date_time_.time_point_;

				break;
			}
			case DateTimeInTradePeriodEnum::InPMTradePeriod: {
				LogFlow("当前时间处于下午交易时间段，可以进行交易，"<<this<<"将重启timer_thread");
				resume(); // 启动timer

				sleep_to_time_point =
						trade_period_info.PM_close_date_time_.time_point_;

				break;
			}
			case DateTimeInTradePeriodEnum::AfterPMTradePeriod: {
				/// 当前处于下午交易闭市之后，则添加日终handler
				LogFlow("当前时间处于下午休市时间段后，不可以进行交易，"<<this<<"将暂停timer_thread");
				pause(); // 停止timer
				readd_task();
				sleep_to_time_point = get_next_trady_day_begin_time_point();

				TradeDateTime when_day_end_date_time(
						trade_period_info.AM_open_date_time_.date_,
						when_day_end_);
				if (day_end_handler_) {
					non_trade_period_timer_.add_task(
							when_day_end_date_time.time_point_,
							day_end_handler_, "日终handler");
				}
				break;
			}
			default: {
				throw std::runtime_error("时间段计算错误，出现了不可能的值");
			}
			} /* end of switch */

			/// 阻塞并睡
			{
				ScopedLockT lock(trade_timer_mutex_);

				/// 当执行到下面的代码的时候，之前基于in_trade_period()所做的判定可能已经不成立了，这就是典型的time of check to time of use, 并且当前时间点和指定的睡眠时间点之间的关系可能也不同了，所以此处最好在此调用一次已进行确认
				/// wait_until的入参time_pint如果小于CppTimer::ClockT::now()，则trade_timer_cv_将永远不会超时，则这就会导致定时失败，为了避免此问题，所以在调用它之前必须要先对比一次时间，如果小于，则不调用此函数，而是直接退回
				/// 如何复现此问题：如果time_point非常接近CppTimer::ClockT::now()，两者相差仅仅一分钟，则此时会非常任意进入此分支
				int sleep_time_in_millsecond = std::chrono::duration_cast<
						std::chrono::milliseconds>(
						sleep_to_time_point - ClockT::now()).count(); // 睡眠时间
				/// 如果睡眠时间只有0.01s，则不睡
				if (sleep_time_in_millsecond <= 10) {
					LogFlow(this<<"的睡眠时间太短，将不睡");
					return;
				} else {
					LogFlow(
							this<<"将睡至["<<Clock<std::chrono::milliseconds>::to_string(sleep_to_time_point));
					trade_timer_cv_.wait_until(lock, sleep_to_time_point,
							[this] {return this->stop_trade_timer_;}); // 一直睡到交易日结束
				}
			}

			if (this->stop_trade_timer_) {
				LogFlow(this<<"被停止，将退出执行");
				return;
			}
		}
	}
}

TradeTimer::~TradeTimer() {
	{
		ScopedLockT lock(trade_timer_mutex_);
		stop_trade_timer_ = true; // 会造成阻塞
	}
	trade_timer_cv_.notify_all();
	trade_timer_thread_.join();
}

TimePointT TradeTimer::get_next_trady_day_begin_time_point() {
	TimePointT time_point;
	/// 如果用户设置了日初handler，则应该让调度线程睡到when_day_begin_
	if (day_begin_handler_) {
		/// TODO 此处的计算是错误的，需要进行更正
//		time_point = when_day_begin_;
	} else {
		time_point =
				trade_datetime_reader_.get_next_open_datetime().time_point_;
	}
	return time_point;
}

void TradeTimer::readd_task() {
	if (not task_queue_empty()) {
		LogFlow(this<<"的task_queue非空，为防止出错，先重新添加所有的task");
		clear_task_queue();
	}
	for (auto begin = task_back_up_.begin(), end = task_back_up_.end();
			begin != end; ++begin) {
		do_add_task(begin->get_interval(), begin->handler_, begin->name_,
				false);
	}
}

//void TradeTimer::add_task(const IntervalT& interval, HandlerT&& handler,
//		const std::string& name = "") {
//
//}

void TradeTimer::add_task(const IntervalT& interval, HandlerT&& handler,
		const std::string& name) {
	LogFlow(this<<"添加定时任务["<<name);
	do_add_task(interval, handler, name, true);
}

void TradeTimer::do_add_task(const IntervalT& interval, const HandlerT& handler,
		const std::string& name, bool back_up) {
	TradePeriodInfo trade_period_info; //交易时间段信息
	trade_datetime_reader_.get_trade_period_info(trade_period_info);

	TimePointT when_begin; // 开始时间
	TimePointT when_end; // 结束执行时间

	switch (trade_period_info.in_which_trade_period_) {

	/// 当处于以下两个阶段则表示今天已经不可进行交易，则将task的开始执行时间设置为下一交易日的开市时间
	case DateTimeInTradePeriodEnum::AfterPMTradePeriod:
	case DateTimeInTradePeriodEnum::NonTradeDate: {

		LogFlow("今天不可进行交易，"<<this<<"将设置task的开始执行时间为下一个交易日的开市时间");
		/// 获得当前日期的下一个交易日期
		TradeDate next_trade_date =
				trade_datetime_reader_.get_non_trade_date_reader().next_trade_date(
						trade_period_info.now_.date_);
		TradeDateTime next_trade_date_AM_open_time(next_trade_date,
				trade_datetime_reader_.get_trade_time_reader().AM_open_time());
		TradeDateTime next_trade_date_AM_close_time(next_trade_date,
				trade_datetime_reader_.get_trade_time_reader().AM_close_time());
		when_begin = next_trade_date_AM_open_time.time_point_;
		when_end = next_trade_date_AM_close_time.time_point_;
		LogDebug(
				"开始执行时间["<<next_trade_date_AM_open_time<<"]结束时间["<<next_trade_date_AM_close_time<<"]");
		break;
	}
		/// 以下两个阶段表示当前时间处于上午交易时间段
	case DateTimeInTradePeriodEnum::BeforeAMTradePeriod: {
		LogFlow("当前时间处于上午交易时间段之前，"<<this<<"将设置task的执行时间为开市时间");
		when_begin = trade_period_info.AM_open_date_time_.time_point_;
		when_end = trade_period_info.AM_close_date_time_.time_point_;
		LogDebug(
				"开始执行时间["<<trade_period_info.AM_open_date_time_<<"]结束时间["<<trade_period_info.AM_close_date_time_<<"]");
		break;
	}
	case DateTimeInTradePeriodEnum::InAMTradePeriod: {
		LogFlow("当前时间处于上午交易时间段，"<<this<<"将设置task的执行时间为当前时间");
		when_begin = trade_period_info.now_.time_point_;
		when_end = trade_period_info.AM_close_date_time_.time_point_;
		LogDebug(
				"开始执行时间["<<trade_period_info.now_<<"]结束时间["<<trade_period_info.AM_close_date_time_<<"]");
		break;
	}
	case DateTimeInTradePeriodEnum::InNoonClose: {
		LogFlow("当前时间处于中午休市时间段，"<<this<<"将设置task的执行时间为下午开市时间");
		when_begin = trade_period_info.PM_open_date_time_.time_point_;
		when_end = trade_period_info.PM_close_date_time_.time_point_;
		LogDebug(
				"开始执行时间["<<trade_period_info.PM_open_date_time_<<"]结束时间["<<trade_period_info.PM_close_date_time_<<"]");
		break;
	}
	case DateTimeInTradePeriodEnum::InPMTradePeriod: {
		LogFlow("当前时间处于下午交易时间段，"<<this<<"将设置task的执行时间为当前时间");
		when_begin = trade_period_info.now_.time_point_;
		when_end = trade_period_info.PM_close_date_time_.time_point_;
		LogDebug(
				"开始执行时间["<<trade_period_info.now_<<"]结束时间["<<trade_period_info.PM_close_date_time_<<"]");
		break;
	}
	default: {
		throw std::runtime_error("时间段计算错误，出现了不可能的值");
	}
	} /* end of switch */

	SyncTimer::add_task(
			TimedTask(when_begin, when_end, interval, handler, name));
	if (back_up) {
		task_back_up_.emplace_back(when_begin, when_end, interval, handler,
				name);
	}
}
} /* namespace TimeWithThreadPool */

