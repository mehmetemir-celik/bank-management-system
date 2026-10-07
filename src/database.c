#include "../include/models.h"
#include "../include/database.h"
#include <stdio.h>

static int get_account_index_by_number(int target_number) {
    FILE* file_ptr = fopen("data/accounts.bin", "rb");
    if(file_ptr == NULL) return -1;
    Account account;
    for(int index = 0; fread(&account, sizeof(Account), 1, file_ptr); index++) {
        if(account.number == target_number) {
            fclose(file_ptr);
            return index;
        }
    }
    fclose(file_ptr);
    return -2;
}

static int get_file_size(char* file_name) {
    FILE* file_ptr = fopen(file_name, "rb");
    if(file_ptr == NULL) return 0;
    fseek(file_ptr, 0, SEEK_END);
    int file_size = ftell(file_ptr);
    fclose(file_ptr);
    return file_size;
}

int get_next_account_number() {
    int file_size = get_file_size("data/accounts.bin");
    int account_count = file_size / sizeof(Account);
    return INITIAL_ACCOUNT_NUMBER + account_count;
}

DBResult append_new_account(Account* account_ptr) {
    FILE* file_ptr = fopen("data/accounts.bin", "ab");
    if (file_ptr == NULL) return DB_ERR_FILE;
    fwrite(account_ptr, sizeof(Account), 1, file_ptr);
    fclose(file_ptr);
    return DB_SUCCESS;
}

DBResult get_account_by_number(Account* account_ptr, int target_number) {
    int index = get_account_index_by_number(target_number);
    if(index == -1) return DB_ERR_FILE;
    if(index == -2) return DB_ERR_NOT_FOUND;
    FILE* file_ptr = fopen("data/accounts.bin", "rb");
    if(file_ptr == NULL) return DB_ERR_FILE;
    fseek(file_ptr, index * sizeof(Account), SEEK_SET);
    fread(account_ptr, sizeof(Account), 1, file_ptr);
    fclose(file_ptr);
    return DB_SUCCESS;
}

DBResult update_account(Account* updated_account_ptr) {
    int index = get_account_index_by_number(updated_account_ptr->number);
    if(index == -1) return DB_ERR_FILE;
    if(index == -2) return DB_ERR_NOT_FOUND;
    FILE* file_ptr = fopen("data/accounts.bin", "rb+");
    if(file_ptr == NULL) return DB_ERR_FILE;
    fseek(file_ptr, index * sizeof(Account), SEEK_SET);
    fwrite(updated_account_ptr, sizeof(Account), 1, file_ptr);
    fclose(file_ptr);
    return DB_SUCCESS;
}

DBResult log_transaction(int account_number, char* process_type, double amount) {
    char file_name[50];
    sprintf(file_name, "data/transactions/%d.txt", account_number);
    FILE* file_ptr = fopen(file_name, "a");
    if (file_ptr == NULL) return DB_ERR_FILE;
    fprintf(file_ptr, "%-15s%10.2lf\n", process_type, amount);
    fclose(file_ptr);
    return DB_SUCCESS;
}

DBResult get_transaction_history(int account_number ,char* history , int history_size) {
    char file_name[50];
    sprintf(file_name, "data/transactions/%d.txt", account_number);
    FILE* file_ptr = fopen(file_name, "r");
    if (file_ptr == NULL) {
        history[0] = '\0';
        return DB_SUCCESS;
    }
    int size = fread(history, 1, history_size - 1, file_ptr);
    history[size] = '\0';
    fclose(file_ptr);
    return DB_SUCCESS;
}