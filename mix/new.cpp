#include <iostream>
using namespace std;
//#include <fstream>
#include <cstring>
#include "jobseeker_company.h"
#include "jobclass.h"
//#include "userclass.h"
void resgister();
int main()
{   
    char choice[10];
    cout<<"Welcome to 'CO fast work'"<<endl;
    cout<<"main menu"<<endl;
    cout<<"1.register"<<endl;
    cout<<"2.login"<<endl;
    cout<<"3.exit"<<endl;
    cout<<"choose:";
    cin>>choice;
    if(strcmp(choice,"1")==0)
        resgister();
    else if(strcmp(choice,"2")==0)
        cout<<"login";
    else if(strcmp(choice,"3")==0)
        cout<<"exit";
}
void resgister()
{
    string username,password,mail,name,role;

    cout<<"create register your account"<<endl;
    cin.ignore();//clear buffer
    cout<<"name:";    
    getline(cin, name);//get input with white space
    cout<<"password:";
    getline(cin,password);
    cout<<"mail:";
    getline(cin,mail);
    cout<<"username:";
    getline(cin,username);
    user t;
    t.regisster(name,password,mail,username);
}