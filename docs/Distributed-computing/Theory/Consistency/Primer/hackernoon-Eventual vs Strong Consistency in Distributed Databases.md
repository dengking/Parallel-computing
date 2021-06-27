# [Eventual vs Strong Consistency in Distributed Databases](https://hackernoon.com/eventual-vs-strong-consistency-in-distributed-databases-282fdad37cf7)

Explanation of this topic starts with an analogy, taking an example from real life to understand the concept better.

I have the habit of writing something I call *Tech Notes* on my laptop daily to summarize technical concepts that I learn. It helps me to recollect them in an easier way whenever I want to.

But sometimes I used to worry about my laptop being stolen or what if it crashes. In response to the fear of losing *Tech Notes*, I started backing them up on my external Hard Disk. To further reduce the possibility of losing them, I also purchased a subscription of Dropbox.



![img](https://hackernoon.com/hn-images/1*PiQhvAicV4TUXohXNdNMuA.png)

Master-Slave Model (Image courtesy: [Dropbox](https://cfl.dropboxstatic.com/static/images/logo_catalog/dropbox_logo_glyph_m1.svg), [Apple](https://images.techhive.com/images/article/2014/12/macbook_air-100538062-large.jpg), [Seagate](http://i.ebayimg.com/00/s/MjkyWDUwMA==/z/4zYAAMXQVT9SsmxD/$_3.JPG?set_id=2))

Every fortnight, I update my external Hard Disk with revised and newly written *Tech Notes* and Dropbox gets updated as soon as I connect my laptop to the internet.

*Here, I am using Hard Disk and Dropbox as source of reading Tech Notes while laptop is being used for reading as well as writing them. (Master-Slave Model)*

> Redundancy introduces Reliability.

Now let’s get to the point.

### Case 1: Eventual Consistency

Whenever we use multiple replicas of a database to store data and let’s say a **write request** comes to one of the **replicas**. In such a situation, Databases had to discover a strategy to make this **write request** at one replica reach other replicas so that they all could also write data of the request and become **consistent**.

> **Consistency** here means that a **read request** for an entity made to any of the nodes of the database should return the same data.

**Eventual consistency** makes sure that data of each node of the database gets **consistent** eventually. Time taken by the nodes of the database to get **consistent** may or may not be defined.

Data getting **consistent** eventually means it will take time for updates to reach other **replicas**. So what?
This implies that if someone reads from a **replica** which is not updated yet (since **replicas** are updated eventually) then it may return **stale data**.



![img](https://hackernoon.com/hn-images/1*PFgHx8UYLhk3L5ePPmailQ.png)

My Hard Disk also keeps **stale data** for a period of 15 days as it gets updated fortnightly. Let’s assume John, my friend comes after few days of updation and asks for my Hard Disk.

```
John: I want your hard disk to read your Tech Notes.I: Sure, why not. But it hasn’t been updated since last few days.John: I am fine with it.
```

Now Hard Disk was supplied to John immediately *(low latency)* at the risk of having **stale data** in it. But I am sure about the fact that it will get updated when the next fortnight starts.

> **Eventual consistency** offers low latency at the risk of returning **stale data**

While on the other hand, we have something known as **Strong Consistency**.



### Case 2: Strong Consistency

It says data will get passed on to all the **replicas** as soon as a write request comes to one of the replicas of the database.
But during the time these **replicas** are being updated with new data, response to any subsequent read/write requests by any of the replicas will get delayed as all replicas are busy in keeping each other **consistent**.

As soon as they become **consistent**, they start to take care of the requests that have come at their door.

> Nothing is for free.



![img](https://hackernoon.com/hn-images/1*klFtrGr8U-XmyiZ1CJx-0w.png)

This time my friend Veronica comes and asks for my Tech Notes.

```
Veronica: I want your latest Tech Notes.I: Sure, why not. I will share a Dropbox link with you.   But Veronica, access it after few minutes as I have written a   new Tech Note on laptop which will get synced with my    Dropbox account in 2–3 minutes.
```

Now Veronica was able to access up-to-date *Tech Notes* but after few minutes of delay.



### Conclusion

- Strong Consistency offers up-to-date data but at the cost of **high latency**.
- While Eventual consistency offers **low latency** but may reply to read requests with **stale data** since all nodes of the database may not have the updated data.

Please hit the ♥ button if you liked the article so that other Medium users might find it and dig it too.

