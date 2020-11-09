# Automatic failover

Automatic failover是实现HA的关键，本文总结如何使用ZK来实现automatic failover，不同的architecture 实现的方式有所不同，所以本文分类讨论。

## Master/slave architecture automatic failover

基于master/slave架构的automatic failover，核心是如何进行leader selection，在`./Lead-election`章节进行了介绍。

