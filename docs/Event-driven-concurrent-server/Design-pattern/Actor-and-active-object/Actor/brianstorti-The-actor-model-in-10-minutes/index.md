# brianstorti [The actor model in 10 minutes](https://www.brianstorti.com/the-actor-model/)

## The model

The actor model is a conceptual model to deal with concurrent computation. It defines some general rules for how the system’s components should behave and interact with each other. The most famous language that uses this model is probably `Erlang`. I’ll try to focus more on the model itself and not in how it’s implemented in different languages or libraries.



## Actors

An actor is the primitive unit of computation. It’s the *thing* that receives a message and do some kind of computation based on it.

The idea is very similar to what we have in object-oriented languages: An object receives a message (a method call) and does something depending on which message it receives (which method we are calling).
The main difference is that actors are completely isolated from each other and they will never share memory. It’s also worth noting that an actor can maintain a private state that can never be changed directly by another actor.



### Actors have mailboxes

It’s important to understand that, although multiple actors can run at the same time, an actor will process a given message sequentially. This means that if you send 3 messages to the **same** actor, it will just execute one at a time. To have these 3 messages being executed concurrently, you need to create 3 actors and send one message to each.

Messages are sent asynchronously to an actor, that needs to store them somewhere while it’s processing another message. The mailbox is the place where these messages are stored.

![img](./actors.png)

Actors communicate with each other by sending asynchronous messages. Those messages are stored in other actors' mailboxes until they're processed.