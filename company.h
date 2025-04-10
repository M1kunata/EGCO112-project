#include "bst.h"
#include "display.h"
#include <cstring>
void read_job(BST &, string, string);
void company_menu(string companyname)
{
    while (1)
    {
        display_choose_company_menu();
        char choice[10];
        cin >> choice;
        if (strcmp(choice, "1") == 0)
        {
            while (1)
            {
                string in;
                input(companyname);
                display_add_another();
                cin >> in;
                if (in == "1")
                    clear_screen(); // clear screen
                // เพิ่มงาน
                else if (in == "2")
                    break;
            }
        }
        else if (strcmp(choice, "2") == 0)
        {
            char isfilter[10];
            char where[10] = "0";
            while (1)
            {
                display_Manage_Posted_Jobs();
                cin >> isfilter;
                if (strcmp(isfilter, "1") == 0)
                {
                    BST dis_root;
                    read_job(dis_root, companyname, "nofilter"); // แค่เฉพาะบริษัท
                    cout << "where you want to go next?" << endl;
                    cout << "1.choose view again" << endl;
                    cout << "2.back to menu" << endl;
                    cout << "choose:";
                    cin >> where;
                    if (strcmp(where, "1") == 0)
                    {
                        clear_screen(); // clear screen
                    }
                    else if (strcmp(where, "2") == 0)
                    {
                        break;
                    }
                }
                else if (strcmp(isfilter, "2") == 0)
                {
                    int id;
                    BST edit_root,collection;
                    read_job(collection,"all","nofilter");
                    while(1)
                    {
                        char thing[10];
                        read_job(edit_root, companyname, "nofilter");
                        cout << "which id you want to edit?" << endl;
                        cout << "choose:";
                        string sid;
                        cin >> sid;
                        id = stoi(sid);
                        if(search_check(edit_root.root,id))
                            break;
                        else{
                            cout<<"pls choose the correct id!!"<<endl;
                        }
                    }
                    cout << "begin to edt...." << endl;
                    string type, compa, loca, req,status;
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
                    //cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
                    cout<<"status(recuiting OR end):";cin>>status;
                    job edit(id,type,companyname,loca,max,min,status,req);
                    search(collection.root, id, edit);
                    ofstream rewrite("job_data.txt");
                    rewrite.close();
                    clone_to_file(collection.root);
                }
            }
        }
        else if (strcmp(choice, "3") == 0)
            break;
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
            if(comname=="all")
            {
                insertNode(&one,n1,sor);
                n1.clear_vector();
            }
            else{
                if (n1.check_com(comname))
                {
                    insertNode(&one, n1, sor); // ยัดเข้าbst
                }
                n1.clear_vector();
            }
        }
        if(comname!="all")
             inorder(one.root); // เรียงออกมาให้ดูตอนนี้เรียงตามลำดับid
    }
    Out.close();
}
