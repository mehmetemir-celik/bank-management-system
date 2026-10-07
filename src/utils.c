#include "../include/utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void clear_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}


void input_string(char* string, int length) {
    fgets(string, length, stdin);
    char* ptr = strchr(string, '\n');
    if(ptr == NULL) clear_buffer();
    else *ptr = '\0';
}

void input_int(int* ptr) {
    scanf("%d", ptr);
    clear_buffer();
}

void input_double(double* ptr) {
    scanf("%lf", ptr);
    clear_buffer();
}

void retry_message(char* message) {
    printf(
    "\n"
    "\n%s"
    "\nPress enter to try again..."
    , message);
    clear_buffer();
}

void continue_message(char* message) {
    printf(
    "\n"
    "\n%s"
    "\nPress enter to continue..."
    , message);
    clear_buffer();
}