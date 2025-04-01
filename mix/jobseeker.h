#include "userclass.h"
class jobseeker:public user
{
    private:
        bool resume_stas;
    public:
        jobseeker(string fullname,string pas,string mail,string user,bool resume_stas=false);
        
};
jobseeker::jobseeker(string fullname,string pas,string mail,string user,bool stas)//regis
{
    regisster(fullname,pas,mail,user);   
    resume_stas=stas;
}
