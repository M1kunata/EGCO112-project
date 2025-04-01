#ifndef jobseeker_company_h
#define jobseeker_company_h
#include "userclass.h"
#include <iostream>
using namespace std;
class seekerjob:public user
{
    private:
        bool resume_stas;
    public:
        seekerjob(string fullname,string pas,string mail,string user,bool resume_stas=false);
        void update_resume_stas(bool stas);
};
void seekerjob::update_resume_stas(bool stas)
{
    resume_stas=stas;
}
seekerjob::seekerjob(string fullname,string pas,string mail,string user,bool stas)//regis
{
    regisster(fullname,pas,mail,user);   
    resume_stas=stas;
}

class company:public user
{
    private:
        string about,com_name;
    public:
        void show_data_com();
};




#endif