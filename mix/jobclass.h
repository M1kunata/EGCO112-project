#ifndef jobclass_h
#define jobclass_h
#include <iostream>
#include <fstream>
using namespace std;
class job
{
    private:
        int tag_num=0;
        string jobtitle,company,decription,location,status;
        float salary;
    public:
        void add_job(string head,string company,string detail,string location,float money,string status="recruiting");
        void read_job();
        void edit_job();
};
void job::add_job(string head,string company,string detail,string loca,float money,stirng stas)
{
    read_job();
    tag_num++;
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
        iss>>tag_num>>jobtitle>>company>>detail>>location>>money>>status;//แยกข้อมูลใส่ตัวแปร
    }
    }
}



#endif