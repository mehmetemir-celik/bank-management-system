#ifndef SERVICES_H
#define SERVICES_H

#include "models.h"

typedef enum {
    SERVICE_SUCCESS,
    SERVICE_ERR_INVALID_INPUT,
    SERVICE_ERR_NOT_FOUND,
    SERVICE_ERR_WRONG_PASSWORD,
    SERVICE_ERR_INSUFFICIENT_FUNDS,
    SERVICE_ERR_DATABASE
}ServiceResult;

ServiceResult create_account(char* name, char* password, int* new_account_number);
ServiceResult log_in(int number, char* password, Account* logged_in_account_ptr);
ServiceResult deposit(Account* account_ptr, double amount);
ServiceResult withdraw(Account* account_ptr, double amount);
ServiceResult transfer(Account* sender_ptr, int receiver_number, double amount);
ServiceResult change_password(Account* account_ptr, char* new_password);
ServiceResult get_account_history(int account_number, char* history, int history_size);

#endif