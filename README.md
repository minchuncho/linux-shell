# Notes on Linux Shell

## Build-in Command

Commands such as `exit` or `cd` cannot be carried out by child processes because the parent process is beyond the change of any child process.  
Hence, we need to make such commands **build-in**.

## Piping

When there are several commands in order and one command's output is the input of the command right next to it, **pipe** comes in handy.  
However, before talking about how pipe works, we need to first understand what file descriptor is.  

#### File Descriptor
![image](https://user-images.githubusercontent.com/47070238/153165535-6e786e97-d55b-4036-9008-a4674dcb1dfb.png)  
By defult, each process comes complete with three file descriptors: `stdin(0)`, `stdout(1)`, and `stderr(2)`, which they point to specific files, respectively.  
In other words, the process read input from the file which `stdin` points to and output to the file which `stdout` points to.  

> File descriptors are shared among processes by `fork`. \
> https://www.ibm.com/docs/en/aix/7.1?topic=volumes-using-file-descriptors

#### Pipe
![image](https://user-images.githubusercontent.com/47070238/153124676-0b2ba04b-1c3e-477c-b0af-aa8254e90210.png)  
In the above figure, process 0 hopes to output to process 1, therefore, we need to replace process 0's `stdout` and prcess 1's `stdin` using a pipe.  
By the virtue of file descriptor, we ask for two new file descriptors to represent the pipe's two ends through the function `pipe(fd)`.  
Now, the process has a pipe with two ends: `fd[0]=3` as input and `fd[1]=4` as output.  
We then reassign process 0's `stdout` to where `fd[1]` points to, i.e., we want process 0 to write to the file where fd[1] points to.  
```
dup2(fd[1], STDOUT_FILENO);
```  
Also, we reassign process 1's `stdin` to where `fd[0]` points to, i.e., we want process 1 to read from where fd[0] points to.  
```
dup2(fd[0], STDIN_FILENO);
```  
Conceptually, the pipe is like a **memory buffer** which process 0 writes data to and process 1 reads data from.  
Afterwards, we can turn off `fd[0]` and `fd[1]` by calling `close(fd[0])` and `close(fd[1])` since they're no longer in use. (Both files have been handled by `stdin` and `stdout`.)

In my codes, the parent process is in charge of closing and opening pipes while each child process is responsible for linking pipe ends to the right open files.  
Though the file descriptors are shared among processes due to `fork()`, we don't want to do any change to the parent process's file descriptors in order that in the end, the last commmand works correctly.

> Note that the amount of pipes is limited so we must not turn on all pipes at once.
