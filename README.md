# Minimal-Shell
**Name:** Patrick Kamau
**Reg No:** SCT212-0047/2024

## How to Build and Run

```
 gcc -Wall -Wextra -o msh msh.c 
./msh 
```

To exit the shell, type `exit` or press Ctrl-D.

## Features Implemented

| Task | Feature | Status |
|------|---------|--------|
| 1 | Prompt loop, reading input with `getline`, clean exit on Ctrl-D | Complete |
| 2 | Tokenising input into arguments, ignoring blank lines | Complete |
| 3 | Running external commands with `fork` / `execvp` / `waitpid` | Complete |
| 4 | Built-ins: `cd`, `help`, `exit` | Complete |
| 5 | Showing the previous exit status in the prompt `msh[N]>` | Complete |
| 6 | Background execution with trailing `&`, reaping with `WNOHANG` | Complete |
| 7 | Output redirection with `>` (mode 0644, done in the child) | Complete (see limitations) |
| 8 | Ctrl-C stops the foreground command but not the shell | Complete (see limitations) |

## Design Explanation.
First there is a while loop which displays the prompt then it is followed by a function strtok() which
is used to convert strings to tokens to allow command execution . For instance it may split 2 lines into spaces.
Then there is Fork() process whereby a new process is formed from an existing process. The new process is called a child and 
it acquires all the propertires of the parent thus where the execvp() comes in which replaces the whole program of the child to a new program (ls) using arg as its arguments. While the child(ls) is running the parent(shell) is waiting for the child to complete so that the prompt doesn't comeback until it is finished and to collect the child's exit status. If nobody collects it the child stays as a zombie. There is also use of last_status which shows the last status of a command that exited. Also cd can't be an external program because there is no way a child can change the current working directory of its parent.Exit is also a built-in since when the child runs exit it terminates and the parent process wake up from waitpd() prints the last status of the exited command and keep running.
Zombie processes are avoided by use of WNOHANG() which its work is to look for a child that has been terminated , clean it so as to prevent it from reaching the shell.

## Known Limitations

- **Whitespace required around operators:** Commands must have spaces around `>` and `&` (e.g., `ls > out.txt` works, while `ls>out.txt` does not) because parsing splits strictly on whitespace.
- **No quotes or escaping:** Arguments with spaces inside quotes (such as `echo "hello world"`) split into separate tokens.
- **Single-command execution only:** Pipes (`|`), input redirection (`<`), and append redirection (`>>`) are not implemented.
- **Fixed argument cap:** Each command supports up to 63 arguments (`MAX_ARGS = 64`).

## Sources and AI-Tool Declaration
Some of the sources i have used are mainly geeks for geeks https://www.geeksforgeeks.org/c/fork-system-call/ -for fork process
got some ideas from https://stackoverflow.com/questions/74427153/managing-processes-in-c
https://levelup.gitconnected.com/mastering-fork-and-exec-in-c-a-beginners-guide-to-process-management-in-unix-like-operating-81b2b19b4dfe
https://linuxhint.com/fork_linux_system_call_c/
https://www.codequoi.com/en/creating-and-killing-child-processes-in-c/
https://www.geeksforgeeks.org/c/zombie-and-orphan-processes-in-c/ for zombie processes
also got some ideas from this video https://share.google/MIW8VXXfIPFjAROT4

Also i had the help of google ai mode since when i was starting this project before i figured that i am suppossed to use a loop i was writing my display prompt then after running it terminates . It helped me understand that i should use a loop and also it has acted as a brainstorming partner to where i had a barrier and ofcourse also stack over flow came in handy https://stackoverflow.com/questions/25826294/own-shell-in-c-using-execv and also geeks for geeks https://www.geeksforgeeks.org/c/making-linux-shell-c/