#ifndef UTILS_H
#define UTILS_H

void clear_buffer();
void clear_screen();

void input_string(char* string, int size);
void input_int(int* ptr);
void input_double(double* ptr);

void retry_message(char* message);
void continue_message(char* message);

#endif