#include "display.h"
#include "bstclass.h"
#include "jobapp.h"
#include "Gamyui/usergamyui.h"
#include <cstring>
using namespace std;
void read_job(BST &, string, string);
void add_job(string compa);
void rechecktomenu(string);
void edit_job(string);
void updatestatusfromappli(string id_job, string);
void company_dashboard(user *company)
{
    string companyname = company->getUsername();
    while (1)
    { // เริ่ม ลูปเมนู
        display_choose_company_menu(companyname);
        char choice[10];
        cin >> choice;
        if (strcmp(choice, "1") == 0)
        {
            system("clear");
            company->display();
            cout << "\n[Press Enter to go back to the menu]";
            cin.ignore();
            cin.get();
        }
        else if(strcmp(choice,"2")==0)
        {
            cin.ignore(50,'\n');
            editUserInfo(company);
        }
        else if (strcmp(choice, "3") == 0)
        {
            add_job(companyname); // อยู่ข้างล่าง
        }
        else if (strcmp(choice, "4") == 0)
        {
            char isfilter[10];
            while (1)
            {
                display_Manage_Posted_Jobs();
                cin >> isfilter;
                if (strcmp(isfilter, "1") == 0)
                {
                    rechecktomenu(companyname);
                    char where[10] = "0";
                    cin >> where;
                    if (strcmp(where, "1") == 0)
                    {
                        clear_screen(); // clear screen
                    }
                    else if (strcmp(where, "2") == 0)
                    {
                        break;
                    }
                    else cout<<"Invalid choice"<<endl;
                }
                else if (strcmp(isfilter, "2") == 0)
                {
                    edit_job(companyname);
                }
                else if (strcmp(isfilter, "3") == 0)
                    break;
                else cout<<"Invalid choice"<<endl;
            }
        }
        else if (strcmp(choice, "5") == 0) // อันนี้นะไรอัน
        {
            int p=1;
            string idforjob,idforappli,check;
            int id_job;
            char numtoaccept[10];
            view_applications_by_company(companyname);
            while(1)
            {    
                try {
                int i=0,continue1=0;
                  
                    vector<vector<string>> applications;
                    applications = read_applications();
                    while(1)
                    {
                        cout<<"'type back to go back else type anything'"<<endl;
                        cin>>check;
                        if (check=="back")
                        {
                        break;
                        }
                        cout << "choose job Application:" ;
                        cin >> id_job;
                        if(cin.fail())
                           throw "Don't stupid!!";
                        for (const auto &application : applications)
                        {
                            if (application[0] == to_string(id_job)&&application[3]==companyname)
                            {   
                                i=1;
                                continue1=1;
                                idforjob=application[1];
                                idforappli=application[0];
                                break;
                            }
                        }
                        if(i==1) break;
                        else cout<<"Invalid ID"<<endl;   
                    }
                    if(continue1==1)
                    {
                        updateApplicationStatus(idforappli,idforjob, "accepted");//อัพเดทสถานะงานappli
                        updatestatusfromappli(idforjob, "accept");//อัะเดทสถานะไฟล์งานบริษัท
                    }
                }
            
            catch(const char* n)
                    {
                        p=0;
                        cin.clear();
                        cin.ignore(500,'\n');
                        cout<<n<<endl;
                    }
                if(p==1)break;
        } 
    }
        else if (strcmp(choice, "6") == 0)
            break;
        else {
            cout<<"invalid choice"<<endl;
        }
    }
}
void edit_job(string companyname)
{
    int id;
    BST edit_root, collection;
    read_job(collection, "all", "nofilter");
    
    while (1)
    {
        string check;
    cout<<"connect to editing job"<<endl;
    cout<<"type back to back else type anything"<<endl;
    cin>>check;
    if(check=="back")
        return;
        char thing[10];
        read_job(edit_root, companyname, "nofilter");
        while(1)
        {
        cout << "which id you want to edit?" << endl;
        cout << "choose:";
            string sid;
            cin >> sid;
            id = stoi(sid);
            if (edit_root.exists(id))
                break;
            else
            {
                string ok = "";
                while (1)
                {
                    cout << "pls choose the correct id!!" << endl;
                    cout << "OK?" << endl
                         << "1.ok" << endl
                         << "2.no";
                    cin >> ok;
                    if (ok == "1")
                        break;
                }
            }
        }
        cout << "begin to edt...." << endl;
        string type, compa, loca, req, status;
        double max, min;
        req = "";
        // clear screen!
        cout << "JOB:" << endl;
        cin.ignore(); // ถ้ามีbufferเอาคอมเมนออก
        cout << "JOB type:";
        getline(cin, type);
        while (1)
        {
            string skill;
            cout << "requirment(stop type back):";
            cin >> skill;
            if (skill == "back")
                break;
            else
                req += (skill + ",");
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "location:";
        getline(cin, loca);
        // cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "max salary:";
        cin >> max;
        while (1)
        {
            cout << "min salary:";
            cin >> min;
            if (min <= max)
                break;
            else
                cout << "pls input the real infomation" << endl;
        }
        while(1)
        {
        cout << "status(r[recruiting] OR a[accept]):";
        cin >> status;
        if(status=="r")status="recruiting";
        if(status=="a")status="accept";
        if(status=="recruiting"||status=="accept")
            break;
        else cout<<"type in bracket"<<endl;
        }
        job edit(id, type, companyname, loca, max, min, status, req);
        collection.editJob(id, edit);
        ofstream rewrite("job_data.txt");
        rewrite.close();
        collection.saveToFile("job_data.txt", "NULL");
    }
}
void rechecktomenu(string compa)
{
    BST dis_root;
    read_job(dis_root, compa, "nofilter"); // แค่เฉพาะบริษัท
    cout << "where you want to go next?" << endl;
    cout << "1.choose view again" << endl;
    cout << "2.back to menu" << endl;
    cout << "choose:";
}
void add_job(string compa)
{
    string check;
    cout<<"type back to back else type anything"<<endl;
    cin>>check;
    if(check=="back")
        return;
    else{
    while (1)
    { // ลูปเพิ่มงาน
        string in;
        int i=0;
        input(compa);
        while(1)
        {    
        display_add_another();
            cin >> in;
            if (in == "1")
            {
                clear_screen(); // clear screen
                break;
            }    // เพิ่มงาน
            else if (in == "2")
                {
                    i=1;
                    break;
                }
            else cout<<"Invalid choice"<<endl;
        }
        if(i==1)break;
    }
    }
}
void read_job(BST &one, string comname, string sor) // เพื่อเพิ่มเข้า bst
{
    int tag, max, min;
    string type, compa, loca, stat;
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
            tag = stoi(num);
            clear_qoate(iss);
            getline(iss, type, '"');
            clear_qoate(iss);
            getline(iss, compa, '"');
            clear_qoate(iss);
            getline(iss, require, '"');
            clear_qoate(iss);
            getline(iss, loca, '"');
            clear_qoate(iss);
            getline(iss, num, '"');
            max = stoi(num);
            clear_qoate(iss);
            getline(iss, num, '"');
            min = stoi(num);
            clear_qoate(iss);
            getline(iss, stat, '"');
            job n1(tag, type, compa, loca, max, min, stat, require);
            if (comname == "all")
            {
                one.insertNode(n1, sor);
                n1.clear_vector();
            }
            else
            {
                if (n1.check_com(comname))
                {
                    one.insertNode(n1, sor); // ยัดเข้าbst
                }
                n1.clear_vector();
            }
        }
        if (comname != "all"){
            cout << left << setw(6) << "ID"
            << setw(15) << "Job Title"
            << setw(15) << "Company"
            << setw(25) << "Requirements"
            << setw(20) << "Location"
            << setw(15) << "Max Salary"
            << setw(15) << "Min Salary"
            << setw(12) << "Status" << endl;
       cout << string(125, '-') << endl;
            one.displayInOrder(); // เรียงออกมาให้ดูตอนนี้เรียงตามลำดับid
        }
        }
    Out.close();
}
void updatestatusfromappli(string id_job, string stat)
{
    int tag_num, tag;
    string jobtype, company, location, status, requir; // status pending end
    double max_sal, min_sal;
    BST update;
    read_job(update, "all", "nofilter");
    job updateappli;
    tag = stoi(id_job);
    update.read_bst(tag, updateappli);
    updateappli.getdata(tag_num, jobtype, company, location, max_sal, min_sal, status, requir);
    status = stat;
    job readytoup(tag_num, jobtype, company, location, max_sal, min_sal, status, requir);
    update.editJob(tag, readytoup);
    ofstream rewrite("job_data.txt");
    rewrite.close();
    update.saveToFile("job_data.txt", "NULL");
}