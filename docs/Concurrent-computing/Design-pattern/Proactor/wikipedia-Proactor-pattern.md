# [Proactor pattern](https://en.wikipedia.org/wiki/Proactor_pattern)

**Proactor** is a [software design pattern](https://en.wikipedia.org/wiki/Design_pattern_(computer_science)) for [event handling](https://en.wikipedia.org/wiki/Event_handling) in which long running activities are running in an asynchronous part. A *completion handler* is called after the asynchronous part has terminated. The proactor pattern can be considered to be an [asynchronous](https://en.wiktionary.org/wiki/asynchronous) variant of the [synchronous](https://en.wikipedia.org/wiki/Synchronization_(computer_science)) [reactor pattern](https://en.wikipedia.org/wiki/Reactor_pattern).[[1\]](https://en.wikipedia.org/wiki/Proactor_pattern#cite_note-1)





## Interaction

Operation specific actors:

- The *Proactive Initiator* starts the asynchronous operation via the Asynchronous Operation Processor and defines the Completion Handler
- *Completion Handler* is a call at the end of the operation from the Asynchronous Operation Processor
- *Asynchronous Operation*

Standardized actors

- The *Asynchronous Operation Processor* controls the whole asynchronous operation
- The *Completion Dispatcher* handles the call, depending on the execution environment.



