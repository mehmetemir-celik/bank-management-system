#ifndef UI_H
#define UI_H

#include "models.h"

typedef enum {
    UI_WELCOME,
    UI_CREATE_ACCOUNT,
    UI_LOG_IN,
    UI_LOGGED_IN,
    UI_WITHDRAW,
    UI_DEPOSIT,
    UI_TRANSFER,
    UI_CHANGE_PASSWORD,
    UI_SHOW_HISTORY,
    UI_EXIT
}UIState;

UIState ui_welcome();
UIState ui_create_account();
UIState ui_log_in(Account* logged_in_account_ptr);
UIState ui_logged_in(Account* logged_in_account_ptr);
UIState ui_withdraw(Account* logged_in_account_ptr);
UIState ui_deposit(Account* logged_in_account_ptr);
UIState ui_transfer(Account* logged_in_account_ptr);
UIState ui_change_password(Account* logged_in_account_ptr);
UIState ui_show_history(Account* logged_in_account_ptr);
void ui_exit();

#endif