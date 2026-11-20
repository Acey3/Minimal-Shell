# Test Plan

Tested on: <e.g. Ubuntu 22.04 / WSL>
Build: `gcc -Wall -Wextra -o msh msh.c`

## Required Tests

| # | Input | Expected behaviour | Actual result |
|---|-------|--------------------|---------------|
| 1 | (blank line) | New prompt, no error | it passes new prompt |
| 2 | `echo hello world` | Prints text; status 0 |Prints hello world |
| 3 | `ls /nonexistent` | Error from ls; non-zero status |ls: cannot access '/nonexistent': No such file or directory |
| 4 | `nosuchcommand` | Error message; status 127 | execvp failed: No such file or directory|
| 5 | `cd /tmp` then `pwd` | Prints /tmp |prints /tmp |
| 6 | `sleep 3 &` | Prompt returns at once; PID shown |[115056] |
| 7 | `echo hi > f.txt` then `cat f.txt` | File contains hi |file contains hi |
| 8 | `sleep 100` then Ctrl-C | Sleep stops; shell survives; status 130 | status 130 ^Cmsh[130]>|
| 9 | Ctrl-D at the prompt | Shell exits cleanly |Exits cleanly |

## My Own Tests

| # | Input | Expected behaviour | Actual result |
|---|-------|--------------------|---------------|
| 10 | `cd /nonexistent` | Prints "cd failed: No such file or directory"; shell keeps running | prints cd failed: No such file or directory|
| 11 | `echo one > f.txt` then `echo two > f.txt` then `cat f.txt` | File contains only `two` (old content truncated); `ls -l f.txt`shows `-rw-r--r--`|msh[0]>cat f.txt twomsh[0]>ls -l f.txt -rwxrwxrwx 1 acey3 txt|
| 12 | `sleep 2 &`, wait 3 seconds, press Enter, then `ps` | PID shown immediately; after Enter, `ps` shows no `sleep` and no `<defunct>` process |msh[0]>ps 115813 pts/6  true it shows no sleep|


