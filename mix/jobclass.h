#ifndef jobclass_h
#define jobclass_h
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include "bst.h"
using namespace std;
void input();
class job
{
    private:
        int tag_num;
        string jobtype,company,location,status;//status pending end
        vector<string> requiresskill;
        double max_salary,min_salary;
    public:
        job(int tag_num=0);
        void add_job(string jobtype,string company,string requir,string location,double max,double min,string status="recruiting");
        void update_numofjob();
        void edit_job();
        friend void read_job(BST *,job *);

};
job::job(int tag)
{
    tag_num=tag;
}
void input()
{
    string type,compa,loca,req; 
    double max,min;
    //clear screen!
    cout<<"JOB:"<<endl;
    cin.ignore(); //ถ้ามีbufferเอาคอมเมนออก
    cout<<"JOB type:";
    getline(cin,type);
    cout<<"requirment(,):";
    getline(cin,req);
    cout<<"location:";
    getline(cin,loca);
    cout<<"BY company:";
    getline(cin,compa);
    cout<<"max salary:";
    cin>>max;
    cout<<"min salary:";
    cin>>min;
    job neow;
    neow.add_job(type,compa,req,loca,max,min);
}

void job::add_job(string jobtype,string company,string requir,string loca,double max,double min,string stas)
{
    update_numofjob();
    tag_num++;
    ofstream F;
    F.open("job_data.txt",ios::app);//เปิดไฟล์โหมดเติมท้ายกับสร้าง
    F<<"\""<<tag_num<<"\",\""<<jobtype<<"\",\""<<company<<"\",\""<<requir<<"\",\""<<loca<<"\",\""<<max<<"\",\""<<min<<"\",\""<<stas<<"\""<<endl;
    F.close();
}
void clear_qoate(stringstream &qoute) 
{
    string buffer;
    getline(qoute,buffer,'"');
}
void job::update_numofjob()
{
    ifstream Out("job_data.txt");
    if(Out)
    {
    string line;
    while(getline(Out,line))//อ่านค่าจากไฟล์
    {
        stringstream iss(line);
        string require,st_req,quote,num;
        clear_qoate(iss); getline(iss,num,'"');tag_num=stoi(num);   
    }
    Out.close();
}
}
void read_job(BST *sort,job *n)//เพื่อเพิ่มเข้า bst
{
    ifstream Out("job_data.txt");
    if(Out)
    {
    string line;
    while(getline(Out,line))//อ่านค่าจากไฟล์
    {
        stringstream iss(line);
        string require,st_req,quote,num;
        clear_qoate(iss); getline(iss,num,'"');n->tag_num=stoi(num);
        clear_qoate(iss); getline(iss, n->jobtype, '"');
        clear_qoate(iss); getline(iss, n->company, '"');
        clear_qoate(iss); getline(iss, require, '"');
        clear_qoate(iss); getline(iss, n->location, '"');
        clear_qoate(iss); getline(iss, num, '"');n->max_salary=stoi(num);
        clear_qoate(iss); getline(iss,  num, '"');n->min_salary=stoi(num);
        stringstream ss(require);
        while (getline(ss, st_req, ',')) {
            n->requiresskill.push_back(st_req);
        }
        insertNode(sort,&n);
    }
    }
    Out.close();
}



#endif