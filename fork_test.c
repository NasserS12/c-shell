#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(){

    char *args[] = {"ls", "-l", NULL};

    pid_t pid = fork();

    if(pid == 0){
        execvp(args[0],args);
        printf("hello after exec ");
        printf("I am the child, my pid is : %d\n",getpid());
    }else{
        waitpid(pid, NULL, 0);
        printf("parent done ");
        printf("I am the parent, my pid is: %d\n",getpid());
    }

    return 0;
}