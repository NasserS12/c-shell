shell: shell.o history.o builtins.o parse.o exec.o alias.o
	gcc -Wall -o shell shell.o history.o builtins.o parse.o exec.o alias.o

shell.o: src/shell.c include/history.h include/builtins.h include/parse.h include/exec.h include/alias.h
	gcc -Wall -Iinclude -c src/shell.c

history.o: src/history.c include/history.h
	gcc -Wall -Iinclude -c src/history.c

builtins.o: src/builtins.c include/builtins.h include/alias.h
	gcc -Wall -Iinclude -c src/builtins.c

parse.o: src/parse.c include/parse.h
	gcc -Wall -Iinclude -c src/parse.c

exec.o: src/exec.c include/exec.h include/parse.h
	gcc -Wall -Iinclude -c src/exec.c

alias.o: src/alias.c include/alias.h
	gcc -Wall -Iinclude -c src/alias.c

clean:
	rm -f *.o shell
