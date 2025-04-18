#include "../display.h"
#include "edit.h"
#include "usergamyui.h"
#include "../company.h"
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

        if (main_input == "1") {
            system("clear");
            user_register();
        }
        else if (main_input == "2") {
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
        else if (main_input == "3") {
            system("clear");
            exit();
        }
        else {
            system("clear");
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