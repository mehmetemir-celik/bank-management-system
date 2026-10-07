#include "../include/models.h"
#include "../include/database.h"
#include "../include/services.h"
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

static bool validate_name(char* name) {
    if(name == NULL) return false;
    int length = strlen(name);
    if(length > MAX_NAME_LEN || length < 1) return false;
    for(int i = 0; i < length; i++) if(!isalpha(name[i]) && name[i] != ' ') return false;
    return true;
}

static bool validate_password(char* password) {
    if(password == NULL) return false;
    int length = strlen(password);
    if(length > MAX_PASS_LEN || length < MIN_PASS_LEN) return false;
    bool upper = false, lower = false, digit = false;
    for(int i = 0; i < length; i++) {
        if(islower(password[i])) lower = true;
        if(isupper(password[i])) upper = true;
        if(isdigit(password[i])) digit = true;
    }
    return lower && upper && digit;
}

ServiceResult create_account(char* name, char* password, int* new_account_number) {
    if (!validate_name(name) || !validate_password(password)) return SERVICE_ERR_INVALID_INPUT;
    Account account;
    account.number = get_next_account_number();
    account.balance = 0.0;
    strcpy(account.name, name);
    strcpy(account.password, password);
    DBResult result = append_new_account(&account);
    if(result == DB_ERR_FILE) return SERVICE_ERR_DATABASE;
    *new_account_number = account.number;
    return SERVICE_SUCCESS;
}

ServiceResult log_in(int number, char* password, Account* logged_in_account_ptr) {
    if (password == NULL || logged_in_account_ptr == NULL) return SERVICE_ERR_INVALID_INPUT;
    Account copied_account;
    DBResult result = get_account_by_number(&copied_account, number);
    if(result == DB_ERR_FILE) return SERVICE_ERR_DATABASE;
    if(result == DB_ERR_NOT_FOUND) return SERVICE_ERR_NOT_FOUND;
    if(strcmp(password, copied_account.password) != 0) return SERVICE_ERR_WRONG_PASSWORD;
    *logged_in_account_ptr = copied_account;
    return SERVICE_SUCCESS;
}

ServiceResult deposit(Account* account_ptr, double amount) {
    if (account_ptr == NULL || amount <= 0) return SERVICE_ERR_INVALID_INPUT;
    account_ptr->balance += amount;
    DBResult result = update_account(account_ptr);
    if(result == DB_ERR_FILE) {
        account_ptr->balance -= amount;
        return SERVICE_ERR_DATABASE;
    }
    log_transaction(account_ptr->number, "DEPOSIT", amount);
    return SERVICE_SUCCESS;
}

ServiceResult withdraw(Account* account_ptr, double amount) {
    if(account_ptr == NULL || amount <= 0) return SERVICE_ERR_INVALID_INPUT;
    if(account_ptr->balance < amount) return SERVICE_ERR_INSUFFICIENT_FUNDS;
    account_ptr->balance -= amount;
    DBResult result = update_account(account_ptr);
    if(result == DB_ERR_FILE) {
        account_ptr->balance += amount;
        return SERVICE_ERR_DATABASE;
    }
    log_transaction(account_ptr->number, "WITHDRAWAL", amount);
    return SERVICE_SUCCESS;
}


ServiceResult transfer(Account* sender_ptr, int receiver_number, double amount) {
    if(sender_ptr == NULL || sender_ptr->number == receiver_number || amount <= 0) return SERVICE_ERR_INVALID_INPUT;
    if(sender_ptr->balance < amount) return SERVICE_ERR_INSUFFICIENT_FUNDS;
    Account reciever;
    DBResult result = get_account_by_number(&reciever, receiver_number);
    if(result == DB_ERR_FILE) return SERVICE_ERR_DATABASE;
    if(result == DB_ERR_NOT_FOUND) return SERVICE_ERR_NOT_FOUND;
    sender_ptr->balance -= amount;
    reciever.balance += amount;
    result = update_account(sender_ptr);
    if(result == DB_ERR_FILE) {
        sender_ptr->balance += amount;
        return SERVICE_ERR_DATABASE;
    }
    result = update_account(&reciever);
    if(result == DB_ERR_FILE) {
        sender_ptr->balance += amount;
        update_account(sender_ptr);
        return SERVICE_ERR_DATABASE;
    }
    log_transaction(sender_ptr->number, "TRANSFER OUT", amount);
    log_transaction(reciever.number, "TRANSFER IN", amount);
    return SERVICE_SUCCESS;
}

ServiceResult change_password(Account* account_ptr, char* new_password) {
    if(account_ptr == NULL || new_password == NULL) return SERVICE_ERR_INVALID_INPUT;
    if(!validate_password(new_password)) return SERVICE_ERR_INVALID_INPUT;
    char old_password[MAX_PASS_LEN + 1];
    strcpy(old_password, account_ptr->password);
    strcpy(account_ptr->password, new_password);
    DBResult result = update_account(account_ptr);
    if(result == DB_ERR_FILE) {
        strcpy(account_ptr->password, old_password);
        return SERVICE_ERR_DATABASE;
    }
    return SERVICE_SUCCESS;
}

ServiceResult get_account_history(int account_number, char* history, int history_size) {
    if(history == NULL) return SERVICE_ERR_INVALID_INPUT;
    get_transaction_history(account_number, history, history_size);
    if (history[0] == '\0') {
        return SERVICE_ERR_NOT_FOUND;
    }
    return SERVICE_SUCCESS;
}