#ifndef DATABASE_H
#define DATABASE_H

#include "models.h"

typedef enum {
    DB_SUCCESS,
    DB_ERR_NOT_FOUND,
    DB_ERR_FILE
} DBResult;

int get_next_account_number();
DBResult append_new_account(Account* account_ptr);
DBResult get_account_by_number(Account* account_ptr, int target_number);
DBResult update_account(Account* account_ptr);
DBResult log_transaction(int account_number, char* process_type, double amount);
DBResult get_transaction_history(int account_number ,char* history , int history_size);

#endif