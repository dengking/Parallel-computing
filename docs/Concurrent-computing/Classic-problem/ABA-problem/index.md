# ABA problem

在阅读 preshing [An Introduction to Lock-Free Programming](https://preshing.com/20120612/an-introduction-to-lock-free-programming/) 时，其中介绍了ABA problem。



## Wikipedia [ABA problem](https://infogalactic.com/info/ABA_problem)

In [multithreaded](https://en.wikipedia.org/wiki/Thread_(computer_science)) [computing](https://en.wikipedia.org/wiki/Computer_science), the **ABA problem** occurs during synchronization, when a location is read twice, has the same value for both reads, and "value is the same" is used to indicate "nothing has changed". However, another thread can execute between the two reads and change the value, do other work, then change the value back, thus fooling the first thread into thinking "nothing has changed" even though the second thread did work that violates that assumption.

In [multithreaded](https://en.wikipedia.org/wiki/Thread_(computer_science)) [computing](https://en.wikipedia.org/wiki/Computer_science), the **ABA problem** occurs during synchronization, when a location is read twice, has the same value for both reads, and "value is the same" is used to indicate "nothing has changed". However, another thread can execute between the two reads and change the value, do other work, then change the value back, thus fooling the first thread into thinking "nothing has changed" even though the second thread did work that violates that assumption.

The ABA problem occurs when multiple [threads](https://en.wikipedia.org/wiki/Thread_(computer_science)) (or [processes](https://en.wikipedia.org/wiki/Process_(computing))) accessing shared data interleave. Below is the sequence of events that will result in the ABA problem:

- Process ${P_{1}}$ reads value A from shared memory,
- ${P_{1}}$ is [preempted](https://en.wikipedia.org/wiki/Preemption_(computing)), allowing process ${P_{2}}$ to run,
- ${P_{2}}$modifies the shared memory value A to value B and back to A before preemption,
- ${P_{1}}$ begins execution again, sees that the shared memory value has not changed and continues.

Although ${P_{1}}$ can continue executing, it is possible that the behavior will not be correct due to the "hidden" modification in shared memory.

A common case of the ABA problem is encountered when implementing a [lock-free](https://en.wikipedia.org/wiki/Lock-free) data structure. If an item is removed from the list, deleted, and then a new item is allocated and added to the list, it is common for the allocated object to be at the same location as the deleted object due to [MRU](https://en.wikipedia.org/wiki/Cache_replacement_policies#Most_recently_used_(MRU)) memory allocation. A pointer to the new item is thus often equal to a pointer to the old item, causing an ABA problem.

> NOTE: [MRU](https://en.wikipedia.org/wiki/Cache_replacement_policies#Most_recently_used_(MRU))所表示的是Most recently used

### Examples

Consider a software example of ABA using a [lock-free](https://en.wikipedia.org/wiki/Lock-free) [stack](https://en.wikipedia.org/wiki/Stack_(data_structure)):

```C++
#include <iostream>
#include <utility>
#include <vector>
#include <atomic>

using namespace std;

class Obj
{
public:
	Obj *next { nullptr };
};
/* Naive lock-free stack which suffers from ABA problem.*/
class Stack
{
	std::atomic<Obj*> top_ptr;
	//
	// Pops the top object and returns a pointer to it.
	//
	Obj* Pop()
	{
		while (1)
		{
			Obj *ret_ptr = top_ptr;
			if (!ret_ptr)
				return nullptr;
			// For simplicity, suppose that we can ensure that this dereference is safe
			// (i.e., that no other thread has popped the stack in the meantime).
			Obj *next_ptr = ret_ptr->next;
			// If the top node is still ret, then assume no one has changed the stack.
			// (That statement is not always true because of the ABA problem)
			// Atomically replace top with next.
			if (top_ptr.compare_exchange_weak(ret_ptr, next_ptr))
			{
				return ret_ptr;
			}
			// The stack has changed, start over.
		}
	}
	//
	// Pushes the object specified by obj_ptr to stack.
	//
	void Push(Obj *obj_ptr)
	{
		while (1)
		{
			Obj *next_ptr = top_ptr;
			obj_ptr->next = next_ptr;
			// If the top node is still next, then assume no one has changed the stack.
			// (That statement is not always true because of the ABA problem)
			// Atomically replace top with obj.
			if (top_ptr.compare_exchange_weak(next_ptr, obj_ptr))
			{
				return;
			}
			// The stack has changed, start over.
		}
	}
};

int main()
{

}
// g++ -std=c++11  -Wall -pedantic -pthread main.cpp && ./a.out


```

### Workarounds

#### Tagged state reference

#### Intermediate nodes

#### Deferred reclamation