class user{
    private:
    string username;
    string password;
    string email;
    string name;
public:
    void regisster(string fullname,string pas,string mail,string user);
   
};
void user::regisster(string fullname,string pas,string mail,string user)
{
    username=user;
    password=pas;
    email=mail;
    name=fullname;
    ofstream F;
    F.open("user_data.txt",ios::app);//เปิดไฟล์โหมดเติมท้ายกับสร้าง
    F <<name<<" "<<pas<<" "<<email<<" "<<user<<endl;
    F.close();
}