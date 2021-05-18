# mariusbancila [Dining philosophers in C++11: Chandy-Misra algorithm](https://mariusbancila.ro/blog/2017/01/20/dining-philosophers-in-c11-chandy-misra-algorithm/)

In my previous post, [Dining Philosophers in C++11](https://mariusbancila.ro/blog/2017/01/16/dining-philosophers-in-cpp11/), I have provided an implementation for the dining philosophers problem using modern C++ features, such as threads and mutexes. However, it was [noted in the comments](https://mariusbancila.ro/blog/2017/01/16/dining-philosophers-in-cpp11/#comment-390346) that the implementation did not prevent the philosophers starving to death when you remove the waiting times.

An algorithm that prevents the philosophers from starving was proposed by Mani Chandy and J. Misra and is known as the [Chandy/Misra solution](https://en.wikipedia.org/wiki/Dining_philosophers_problem#Chandy.2FMisra_solution). This is a bit different than the original problem because it requires the philosophers to communicate with each other. The algorithm, as described on Wikipedia, is the following:

> 1. For every pair of philosophers contending for a resource, create a fork and give it to the philosopher with the lower ID (n for agent Pn). Each fork can either be dirty or clean. Initially, all forks are dirty.
> 2. When a philosopher wants to use a set of resources (i.e. eat), said philosopher must obtain the forks from their contending neighbors. For all such forks the philosopher does not have, they send a request message.
> 3. When a philosopher with a fork receives a request message, they keep the fork if it is clean, but give it up when it is dirty. If the philosopher sends the fork over, they clean the fork before doing so.
> 4. After a philosopher is done eating, all their forks become dirty. If another philosopher had previously requested one of the forks, the philosopher that has just finished eating cleans the fork and sends it.

