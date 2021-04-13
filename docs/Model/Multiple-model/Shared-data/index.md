# Shared data

显然对于不同的concurrency/parallel element/unit，它们的shared data是可能不同的，下面是一些简单的例子:

1、DBMS  table

2、file

3、memory

4、global variable

参见: wikipedia [Reentrancy (computing)](https://en.wikipedia.org/wiki/Reentrancy_(computing))

> Data has a characteristic called [scope](https://en.wikipedia.org/wiki/Scope_and_extent), which describes where in a program the data may be used. Data scope is either [global](https://en.wikipedia.org/wiki/Global_variable) (outside the [scope](https://en.wikipedia.org/wiki/Variable_scope) of any function and with an indefinite extent) or [local](https://en.wikipedia.org/wiki/Local_variables,_recursion_and_reentrancy) (created each time a function is called and destroyed upon exit).
>
> Local data is not shared by any routines, re-entering or not; therefore, it does not affect re-entrance. Global data is defined outside functions and can be accessed by more than one function, either in the form of [global variables](https://en.wikipedia.org/wiki/Global_variable) (data shared between all functions), or as [static variables](https://en.wikipedia.org/wiki/Static_variable) (data shared by all functions of the same name). 

5、......

> NOTE: 后续进行补充