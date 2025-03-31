#include <iostream>
using namespace std;
#include<fstream>
#include "jobseeker.h"
void resgister();
int main()
{   
    resgister();
}
void resgister()
{
    string username,password,mail,name;
    cout<<"create register your account"<<endl;
    //cin.ignore();//clear buffer
    cout<<"name:";    
    getline(cin, username);//get input with white space
    cout<<"password:";
    getline(cin,password);
    cout<<"mail:";
    getline(cin,mail);
    cout<<"username:";
    getline(cin,name);
    user t;
    t.regisster(username,password,mail,name);
}