#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

typedef struct {
    char* input_file;
    char* output_file;  
    char* append_file;
} Redirection;

char* remove_quotes(char* str) {
    if (str[0] == '"' && str[strlen(str)-1] == '"') {
        str[strlen(str)-1] = '\0';
        return str + 1;
    }
    return str;
}

void setup_redirection(Redirection* redir) {
    if (redir->input_file != NULL) {
        int fd = open(redir->input_file, O_RDONLY);
        if (fd == -1) { perror("open input file"); exit(1); }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }
    
    if (redir->output_file != NULL) {
        int fd = open(redir->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1) { perror("open output file"); exit(1); }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
    
    if (redir->append_file != NULL) {
        int fd = open(redir->append_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd == -1) { perror("open append file"); exit(1); }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
}

void execute_pipeline(char* args[][MAX_ARGS], int cmd_count, Redirection* redirections) {
    int i;
    int in_fd = 0;
    int fd[2];
    
    for (i = 0; i < cmd_count; i++) {
        if (i < cmd_count - 1 && pipe(fd) == -1) {
            perror("pipe"); exit(EXIT_FAILURE); 
        }
        
        pid_t pid = fork();
        if (pid == 0) {
            if (i > 0) { dup2(in_fd, STDIN_FILENO); close(in_fd); }
            if (i < cmd_count - 1) { close(fd[0]); dup2(fd[1], STDOUT_FILENO); close(fd[1]); }
            
            if (i == 0 && redirections[0].input_file != NULL) {
                int fd_in = open(redirections[0].input_file, O_RDONLY);
                if (fd_in != -1) { dup2(fd_in, STDIN_FILENO); close(fd_in); }
            }
            
            if (i == cmd_count - 1) { setup_redirection(&redirections[i]); }
            
            execvp(args[i][0], args[i]);
            perror("execvp"); exit(1);
        }
        else if (pid < 0) { perror("fork"); exit(1); }
        
        if (i > 0) { close(in_fd); }
        if (i < cmd_count - 1) { close(fd[1]); in_fd = fd[0]; }
    }
    
    for (i = 0; i < cmd_count; i++) { wait(NULL); }
}

void parse_command(char* command, char* args[], Redirection* redir) {
    int arg_count = 0;
    char* token;
    char* saveptr_cmd;
    
    redir->input_file = redir->output_file = redir->append_file = NULL;

    while (*command == ' ') command++;
    

    token = strtok_r(command, " ", &saveptr_cmd); 
    while (token != NULL && arg_count < MAX_ARGS - 1) {
        if (strcmp(token, "<") == 0 || strcmp(token, ">") == 0 || strcmp(token, ">>") == 0) {

            char* next_token = strtok_r(NULL, " ", &saveptr_cmd); //Функция strtok надо получше ознакомиться с документацией
            if (next_token != NULL) {
                char* filename = remove_quotes(next_token);
                if (strcmp(token, "<") == 0) redir->input_file = filename;
                else if (strcmp(token, ">") == 0) redir->output_file = filename;
                else if (strcmp(token, ">>") == 0) redir->append_file = filename;
            }
        } else {
            args[arg_count++] = remove_quotes(token);
        }
        token = strtok_r(NULL, " ", &saveptr_cmd); 
    }
    args[arg_count] = NULL;
}

void execute_single_command(char* args[], Redirection* redir) {
    pid_t pid = fork();
    if (pid == 0) {
        setup_redirection(redir);
        execvp(args[0], args);
        printf("Ошибка: команда '%s' не найдена\n", args[0]);
        exit(1);
    }
    else if (pid > 0) {
        wait(NULL);
    }
    else {
        perror("fork");
    }
}

int parse_input(char* input, char* commands[], char* args[][MAX_ARGS], Redirection* redirections) {
    int cmd_count = 0;
    char* saveptr_input; 
    
    commands[cmd_count] = strtok_r(input, "|", &saveptr_input); 
    
    while(commands[cmd_count] != NULL && cmd_count < MAX_ARGS - 1) {
        parse_command(commands[cmd_count], args[cmd_count], &redirections[cmd_count]);
        cmd_count++;
        commands[cmd_count] = strtok_r(NULL, "|", &saveptr_input); 
    }
    
    return cmd_count;
}


int main() {
    char input[MAX_INPUT];
    char* args[MAX_ARGS][MAX_ARGS];
    char* commands[MAX_ARGS];
    Redirection redirections[MAX_ARGS];

    while(1) {
        printf("myshell> ");
        fflush(stdout);

        if(fgets(input, sizeof(input), stdin) == NULL) {
            printf("Выход (Ctrl+D)\n");
            break;
        }
        
        input[strcspn(input, "\n")] = 0;
        if (strlen(input) == 0) continue;
        if (strcmp(input, "exit") == 0) { printf("Выход из shell\n"); break; }
        
        int cmd_count = parse_input(input, commands, args, redirections);
        
        if (cmd_count == 1) {
            execute_single_command(args[0], &redirections[0]);
        } else {
            execute_pipeline(args, cmd_count, redirections);
        }
    }
    
    return 0;
}