# process-management-separation-of-processes

(03.11.2022)

Part 1. Startup, Termination, and Process Parameters. Make a program that: 

1. Receives and prints information about the parameters of its process using system ones 
calls to getpid(), getgid(), getsid(), etc. 
2. Performs branching of the process using the fork() system call. 
3. For the parent process and the child process, separately prints their identifiers in 
cycles 
4. Make the parent process wait for the child to complete. 
5. Notifies about completion of parent and child processes. 
6. Explain the obtained results 

Part 2. Demonization of the process. Make a program that:

1. Opens the log text file for recording and makes a record of the program start in it. 
2. Performs demonization of its functionality according to the following algorithm: 
- Executes fork() 
- For the parent process, it makes an entry in the log about the generation of the child and closes it 
itself by calling exit(). 
- For a descendant pocess: 
- does setsid() 
- changes the current directory to “/” 
- closes all descriptors opened by the parent 
- opens “/dev/null” for writing to three standard threads 
3. Opens the log and makes an entry in it about the parameters of the daemonized process. 
4. Performs time exposure in an infinite loop. 
5. Explain the obtained results
