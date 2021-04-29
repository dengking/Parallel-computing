# Load balancing 

## wikipedia [Load balancing (computing)](https://en.wikipedia.org/wiki/Load_balancing_(computing))

In [computing](https://en.wikipedia.org/wiki/Computing), **load balancing** refers to the process of distributing a set of [tasks](https://en.wikipedia.org/wiki/Task_(computing)) over a set of [resources](https://en.wikipedia.org/wiki/System_resource) (computing units), with the aim of making their overall processing more efficient. Load balancing techniques can optimize the response time for each task, avoiding unevenly overloading compute nodes while other compute nodes are left idle.



## developer.51cto [打工人，支撑亿级高并发的系统长啥样?](https://developer.51cto.com/art/202011/630464.htm) # Dubbo 负载均衡策略

Dubbo 负载均衡策略：

1、加权随机：假设我们有一组服务器 servers=[A, B, C]，他们对应的权重为 weights=[5, 3, 2]，权重总和为 10。

现在把这些权重值平铺在一维坐标值上，[0, 5) 区间属于服务器 A，[5, 8) 区间属于服务器 B，[8, 10) 区间属于服务器 C。

接下来通过随机数生成器生成一个范围在 [0, 10) 之间的随机数，然后计算这个随机数会落到哪个区间上就可以了。

2、最小活跃数：每个服务提供者对应一个活跃数 active，初始情况下，所有服务提供者活跃数均为 0。每收到一个请求，活跃数加 1，完成请求后则将活跃数减 1。

在服务运行一段时间后，性能好的服务提供者处理请求的速度更快，因此活跃数下降的也越快，此时这样的服务提供者能够优先获取到新的服务请求。

3、一致性 hash：通过 hash 算法，把 provider 的 invoke 和随机节点生成 hash，并将这个 hash 投射到 [0, 2^32 - 1] 的圆环上，查询的时候根据 key 进行 md5 然后进行 hash，得到第一个节点的值大于等于当前 hash 的 invoker。

4、加权轮询：比如服务器 A、B、C 权重比为 5:2:1，那么在 8 次请求中，服务器 A 将收到其中的 5 次请求，服务器 B 会收到其中的 2 次请求，服务器 C 则收到其中的 1 次请求。

## dubbo [Dubbo#Load Balance](https://dubbo.apache.org/docs/v2.7/user/examples/loadbalance/)



## TODO

### serverfault [what-kind-of-load-balancing-algorithms-are-there](https://serverfault.com/questions/112292/what-kind-of-load-balancing-algorithms-are-there)




```c++
/// 往队列中插入消息
void CQuoteOrderWorker::enqueue() {
	/// 算法:算法类似于将一个长绳折叠放入一个固定宽度的槽中，需要将长绳不断进行折叠，显然除去最后一折，每一折的长度等于槽的宽度
	/// 最后一折的长度不一定等于槽的宽度；

	/// 券的数量
	int stock_num = stock_reader_p_->stocks().size();
	/// fold可以理解为“折”，或“趟”
	int fold_num = ceil(stock_num / quote_thread_num);

	/// 索引
	int stock_index = 0;
	/// 对[0, fold_num-2]折，每一折的长度等于槽的宽度，可以完全放入
	for (int fold_index = 0; fold_index < fold_num - 1; ++fold_index) {
		for (int queue_index = 0; queue_index < quote_thread_num;
				queue_index++) {
			stock_index = fold_index * quote_thread_num + queue_index;
			Stock* stock_p = stock_reader_p_->stocks()[stock_index];
			QuoteMessage* message_p = new QuoteMessage(stock_p->market_,
					stock_p->stock_code_);
			mtx_Quote[queue_index]->Acquire();
			m_GQuoteMessage[queue_index]->push(message_p);
			mtx_Quote[queue_index]->Release();
		}
	}

	/// 对第fold-1折
	for (int queue_index = 0; queue_index < quote_thread_num; queue_index++) {
		stock_index = stock_num - (fold_num - 2) * quote_thread_num - 1
				+ queue_index;
		if (stock_index <= stock_num - 1) {
			Stock* stock_p = stock_reader_p_->stocks()[stock_index];
			QuoteMessage* message_p = new QuoteMessage(stock_p->market_,
					stock_p->stock_code_);
			m_GQuoteMessage[queue_index]->push(message_p);
		} else {	/// 已经放完了，则可以退出
			break;
		}
	}
}
```



```c++
void CQuoteInfoWorker::enqueue(QuoteMessage* quote_message_p) {
	/// 行情线程和特征处理线程个数不一定一样，成员变量feature_thread_nos的长度等于特征线程数，
	/// 每次pop出来一个后在push进队尾，可以实现下游线程的负载均衡
	///
	FeatureMessage* feature_message_p = new FeatureMessage(
			quote_message_p->market_, quote_message_p->stock_);
	/// feature_message将插入到下面这个队列中
	int feature_thread_no = feature_thread_nos->front();
	mtx_Feature[feature_thread_no]->Acquire();
	m_GFeatureMessage[feature_thread_no]->push(feature_message_p);
	mtx_Feature[feature_thread_no]->Release();
	/// 进入到队尾
	feature_thread_nos->pop();
	feature_thread_nos->push(feature_thread_no);
}
```

### udaparts [Tutoral 8 – Treat SocketPro Server as a Router for Load Balancing](http://www.udaparts.com/document/articles/snpisec.htm)