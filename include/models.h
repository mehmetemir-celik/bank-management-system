#ifndef MODELS_H
#define MODELS_H

#include "config.h"

typedef struct {
    char name[MAX_NAME_LEN + 1];
    char password[MAX_PASS_LEN + 1];
    int number;
    double balance;
}Account;

#endif