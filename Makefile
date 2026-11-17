shell: shell.o history.o builtins.o parse.o exec.o alias.o
	gcc -Wall -o shell shell.o history.o builtins.o parse.o exec.o alias.o

history.o: history.c history.h
	gcc -Wall -c history.c

parse.o: parse.c parse.h
	gcc -Wall -c parse.c

exec.o: exec.c exec.h parse.h
	gcc -Wall -c exec.c

shell.o: shell.c history.h builtins.h parse.h exec.h alias.h
	gcc -Wall -c shell.c

builtins.o: builtins.c builtins.h alias.h
	gcc -Wall -c builtins.c

alias.o: alias.c alias.h
	gcc -Wall -c alias.c

clean:
	rm -f *.o shell


