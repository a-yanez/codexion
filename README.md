This project has been created as part of the 42 curriculum by *ayanez-o*.

# **Description**
*Codexion* is a project which introduces the students of 42 to the concept of concurrency. Concurrency means that multiple processes can start, run and be completed in overlapping time periods.

The concept of concurrency comes encompases several other ones such as:
- **Parallelism**: Simultaneous execution of tasks.
- **Context switch**: In simple terms it refers to pausing a task, often to perfom another one, so that it can be resumed later.
- **Multithreading**: A technique in which a process (an independent program in execution) is divided in multiple threads that operate concurrently.
- **Thread**: An independent sequence of execution within a process. All threads within a process share the same memory space and operating system resources.
- **Synchronization**: Coordinating access to shared resources.

For this project, a variation of the *dining philosophers* problem is simulated changing the philosophers for coders.

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
Given a set of 8 parameters, the coders must complete several cycles without burning out. The dongles also have two possible scheduling modes: FIFO and EDF. In FIFO (*First In-First Out*) the first coder to take a dongle has the priority to use it. In EDF (*Earliest Deadline First*), the coder closer to burn out gets the priority.

## Design choices
### Blocking cases handling
#### Deadlock prevention
Even though all the coders are made to start simultaneously to code, *circular wait* is prevented by making the coders "left" or "right" handed if their identification number is even or odd, respectively. A "left" handed *i* coder will first try to take the dongle *i* and then dongle *i + 1*. A "right" hander coder first approach to dongle *i + 1* and then dongle *i*. For the coder with number *number_of_coders* (see the *Parameters* section), the *i + 1* dongle is the dongle number 1.

#### Starvation prevention
Besides the queue for the dongle, which compares deadlines differences between coders an additional drop mechanism was implemented for EDF scheduling. If a coder is holds a dongle and attempts to take a dongle that's being used, it must drop the held dongle and start over again. This prevents a coder that is unable to complete its compiling cycle from holding a dongle that a neighboring coder, with a potentially closer burn out deadline, might need. 

#### Cooldown handling
Two mechanisms are implemented to handle dongle cooldown:
1. After compiling, dongles are released and the *last_used* time is registered in the dongle. This time can be compared to the current time by a waiting coder to calculate if the cooldown has passed and use the dongle or to wait sleeping.
2. At the same time, during dongle release, a *cooldown finalization time* is setup, which can be used by functions such as *pthread_conditional_timedwait* to make a coder sleep for a precise ammount of time.

#### Burnout detection
A monitor is created which is in charge of stopping the simulation if a coder burns out. The monitors main routine is a loop that constantly checks all the coders' *last_compile_start* time and compares it to the current time to determine if a coder burned out. If a coder already finished all its compile cycles, it is spared from the check.

#### Log serialization
To prevent message interleaving. All coders must use the same *print* mutex to report an event. This means that only one coder can print at a time.

### Thread synchronization mechanisms
- *pthread_create* and *pthread_join* were used to create every thread for every coder and to make the main thread wait for them to finished, either if they finish the compiling cycles or if they stopped because a coder burned out.
- *pthread_mutex_lock*, *pthread_mutex_unlock* and *pthread_mutex_t* was used to create mutex type variables to lock and unlock certain parts of the code. Shared resources between all threads were protected by a single mutex, while resources related to each coder and each dongle were protected by mutexes specific to every coder and every dongle 		

	Shared resources included the synchronized beginning signal, burn out termination signal, print capabilities and cycle completion signal. Specific resources included dongle availability indicators, coders' compilation timestamps and completed cycles. 
- *pthread_cond_t* was used to create condition variables. These were used within dongle taking loops to wait if a dongle was being used or to indicate changes in the queue due to scheduling. Within release/drop functions, these variables were used to indicate that a dongle was avaible and could be taken.
- *pthread_cond_wait* and *pthread_cond_timedwait* were used within the previously mentioned conditional loops to make coder threads to sleep until the conditional signal was broadcasted due to dongle release/drop or queue changes.
- *pthread_cond_broadcast* was used to signal the changes in the mentioned conditional variables to all sleeping threads waiting for those conditions to change.

# **Instructions**
The *codexion* executable can be compiled using the included *Makefile* by typing `make` or `make all` on the command line.

Additionally, temporary files can be erased using `make clean` or, to erase the binary file as well, `make fclean`. To recompile the executable again type `make re`.

With the executable ready, the *codexion* program can be used by typing:
```
./codexion <number_of_coders> <time_to_burnout> <time_to_compile> <time_to_debug> <time_to_refactor> <number_of_compiles_required> <dongle_cooldown> <scheduler>
```

All 8 parameters are mandatory and must be provided in that order. If any one of them is missing, the simulation will not run. Only 0 and positive integers are allowed for the first 7 parameters. For the eighth one, the value must be exactly one of *fifo* or *edf*.

## Parameters
1. ***number_of_coders***: The number of coders and also the number of dongles.
2. ***time_to_burnout*** (in milliseconds): If a coder did not start compiling within *time_to_burnout* milliseconds since the beginning of their last compile or the beginning of the simulation, they burn out.
3. ***time_to_compile*** (in milliseconds): The time it takes for a coder to compile. During that time, a coder holds two dongles.
4. ***time_to_debug*** (in milliseconds): The time a coder will spend debugging.
5. ***time_to_refactor*** (in milliseconds): The time a coder will spend refactoring. After completing the refactoring phase, the coder will immediately attempt to acquire dongles and start compiling again.
6. ***number_of_compiles_required***: If all coders have compiled at least this many times, the simulation stops. Otherwise, it stops when a coder burns out.
7. ***dongle_cooldown*** (in milliseconds): After being released, a dongle is unavailable until its cooldown has passed.
8. ***scheduler***: The arbitration policy used by dongles to decide who gets them when multiple coders request them (FIFO or EDF).

## Output
If the parameters are valid, the program will print on the *stdout* a log like this:
```
0 1 has taken a dongle
1 1 has taken a dongle
1 1 is compiling
201 1 is debugging
401 1 is refactoring
402 2 has taken a dongle
403 2 has taken a dongle
403 2 is compiling
603 2 is debugging
803 2 is refactoring
1204 3 burned out
```
The first number represent a timestamp in ms, the second one the number of coder (ranging from 1 to n) followed by an event. A burn out message will appear only if a coder reach that state and it will be missing if all the coders completed their compiling cycles successfully.

# **Resources**
- [Hierarchical Ordening of Sequential Processes](https://www.cs.utexas.edu/~EWD/ewd03xx/EWD310.PDF) by Edsger W. Dijkstra.
- [The dining philosophers problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem) in Wikipedia.
- [Multithreading](https://www.geeksforgeeks.org/c/multithreading-in-c/) in Geeksforgeeks.
- [Threads, Mutexes and Concurrent Programming in C](https://www.codequoi.com/en/threads-mutexes-and-concurrent-programming-in-c/#synchronization-errors) by Mia Combeau.
- [How can I code a monitor in C?](https://stackoverflow.com/questions/3827598/how-can-i-code-a-monitor-in-c) in Stackoverflow.
- [What is the difference between concurrency and parallelism?](https://stackoverflow.com/questions/1050222/what-is-the-difference-between-concurrency-and-parallelism) in Stackoverflow.

AI was used mainly for debugging and to implement safer functions related for time setup and time difference calculation.