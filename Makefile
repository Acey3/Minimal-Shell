CC     = gcc
CFLAGS = -Wall -Wextra -g

msh: msh.c
	$(CC) $(CFLAGS) -o msh msh.c

clean:
	rm -f msh *.o
