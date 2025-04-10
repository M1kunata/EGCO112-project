#include <iostream>
using namespace std;

void clear_screen()
{
    cout << "\033[2J\033[1;1H";//clear screen
}

void display_choose_company_menu()
{
    clear_screen();
    cout<<"--Company menu--"<<endl;
    cout<<"1.Add job"<<endl;
    cout<<"2.Manage Posted Jobs"<<endl;
    cout<<"3.edit job"<<endl;
    cout<<"4.exit"<<endl;
    cout<<"choose:";
}

void display_add_another(){
    cout<<"Do you want to add another?"<<endl;
                cout<<"1.yes"<<endl;
                cout<<"2.no"<<endl;
                cout<<"choose:";
}

void display_Manage_Posted_Jobs(){
    cout<<"Manage Posted Jobs"<<endl;
    cout<<"1.all job"<<endl;
    cout<<"2.with filter"<<endl;
    cout<<"choose:";
}

/*void display_

void display_

void display_

void display_*/