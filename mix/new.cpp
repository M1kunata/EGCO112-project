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
    string username,password,mail,name;
    cout<<"create register your account"<<endl;
    //cin.ignore();//clear buffer
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