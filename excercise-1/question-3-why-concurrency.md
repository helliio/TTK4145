> Why do we use concurrent execution (multithreading/multiprocessing, or the like)?
> List a fair few reasons.
> - How can adding more concurrent tasks make programming simpler?
>   (Think about translation from idea to code...)

Some task require us to do several tasks at once.
For example: copying a file, while at the same time displaying the progress of this action to the user.
Without concurrency, we need to introduce our own form of task scheduling; such as updating the UI in between copying small amounts of data.
With concurrency, this task management becomes implict.

> - And how can it make programming more difficult? (Maybe come back to
>   this after you have worked on part 4 of this exercise)

If we are not careful, concurrency can introduce observable nondeterminizim to our program.
There usually are parts of a program that we wish to happen atomically.
That is to say: as if all other threads were paused.

In part 4 of this excercise, we demonstrate that we get nondeterministic behaviour.
The shared resourse `accumulator` or `i`, is read and updated by two threads concurrently.
The control interleaving causes some of the changes to the variable to be lost.
The result is that we end up with something else than if we were to execute the two threads one after the other.

> What are the differences between processes, threads, green threads,
> and coroutines?

Both threads and processes are created by cloning another thread.
The difference is that creating a process will assign it a new copy of the memory map, file descriptor table and other kernel data structures.
A thread shares these resources with the thread it was cloned from, but gets a new entry in the process sheduler and a new program counter and stack.

A green thread is not managed nor known to the operating system.
All the grean threads run in one OS thread, and are managed by the runtime of the program.

A coroutine is like a green thread, except it has language support for *yielding* values to the caller.

> - Which one of these do `pthread_create()` (C/POSIX),
>   `threading.Thread()` (Python), `go` (Go) create?

  - POXIX: threads
  - CPython threading module: threads, but limited by the GIL
  - Go: "goroutines": coroutines maybe?

> - How does pythons Global Interpreter Lock (GIL) influence the way a
>   python Thread behaves?

The GIL prevents multiple threads to run at the same time.
In essence this disables any SMP.

> - With this in mind: What is the workaround for the GIL (Hint: it's
>   another module)? How do you then share resources (variables/data)?

One can instead fork a new process using the multiprocessing module.
There are several types of shared resoures one can make use of:
  - Message queues
  - Shared memory
  - Files and sockets

> - What does `func GOMAXPROCS(n int) int` change?

It determines the maximum number of threads the runtime will have at any time.
