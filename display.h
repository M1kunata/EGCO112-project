#include <iostream>
using namespace std;
string display_choose_company_menu()
{
    cout << "\033[2J\033[1;1H";//clear screen
    cout<<"--Company menu--"<<endl;
    cout<<"1.job announcement"<<endl;
    cout<<"2.view job"<<endl;
    cout<<"3.edit job"<<endl;
    cout<<"4.exit"<<endl;
    cout<<"choose:";string choice;cin>>choice;
    return choice;
}
void clean()
{
    cout << "\033[2J\033[1;1H";//clear screen
}