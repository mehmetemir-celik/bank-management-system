#include "../include/ui.h"
#include "../include/models.h"
#include <stdbool.h>

int main() {
    Account logged_in_account;
    UIState state = UI_WELCOME;
    while(true) {
        switch(state) {
            case UI_WELCOME: state = ui_welcome(); break;
            case UI_CREATE_ACCOUNT: state = ui_create_account(); break;
            case UI_LOG_IN: state = ui_log_in(&logged_in_account); break;
            case UI_LOGGED_IN: state = ui_logged_in(&logged_in_account); break;
            case UI_WITHDRAW: state = ui_withdraw(&logged_in_account); break;
            case UI_DEPOSIT: state = ui_deposit(&logged_in_account); break;
            case UI_TRANSFER: state = ui_transfer(&logged_in_account); break;
            case UI_CHANGE_PASSWORD: state = ui_change_password(&logged_in_account); break;
            case UI_SHOW_HISTORY: state = ui_show_history(&logged_in_account); break;
            case UI_EXIT: ui_exit(); return 0;
        }
    }
}