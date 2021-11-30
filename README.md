# Signals in xv6

Welcome! In today's lab, we will be implementing signals in xv6.
If you did not know, signals are a form of inter-process communication,
in which each process will have a set of special functions called signal handlers.

Each signal handler is associated with a integer number which represents
the signal that causes the signal handler to be called.

You can clone the repo by using:
```bash
git clone https://github.com/slustig210/xv6-signals-fork.git
```

## Part 1 - Introduction to signals (15 points)

In today's implementation, we will store signals and signal handlers
in the process struct. This struct is defined in `proc.h`. Check out some of
the fields in the `proc` struct, and especially take a look at `signal` and
`sighandlers`.

```c
// Per-process state
struct proc {
  uint sz;                     // Size of process memory (bytes)
  pde_t* pgdir;                // Page table
  char *kstack;                // Bottom of kernel stack for this process
  enum procstate state;        // Process state
  int pid;                     // Process ID
  struct proc *parent;         // Parent process
  struct trapframe *tf;        // Trap frame for current syscall
  struct context *context;     // swtch() here to run process
  void *chan;                  // If non-zero, sleeping on chan
  int killed;                  // If non-zero, have been killed
  struct file *ofile[NOFILE];  // Open files
  struct inode *cwd;           // Current directory
  char name[16];               // Process name (debugging)

  // signal says which signals have yet to be handled
  // signal_handlers say which function to perform upon receiving a signal
  uint signal; // 32 bits
  sighandler_t sighandlers[NSIGNAL];
};
```

`signal` is a 32-bit unsigned integer.
This number will store our currently unhandled signals for each process.
The index of each bit (0 to 31) that is set to 1 will determine what signals
have been sent and have yet to be handled for each process. For example,
the number `0b1` will correspond to the signal of index 0, the number `0b10` will
correspond to the signal of index 1, and so on.

`sighandlers` is an array of signal handlers, which are functions of the type `void sighandler(void)`. 
The function at each index of the array will be called when the signal number of that index is received by the process.
In today's lab, each of these will be initialized to 0 on creation of a new user process.

## Part 2 - Implementing some system calls (15 points)

For our implementation, we will be implementing 2 system calls:
```c
int signal(int signum, sighandler_t sighandler);
int sigsend(int pid, int signum);
```
Signal assigns, for the current process, the given signal handler to the given signal number.
Sigsend finds the process with the given pid and sends a signal with the given signal number.
Both return 0 on success and -1 on error.

## Part 3 - Processing signals (15 points)

There are only a few other things we need to do other than implementing the system calls to complete the implementation.
First, we must update the `scheduler()` function in `proc.c` to register the signal handlers upon a process receiving a signal.
It must check which signals the process received, register each handler we need for that set of signals, and reset the value
of `signal` (in the proc struct) back to 0. To register a handler we have the `register_handler` function defined in `proc.c`. This function adds a stack frame in the process, which will mean the signal handler will be called immediately when the process starts running again.

## Part 4 - Finishing up our implementation (15 points)

We have 2 more features left to implement: we must initialize the values of `signal` and `sighandlers` upon creating a new process. This also applies to creating a process with `fork()`, in which case we must copy the `sighandlers` from the parent process to the child process. This is done in `fork()` in `proc.c`.

Lastly, we must implement code in `exit()` which will send a `SIGCHLD` signal to the parent process every time a child exits.

## Part 5 - You're done! (5 points)

After all these steps, the implementation will be complete. I encourage you to play around with the implementation; you can modify `signaltest.c` as you please.
