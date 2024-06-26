# Polling 

轮询。

## wikipedia [Polling (computer science)](https://en.wikipedia.org/wiki/Polling_(computer_science))

**Polling**, or **polled** operation, in [computer science](https://en.wikipedia.org/wiki/Computer_science), refers to actively **sampling** the status of an [external device](https://en.wikipedia.org/wiki/External_device) by a [client program](https://en.wikipedia.org/wiki/Client_program) as a synchronous(同步的) activity. **Polling** is most often used in terms of [input/output](https://en.wikipedia.org/wiki/Input/output) (I/O), and is also referred to as **polled I/O** or **software-driven I/O**.

> NOTE: 
>
> 关键字: sample；synchronous； 

## Description

**Polling** is the process where the computer or controlling device waits for an [external device](https://en.wikipedia.org/wiki/External_device) to check for its readiness or state, often with low-level hardware. For example, when a [printer](https://en.wikipedia.org/wiki/Printer_(computing)) is connected via a [parallel port](https://en.wikipedia.org/wiki/Parallel_port), the computer **waits** until the printer has received the next character. These processes can be as minute as only reading [one bit](https://en.wikipedia.org/wiki/Status_register). Τhis is sometimes used synonymously（同义） with [**busy-wait**](https://en.wikipedia.org/wiki/Busy_waiting) polling. In this situation, when an I/O operation is required, the computer does nothing other than check the status of the I/O device until it is ready, at which point the device is accessed. In other words, the computer waits until the device is ready. **Polling** also refers to the situation where a device is repeatedly checked for readiness, and if it is not, the computer returns to a different task. Although not as wasteful of [CPU](https://en.wikipedia.org/wiki/CPU) cycles as busy waiting, this is generally not as efficient as the alternative to polling, [interrupt](https://en.wikipedia.org/wiki/Interrupt)-driven I/O.

> NOTE:上面这段话中描述了两种**polling**，第一种为[**busy-wait**](https://en.wikipedia.org/wiki/Busy_waiting) polling，第二种的做法与[**busy-wait**](https://en.wikipedia.org/wiki/Busy_waiting) polling不同，它不会一直loop；最后一段话则将polling与[interrupt](https://en.wikipedia.org/wiki/Interrupt)-driven I/O进行了对比，作者直接指出这两种polling在性能上都不及[interrupt](https://en.wikipedia.org/wiki/Interrupt)-driven I/O。



### Algorithm

**Polling** can be described in the following steps (add a pic):

Host actions:

1. The host repeatedly reads the [busy bit](https://en.wikipedia.org/wiki/Status_register) of the controller until it becomes clear.
2. When clear, the host writes in the command [register](https://en.wikipedia.org/wiki/Hardware_register) and writes a byte into the data-out register.
3. The host sets the command-ready bit (set to 1).

Controller actions:

1. When the controller senses command-ready bit is set, it sets busy bit.
2. The controller reads the command register and since write bit is set, it performs necessary I/O operations on the device. If the read bit is set to one instead of write bit, data from device is loaded into data-in register, which is further read by the host.
3. The controller clears the command-ready bit once everything is over, it clears error bit to show successful operation and reset busy bit (0).



## Poll message

A **poll message** is a control-acknowledgment message(控制确认消息).

In a multidrop line arrangement (a central [computer](https://en.wikipedia.org/wiki/Computer) and different terminals in which the [terminals](https://en.wikipedia.org/wiki/Terminal_(telecommunication)) share a single communication line to and from the computer), the system uses a [master/slave](https://en.wikipedia.org/wiki/Master/slave_(technology)) polling arrangement whereby the central computer sends message (called **polling message**) to a specific terminal on the outgoing line. All terminals listen to the outgoing line, but only the terminal that is polled replies by sending any information that it has ready for transmission on the incoming line.[[1\]](https://en.wikipedia.org/wiki/Polling_%28computer_science%29#cite_note-mpb-1)

In [star networks](https://en.wikipedia.org/wiki/Star_network), which, in its simplest form, consists of one central [switch](https://en.wikipedia.org/wiki/Network_switch), [hub](https://en.wikipedia.org/wiki/Ethernet_hub), or computer that acts as a conduit to transmit messages, polling is not required to avoid chaos on the lines, but it is often used to allow the master to acquire input in an orderly fashion. These poll messages differ from those of the multidrop lines case because there are no site addresses needed, and each terminal only receives those polls that are directed to it.[[1\]](https://en.wikipedia.org/wiki/Polling_%28computer_science%29#cite_note-mpb-1)



