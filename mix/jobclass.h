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
    string jobtype, company, location, status; // status pending end
    vector<string> requiresskill;
    double max_salary, min_salary;

public:
    job(int tag_num = 0, string jobtype = "unknown", string company = "unknown", string location = "unknown",double max_salary=0,double min_salary=0, string status = "recruiting", string skill = "unknown");
    bool operator<=(job &another);
    bool operator>(job &another);
    void add_job(string jobtype, string company, string requir, string location, double max, double min, string status = "recruiting");
    void update_numofjob();
    void edit_job();
    void clear_vector();
    void display();
};
void job::display()
{
    cout << tag_num << jobtype << endl;
    cout << company << endl;
    cout << location << endl;
    cout << max_salary << ":" << min_salary << endl;
    cout << status << endl;
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
void input()
{
    string type, compa, loca, req;
    double max, min;
    // clear screen!
    cout << "JOB:" << endl;
    cin.ignore(); // ถ้ามีbufferเอาคอมเมนออก
    cout << "JOB type:";
    getline(cin, type);
    cout << "requirment(,):";
    getline(cin, req);
    cout << "location:";
    getline(cin, loca);
    cout << "BY company:";
    getline(cin, compa);
    cout << "max salary:";
    cin >> max;
    cout << "min salary:";
    cin >> min;
    job neow;
    neow.add_job(type, compa, req, loca, max, min);
}

void job::add_job(string jobtype, string company, string requir, string loca, double max, double min, string stas)
{
    update_numofjob();
    tag_num++;
    ofstream F;
    F.open("job_data.txt", ios::app); // เปิดไฟล์โหมดเติมท้ายกับสร้าง
    F << "\"" << tag_num << "\",\"" << jobtype << "\",\"" << company << "\",\"" << requir << "\",\"" << loca << "\",\"" << max << "\",\"" << min << "\",\"" << stas << "\"" << endl;
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
            string require, st_req, quote, num;
            clear_qoate(iss);
            getline(iss, num, '"');
            tag_num = stoi(num);
        }
        Out.close();
    }
}

#endif