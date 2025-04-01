#include <iostream>
using namespace std;
//#include "userclass.h"
#include <fstream>
#include "jobseeker.h"
void resgister();
int main()
{   
    resgister();
}
void resgister()
{
    user m;
    string username,password,mail,name;
    cout<<"create register your account"<<endl;
    cout<<"name:";
    getline(cin,name);
    cout<<"password:";
    cin>>password;
    cout<<"email:";
    cin>>mail;
    cout<<"username:";
    cin>>username;
    m.regisster(name,password,mail,username);
}