This project has been created as part of the 42 curriculum by *ayanez-o*.

# **Description**
*Codexion* is a project which introduces the students of 42 to the concept of concurrency. Concurrency means that multiple processes can start, run and be completed in overlapping time periods.

The concept of concurrency comes encompases several other ones such as:
- **Parallelism**: Simultaneous execution of tasks.
- **Context switch**: In simple terms it refers to pausing a task, often to perfom another one, so that it can be resumed later.
- **Multithreading**: A technique in which a process (an independent program in execution) is divided in multiple threads that operate concurrently.
- **Thread**: An independent sequence of execution within a process. All threads within a process share the same memory space and operating system resources.
- **Synchronization**: Coordinating access to shared resources.

For this project, a variation of the *dining philosophers* problem is simulated changing the philosophers for coders and USB dongles instead of forks.

## The dining philosophers
The dining philosophers problem is an example used in concurrent algorithm design to illustrate synchronization issues and techniques for resolving them.

Five philosophers (labeled P0-P4) dine together in the same table. They alternate between thinking and eating. Each one of them has a plate in front of them filled with a difficult type of spaghetti that needs two forks to be eaten. There are also five forks, all of them placed between plates (F0-F4). As they have to be shared by the neighboring philosophers, no two neighbors can eat simultaneously.
```
	           P0 (Think)
	         /    \
	      F4        F0
	     /            \
	   P4 (Eat)       P1 (Think)
	    |              |
	    F3            F1
	     \            /
	      P3 --F2-- P2
	      (Think)   (Eat)
```

The eating and thinking phases of the philosophers have to be synchronized in order to avoid all of them to starve in the case all of them enter in an eating phase at the same time and get stucked trying to hold two forks (*deadlock*). Which could happen, for instance, if their normal behavior is holding the fork on the left side first and then the one on their right side.  

## Codexion variation
In the case of codexion, the problem is basically the same. However, instead of philosophers we have coders who try to complete cycles of compiling, debugging and refactoring. In order to compile, they have to hold two USB dongles. After compiling, they can proceed with the debugging and refactoring phases. If they fail to start to compile after a determined time they will burnout.

## The challenge
Given a set parameters, the coders must complete several cycles without burning out. The dongles also have two possible scheduling modes: FIFO and EDF. In FIFO (*First In-First Out*) the first coder to take a dongle has the priority to use it. In EDF (*Earliest Deadline First*), the coder closer to burn out gets the priority.

# **Instructions**
Use the makefile included in the repo.

# **Resources**
- [Hierarchical Ordening of Sequential Processes](https://www.cs.utexas.edu/~EWD/ewd03xx/EWD310.PDF) by Edsger W. Dijkstra.
- [The dining philosophers problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem) in Wikipedia.
- [Multithreading] (https://www.geeksforgeeks.org/c/multithreading-in-c/) in Geeksforgeeks.
- [Threads, Mutexes and Concurrent Programming in C](https://www.codequoi.com/en/threads-mutexes-and-concurrent-programming-in-c/#synchronization-errors) by Mia Combeau.
- [How can I code a monitor in C?](https://stackoverflow.com/questions/3827598/how-can-i-code-a-monitor-in-c) in Stackoverflow.
- [What is the difference between concurrency and parallelism?](https://stackoverflow.com/questions/1050222/what-is-the-difference-between-concurrency-and-parallelism) in Stackoverflow.
