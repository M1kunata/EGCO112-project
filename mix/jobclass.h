#ifndef jobclass_h
#define jobclass_h
#include <iostream>
#include <fstream>
using namespace std;
class job
{
    private:
        string jobtitle,company,decription,location,status;
        float salary;
    public:
        void add_job(string head,string company,string detail,string loca,float money);
        
};
void job::add_job(string head,string company,string detail,string loca,float money)
{
    ofstream F;
    F.open("job_data.txt",ios::app);//เปิดไฟล์โหมดเติมท้ายกับสร้าง
    F<<head<<" "<<company<<" "<<detail<<" "<<money<<endl;
    F.close();
}




#endif