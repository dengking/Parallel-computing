# sceweb.uhcl.edu [Concepts:  Concurrency](https://sceweb.uhcl.edu/helm/RationalUnifiedProcess/process/workflow/ana_desi/co_cncry.htm)

> NOTE: 
>
> 1、在搜索 "Mailbox golang" 时，搜索到的，其中关于"Active Object Model"的介绍，引起了我的注意

## The Active Object Model

Objects with their own threads of control are called “active objects”. In order to support asynchronous communication with other active objects, each active object is provided with a message queue or “mailbox.” When an object is created, the environment gives it its own thread of control, which the object encapsulates until it dies. Like a passive object, the active object is idle until the arrival of a message from outside. The object executes whatever code is appropriate to process the message. Any messages which arrive while the object is busy are enqueued in the mailbox. When the object completes the processing of a message, it returns to pick up the next waiting message in the mailbox, or waits for one to arrive. Good candidates for active objects in the elevator system include the elevators themselves, the call stations on each floor, and the dispatcher.