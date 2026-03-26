# Pthreads_Task

# Multi-Threaded Data Logger
A C-based system that uses POSIX Threads and Message Queues to capture, process, and log data in real-time.

Input Thread: Captures numbers from the user.

Format Thread: Calculates the Log10 scale and displays it.

Log Thread: Saves all data safely to a persistent text file.