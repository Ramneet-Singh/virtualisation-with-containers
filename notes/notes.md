- Kernel is the central software that manages and allocates computer resources (i.e. the CPU, RAM and devices).

# Tasks performed by the Kernel

- **Process Scheduling**: Linux is a *preemptive multitasking* operating system.
- **Memory Management**: Physical memory (RAM) remains a limited resource that the kernel must share among processes in an equitable and efficient fashion. Linux uses virtual memory management, with two main advantages:
    - Processes are isolated from one another and from the kernel, so that one process can’t read or modify the memory of another process or the kernel.
    - Only part of a process needs to be kept in memory, thereby lowering the memory requirements of each process and allowing more processes to be held in RAM simultaneously. This leads to better CPU utilization, since it increases the likelihood that, at any moment in time, there is at least one process that the CPU(s) can execute.
- **Provision of a file system**: The kernel provides a file system on disk, allowing files to be created, retrieved, updated, deleted, and so on.
- **Creation and termination of processes**
- **Access to devices**
- **Networking**
- **Provision of a system call Application Programming Interface (API)**

- In addition to the above features, multiuser operating systems such as Linux generally provide users with the abstraction of a *virtual private computer*; that is, each user can log on to the system and operate largely independently of other users.

- The child inherits copies of the parent’s data, stack, and heap segments, which it may then modify independently of the parent’s copies. (The program text, which is placed in memory marked as read-only, is shared by the two processes.)

- Traditionally, on UNIX systems, a privileged process is one whose effective user ID is 0 (superuser). Such a process bypasses the permission restrictions normally applied by the kernel. 

- Another way a process may become privileged is via the set-user-ID mechanism, which allows a process to assume an effective user ID that is the same as the user ID of the program file that it is executing.

- When referring to these file descriptors in a program, we can use either the numbers (0, 1, or 2) or, preferably, the POSIX standard names defined in <unistd.h>.

- Although the variables stdin, stdout, and stderr initially refer to the process’s standard input, output, and error, they can be changed to refer to any file by using the freopen() library function. As part of its operation, freopen() may change the file descriptor underlying the reopened stream. In other words, after an freopen() on stdout, for example, it is no longer safe to assume that the underlying file descriptor is still 1.

- Process states
    - New
    - Ready (in memory) (no cpu)
    - Running (cpu)
    - Waiting (could be off memory) (no cpu)
    - Terminate

- new --> ready
- ready <--> wait (Can it go from ready to wait? Not sure)
- ready <--> run (could go from run-->ready because of interrupts)
- run --> terminate (voluntary)
- waiting --> terminate (involuntary through signals)
- ready --> terminate (involuntary through signals)
- ready --> wait

- write() and read() calls are atomic.
- execv() overwrites code, data and stack.

- Is wait() polling?
- wait() vs waitpid() with options=0?
- Can register cleanup functions with atexit()

- When parent calls wait(), it is put into the waiting queue. When child exits, the kernel delivers a SIGCHLD signal to the process. During the time between when the child exits and when it responds to this signal, the parent is in the ready queue because it will need to run to respond to the signal.

# Isolation

1. Do you have access to si?
2. What is your identity? Guest 1 or Guest 2?
3. What is the action E'? Do you have the credentials/privileges to make that action?

Process --syscall--> Function --Privilege--> Resource
            /|\                /|\
             |                  |
             |                  |
            Two Mechanisms for Isolation

# Privilege

- Views
- Permissions
- Credentials
- Capabilities

- Real ID: Inherited from parent process
- Effective ID: Governing the permissions of the process. Can be changed using setuid bit on files or syscalls.
- Saved Set ID: For EID->RID Conversion.
