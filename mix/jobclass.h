#ifndef jobclass_h
#define jobclass_h
#include <iostream>
#include <fstream>
using namespace std;
class job
{
    private:
        int tag_num=0;
        string jobtitle,company,decription,location,status;//status pending end
        double salary;
    public:
        void add_job(string head,string company,string detail,string location,double money,string status="recruiting");
        void read_job();
        void edit_job();
};
void job::add_job(string head,string company,string detail,string loca,double money,string stas)
{
    read_job();
    tag_num++;
    //clear screen!
    cout<<"JOB:"<<endl;
    //cin.ignore(); //ถ้ามีbufferเอาคอมเมนออก
    cout<<"requirment:";
    getline(cin,head);
    cout<<"decription:";
    getline(cin,detail);
    cout<<"location:";
    getline(cin,loca);
    cout<<"salary:";
    cin>>money;
    ofstream F;
    F.open("job_data.txt",ios::app);//เปิดไฟล์โหมดเติมท้ายกับสร้าง
    F<<tag_num<<" "<<head<<" "<<company<<" "<<detail<<" "<<loca<<" "<<money<<" "<<stas<<endl;
    F.close();
}
void job::read_job()
{
    ifstream Out("job_data.txt");
    if(Out)
    {
    string line;
    while(getline(Out,line))//อ่านค่าจากไฟล์
    {
        istringstream iss(line);
        iss>>tag_num>>jobtitle>>company>>decription>>location>>salary>>status;//แยกข้อมูลใส่ตัวแปร
        
    }
    }
    Out.close();
}



#endif