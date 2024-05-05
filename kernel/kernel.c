#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "process.h"
#include "system.h"
void print_minios(char* str);

int main() {
    print_minios("[MiniOS SSU] Hello, World!");

    char *input;

    while(1) {
        // readline을 사용하여 입력 받기
        input = readline("커맨드를 입력하세요(종료:exit) : ");

        if (strcmp(input,"exit") == 0) {
            break;
        }

        else if (strcmp(input,"minisystem") == 0){
            minisystem();
        }
        else if (strcmp(input,"process duplicate") == 0){
            duplicate();
        }
        else if (strcmp(input,"ipc") == 0){
 	    ipc_test();
        }
        else if (strcmp(input,"multi pi") == 0){
 	    multi();
        }
        else if (strcmp(input,"process") == 0){
 	    process();
        }
        else if (strcmp(input,"single pi") == 0){
 	    single();
 	}
        else 
        {
        	printf("%s\n", input);
        	system(input);
        }
    }

    // 메모리 해제
        free(input);
    print_minios("[MiniOS SSU] MiniOS Shutdown........");

    return(1);
}


void print_minios(char* str) {
        printf("%s\n",str);
}
