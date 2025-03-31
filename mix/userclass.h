class user{
    private:
    string username;
    string password;
    string email;
    string name;
public:
    void regisster(string fullname,string pas,string mail,string user);
    void edit_data(string username);
};
void user::regisster(string fullname,string pas,string mail,string user)
{
    username=fullname;
    password=pas;
    email=mail;
    name=user;
    ofstream MyFile(name);//สร้างไฟล์
    MyFile <<username<<" "<<password<<" "<<email<<" "<<name<<endl;//ใส่ค่า
    MyFile.close();//ปิดไฟล์ 
}
void user::edit_data(string username)
{
    string *position,line;
    fstream MYFILE(username);
    getline(MYFILE,line);
}