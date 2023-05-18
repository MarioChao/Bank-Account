# Dev Blog for "Bank Account"

_My fifth github project for C++ class_

## How to use

Run in Replit:<br>
[![Run on Repl.it](https://replit.com/badge/github/MarioChao/Bank-Account)](https://replit.com/new/github/MarioChao/Bank-Account)

1. Run the program using "`make`" or replit's "Run" button
2. At the start, there's a list of commands
    > create, list, access, deposit, withdraw, transfer, show number, show info, show history, log out, delete
3. Start by creating an account, enter "`create`"
4. It will ask for an account type (`0 ~ 999`) and initial balance (`double`)
5. Enter a password for your account, \*__Remember This Password!__\*
6. You will automaticallly be logged into the newly created account
7. Check your bank account info using "`show number`", "`show info`", or "`show history`"
8. Modify your account balance using "`deposit`", "`withdraw`", or "`transfer`"
9. Delete your account using "`delete`" (will have a confirmation input)
10. Log out of your account to using "`log out`" to prevent identity theft
11. List the currently stored accounts using "`list`"
12. Log into one of the accounts using "`access`" followed by account number and password
13. Quit the program, and the accounts will be saved and loaded the next time you run the program

## Issues faced

* I need to free the memory of the "`Transaction`" object pointer in each bank account:
    * Created the need to define a __destructor__ for "`BankAccount`" class.
    * Since "`BankSystem`" uses a vector of "`BankAccount`" objects, this created the need for a defined __move constructor__ when trying to create new bank accounts.
    * This caused the implicit __copy constructor__ to get deleted, resulting in compile errors, so one would also need to define it as well.
    * When deleting accounts, I used swap, which requires a __move assignment operator__ to be defined.
    * This demonstrates the 4/5 of C++'s [Rule of 5](https://en.cppreference.com/w/cpp/language/rule_of_three).
* About saving account information:
    * The system for saving an account and retrieving it later on is quite complicated.
    * This saving / loading system, which I accomplished using "`fstream`" and "`filesystem`" library, is something I'm not familiar with.
    * I'm using g++ in VSCode, so I need to modify my makefile to compile the program in C++17 or above by adding "-std=c++17" to the flags.
* Security system:
    * Passwords need a way to be saved somehow securely.
    * I used C++'s hash function in the "`functional`" library to encode and save passwords.
    * To check the correct password, I just need to encode the user input and check if it matches the saved password.
* To get the current date, I found the answer online and used the "`ctime`" library.
    * In one line:
        > `string date = std::c_time(new time_t(time(0)));`

These are the main issues that I encountered when making this project.