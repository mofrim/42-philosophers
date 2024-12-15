# 42's Philosophers Notes

-  Even NumOfPhilos => time_to_die = time_to_eat + time_to_sleep + 10
-  Odd NumOfPhilos => time_to_die = 2*time_to_eat + time_to_sleep + 10

How to mitigate Deadlocks and Dying:
1) Make half of the Philosophers sleep by the start of the program. F.ex. take
   all philos with odd num to sleep tte/2
2) 

# a chat about join vs. detach

    is pthread_detach always to be preferred over pthread_join?

Whether to use pthread_detach or pthread_join in a multithreaded C program
depends on the specific requirements of your application and the behavior you
want to achieve. Each function serves a different purpose, and neither is
universally preferred over the other. Here are some considerations to help you
decide which one to use in different scenarios:
When to Use pthread_detach

    Fire-and-Forget Threads: If you create a thread that performs a task and you
    do not need to wait for it to finish or retrieve its return value,
    pthread_detach is appropriate. This is common in scenarios like logging,
    background processing, or handling events where the main thread does not
    need to synchronize with the worker thread.

    Resource Management: Using pthread_detach allows the system to automatically
    reclaim resources used by the thread when it terminates. This can simplify
    resource management and reduce the risk of memory leaks.

    Simplicity: If your application design allows for it, using detached threads
    can simplify the code by eliminating the need for join operations, making it
    easier to read and maintain.

When to Use pthread_join

    Synchronization: If you need to wait for a thread to complete its execution
    before proceeding (for example, to ensure that a critical section of code is
    executed only after the thread has finished), you should use pthread_join.
    This is essential in scenarios where the result of the thread's work is
    needed by the main thread.

    Return Values: If the thread returns a value that you need to capture,
    pthread_join is necessary. It allows you to retrieve the return value of the
    thread when it finishes.

    Thread Lifecycle Management: If you need to manage the lifecycle of threads
    more explicitly, such as ensuring that certain threads complete before
    exiting the main program, pthread_join is the right choice.

Summary

    Use pthread_detach when you want to create a thread that runs independently,
    and you do not need to synchronize with it or retrieve its result.
    Use pthread_join when you need to wait for a thread to finish, retrieve its
    return value, or ensure that certain operations are completed before
    proceeding.

Example Scenarios

    Logging System: As discussed earlier, a logging thread can be detached since
    the main application does not need to wait for it.
    Data Processing: If you have a thread that processes data and the main
    thread needs the processed data, you would use pthread_join to ensure the
    data is ready before continuing.

In conclusion, the choice between pthread_detach and pthread_join should be
based on the specific needs of your application, and both functions have their
appropriate use cases.
