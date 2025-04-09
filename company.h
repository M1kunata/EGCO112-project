#include "bst.h"
#include <cstring>
void read_job(string,string);
void company_menu(string companyname)
{   
    while(1)
    {
        display_choose_company_menu();
        string choice;
        cin>>choice;
        return choice;
        if(strcmp(choice,"1")==0)
        {
            while(1)
            {
                string in;
                input(companyname);
                display_add_another();
                cin>>in;
                if(in=="1")
                    clear_screen();//clear screen
                    //เพิ่มงาน
                else if(in=="2")
                    break;
            }
        }
        else if(strcmp(choice,"2")==0)
        {
            char isfilter[10];
            char where[10]="0";
            while(1)
            {
            display_Manage_Posted_Jobs();
            cin>>isfilter;
            if(strcmp(isfilter,"1")==0)
            {
                read_job(companyname,"nofilter");//แค่เฉพาะบริษัท
                cout<<"where you want to go next?"<<endl;
                cout<<"1.choose view again"<<endl;
                cout<<"2.back to menu"<<endl;
                cout<<"choose:";
                cin>>where;
                if(strcmp(where,"1")==0)
                {
                    clear_screen();//clear screen
                }
                else if(strcmp(where,"2")==0)
                {
                    break;
                }
            }
            else if(strcmp(isfilter,"2")==0)
            {
                char thing[10];
                cout<<"Sort by"<<endl;
                cout<<"1.salary"<<endl;
                cout<<"2.status"<<endl;
                cout<<"choose:";
                cin>>thing;
                if(strcmp(thing,"1")==0)
                {
                    read_job(companyname,"salary");
                }
            }
        }
    }
    else if(strcmp(choice,"3")==0)
        break;
}
    
}
void read_job(string comname,string sor )//เพื่อเพิ่มเข้า bst
{
    BST one;
    int tag,max,min;
    string type,compa,loca,stat;
    ifstream Out("job_data.txt");
    if(Out)
    {
    string line;
    while(getline(Out,line))//อ่านค่าจากไฟล์
    {
        stringstream iss(line);
        string require,st_req,quote,num;
        clear_qoate(iss); getline(iss,num,'"');tag=stoi(num);
        clear_qoate(iss); getline(iss, type, '"');
        clear_qoate(iss); getline(iss, compa, '"');
        clear_qoate(iss); getline(iss, require, '"');
        clear_qoate(iss); getline(iss, loca, '"');
        clear_qoate(iss); getline(iss, num, '"');max=stoi(num);
        clear_qoate(iss); getline(iss,  num, '"');min=stoi(num);
        clear_qoate(iss);getline(iss,stat,'"');
        job n1(tag,type,compa,loca,max,min,stat,require);
        if(n1.check(comname))   
            {
                insertNode(&one,n1,sor);//ยัดเข้าbst
            }
                n1.clear_vector();
    }
        inorder(one.root);//เรียงออกมาให้ดูตอนนี้เรียงตามลำดับid
}
    Out.close();
}
