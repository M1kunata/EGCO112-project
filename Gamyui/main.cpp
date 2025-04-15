#include "../display.h"
#include "edit.h"
#include "usergamyui.h"
void welcome();
void main_menu();
void exit();
void user_register();
user* user_login();             
void user_dashboard(user* u);  



void main_menu()
{
    string main_input;
user* currentUser = nullptr; // ✅ จำผู้ใช้ที่ login

do {
    display_mainmenu();
    getline(cin, main_input); // เปลี่ยนจาก cin >> เป็น getline

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