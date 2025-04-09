void welcome();
void main_menu();
void user_register();
void user_login();
void user_reset();
void exit();

void welcome()
{
    cout<< "  ____ ___    _____         _    __        __         _    "<<endl;
    cout<< " / ___/ _ \\  |  ___|_ _ ___| |_  \\ \\      / /__  _ __| | __"<<endl;
    cout<< "| |  | | | | | |_ / _` / __| __|  \\ \\ /\\ / / _ \\| '__| |/ /"<<endl;
    cout<< "| |__| |_| | |  _| (_| \\__ \\ |_    \\ V  V / (_) | |  |   < "<<endl;
    cout<< " \\____\\___/  |_|  \\__,_|___/\\__|    \\_/\\_/ \\___/|_|  |_|\\_\\"<<endl;
    cout<<endl;
}

void main_menu()
{
    int main_choice;
    do
    {
        cout<<"== Main Menu =="<<endl;
        cout<<"1. Register"<<endl;
        cout<<"2. Login"<<endl;
        cout<<"3. Exit"<<endl;
        cout<<endl;
        cout<<"Choose an option: ";
        cin>>main_choice;

        switch(main_choice)
        {
            case 1:
            system("clear");
            user_register();
            break;

            case 2:
            system("clear");
            user_login();
            break;

            case 3:
            system("clear");
            exit();
            break;

            default:
            system("clear");
            cout<<"Invalid choice. Please enter a number between 1 and 3."<<endl;
            break;
        }
    }while (main_choice != 1 && main_choice != 2 && main_choice != 3);
}

void user_register()
{
    int register_choice;
    do
    {
        cout<<"== Register =="<<endl;
        cout<<"1. User"<<endl;
        cout<<"2. Company"<<endl;
        cout<<endl;
        cout<<"Choose an option: ";
        cin>>register_choice;
        switch(register_choice)
        {
            case 1:
            system("clear");
            break;

            case 2:
            system("clear");
            break;

            default:
            system("clear");
            cout<<"Invalid choice. Please enter a number between 1 and 3."<<endl;
            break;
        }
    }while (register_choice != 1 && register_choice != 2 && register_choice != 3);

}

void user_login()
{
    cout<<"== Login =="<<endl;
    cout<<"1. User"<<endl;
    cout<<"2. Company"<<endl;
    cout<<endl;
}

void user_reset()
{

}

void exit()
{
    cout<< "______ _                 _                 "<<endl;       
    cout<< "|_   _| |__   __ _ _ __ | | __  _   _  ___  _   _ "<<endl;
    cout<< "  | | | '_ \\ / _` | '_ \\| |/ / | | | |/ _ \\| | | |"<<endl;
    cout<< "  | | | | | | (_| | | | |   <  | |_| | (_) | |_| |"<<endl;
    cout<< "  |_| |_| |_|\\__,_|_| |_|_|\\_\\  \\__, |\\___/ \\__,_|"<<endl;
    cout<< "                                |___/             "<<endl;
}

int main()
{
    welcome();
    main_menu();

    return 0;
}