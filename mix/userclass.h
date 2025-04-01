#ifndef userclass_h
#define userclass_h
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
class user{
    private:
    int id=0;
    string username;
    string password;
    string email;
    string name;
public:
    void regisster(string fullname,string pas,string mail,string user);
    void edit_user();
};
void user::edit_user()
{
    char choice[10];
    cout<<"'What data you want to edit'"<<endl;
    ifstream Out("user_data.txt");
    if(Out)
    {
    string line;
    while(getline(Out,line))//อ่านค่าจากไฟล์
    {
        istringstream iss(line);
        iss>>id>>name>>password>>email>>username;//แยกข้อมูลใส่ตัวแปร
    }
    }
    Out.close();
}
void user::regisster(string fullname,string pas,string mail,string user)
{
    ifstream Out("user_data.txt");
    if(!Out)
    {
    string line;
    while(getline(Out,line))//อ่านค่าจากไฟล์
    {
        istringstream iss(line);
        iss>>id>>name>>password>>email>>username;//แยกข้อมูลใส่ตัวแปร
    }
    }
    Out.close();
    id++;
    username=user;
    password=pas;
    email=mail;
    name=fullname;
    ofstream F;
    F.open("user_data.txt",ios::app);//เปิดไฟล์โหมดเติมท้ายกับสร้าง
    F <<id<<" "<<name<<" "<<pas<<" "<<email<<" "<<user<<endl;
    F.close();
}



#endif