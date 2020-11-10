# [A Technique for Active Object Aggregation in C++](https://www.codeproject.com/articles/9474/a-technique-for-active-object-aggregation-in-c)

## Introduction

 I'm a big fan of the **Active Object pattern**. At some level, a good deal of software can be specified in terms of how a set of components respond to events in a system. The description of such components' behaviors can be more easily understood if it is assumed they only occur one at a time. The **Active Object** simplifies design planning by ensuring that an object's specified behaviors are synchronized. It decouples the moment an event occurs from the moment it is responded to accept events from an asynchronous system. 

