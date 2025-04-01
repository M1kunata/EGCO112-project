class jobclass
{
    private:
        string jobtitle,company,decription,location;
        float salary;
    public:
        friend jobseeker;
        void add_job(string head,string company,string detail,string loca,float money);
};