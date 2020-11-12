# Leader Election

本章讨论zookeeper实现Leader Election，同时也能够实现automatic failover。

## zookeeper [A Guide to Creating Higher-level Constructs with ZooKeeper#Leader Election](https://zookeeper.apache.org/doc/r3.6.2/recipes.html#sc_leaderElection)

A simple way of doing **leader election** with ZooKeeper is to use the **SEQUENCE|EPHEMERAL** flags when creating znodes that represent "proposals" of clients. The idea is to have a znode, say "`/election`", such that each znode creates a child znode "`/election/guid-n_`" with both flags `SEQUENCE|EPHEMERAL`. With the sequence flag, ZooKeeper automatically appends a sequence number that is greater than any one previously appended to a child of "`/election`". The proces(进程) that created the znode with the smallest appended sequence number is the leader.

That's not all, though. It is important to watch for **failures** of the **leader**, so that a new client arises as the new **leader** in the case the current leader fails. A trivial solution is to have all application processes watching upon the **current smallest znode**, and checking if they are the new leader when the smallest znode goes away (note that the smallest znode will go away if the leader fails because the node is **ephemeral**). But this causes a **herd effect**(羊群效应): upon a failure of the current leader, all other processes receive a notification, and execute `getChildren` on "`/election`" to obtain the current list of children of "`/election`". If the number of clients is large, it causes a spike(毛刺) on the number of operations that ZooKeeper servers have to process. To avoid the **herd effect**, it is sufficient to watch for the next znode down on the sequence of znodes. If a client receives a notification that the znode it is watching is gone, then it becomes the new leader in the case that there is no smaller znode. Note that this avoids the **herd effect** by not having all clients watching the same znode.

Here's the pseudo code:

Let `ELECTION` be a path of choice of the application. To volunteer to be a leader:

1. Create znode `z` with path "`ELECTION/guid-n_`" with both `SEQUENCE` and `EPHEMERAL` flags;
2. Let `C` be the children of "`ELECTION`", and `i` be the sequence number of `z`;
3. Watch for changes on "`ELECTION/guid-n_j`", where `j` is the largest sequence number such that `j < i` and `n_j` is a znode in C;

Upon receiving a notification of znode deletion:

1. Let `C` be the new set of children of `ELECTION`;
2. If `z` is the smallest node in `C`, then execute leader procedure;
3. Otherwise, watch for changes on "`ELECTION/guid-n_j`", where `j` is the largest sequence number such that `j < i` and `n_j` is a znode in C;

Notes:

- Note that the znode having no preceding znode on the list of children do not imply that the creator of this znode is aware that it is the current leader. Applications may consider creating a separate znode to acknowledge that the leader has executed the leader procedure.
- See the [note for Locks](https://zookeeper.apache.org/doc/r3.6.2/recipes.html#sc_recipes_GuidNote) on how to use the **guid** in the node.

## hadoopinrealworld [What is ZooKeeper and it’s Use Case](https://www.hadoopinrealworld.com/what-is-zookeeper-and-its-use-case/)

### Leader Election

Leader election is one of the common use case for ZooKeeper. Let’s see how it works.

Let’s say we have 2 resource managers. Each resource manager will participate in the **leader election** process to decide who becomes the active resource manager. Resource managers use ZooKeeper to elect a leader among themselves. Let’s see how it works. Both resource mangers will attempt to create a znode named `ActiveStandbyElectorLock` in ZooKeeper and only one resource manager will be able to create `ActiveStandbyElectorLock` znode.

Whoever creates this znode first will become the **active resource manager**. So if resource manger 1 creates `ActiveStandbyElectorLock` first, it will become the active node and if resource manger 2 creates `ActiveStandbyElectorLock`, it will be become the active node.

Let’s say resource manger 1 created the `ActiveStandbyElectorLock` znode first, resource manager 2 will also attempt to create `ActiveStandbyElectorLock` but it won’t be able to because `ActiveStandbyElectorLock` is already created by resource manager 1 so resource manager 1 becomes the active resource manager and resource manager 2 becomes the stand by node and goes in to stand by mode. So far clear?

So we have now elected a leader. Resource manager 1 is the active resource manager now.



### Failover

With Resource Manager 1 as the active node, everything is working good, up until one fine morning when the RAM in resource manager 1 has gone bad. So resource manager 1 went down. Now our expectation is resource manager 2 should become the active node automatically. Let’s now understand how failover from active node to standby node works.

With ZooKeeper, we can add a watch to any node, watchers are extremely helpful because if we are watching a znode and when that znode gets updated or deleted, whoever is watching the znode will get notified.

Remember, whoever creates the `ActiveStandbyElectorLock` ephemeral znode first becomes the **active** resoure manager. In our case resource manager 1 beat resource manger 2 by creating `ActiveStandbyElectorLock` first. When resource manager 2 tried to create `ActiveStandbyElectorLock` znode, it couldn’t because it was already created. So resource manager 2 realized that there is already an active resource manager elected and it became the **stand by node**. Before resource manager 2 becomes stand by, it will add a **watch** on the `ActiveStandbyElectorLock` znode.

When resource manager 1 goes down due to the RAM failure, it’s session with ZooKeeper will become dead and since `ActiveStandbyElectorLock` is an ephemeral node, ZooKeeper will then delete the `ActiveStandbyElectorLock` node as soon the application who created the znode loses connection with ZooKeeper. Since resource manger 2 is watching the `ActiveStandbyElectorLock` node, it will get a notification stating `ActiveStandbyElectorLock` is gone.

Resource Manager 2 will now attempt to create `ActiveStandbyElectorLock` node and since it is the only one trying to create the node, it will succeed in creating the `ActiveStandbyElectorLock` znode and will become the active resource manager.

### Fencing(剑击)

A RAM failure is not recoverable but assume resource manager 1 experienced a connectivity issue with ZooKeeper for a brief moment. This will cause ZooKeeper to delete the ActiveStandbyElectorLock ephemeral znode, because for ZooKeeper it lost the session with resource manger 1 – it doesn’t matter few seconds or minutes. As soon the session is lost, ZooKeeper will delete the ActiveStandbyElectorLock ephemeral znode.

But in this case resource manager 1 is still active at least it thinks it is active. By this time resource manger 2 becomes active. We don’t want 2 resource managers thinking or acting as active at the same time, this will cause lot of issues for the applications running in the cluster and it is referred to as split brain scenario.

So before resource manager 2 becomes active it will read ActiveBreadCrumb znode to find who was the active node, in our case it will be resource manager 1 and will attempt to kill the resource manager 1 process, this process is referred to as fencing. This way we are certain that there is only one active resource manager at time. After the fencing attempt, resource manager 2 will write it’s information to the ActiveBreadCrumb znode since it is now the active resource manager.

This is how ZooKeeper is used in leader election and failover process with YARN or resource manager high availability. This same concept is applied to achieve NameNode high availability as well. So when you have multiple process or application trying to perform the same action but you need coordination between the applications so they don’t step on each other “toes” you can use ZooKeeper as a coordinator for the applications.



## cnblogs [基于zookeeper的主备切换方法](https://www.cnblogs.com/aquester/p/9891538.html)

继承`CZookeeperHelper`即可快速实现主备切换：

https://github.com/eyjian/libmooon/blob/master/include/mooon/net/zookeeper_helper.h

zookeeper的`ZOO_EPHEMERAL`节点（如果`ZOO_EPHEMERAL`满足不了需求，可以考虑和`ZOO_SEQUENCE`结合使用），在会话关闭或过期时，会自动删除，利用这一**特性**可以实现两个或多节点间的主备切换。

### 实现方法

1）在进程启动时调用`zookeeper_init()`初始化：

```C++
bool X::init_zookeeper()
{
	// 第一次调用时_clientid总是为NULL，
	// 状态为ZOO_EXPIRED_SESSION_STATE时，需要重新调用zookeeper_init，
	// 这个时候可传入的_clientid为前一次zookeeper_init()产生的_clientid

	// 请注意zookeeper_init()是一个异步调用，返回非NULL并不表示会话建立成功，
	// 只有当zk_watcher中的type为ZOO_SESSION_EVENT和state为ZOO_CONNECTED_STATE时，
	// 才真正表示会话建立成功。
	_zhandle = zookeeper_init(zk_hosts, zk_watcher, 5000, _clientid, this, 0);
	if (NULL == _zhandle)
	{
		MYLOG_ERROR("init zookeeper failed: %s\n", zerror(errno));
		return false;
	}

	MYLOG_INFO("init zookeeper(%s) successfully\n", zk_hosts);
	return true;
}

```

2）进入工作之前，先尝试切换成主，只有成功切换成主后才进入work

```C++
bool X::run()
{
	while (true)
	{
		int num_items = 0;
		// 备机最简单的方法是每隔一定时间，如1秒就尝试转成master，
		// 如果不使用轮询，则可以采用监视_zk_path的方式
		mooon::sys::CUtils::millisleep(1000);

		// 如果不是master，则尝试转成master，如果转成不成功则继续下一次尝试
		if (!is_master() && !change_to_master())
			continue;

		do_work();
	}
}

bool X::is_master() const
{
	return _is_master;
}

bool X::change_to_master()
{
	static uint64_t log_counter = 0; // 打log计数器，备状态时的日志输出

	// ZOO_EPHEMERAL|ZOO_SEQUENCE
	// _myip为本地IP地址，可以通过它来判断当前谁是master
	// _zk_path值示例：/master/test，注意需要先保证/master已存在
	int errcode = zoo_create(_zhandle, _zk_path.c_str(), _myip.c_str(), _myip.size() + 1, &ZOO_OPEN_ACL_UNSAFE, ZOO_EPHEMERAL, NULL, 0);

	// (-4)connection loss，比如为zookeeper_init()指定了无效的hosts（一个有效的host也没有）
	if (errcode != ZOK)
	{
		_is_master = false;

		// 减少为备状态时的日志输出
		if (0 == log_counter++ % 600)
		{
			MYLOG_DEBUG("become master[%s] failed: (%d)%s\n", _zk_path.c_str(), errcode, zerror(errcode));
		}

		return false;
	}
	else
	{
		_is_master = true;
		log_counter = 0;
		MYLOG_INFO("becase master[%s]\n", _zk_path.c_str());

		// sleep一下，以便让原master正在进行的完成
		mooon::sys::CUtils::millisleep(2000);
		return true;
	}
}

```







3）当zookeeper会话成功建立或过期时均会触发`zk_watcher`，可通过`type`和`state`来区分

```C++
void zk_watcher(zhandle_t *zh, int type, int state, const char *path, void *context)
{
	X *x = static_cast<X*>(context);
	MYLOG_DEBUG("zh=%p, type=%d, state=%d, context=%p, path=%s\n", zh, type, state, context, path);

	// zookeeper_init成功时type为ZOO_SESSION_EVENT，state为ZOO_CONNECTED_STATE
	if ((ZOO_SESSION_EVENT == type) && (ZOO_CONNECTED_STATE == state))
	{
		x->on_zookeeper_connected(path);
	}
	else if ((ZOO_SESSION_EVENT == type) && (ZOO_EXPIRED_SESSION_STATE == state))
	{
		// 需要重新调用zookeeper_init()，简单点可以退出当前进程重启
		x->on_zookeeper_expired();
	}
}

```

## Implementation 

### github [Automatic Redis Failover](https://github.com/ryanlecompte/redis_failover)

> NOTE: Ruby语言实现的

This gem (built using [ZK](https://github.com/slyphon/zk)) attempts to address these **failover** scenarios. One or more **Node Manager daemons** run as **background processes** and monitor all of your configured master/slave nodes. When the **daemon** starts up, it automatically discovers the **current master/slaves**. **Background watchers** are setup for each of the **redis nodes**. As soon as a node is detected as being offline, it will be moved to an "unavailable" state. If the node that went offline was the master, then one of the slaves will be promoted as the new master. All existing slaves will be automatically reconfigured to point to the new master for replication. All nodes marked as unavailable will be periodically checked to see if they have been brought back online. If so, the newly available nodes will be configured as slaves and brought back into the list of available nodes. Note that detection of a node going down should be nearly instantaneous, since the mechanism used to keep tabs on a node is via a blocking Redis BLPOP call (no polling). This call fails nearly immediately when the node actually goes offline. To avoid false positives (i.e., intermittent flaky network interruption), the Node Manager will only mark a node as unavailable if it fails to communicate with it 3 times (this is configurable via --max-failures, see configuration options below). Note that you can (and should) deploy multiple Node Manager daemons since they each report periodic health reports/snapshots of the redis servers. A "node strategy" is used to determine if a node is actually unavailable. By default a majority strategy is used, but you can also configure "consensus" or "single" as well.



### github [maweina](https://github.com/maweina)/[zookeeper-leader-election](https://github.com/maweina/zookeeper-leader-election)

> NOTE: Java语言实现的

### github [tgockel](https://github.com/tgockel)/[zookeeper-cpp](https://github.com/tgockel/zookeeper-cpp)

> NOTE: C++语言实现的

## TODO

cnblogs [zookeeper主备切换学习](https://www.cnblogs.com/chen45289/p/7820063.html)

