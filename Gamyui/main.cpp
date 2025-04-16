#include "../display.h"
#include "edit.h"
#include "usergamyui.h"
void welcome();
void main_menu();
void exit();
void user_register();
user* user_login();             
void company_dashboard(user* currentUser);
void jobseeker_dashboard(user* currentUser);  



void main_menu()
{
    string main_input;
user* currentUser = nullptr; // ✅ จำผู้ใช้ที่ login

do {
    display_mainmenu();
    getline(cin, main_input); // เปลี่ยนจาก cin >> เป็น getline

<<<<<<< HEAD
        if (main_choice == 1) {
            system("clear");
            user_register();
        }
        else if (main_choice == 2) {
            system("clear");
            user* currentUser = user_login();
            if (currentUser != nullptr) {
                if (currentUser->getRole() == "jobseeker") {
                    jobseeker_dashboard(currentUser);
                } else if (currentUser->getRole() == "company") {
                    company_dashboard(currentUser);
                }
                delete currentUser;
                currentUser = nullptr;
            }
            
        }
        else if (main_choice == 3) {
            system("clear");
            exit();
        }
        else {
            system("clear");
            cout << "Invalid choice. Please enter a number between 1 and 3." << endl;
=======
    if (main_input == "1") {
        system("clear");
        user_register();
    }
    else if (main_input == "2") {
        system("clear");
        currentUser = user_login();

        if (currentUser != nullptr) {
            user_dashboard(currentUser); 
            delete currentUser;
            currentUser = nullptr;
>>>>>>> 8312d9331107a0f4f6e1459714d396fa7b4cca52
        }
    }
    else if (main_input == "3") {
        exit();
    }
    else {
        clear_screen();
        cout << "Invalid choice. Please enter a number between 1 and 3." << endl;
    }

    } while (main_input != "3");
}

int main()
{
    welcome();
    main_menu();

    return 0;
}