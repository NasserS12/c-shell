#include <signal.h>
#include "parse.h"

extern volatile sig_atomic_t running;

void run_pipe(char **args, char **cmd2);
void run_command(char **args, struct command *cmd);