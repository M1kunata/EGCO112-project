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
    username=fullname;
    password=pas;
    email=mail;
    name=user;
}