//
//  methods.c
//  UNIX Project
//
//  Created by 피수영 on 2020/10/24.
//

#include "header.h"

void fatal(char *str){
    perror(str);
    exit(1);
}

void registerSignal() {
    signal(SIGINT, handleSignal);
    signal(SIGTSTP, handleSignal);
    signal(SIGQUIT, handleSignal);
    signal(SIGCHLD, handleSignal);
}

void handleSignal(int sig) {
    if(sig == SIGCHLD) {
        while(waitpid(-1, NULL, 0) > 0) {};
        printf("\n");
    } else {}
}

void parsingCmdVector(char** cmdvector, int index) {
    cmdvector[index] = NULL;
    cmdvector[index + 1] = NULL;
    
    int secondIndex = index;
    while(cmdvector[secondIndex] != NULL) {
        cmdvector[secondIndex] = cmdvector[secondIndex + 2];
        secondIndex++;
    }
    cmdvector[index] = NULL;
}

int redirection(char** cmdvector) {
    int index = 0;
    int fd;
    while(cmdvector[index] != NULL) {
        if(!strcmp(cmdvector[index], ">")) {
            if(cmdvector[index + 1] == NULL) {
                printf("Wrong Command\n");
                return -1;
            }
            if((fd = open(cmdvector[index + 1], O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1)
                fatal("redirection()");
            dup2(fd, 1);
            close(fd);
            parsingCmdVector(cmdvector, index);
        } else if(!strcmp(cmdvector[index], "<")) {
            if(cmdvector[index + 1] == NULL) {
                printf("Wrong Command\n");
                return -1;
            }
            if((fd = open(cmdvector[index + 1], O_RDONLY)) == -1)
                fatal("redirection()");
            dup2(fd, 0);
            close(fd);
            parsingCmdVector(cmdvector, index);
        }
        index++;
    }
    return 0;
}

void setPipe(char* cmdline) {
    int p[2];
    char* cmdvectorCopy[MAX_CMD_ARG];
    char* cmdvectorToExec[MAX_CMD_ARG];
    int pipeCnt = 0;
    int cmdcnt = 0;
    pid_t pid;
    
    // 파이프 기호로 각 명령어를 분리 및 분별한다.
    cmdcnt = makelist(cmdline, "|", cmdvectorCopy, MAX_CMD_ARG);
    
    if(cmdcnt > 1) {
        for(int i = 0; i < cmdcnt; i++) {
            pipe(p);
            switch (pid = fork()) {
                case 0:
                    pipeCnt = makelist(cmdvectorCopy[i], " \t", cmdvectorToExec, MAX_CMD_ARG);
                    close(p[0]);
                    dup2(p[1], 1);
                    redirection(cmdvectorToExec);
                    execvp(cmdvectorToExec[0], cmdvectorToExec);
                    fatal("setPipe()");
                    break;
                case -1:
                    fatal("setPipe()");
                default:
                    close(p[1]);
                    dup2(p[0], 0);
            }
        }
    } else {
        pipeCnt = makelist(cmdvectorCopy[cmdcnt - 1], " \t", cmdvectorToExec, MAX_CMD_ARG);
        if(!strcmp(cmdvectorToExec[pipeCnt - 1], "&")) {
            cmdvectorToExec[pipeCnt - 1] = NULL;
        }
        redirection(cmdvectorToExec);
        execvp(cmdvectorToExec[0], cmdvectorToExec);
        fatal("checkPipe()");
    }
    return;
}

void shellLoop() {
    char *prompt = "myshell> ";
//    char *cmdvector[MAX_CMD_ARG];
//    char cmdline[BUFSIZE];
//    char cmdlineCopy[BUFSIZE];
    
    pid_t pid;
    int cmdcnt = 0;
    int background = 0;
    
    registerSignal();
    
    while (1) {
        char *cmdvector[MAX_CMD_ARG];
        char cmdline[BUFSIZE];
        char cmdlineCopy[BUFSIZE];
        int i = 0;
        fputs(prompt, stdout);
        fgets(cmdline, BUFSIZE, stdin);
        cmdline[strlen(cmdline) - 1] = '\0';
        strcpy(cmdlineCopy, cmdline);
        i = makelist(cmdline, " \t", cmdvector, MAX_CMD_ARG);
        cmdcnt = i;
        
        if(cmdvector[0] == NULL)
            continue;
        
        if(!strcmp("&", cmdvector[cmdcnt - 1])) {
            cmdvector[cmdcnt - 1] = NULL;
            myshell_exce(cmdlineCopy);
            continue;
        }
        
        if(!strcmp("exit", cmdvector[0])) {
            myshell_exit();
        } else if(!strcmp("cd", cmdvector[0])) {
            myshell_cd(cmdvector);
        } else {
            switch(pid = fork()){
                case 0:
                    setPipe(cmdlineCopy);
                    return;
                case -1:
                    fatal("shellLoop()");
                    break;
                default:
                    if(background == 1) {
                        background = 0;
                        break;
                    }
                    waitpid(pid, NULL, 0);
            }
        }
    }
}

int makelist(char *s, const char *delimiters, char **list, int MAX_LIST) {
    int   numtokens = 0;
    char *snew = NULL;

    if ((s == NULL) || (delimiters == NULL)) return -1;

    snew = s + strspn(s, delimiters);
    if ((list[numtokens] = strtok(snew, delimiters)) == NULL)
        return numtokens;

    numtokens = 1;

    while (1) {
        if ((list[numtokens] = strtok(NULL, delimiters)) == NULL)
            break;
        if (numtokens == (MAX_LIST - 1))
            return -1;
        numtokens++;
    }

    return numtokens;
}

void myshell_exit() {
    exit(1);
}

void myshell_cd(char **cmdvector) {
    if(cmdvector[1] == NULL) {
        chdir(getenv("HOME"));
        return;
    }
    
    char *pathname = cmdvector[1];
    
    if(chdir(pathname) == -1) {
        char *str = "myshell_cd()";
        fatal(str);
    }
}

void myshell_exce(char *cmdline) {
    pid_t pid;
    int status;
    
    switch (pid = fork()) {
        case -1:
            fatal("myshell_exce()");
            break;
        case 0:
            setPipe(cmdline);
            return;
        default:
            waitpid(pid, &status, WNOHANG);
            break;
    }
}
