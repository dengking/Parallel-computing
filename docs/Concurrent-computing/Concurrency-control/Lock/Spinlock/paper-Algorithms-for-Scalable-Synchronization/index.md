# rochester [Algorithms for Scalable Synchronization on Shared-Memory Multiprocessors](https://www.cs.rochester.edu/research/synchronization/pseudocode/ss.html)

Pseudocode from [article of the above name](http://www.cs.rochester.edu/u/scott/papers/1991_TOCS_synch.pdf), *ACM TOCS*, February 1991. [John M. Mellor-Crummey](http://www.cs.rice.edu/~johnmc) and [Michael L. Scott](https://www.cs.rochester.edu/u/scott), with later additions due to (a) Craig, Landin, and Hagersten, and (b) Auslander, Edelsohn, Krieger, Rosenburg, and Wisniewski. All of these algorithms (except for the non-scalable centralized barrier) perform well in tests on machines with scores of processors. 

> NOTE: 
>
> 1、上面这段话的意思是: 本文下面的Pseudocode，是源自如下论文:
>
> [Algorithm for Scalable Synchronization on Shared-memory Multiprocessors](https://www.cs.rochester.edu/u/scott/papers/1991_TOCS_synch.pdf)
>
> 



## Spinlocks



## Barriers



> NOTE: 
>
> 1、目前还不知道下面的程序是使用什么语言开发的
>
> 2、`^lock` 表示的是pointer to lock，显然`^`类似于C的`*`

## Simple `test_and_set` lock with exponential backoff

```pascal
type lock = (unlocked, locked) // lock类型，可选值为 unlocked、locked

procedure acquire_lock (L : ^lock) // 函数acquire_lock，L是入参，它的类型为 lock
  delay : integer := 1
  while test_and_set (L) = locked     // returns old value
	  pause (delay)                   // consume this many units of time
	  delay := delay * 2

procedure release_lock (L : ^lock)
  lock^ := unlocked
```

## Ticket lock with proportional backoff

> NOTE: 
>
> 1、"proportional"的意思是"比例的，成比例的"，结合下面的code来看，它对应的是
>
> ```pascal
> pause (my_ticket - L->now_serving) // 此处表示的是暂停，显然就是sleep，"my_ticket - L->now_serving"表示的是在我前面有几个人在等待
> ```
>
> 

```pascal
type lock = record
  next_ticket : unsigned integer := 0 // 下一个要服务的号子
  now_serving : unsigned integer := 0 // 当前服务的号子

procedure acquire_lock (L : ^lock)
  my_ticket : unsigned integer := fetch_and_increment (&L->next_ticket) // 每个thread进来都需要取号
	  // returns old value; arithmetic overflow is harmless
  loop
	  pause (my_ticket - L->now_serving) // 此处表示的是暂停，显然就是sleep，"my_ticket - L->now_serving"表示的是在我前面有几个人在等待
		  // consume this many units of time
		  // on most machines, subtraction works correctly despite overflow
	  if L->now_serving = my_ticket // 被叫到号了
		  return

procedure release_lock (L : ^lock)
  L->now_serving := L->now_serving + 1 // 当前用户已经服务完了，可以叫下一个号了

```



## Anderson's array-based queue lock

> NOTE: 
>
> 1、A-lock

```pascal
type lock = record
  // slots 的长度对于 processor的个数
  // has_lock 表示可以获得lock
  slots : array [0..numprocs -1] of (has_lock, must_wait)
	  := (has_lock, must_wait, must_wait, ..., must_wait)
	  // each element of slots should lie in a different memory module
	  // or cache line
  next_slot : integer := 0 // 下一个slot的index

// parameter my_place, below, points to a private variable
// in an enclosing scope

procedure acquire_lock (L : ^lock, my_place : ^integer)
  my_place^ := fetch_and_increment (&L->next_slot)
	  // returns old value
  if my_place^ mod numprocs = 0
	  atomic_add (&L->next_slot, -numprocs)
	  // avoid problems with overflow; return value ignored
  my_place^ := my_place^ mod numprocs
  repeat while L->slots[my_place^] = must_wait      // spin
  L->slots[my_place^] := must_wait                  // init for next time

procedure release_lock (L : ^lock, my_place : ^integer)
  L->slots[(my_place^ + 1) mod numprocs] := has_lock // 释放锁

```

## Graunke and Thakkar's array-based queue lock

> NOTE:
>
> 1、未阅读

## The MCS list-based queue lock

```pascal
type qnode = record
  next : ^qnode // pointer to qnode
  locked : Boolean
type lock = ^qnode      // initialized to nil

// parameter I, below, points to a qnode record allocated
// (in an enclosing scope) in shared memory locally-accessible
// to the invoking processor

procedure acquire_lock (L : ^lock, I : ^qnode)
  I->next := nil
  predecessor : ^qnode := fetch_and_store (L, I)
  if predecessor != nil      // queue was non-empty
	  I->locked := true
	  predecessor->next := I
	  repeat while I->locked              // spin

procedure release_lock (L : ^lock, I: ^qnode)
  if I->next = nil        // no known successor
	  if compare_and_store (L, I, nil)
		  return
		  // compare_and_store returns true iff it stored
	  repeat while I->next = nil          // spin
  I->next->locked := false

```

