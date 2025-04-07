#include <iostream>
using namespace std;
//#include <fstream>
#include <cstring>
#include "jobseeker_company.h"
//#include "jobclass.h"
//#include "userclass.h"
#include "bst.h"
void resgister();
void read_job();
int main()
{   
    char choice[10];
    cout<<"Welcome to 'CO fast work'"<<endl;
    cout<<"main menu"<<endl;
    cout<<"1.register"<<endl;
    cout<<"2.addjob"<<endl;
    cout<<"3.read_job to sort"<<endl;
    cout<<"4.exit"<<endl;
    cout<<"choose:";
    cin>>choice;
    if(strcmp(choice,"1")==0)
        resgister();
    else if(strcmp(choice,"2")==0)
        {
            input();
        }
    else if(strcmp(choice,"3")==0)
    {
        //cout<<"-1"<<endl;
        read_job();
    }
    else if(strcmp(choice,"4")==0)
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
void read_job()//เพื่อเพิ่มเข้า bst
{
    BST one;
    int tag,max,min;
    string type,compa,loca,stat;
    ifstream Out("job_data.txt");
    if(Out)
    {
    string line;
    while(getline(Out,line))//อ่านค่าจากไฟล์
    {
        stringstream iss(line);
        string require,st_req,quote,num;
        clear_qoate(iss); getline(iss,num,'"');tag=stoi(num);
        clear_qoate(iss); getline(iss, type, '"');
        clear_qoate(iss); getline(iss, compa, '"');
        clear_qoate(iss); getline(iss, require, '"');
        clear_qoate(iss); getline(iss, loca, '"');
        clear_qoate(iss); getline(iss, num, '"');max=stoi(num);
        clear_qoate(iss); getline(iss,  num, '"');min=stoi(num);
        clear_qoate(iss);getline(iss,stat,'"');
        job n1(tag,type,compa,loca,max,min,stat,require);   
        insertNode(&one,n1);//ยัดเข้าbst
        n1.clear_vector();
    }
        inorder(one.root);//เรียงออกมาให้ดูตอนนี้เรียงตามลำดับid
}
    Out.close();
}
