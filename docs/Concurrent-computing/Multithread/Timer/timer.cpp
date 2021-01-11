#include "timer.h"

#include "../constant/constants.h"
#include "datetime_utility.h"

namespace CppDateAndTime {

TimedTask::IdT TimedTask::ID = 0;

SyncTimer::SyncTimer(bool pause_timer) :
		pause_timer_(pause_timer) {
	stop_timer_ = false;
	/// TODO std::thread��non-copyable�ģ�Ϊʲô�����д���ǿ��Եģ�
	/// Ӧ���������д��Ĭ����ʹ�õ�move constructor
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
	LogFlow("��ʼ����"<<this);
	{
		ScopedLockT lock(timer_mutex_);
		stop_timer_ = true; // ���������
	}
	if (timer_thread_.joinable()) {
		timer_thread_.join();
	}
}

/// timer_thread_���߳�ִ�к���
void SyncTimer::timer_thread_start_rtn() {
	timer();
}

/// ���thread�����ѵ�condition�У�
/// - ������µ�TimedTask����Ҫע����ǣ�ÿ���������һ��TimedTask������Ҫ��time_thread_���ѣ�
///   ��Ϊ�µ�TimedTask��ִ��ʱ����ܱ����еĳ�ʱʱ����̣����Ա���Ҫ��time_thread_���ѣ�Ȼ��ɸѡ����һ�����糬ʱ��TimedTask
/// - TimedTask��ʱ�ˣ�����������£�����Ҫ��thread������ִ�������ʱ��TimedTask
void SyncTimer::timer() {
	LogFlow(this<<"�����߳�ִ�к���");
	for (;;) {
		{
			ScopedLockT lock(timer_mutex_); // acquire the mutex and it is RAII, so it will release the mutex once out of the block
			/// ��������predicate��Ҫ����false
			/// ����������predicate��Ҫ����true
			/// ��ֹͣtimer_thread��ʱ����Ҫ��ʱ��Ӧ�����ܹ�������������Ϊ���timer_thread��������״̬������ʱ�ֽ�Ҫֹͣtimer_thread��
			/// ���ʱ���޷�ʹtimer_thread�����ѣ����ʱ���޷�����ֹͣ�������ˣ�����൱�ڽ�����dead lock
			/// this->stop_timer_ ���Ϊtrue�����ʾ��Ҫֹͣ��
			/// this->stop_timer_ ���Ϊfalse�����ʾ��ֹͣ����Ҫ���У������� ������Ƿ�ֹͣ��task_queue_�Ƿ�Ϊ�ն���
			timer_cv_.wait(lock,
					[this] {return this->stop_timer_ or ((not this->pause_timer_) and (not this->task_queue_.empty()));}); // ���ֹͣtimer���߶��зǿգ��򷵻�Ϊtrue�����ʾ����wait
			/// ��ʱthread������lock���ڴ��ڼ䣬��ò�Ҫ��lock��release������ֹ����race condition
			if (stop_timer_) { // һ���û�ֹͣ��timer������Ҫ����ֹͣtimer
				LogFlow(this<<"��ֹͣ���˳�");
				return;
			} else {
				TimePointT now = ClockT::now();
				if (task_queue_.top().begin(now)) { // expire
					if (task_queue_.top().end(now)) { // ������TimedTask�Ľ�ֹ���ڣ�ֱ�ӽ���ɾ��
						LogDebug("������е�top�����Ѿ������ˣ�����ɾ��");
						task_queue_.pop();
					} else {
						/// https://stackoverflow.com/a/20149745
						TimedTask task = std::move(
								const_cast<TimedTask&>(task_queue_.top()));
						task_queue_.pop();
						task(); // ִ��task
						if (task.is_periodic()) { // �����periodic������Ҫ�ٴμ��뵽������
							LogDebug(task<<"��������������Ҫ�ٴα�ִ��");
							task.once_more(now);
							task_queue_.push(std::move(task)); // ��ʱ��time_thread_��Ȼlockס��time_mutex_�����Դ˴����ܹ���ȫ�����task��
						}

					}
				} else {
					/// �˴���ʹ��std::this_thread::sleep_for��ԭ�������޷�ʹ�߳���ָ���������±����ѣ�
					/// �ںܶ�����£���������Ҫ��һ��sleeping��thread��ָ�������±����ѵģ����磺
					/// - ֹͣ���߳�
					/// - ����һ��sleeping��threadȥ����һЩ����������
					/// ��������Щ����¾���Ҫ�ܹ�ʹthread������
					/// ��Щ�������wait_until�ܹ��ṩ�ģ�
					/// ���к�������Spurious wakeup�ķ��գ�����Ŀǰ������������������ʹ������Spurious wakeup��Ҳ�����������
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
		throw std::runtime_error("SyncTimer�Ѿ���ֹͣ�ˣ���Ҫ�����task");
	ScopedLockT lock(timer_mutex_);
	LogDebug(this<<"���"<<task);
	task_queue_.emplace(task);
	timer_cv_.notify_one();
}

void SyncTimer::pause() {
	/// ��������predicate��Ҫ����false
	/// ����������predicate��Ҫ����true
	/// ����Ҫ��ͣtimer_thread��ʱ����Ҫ����������������ͣ������pause = true,
	{
		ScopedLockT lock(timer_mutex_);
		pause_timer_ = true;
		LogFlow(this<<"������ͣ");
	}
}

void SyncTimer::resume() {
	{
		ScopedLockT lock(timer_mutex_);
		pause_timer_ = false;
		LogFlow(this<<"��������");
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
	LogFlow("��ʼ����"<<this);
	{
		ScopedLockT lock(expire_task_mutex_);
		stop_expire_task_ = true; // ���������
	}
	expire_task_cv_.notify_all();

	if (expire_task_thread_.joinable()) {
		expire_task_thread_.join();
	}
}

/// ���thread�����ѵ�condition�У�
/// - ������µ�TimedTask����Ҫע����ǣ�ÿ���������һ��TimedTask������Ҫ��time_thread_���ѣ�
///   ��Ϊ�µ�TimedTask��ִ��ʱ����ܱ����еĳ�ʱʱ����̣����Ա���Ҫ��time_thread_���ѣ�Ȼ��ɸѡ����һ�����糬ʱ��TimedTask
/// - TimedTask��ʱ�ˣ�����������£�����Ҫ��thread������ִ�������ʱ��TimedTask
void AsyncTimer::timer() {
	LogFlow(this<<"�����߳�ִ�к���");
	for (;;) {
		{
			ScopedLockT lock(timer_mutex_); // acquire the mutex and it is RAII, so it will release the mutex once out of the block
			/// ��������predicate��Ҫ����false
			/// ����������predicate��Ҫ����true
			/// ��ֹͣtimer_thread��ʱ����Ҫ��ʱ��Ӧ�����ܹ�������������Ϊ���timer_thread��������״̬������ʱ�ֽ�Ҫֹͣtimer_thread��
			/// ���ʱ���޷�ʹtimer_thread�����ѣ����ʱ���޷�����ֹͣ�������ˣ�����൱�ڽ�����dead lock
			/// this->stop_timer_ ���Ϊtrue�����ʾ��Ҫֹͣ��
			/// this->stop_timer_ ���Ϊfalse�����ʾ��ֹͣ����Ҫ���У������� ������Ƿ�ֹͣ��task_queue_�Ƿ�Ϊ�ն���
			timer_cv_.wait(lock,
					[this] {return this->stop_timer_ or ((not this->pause_timer_) and (not this->task_queue_.empty()));}); // ���ֹͣtimer���߶��зǿգ��򷵻�Ϊtrue�����ʾ����wait
			/// ��ʱthread������lock���ڴ��ڼ䣬��ò�Ҫ��lock��release������ֹ����race condition
			if (stop_timer_) { // һ���û�ֹͣ��timer������Ҫ����ֹͣtimer
				LogFlow(this<<"��ֹͣ���˳�");
				return;
			} else {
				TimePointT now = ClockT::now();
				if (task_queue_.top().begin(now)) { // expire
					if (task_queue_.top().end(now)) { // ������TimedTask�Ľ�ֹ���ڣ�ֱ�ӽ���ɾ��
						LogDebug("���е�top�Ѿ������˽���ʱ�䣬"<<this<<"����ɾ��");
						task_queue_.pop();

					} else {
						/// https://stackoverflow.com/a/20149745
						TimedTask task = std::move(
								const_cast<TimedTask&>(task_queue_.top()));
						task_queue_.pop();
						add_expire_task(task);
						if (task.is_periodic()) { // �����periodic������Ҫ�ٴμ��뵽������
							LogDebug(task<<"��������������Ҫ�ٴα�ִ��");
							task.once_more();
							task_queue_.push(std::move(task)); // ��ʱ��time_thread_��Ȼlockס��time_mutex_�����Դ˴����ܹ���ȫ�����task��
						}

					}
				} else {
					/// �˴���ʹ��std::this_thread::sleep_for��ԭ�������޷�ʹ�߳���ָ���������±����ѣ�
					/// �ںܶ�����£���������Ҫ��һ��sleeping��thread��ָ�������±����ѵģ����磺
					/// - ֹͣ���߳�
					/// - ����һ��sleeping��threadȥ����һЩ����������
					/// ��������Щ����¾���Ҫ�ܹ�ʹthread������
					/// ��Щ�������wait_until�ܹ��ṩ�ģ�
					/// ���к�������Spurious wakeup�ķ��գ�����Ŀǰ������������������ʹ������Spurious wakeup��Ҳ�����������
					timer_cv_.wait_until(lock,
							task_queue_.top().get_when_begin()); // Wait until the timer is ready or a timer creation notifies
				}
			}
		}
	}
}

void AsyncTimer::expire_task_thread_start_rtn() {
	LogFlow(this<<"�ĳ�ʱ�����߳̽����߳�ִ�к���");
	for (;;) {
		TimedTask task;
		{
			ScopedLockT lock(expire_task_mutex_); // acquire the mutex and it is RAII, so it will release the mutex once out of the block
			expire_task_cv_.wait(lock,
					[this] {return this->stop_timer_ or not this->expire_task_queue_.empty();});
			if (stop_expire_task_ and this->expire_task_queue_.empty()) {
				/// ���û�������stop_expire_task_����֪���߳�Ҫ����ֹͣ������������²�ȡ�Ĵ�ʩ��Ӧ�ú�timer_thread_��ͬ��
				/// ����������£���Ҫ�������е����е�taskִ����ɺ���ܹ�ֹͣ
				LogFlow("expire_task_thread��ֹͣ���Ҷ���Ϊ�գ����˳�ִ��");
				return;
			}
			task = std::move(expire_task_queue_.front());
			expire_task_queue_.pop(); // ����
		}
		task(); // ִ�лص�����
	}
}

void AsyncTimer::add_expire_task(TimedTask & task) {
	if (stop_expire_task_)
		throw std::runtime_error(
				"AsyncTimer��expire_task_thread�Ѿ���ֹͣ�ˣ���Ҫ�����task");
	{
		ScopedLockT lock(expire_task_mutex_);
		expire_task_queue_.emplace(task);
	}
	expire_task_cv_.notify_one(); // notify callback_thread_ to task
}

// timer��trade_timer_thread_�����п��ƣ���trade_timer_thread_����֮ǰ���Ƚ�timer����ͣ���������Դ���AsyncTimer�Ĺ��캯���������true
TradeTimer::TradeTimer(bool pause_trade_timer) :
		AsyncTimer(true), trade_datetime_reader_(
				SystemConstant::NON_TRADE_DATE_FILE,
				SystemConstant::TRADE_TIME_FILE), stop_trade_timer_(false) {
	if (not trade_datetime_reader_.read()) {
		LogError(this<<"��ȡ�����գ�����ʱ����Ϣʧ�ܣ��߳�����ʧ��");
		throw std::runtime_error("��ȡ�����գ�����ʱ����Ϣʧ��");
	}
	pause_trade_timer_ = pause_trade_timer;
	trade_timer_thread_ = std::thread(
			std::bind(&TradeTimer::trade_timer_thread_start_rtn, this));
}

bool TradeTimer::in_trade_period() {
	return trade_datetime_reader_.in_trade_time();
}

void TradeTimer::pause_trade_timer() {
	/// ��������predicate��Ҫ����false
	/// ����������predicate��Ҫ����true
	/// ����Ҫ��ͣtimer_thread��ʱ����Ҫ����������������ͣ������pause = true,
	{
		ScopedLockT lock(timer_mutex_);
		pause_trade_timer_ = true;
		LogFlow(this<<"������ͣ");
	}
}

void TradeTimer::resume_trade_timer() {
	{
		ScopedLockT lock(timer_mutex_);
		pause_trade_timer_ = false;
		LogFlow(this<<"��������");
	}
	trade_timer_cv_.notify_all();
}

void TradeTimer::trade_timer_thread_start_rtn() {

	/// ÿ�������գ���Ҫ��֤�������յ�ÿһ��ʱ��ζ�ִֻ��һ�Σ���������ͬһ����һ��ʱ��α�ִ�ж�Σ�����һ�Σ���
	/// �����ճ�handler������handler��ִ�����ڷǽ���ʱ��εģ�����ͨ��add_task��TradeTimer��ӵ�task��ִ����һ����Ҫ����ʱ��ε�
	/// Ϊ�˱�������֮��ĳ�ͻ����Ҫ���һ��ר����ִ���ճ�handler������handler��thread
	TimePointT sleep_to_time_point; //�̵߳�˯����ʱ���
	TradePeriodInfo trade_period_info; //����ʱ�����Ϣ

	for (;;) {
		/// Ϊ֧����ͣtrade timer����ӵ����´���
		/// ��pause_trade_timer_Ϊtrueʱ��ִ�д˺������߳̽�����
		{
			ScopedLockT lock(trade_timer_mutex_);

			trade_timer_cv_.wait(lock,
					[this] {return this->stop_trade_timer_ or (not this->pause_trade_timer_);}); // һֱ˯�������ս���
		}
		/// Ҫʹ�߳�ִ�������if-else��䣬ֻ�������������
		/// - pause_trade_timer_����Ϊtrue��������ִ�д˺������߳�
		/// - stop_trade_timer_����Ϊtrue����׼��ֹͣ���߳�
		if (stop_trade_timer_) {
			LogFlow(this<<"��ֹͣ�����˳�ִ��");
			return;
		} else {
			trade_datetime_reader_.get_trade_period_info(trade_period_info);
			switch (trade_period_info.in_which_trade_period_) {
			case DateTimeInTradePeriodEnum::NonTradeDate: {
				/// �����Ƿǽ����գ���˯����һ�����յĿ�ʼtime_point
				LogFlow("���첻�ǽ����գ�"<<this<<"����ͣtimer_thread");
				pause(); // ֹͣtimer

				sleep_to_time_point = get_next_trady_day_begin_time_point();

				break;
			}
			case DateTimeInTradePeriodEnum::BeforeAMTradePeriod: {
				/// ��ǰ�������罻�׿���֮ǰ
				/// - ����ճ�handler
				/// - ��ʱ�߳�˯�� AM_open_time
				/// ֻ�е���������Ż���뵽�����֧��
				/// - �û��������ճ�handler
				/// - �û������ʱ�������˳���
				LogFlow("��ǰʱ�䴦�����罻��ʱ���֮ǰ�������Խ��н��ף�"<<this<<"����ͣtimer_thread");
				pause(); // ֹͣtimer

				if (day_begin_handler_) {
					TradeDateTime when_day_begin_date_time(
							trade_period_info.AM_open_date_time_.date_,
							when_day_begin_);
					non_trade_period_timer_.add_task(
							when_day_begin_date_time.time_point_,
							day_begin_handler_, "�ճ�handler");
				}

				sleep_to_time_point =
						trade_period_info.AM_open_date_time_.time_point_;

				break;
			}
			case DateTimeInTradePeriodEnum::InAMTradePeriod: {
				LogFlow("��ǰʱ�䴦�����罻��ʱ��Σ����Խ��н��ף�"<<this<<"������timer_thread");
				resume(); // ����timer

				sleep_to_time_point =
						trade_period_info.AM_close_date_time_.time_point_;

				break;
			}
			case DateTimeInTradePeriodEnum::InNoonClose: {
				LogFlow("��ǰʱ�䴦����������ʱ��Σ������Խ��н��ף�"<<this<<"����ͣtimer_thread");
				pause(); // ֹͣtimer
				/// �������Է��֣����������Ŀ����ԣ� �������һ�׶Σ�DateTimeInTradePeriodEnum::InAMTradePeriod������ʱ��task_queue_����Ȼ�����ݣ���ִ�е�task����
				/// ����뵽��һ���׶Σ�DateTimeInTradePeriodEnum::InPMTradePeriod��ʱ����Щtask�����ǵĳ�ʱʱ����[20190716 11:30:01.841]���϶��Ѿ���ʱ�ˣ�
				/// ����һ�����뵽��һ�׶Σ���Щtask���ᱻִ�У�����һֱִ�е����ٳ�ʱΪֹ�������Щtask��ִ�м���Ƚ�С�Ļ�����ִ��������ǳ��󣬲�����Щִ�ж���û������ģ�
				/// ����ʹ��readd_task������һ�׶ε�task_queue��գ�Ȼ��֮ǰ��ӵ�task�����һ�飻
				///
				/// 	Line 264341: [tools.cpp:521:WriteToLogQueue]0716 112958.841876(140682914133760)[����][quote_predict.h:229:timed_task]QuotePredict��ʼ���е�[2505]�֣�����time_indexΪ[20190716 11:29:58.841]
				///     Line 270077: [tools.cpp:521:WriteToLogQueue]0716 125959.1000483(140682914133760)[����][quote_predict.h:229:timed_task]QuotePredict��ʼ���е�[2506]�֣�����time_indexΪ[20190716 11:30:01.841]
				///     Line 270125: [tools.cpp:521:WriteToLogQueue]0716 125959.1000790(140682914133760)[����][quote_predict.h:229:timed_task]QuotePredict��ʼ���е�[2507]�֣�����time_indexΪ[20190716 11:30:04.841]
				readd_task();
				sleep_to_time_point =
						trade_period_info.PM_open_date_time_.time_point_;

				break;
			}
			case DateTimeInTradePeriodEnum::InPMTradePeriod: {
				LogFlow("��ǰʱ�䴦�����罻��ʱ��Σ����Խ��н��ף�"<<this<<"������timer_thread");
				resume(); // ����timer

				sleep_to_time_point =
						trade_period_info.PM_close_date_time_.time_point_;

				break;
			}
			case DateTimeInTradePeriodEnum::AfterPMTradePeriod: {
				/// ��ǰ�������罻�ױ���֮�����������handler
				LogFlow("��ǰʱ�䴦����������ʱ��κ󣬲����Խ��н��ף�"<<this<<"����ͣtimer_thread");
				pause(); // ֹͣtimer
				readd_task();
				sleep_to_time_point = get_next_trady_day_begin_time_point();

				TradeDateTime when_day_end_date_time(
						trade_period_info.AM_open_date_time_.date_,
						when_day_end_);
				if (day_end_handler_) {
					non_trade_period_timer_.add_task(
							when_day_end_date_time.time_point_,
							day_end_handler_, "����handler");
				}
				break;
			}
			default: {
				throw std::runtime_error("ʱ��μ�����󣬳����˲����ܵ�ֵ");
			}
			} /* end of switch */

			/// ������˯
			{
				ScopedLockT lock(trade_timer_mutex_);

				/// ��ִ�е�����Ĵ����ʱ��֮ǰ����in_trade_period()�������ж������Ѿ��������ˣ�����ǵ��͵�time of check to time of use, ���ҵ�ǰʱ����ָ����˯��ʱ���֮��Ĺ�ϵ����Ҳ��ͬ�ˣ����Դ˴�����ڴ˵���һ���ѽ���ȷ��
				/// wait_until�����time_pint���С��CppTimer::ClockT::now()����trade_timer_cv_����Զ���ᳬʱ������ͻᵼ�¶�ʱʧ�ܣ�Ϊ�˱�������⣬�����ڵ�����֮ǰ����Ҫ�ȶԱ�һ��ʱ�䣬���С�ڣ��򲻵��ô˺���������ֱ���˻�
				/// ��θ��ִ����⣺���time_point�ǳ��ӽ�CppTimer::ClockT::now()������������һ���ӣ����ʱ��ǳ��������˷�֧
				int sleep_time_in_millsecond = std::chrono::duration_cast<
						std::chrono::milliseconds>(
						sleep_to_time_point - ClockT::now()).count(); // ˯��ʱ��
				/// ���˯��ʱ��ֻ��0.01s����˯
				if (sleep_time_in_millsecond <= 10) {
					LogFlow(this<<"��˯��ʱ��̫�̣�����˯");
					return;
				} else {
					LogFlow(
							this<<"��˯��["<<Clock<std::chrono::milliseconds>::to_string(sleep_to_time_point));
					trade_timer_cv_.wait_until(lock, sleep_to_time_point,
							[this] {return this->stop_trade_timer_;}); // һֱ˯�������ս���
				}
			}

			if (this->stop_trade_timer_) {
				LogFlow(this<<"��ֹͣ�����˳�ִ��");
				return;
			}
		}
	}
}

TradeTimer::~TradeTimer() {
	{
		ScopedLockT lock(trade_timer_mutex_);
		stop_trade_timer_ = true; // ���������
	}
	trade_timer_cv_.notify_all();
	trade_timer_thread_.join();
}

TimePointT TradeTimer::get_next_trady_day_begin_time_point() {
	TimePointT time_point;
	/// ����û��������ճ�handler����Ӧ���õ����߳�˯��when_day_begin_
	if (day_begin_handler_) {
		/// TODO �˴��ļ����Ǵ���ģ���Ҫ���и���
//		time_point = when_day_begin_;
	} else {
		time_point =
				trade_datetime_reader_.get_next_open_datetime().time_point_;
	}
	return time_point;
}

void TradeTimer::readd_task() {
	if (not task_queue_empty()) {
		LogFlow(this<<"��task_queue�ǿգ�Ϊ��ֹ����������������е�task");
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
	LogFlow(this<<"��Ӷ�ʱ����["<<name);
	do_add_task(interval, handler, name, true);
}

void TradeTimer::do_add_task(const IntervalT& interval, const HandlerT& handler,
		const std::string& name, bool back_up) {
	TradePeriodInfo trade_period_info; //����ʱ�����Ϣ
	trade_datetime_reader_.get_trade_period_info(trade_period_info);

	TimePointT when_begin; // ��ʼʱ��
	TimePointT when_end; // ����ִ��ʱ��

	switch (trade_period_info.in_which_trade_period_) {

	/// ���������������׶����ʾ�����Ѿ����ɽ��н��ף���task�Ŀ�ʼִ��ʱ������Ϊ��һ�����յĿ���ʱ��
	case DateTimeInTradePeriodEnum::AfterPMTradePeriod:
	case DateTimeInTradePeriodEnum::NonTradeDate: {

		LogFlow("���첻�ɽ��н��ף�"<<this<<"������task�Ŀ�ʼִ��ʱ��Ϊ��һ�������յĿ���ʱ��");
		/// ��õ�ǰ���ڵ���һ����������
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
				"��ʼִ��ʱ��["<<next_trade_date_AM_open_time<<"]����ʱ��["<<next_trade_date_AM_close_time<<"]");
		break;
	}
		/// ���������׶α�ʾ��ǰʱ�䴦�����罻��ʱ���
	case DateTimeInTradePeriodEnum::BeforeAMTradePeriod: {
		LogFlow("��ǰʱ�䴦�����罻��ʱ���֮ǰ��"<<this<<"������task��ִ��ʱ��Ϊ����ʱ��");
		when_begin = trade_period_info.AM_open_date_time_.time_point_;
		when_end = trade_period_info.AM_close_date_time_.time_point_;
		LogDebug(
				"��ʼִ��ʱ��["<<trade_period_info.AM_open_date_time_<<"]����ʱ��["<<trade_period_info.AM_close_date_time_<<"]");
		break;
	}
	case DateTimeInTradePeriodEnum::InAMTradePeriod: {
		LogFlow("��ǰʱ�䴦�����罻��ʱ��Σ�"<<this<<"������task��ִ��ʱ��Ϊ��ǰʱ��");
		when_begin = trade_period_info.now_.time_point_;
		when_end = trade_period_info.AM_close_date_time_.time_point_;
		LogDebug(
				"��ʼִ��ʱ��["<<trade_period_info.now_<<"]����ʱ��["<<trade_period_info.AM_close_date_time_<<"]");
		break;
	}
	case DateTimeInTradePeriodEnum::InNoonClose: {
		LogFlow("��ǰʱ�䴦����������ʱ��Σ�"<<this<<"������task��ִ��ʱ��Ϊ���翪��ʱ��");
		when_begin = trade_period_info.PM_open_date_time_.time_point_;
		when_end = trade_period_info.PM_close_date_time_.time_point_;
		LogDebug(
				"��ʼִ��ʱ��["<<trade_period_info.PM_open_date_time_<<"]����ʱ��["<<trade_period_info.PM_close_date_time_<<"]");
		break;
	}
	case DateTimeInTradePeriodEnum::InPMTradePeriod: {
		LogFlow("��ǰʱ�䴦�����罻��ʱ��Σ�"<<this<<"������task��ִ��ʱ��Ϊ��ǰʱ��");
		when_begin = trade_period_info.now_.time_point_;
		when_end = trade_period_info.PM_close_date_time_.time_point_;
		LogDebug(
				"��ʼִ��ʱ��["<<trade_period_info.now_<<"]����ʱ��["<<trade_period_info.PM_close_date_time_<<"]");
		break;
	}
	default: {
		throw std::runtime_error("ʱ��μ�����󣬳����˲����ܵ�ֵ");
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

