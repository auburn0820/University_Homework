//
//  header.h
//  UNIX Project
//
//  Created by 피수영 on 2020/10/24.
//

#ifndef header_h
#define header_h

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/fcntl.h>

#define MAX_CMD_ARG 15
#define BUFSIZE 256

extern char *prompt;
extern char *cmdvector[];
extern char cmdline[];
extern pid_t pid;
extern char cmdlineCopy[];
//extern int cmdcnt;

void fatal(char *str);
int makelist(char *s, const char *, char **, int);
void myshell_exit(void);
void myshell_cd(char**);
void myshell_exce(char*);
void shellLoop(void);
void sigHandling(int);
void registerSignal(void);
void handleSignal(int);
int checkRedirection(char**);

#endif /* header_h */
