#include "../display.h"
#include "edit.h"
#include "usergamyui.h"
void welcome();
void main_menu();
void exit();
void user_register();
user* user_login();             
void user_dashboard(user* u);  

void welcome()
{
    cout<< "  ____ ___    _____         _    __        __         _    "<<endl;
    cout<< " / ___/ _ \\  |  ___|_ _ ___| |_  \\ \\      / /__  _ __| | __"<<endl;
    cout<< "| |  | | | | | |_ / _` / __| __|  \\ \\ /\\ / / _ \\| '__| |/ /"<<endl;
    cout<< "| |__| |_| | |  _| (_| \\__ \\ |_    \\ V  V / (_) | |  |   < "<<endl;
    cout<< " \\____\\___/  |_|  \\__,_|___/\\__|    \\_/\\_/ \\___/|_|  |_|\\_\\"<<endl;
    cout<<endl;
}

void main_menu()
{
    int main_choice;
    user* currentUser = nullptr; // ✅ จำผู้ใช้ที่ login

    do
    {
        cout << "== Main Menu ==" << endl;
        cout << "1. Register" << endl;
        cout << "2. Login" << endl;
        cout << "3. Exit" << endl;
        cout << endl;
        cout << "Choose an option: ";
        cin >> main_choice;

        if (main_choice == 1) {
            system("clear");
            user_register();
        }
        else if (main_choice == 2) {
            system("clear");
            currentUser = user_login();

            if (currentUser != nullptr) {
                user_dashboard(currentUser); 
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
        }

    } while (main_choice != 3);
}
void exit()
{
    cout<< "______ _                 _                 "<<endl;       
    cout<< "|_   _| |__   __ _ _ __ | | __  _   _  ___  _   _ "<<endl;
    cout<< "  | | | '_ \\ / _` | '_ \\| |/ / | | | |/ _ \\| | | |"<<endl;
    cout<< "  | | | | | | (_| | | | |   <  | |_| | (_) | |_| |"<<endl;
    cout<< "  |_| |_| |_|\\__,_|_| |_|_|\\_\\  \\__, |\\___/ \\__,_|"<<endl;
    cout<< "                                |___/             "<<endl;
}
int main()
{
    welcome();
    main_menu();

    return 0;
}