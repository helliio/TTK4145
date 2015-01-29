> What is an atomic operation?

An atomic operation is an operation that completes in a single step relative to other threads.
It means that a store operation is atomic when preformed on a shared variable if and only if no other
threads can observe the changes half complete. As for atomic loading, the operation loads the whole
value in a single action.

> What is a semaphore?

A Semaphore is a abstract data type that controlls access to resources by multiple prosesses.
For example there is a dinner table with four chairs if all chairs are filled then no more people can
be seated, as soon as a person leaves a chair, the space is cleared and a new person can be seated. the 
semaphore is controlling how many people can access the resourse at any given time to minimise the chance of
a race condition.

> What is A mutex?

Mutex or mutual exclusion means there is a event that can not happend at the same time. Mutex is a 
crutial part of concurentcy controll, it ensures that no more than one concurrent prosess can enter
a critical section (explained later) at the same time. This is done to prevent the Horrible and Scary
race conditions.

> What is a critical section

A critical section is defined as a section of code that acsesses a shared resource. Two concurrent
prosesses can not be in a critical section at the same time or else we will have to face race conditions.
