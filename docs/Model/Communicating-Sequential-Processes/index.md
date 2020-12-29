# Communicating Sequential Processes

是在了解Golang时，发现的这个概念:

## golangprograms [Golang Concurrency](https://www.golangprograms.com/go-language/concurrency.html):

Communicating Sequential Processes, or CSP for short, is used to describe how systems that feature multiple concurrent models should interact with one another. It typically relies heavily on using **channels** as a medium for passing messages between two or more concurrent processes, and is the underlying mantra of Golang.

> NOTE: 上述 "multiple concurrent models"其实对应的就是multiple model。

## wikipedia [Communicating Sequential Processes](https://en.wikipedia.org/wiki/Communicating_sequential_processes)

In computer science, communicating sequential processes (CSP) is a formal language for describing patterns of interaction in concurrent systems. It is a member of the family of mathematical theories of concurrency known as process algebras, or process calculi, based on message passing via channels. CSP was highly influential in the design of the occam programming language and also influenced the design of programming languages such as Limbo, RaftLib, Go, Crystal, and Clojure's core.async.





## CSP-style concurrency

Python: [zh217](https://github.com/zh217)/[aiochan](https://github.com/zh217/aiochan)