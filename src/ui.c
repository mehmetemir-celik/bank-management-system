#include "../include/utils.h"
#include "../include/models.h"
#include "../include/services.h"
#include "../include/ui.h"
#include "../include/config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

UIState ui_welcome() { 
    int choice = -1;
    clear_screen();
    printf(
        "\n          WELCOME TO BANK          "
        "\n-----------------------------------"
        "\n1) New Account"
        "\n2) Log In"
        "\n3) Exit"
        "\n"
        "\nEnter an operation: "
    );
    input_int(&choice);
    switch(choice) {
        case 1: return UI_CREATE_ACCOUNT;
        case 2: return UI_LOG_IN;
        case 3: return UI_EXIT;
        default: retry_message("Please enter a valid operation."); return UI_WELCOME;
    }
}

UIState ui_create_account() {
    char name[MAX_NAME_LEN + 1];
    char password[MAX_PASS_LEN + 1];
    int number = -1;
    ServiceResult result;
    clear_screen();
    printf(
    "\n          RULES           "
    "\n--------------------------"
    "\n1) Maximum %d characters"
    "\n2) No digits"
    "\n"
    "\nEnter your name (0 to cancel): "
    , MAX_NAME_LEN);
    input_string(name, sizeof(name));
    if(strcmp(name, "0") == 0) return UI_WELCOME;
    clear_screen();
    printf(
        "\n          RULES           "
        "\n--------------------------"
        "\n1) Minimum %d characters"
        "\n2) Maximum %d characters"
        "\n3) One lowercase, one uppercase and one digit at least"
        "\n"
        "\nEnter your password (0 to cancel): "
    , MIN_PASS_LEN, MAX_PASS_LEN);
    input_string(password, sizeof(password));
    if(strcmp(password, "0") == 0) return UI_WELCOME;
    result = create_account(name, password, &number);
    if(result == SERVICE_ERR_INVALID_INPUT) {
        retry_message("Please make sure that your name and password satisfy rules.");
        return UI_CREATE_ACCOUNT;
    }
    if(result == SERVICE_ERR_DATABASE) {
        continue_message("Something went wrong. Try again later.");
        return UI_WELCOME;
    }
    printf(
        "\nSuccessful. Welcome to the bank."
        "\n"
        "\nYour name: %s"
        "\nYour password: %s"
        "\nYour account number: %d"
    , name, password, number);
    continue_message("");
    return UI_WELCOME;
}

UIState ui_log_in(Account* logged_in_account_ptr) {
    if (logged_in_account_ptr == NULL) return UI_WELCOME;
    int number = -1;
    char password[MAX_PASS_LEN + 1];
    clear_screen();
    printf("\nEnter your account number (0 to cancel): ");
    input_int(&number);
    if(number == 0) return UI_WELCOME;
    printf(
        "\nAccount number: %d"
        "\nEnter your password (0 to cancel): "
    , number);
    input_string(password, sizeof(password));
    if(strcmp(password, "0") == 0) return UI_WELCOME;
    ServiceResult result = log_in(number, password, logged_in_account_ptr);
    if(result == SERVICE_ERR_DATABASE || result == SERVICE_ERR_INVALID_INPUT) {
        continue_message("Something went wrong. Please try again later.");
        return UI_WELCOME;
    }
    if(result == SERVICE_ERR_NOT_FOUND) {
        retry_message("Make sure that you entered correct account number.");
        return UI_LOG_IN;
    }
    if(result == SERVICE_ERR_WRONG_PASSWORD) {
        retry_message("Wrong password.");
        return UI_LOG_IN;
    }
    continue_message("Successfully logged in.");
    return UI_LOGGED_IN;
}

UIState ui_logged_in(Account* logged_in_account_ptr) {
    if (logged_in_account_ptr == NULL) return UI_WELCOME;
    int choice = -1;
    clear_screen();
    printf(
        "\nWelcome, %s (%d)"
        "\nYour balance: %.2lf"
        "\n"
        "\n1) Withdraw"
        "\n2) Deposit"
        "\n3) Transfer"
        "\n4) Change Password"
        "\n5) Show History"
        "\n6) Log Out"
        "\n"
        "\nChoose an operation: "
        , logged_in_account_ptr->name, logged_in_account_ptr->number, logged_in_account_ptr->balance);
    input_int(&choice);
    switch(choice) {
        case 1: return UI_WITHDRAW;
        case 2: return UI_DEPOSIT;
        case 3: return UI_TRANSFER;
        case 4: return UI_CHANGE_PASSWORD;
        case 5: return UI_SHOW_HISTORY;
        case 6: continue_message("You logged out."); return UI_WELCOME;
        default: retry_message("Please enter a valid operation."); return UI_LOGGED_IN;
    }
}

UIState ui_deposit(Account* logged_in_account_ptr) {
    double amount = -1.0;
    clear_screen();
    printf(
        "\nYour balance: %.2lf"
        "\n"
        "\nHow much you want to deposit (0 to cancel): "
        , logged_in_account_ptr->balance);
    input_double(&amount);
    if(amount == 0.0) return UI_LOGGED_IN;
    ServiceResult result = deposit(logged_in_account_ptr, amount);
    if(result == SERVICE_ERR_INVALID_INPUT) {
        retry_message("Please enter a valid amount.");
        return UI_DEPOSIT;
    }
    if(result == SERVICE_ERR_DATABASE) {
        continue_message("Something went wrong. Please try again later.");
        return UI_LOGGED_IN;
    }
    continue_message("Successful.");
    return UI_LOGGED_IN;
}

UIState ui_withdraw(Account* logged_in_account_ptr) {
    double amount = -1.0;
    clear_screen();
    printf(
        "\nYour balance: %.2lf"
        "\n"
        "\nHow much you want to draw (0 to cancel): "
        , logged_in_account_ptr->balance);
    input_double(&amount);
    if(amount == 0.0) return UI_LOGGED_IN;
    ServiceResult result = withdraw(logged_in_account_ptr, amount);
    if(result == SERVICE_ERR_INVALID_INPUT) {
        retry_message("Please enter a valid amount.");
        return UI_WITHDRAW;
    }
    if(result == SERVICE_ERR_DATABASE) {
        continue_message("Something went wrong. Please try again later.");
        return UI_LOGGED_IN;
    }
    if(result == SERVICE_ERR_INSUFFICIENT_FUNDS) {
        retry_message("Insufficient balance.");
        return UI_WITHDRAW;
    }
    continue_message("Successful.");
    return UI_LOGGED_IN;
}

UIState ui_transfer(Account* logged_in_account_ptr) {
    int reciever_number = -1;
    double amount = -1.0;
    clear_screen();
    printf("\nEnter incoming account number (0 to cancel): ");
    input_int(&reciever_number);
    if(reciever_number == 0) return UI_LOGGED_IN;
    clear_screen();
    printf(
        "\nYour balance: %.2lf"
        "\n"
        "\nHow much you want to transfer (0 to cancel): "
        , logged_in_account_ptr->balance);
    input_double(&amount);
    if(amount == 0.0) return UI_LOGGED_IN;
    ServiceResult result = transfer(logged_in_account_ptr, reciever_number, amount);
    if(result == SERVICE_ERR_INVALID_INPUT) {
        retry_message("Please enter a valid amount or account number.");
        return UI_TRANSFER;
    }
    if(result == SERVICE_ERR_DATABASE) {
        continue_message("Something went wrong. Please try again later.");
        return UI_LOGGED_IN;
    }
    if(result == SERVICE_ERR_INSUFFICIENT_FUNDS) {
        retry_message("Insufficient balance.");
        return UI_TRANSFER;
    }
    if(result == SERVICE_ERR_NOT_FOUND) {
        retry_message("Receiver account not found.");
        return UI_TRANSFER;
    }
    continue_message("Successful.");
    return UI_LOGGED_IN;
}

UIState ui_change_password(Account* logged_in_account_ptr) {
    char new_password[MAX_PASS_LEN + 1];
    clear_screen();
    printf(
        "\n          RULES           "
        "\n--------------------------"
        "\n1) Minimum %d characters"
        "\n2) Maximum %d characters"
        "\n3) One lowercase, one uppercase and one digit at least"
        "\n"
        "\nEnter your new password (0 to cancel): "
    , MIN_PASS_LEN, MAX_PASS_LEN);
    input_string(new_password, sizeof(new_password));
    if(strcmp(new_password, "0") == 0) return UI_LOGGED_IN;
    ServiceResult result = change_password(logged_in_account_ptr, new_password);
    if(result == SERVICE_ERR_INVALID_INPUT) {
        retry_message("Please make sure that your name and password satisfy rules.");
        return UI_CHANGE_PASSWORD;
    }
    if(result == SERVICE_ERR_DATABASE) {
        continue_message("Something went wrong. Please try again later.");
        return UI_LOGGED_IN;
    }
    continue_message("Password successfully changed.");
    return UI_LOGGED_IN;
}

UIState ui_show_history(Account* logged_in_account_ptr) {
    char history[1024];
    clear_screen();
    ServiceResult result = get_account_history(logged_in_account_ptr->number, history, sizeof(history));
    if(result == SERVICE_ERR_INVALID_INPUT) {
        continue_message("Something went wrong. Please try again later.");
        return UI_LOGGED_IN;
    }
    printf(
        "\n          TRANSACTION HISTORY          "
        "\n---------------------------------------"
    );
    if(result == SERVICE_ERR_NOT_FOUND) {
        printf("\nNo transactions done.");
    }else {
        printf("\n%s", history);
    }
    continue_message("");
    return UI_LOGGED_IN;
}

void ui_exit() {
    printf(
        "\n"
        "\nHave a nice day."
        "\n"
        "\n"
    );
}