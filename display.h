#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
using namespace std;
#include "Gamyui/usergamyui.h"

void clear_screen()
{
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}


//main.cpp
void welcome()
{
    clear_screen();
    cout<< "  ____ ___    _____         _    __        __         _    "<<endl;
    cout<< " / ___/ _ \\  |  ___|_ _ ___| |_  \\ \\      / /__  _ __| | __"<<endl;
    cout<< "| |  | | | | | |_ / _` / __| __|  \\ \\ /\\ / / _ \\| '__| |/ /"<<endl;
    cout<< "| |__| |_| | |  _| (_| \\__ \\ |_    \\ V  V / (_) | |  |   < "<<endl;
    cout<< " \\____\\___/  |_|  \\__,_|___/\\__|    \\_/\\_/ \\___/|_|  |_|\\_\\"<<endl;
    cout<<endl;
}

void exit()
{
    clear_screen();
    cout<< "______ _                 _                 "<<endl;       
    cout<< "|_   _| |__   __ _ _ __ | | __  _   _  ___  _   _ "<<endl;
    cout<< "  | | | '_ \\ / _` | '_ \\| |/ / | | | |/ _ \\| | | |"<<endl;
    cout<< "  | | | | | | (_| | | | |   <  | |_| | (_) | |_| |"<<endl;
    cout<< "  |_| |_| |_|\\__,_|_| |_|_|\\_\\  \\__, |\\___/ \\__,_|"<<endl;
    cout<< "                                |___/             "<<endl;
}

void display_mainmenu()
{
    welcome();
    cout << "== Main Menu ==" << endl;
    cout << "1. Register" << endl;
    cout << "2. Login" << endl;
    cout << "3. Exit" << endl;
    cout << endl;
    cout << "Choose an option: ";
}

void display_security()
{
    cout << "\nChoose a security question to answer:\n";
    cout << "1. School you graduated from\n";
    cout << "2. First pet's name\n";
    cout << "3. Favorite color\n";
    cout << "Enter choice (1-3): ";

}

inline void display_user_dashboard(user* currentUser) {
    clear_screen();
    cout << "=== User Dashboard ===\n";
    cout << "Welcome, " << currentUser->getUsername()<< " (" << currentUser->getRole() << ")\n";
    cout << "1. View My Profile\n";
    cout << "2. Edit My Info\n";
    cout << "3. Logout\n";
    cout << "Choose: ";
}

//usergamyui.h
void display_register()
{
    clear_screen();
    cout << "== Register ==" << endl;
    cout << "1. Job Seeker" << endl;
    cout << "2. Company" << endl;
    cout << "3. Back" << endl;
    cout << "Choose role: ";
}
void display_jobseeker_dashboard(user* currentUser){
    clear_screen();
    cout << "=== Jobseeker Dashboard ===\n";
    cout << "Welcome, " << currentUser->getUsername() << " (" << currentUser->getRole() << ")\n";
    cout << "1. View My Profile\n";
    cout << "2. Edit My Info \n";
    cout << "3. Browse Jobs \n";
    cout << "4. Logout\n";
    cout << "Choose: ";
}



//company.h
void display_choose_company_menu(string compa)
{
    clear_screen();
    cout << "=== Company Dashboard ===\n";
        cout << "Welcome, " << compa << " (" << "company" << ")\n";
        cout << "1. View Company Profile\n";
        cout << "2. Edit Company Profile\n";
        cout << "3. Post Job \n";
        cout << "4. Edit Job\n";
        cout << "5. View Applicants\n";
        cout << "6. Logout\n";
        cout << "Choose: ";
}

void display_add_another(){
    cout<<"Do you want to add another?"<<endl;
                cout<<"1.yes"<<endl;
                cout<<"2.no"<<endl;
                cout<<"choose:";
}

void display_Manage_Posted_Jobs(){
    cout<<"Manage Posted Jobs"<<endl;
    cout<<"1.Only view"<<endl;
    cout<<"2.view to edit"<<endl;
    cout<<"3.back"<<endl;
    cout<<"choose:";
}

inline void display_userInfo(user* u, const string& warning) {
    if (!u) {
        cout << "⚠️  No user to display.\n";
        return;
    }
    clear_screen();
    cout << "📄 === User Info ===\n";
    cout << "1. Username        : " << u->getUsername()<<"(cannot edit)" << endl;
    cout << "2. Name            : " << u->getName() << endl;
    cout << "3. Password        : ********" << endl;
    cout << "4. Email           : " << u->getEmail() << endl;
    cout << "5. Phone           : " << u->getPhone() << endl;
    cout << "6. " << (u->getRole() == "jobseeker" ? "Resume      " : "Description ")
         << "    : " << u->getDocument() << endl;
    cout << "7. " << (u->getRole() == "jobseeker" ? "Skills       " : "Jobs Offered ")
         << "   : " << u->getSkills() << endl;
    cout << "** Role            : " << u->getRole() << " (cannot edit)" << endl;

    cout << "🔧 Enter the number to edit, or type\n"
         << "   s or save  - to confirm all changes\n"
         << "   b or back  - to return to menu without saving\n";

    if (!warning.empty()) {
        cout << warning << "\n";
    } else {
        cout << endl;
    }
    cout << "Your choice: ";
}

inline void display_regisInfo(user* u, const string& warning) {
    if (!u) {
        cout << "⚠️  No user to display.\n";
        return;
    }
    clear_screen();
    cout << "📄 === User Info ===\n";
    cout << "1. Username        : " << u->getUsername() << endl;
    cout << "2. Name            : " << u->getName() << endl;
    cout << "3. Password        : ********" << endl;
    cout << "4. Email           : " << u->getEmail() << endl;
    cout << "5. Phone           : " << u->getPhone() << endl;
    cout << "6. " << (u->getRole() == "jobseeker" ? "Resume      " : "Description ")
         << "    : " << u->getDocument() << endl;
    cout << "7. " << (u->getRole() == "jobseeker" ? "Skills       " : "Jobs Offered ")
         << "   : " << u->getSkills() << endl;
    cout << "** Role            : " << u->getRole() << " (cannot edit)" << endl;

    cout << "🔧 Enter the number to edit, or type\n"
         << "   s or save  - to confirm all changes\n"
         << "   cc or cancel - to cancel all edits\n";

    if (!warning.empty()) {
        cout << warning << "\n";
    } else {
        cout << endl;
    }
    cout << "Your choice: ";
}


#endif