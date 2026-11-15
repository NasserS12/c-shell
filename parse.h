struct command {
    int append;
    char *infile;
    char *outfile;
    char **cmd2;
};


int parse_line(char *line, char **args);
int parse_redirects(char **args, int i, struct command *cmd);
