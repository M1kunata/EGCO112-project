#ifndef jobclass_h
#define jobclass_h
#include <iostream>
#include <vector>
#include <iomanip>
#include <limits>  // สำคัญสำหรับ numeric_limits
#include <sstream>
#include <fstream>
using namespace std;
void input();
class data
{
    public:
        virtual void anounment()
        {
            cout<<"ready to get data"<<endl;
        }
};
class job:public data
{
private:
    int tag_num;
    string jobtype, company, location, status; // status pending end
    vector<string> requiresskill;
    double max_salary, min_salary;

public:
    job(int tag_num = 0, string jobtype = "unknown", string company = "unknown", string location = "unknown",double max_salary=0,double min_salary=0, string status = "recruiting", string skill = "unknown");
    bool operator<=(job &another);
    void setjob(job &another);
    bool operator>(job &another);
    void add_job(string jobtype, string company, string requir, string location, double max, double min, string status = "recruiting");
    void update_numofjob();
    void clear_vector();
    void display();
    void getidcompa(int&,string&);
    bool check_com(string comcheck);
    bool compare(string sor,int data);
    void getdata(int &tag,string &job, string &compa, string &loca,double &max_salary,double &min_salary, string &stat, string &skill);
    void anounment() override
    {
        cout<<"already add job!!"<<endl;
    }
};
void job::getidcompa(int &tag,string &compa)
{
    tag=tag_num;
    compa=company;
}
void job::setjob(job &another)
{
    tag_num=another.tag_num;
    jobtype=another.jobtype;
    company=another.company;
    location=another.location;
    status=another.status;
    requiresskill=another.requiresskill;
    max_salary=another.max_salary;
    min_salary=another.min_salary;
}
void job::getdata(int &tag,string &job, string &compa, string &loca,double &max,double& min, string &stat, string &skill)
{
    tag=tag_num;
    job=jobtype;
    compa=company;
    loca=location;
    max=max_salary;
    min=min_salary;
    stat=status;
    for(int i=0;i<requiresskill.size();i++)
    {
            skill+=(requiresskill[i]+",");
    }    
}
bool job::compare(string sor,int data)
{
    if(sor=="equal")
        return data==tag_num;
    else if(sor=="less")
        return data<tag_num;
    else return 0;
}
bool job::check_com(string comcheck)
{
    return company==comcheck;
}
void job::display()
{
    string full;
    cout << left << setw(6) << tag_num
    << setw(15) << jobtype
    << setw(15) << company;
    for (const auto& f : requiresskill) {
        full+=(f+",");
    }
    cout<<setw(25)<<full
    << setw(20) << location
    << setw(15) << max_salary
    << setw(15) << min_salary
    << setw(12) << status<< endl;
}
void job::clear_vector()
{
    requiresskill.clear();
}
job::job(int tag, string type, string compa, string loca,double max,double min, string stat, string skill)
{
    tag_num = tag;
    jobtype = type;
    company = compa;
    location = loca;
    status = stat;
    max_salary=max;
    min_salary=min;
    string st_req;
    stringstream ss(skill);
    while (getline(ss, st_req, ','))
    {
        requiresskill.push_back(st_req);
    }
}
bool job::operator>(job &another)
{
    return tag_num > another.tag_num;
}
bool job::operator<=(job &another)
{
    return tag_num <= another.tag_num;
}
void input(string name)
{
    clear_screen();
    data *temo;
    string type, compa, loca, req;
    double max, min;
    req="";
    temo->data::anounment();
    // clear screen!
    cout << "JOB" << endl;
    cin.ignore(); // ถ้ามีbufferเอาคอมเมนออก
    cout << "JOB type:";
    getline(cin, type);
    while(1)
    {
        string skill;
        cout << "requirment(stop type back):";
        getline(cin,skill);
        if(skill=="back")
             break;
        else req+=(skill+",");
    }
    cout << "location:";
    getline(cin, loca);
    /*cout << "BY company:";
    getline(cin, compa);*/
    compa=name;
    cout << "max salary:";
    cin >> max;
    while(1)
    {
    cout << "min salary:";
    cin >> min;
    if(min<=max)
        break;
    else cout<<"pls input the real infomation"<<endl;
    }
    clear_screen();
    cout<<endl;
    job neow;
    neow.add_job(type, compa, req, loca, max, min);
    temo=&neow;
    temo->anounment();
}

void job::add_job(string jobtype, string company, string requir, string loca, double max, double min, string stas)
{
    update_numofjob();
    tag_num++;
    ofstream F;
    F.open("job_data.txt", ios::app); // เปิดไฟล์โหมดเติมท้ายกับสร้าง
    F << "\"" << tag_num << "\" \"" << jobtype << "\" \"" << company << "\" \"" << requir << "\" \"" << loca << "\" \"" << max << "\" \"" << min << "\" \"" << stas << "\"" << endl;
    F.close();
}
void clear_qoate(stringstream &qoute)
{
    string buffer;
    getline(qoute, buffer, '"');
}
void job::update_numofjob()
{
    ifstream Out("job_data.txt");
    if (Out)
    {
        string line;
        while (getline(Out, line)) // อ่านค่าจากไฟล์
        {
            stringstream iss(line);
            string num;
            clear_qoate(iss);
            getline(iss, num, '"');
            tag_num = stoi(num);
        }
        Out.close();
    }
}

#endif