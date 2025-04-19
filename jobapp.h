#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include "bstclass.h"
using namespace std;

void apply_job(string user_id, string job_id,string sort);
void addNewApplication(string user_id, string job_id);
int read_jobs(BST &, int,string sort);
vector<vector<string>> read_applications();

vector<string> extractFields(const string &line);

void apply_job(string user_id, string job_id,string sort)
{
    BST one;
    string back;
    int selected_job;
    read_jobs(one, 0,sort);
    cout << "type back to go back else type anything" << endl;
    cin >> back;
    if (back == "back")
        return;
    cout << "Select a job (ID) you would like to apply:" << endl;
    cin >> selected_job;
    addNewApplication(user_id, to_string(selected_job));
}

std::string rebuildLine(const std::vector<std::string> &fields)
{
    std::string result;
    for (size_t i = 0; i < fields.size(); ++i)
    {
        result += "\"" + fields[i] + "\"";
        if (i != fields.size() - 1)
            result += " ";
    }
    return result;
}

void updateApplicationStatus(string application_id, string jobid, string status)
{

    ifstream inFile("application_data.txt");
    vector<string> lines;
    string line;
    int targetline = stoi(application_id);
    int currentline = 1;
    while (getline(inFile, line))
    {
        auto fields = extractFields(line);
        // cout << "Checking " << fields[0] << " vs " << application_id << endl;
        // cin.get();
        if (fields.size() > 5 && fields[0] == application_id)
        {
            fields[5] = status;
            line = rebuildLine(fields);
        }
        else if (fields.size() > 5 && fields[1] == jobid)
        {
            fields[5] = "rejected";
            line = rebuildLine(fields);
        }
        lines.push_back(line);
        currentline++;
    }
    inFile.close();

    std::ofstream outFile("application_data.txt");
    for (const auto &l : lines)
    {
        outFile << l << "\n";
    }
    outFile.close();
}

void addNewApplication(string user_id, string job_id)
{
    BST one;
    job clone;
    read_jobs(one, 1,"");
    int tag_num, tag;
    string jobtype, company, location, status, requir; // status pending end
    double max_sal, min_sal;
    tag = stoi(job_id);
    // if(one.exists(tag)) //=check ตอนทำ
    // cout<<"find"<<endl;
    one.read_bst(tag, clone);
    // clone.display();
    clone.getdata(tag_num, jobtype, company, location, max_sal, min_sal, status, requir);
    // cin.get();
    ifstream file("application_data.txt");
    string line;
    int lineno = 1;
    while (getline(file, line))
    {
        if (!line.empty())
            lineno++;
    }
    file.close();
    string newLine = "\"" + to_string(lineno) + "\" \"" + job_id + "\" \"" + jobtype + "\" \"" + company + "\" \"" + user_id + "\" \"" + "pending" + "\"";
    // Append the new application to the file
    ofstream outFile("application_data.txt", ios::app);
    if (!outFile)
    {
        cerr << "Failed to open file." << endl;
        return;
    }
    outFile << newLine << endl;
    outFile.close();
    file.close();
}

void view_applications_by_company(string company_id)
{
    vector<vector<string>> applications;
    BST one;
    read_jobs(one, 1,"");
    applications = read_applications();
    // Header
    cout << left << setw(15) << "Application ID"
         << setw(15) << "Job ID"
         << setw(15) << "Job Name"
         << setw(15) << "Company ID"
         << setw(15) << "JobSeeker ID" << endl;
    cout << string(55, '-') << endl;
    for (const auto &application : applications)
    {
        if (application[3] == company_id && application[5] == "pending")
        {
            cout << left << setw(15) << application[0]
                 << setw(15) << application[1]
                 << setw(15) << application[2]
                 << setw(15) << application[3]
                 << setw(15) << application[4] << endl;
        }
    }
}

void view_applications_by_jobseeker(string jobseeker_id)
{
    vector<vector<string>> applications;

    applications = read_applications();
    // Header
    cout << left << setw(15) << "Application ID"
         << setw(15) << "Company ID"
         << setw(15) << "JobSeeker ID" << endl;
    cout << string(55, '-') << endl;
    for (const auto &application : applications)
    {
        if (application[2] == jobseeker_id)
        {
            cout << left << setw(15) << application[0]
                 << setw(15) << application[1]
                 << setw(15) << application[2] << endl;
        }
    }
}

vector<vector<string>> read_applications()
{
    ifstream file("application_data.txt");

    if (!file)
    {
        cerr << "Failed to open file." << endl;
        return {};
    }

    string line;
    int lineNumber = 1;
    vector<vector<string>> applications;
    while (getline(file, line))
    {
        vector<string> fields = extractFields(line);
        applications.push_back(fields);
    }

    file.close();
    return applications;
}

int read_jobs(BST &one, int show,string sort)
{
    ifstream file("job_data.txt");
    if (!file)
    {
        cerr << "Failed to open file." << endl;
        return 1;
    }

    string line;
    int lineNumber = 1;
    if (show == 0)
    {
        // Header
        cout << left << setw(6) << "ID"
             << setw(15) << "Job Title"
             << setw(15) << "Company"
             << setw(25) << "Requirements"
             << setw(20) << "Location"
             << setw(15) << "Max Salary"
             << setw(15) << "Min Salary"
             << setw(12) << "Status" << endl;
        cout << string(125, '-') << endl;
    }
    while (getline(file, line))
    {
        vector<string> fields = extractFields(line);
        string id = fields[0];
        string job_title = fields[1];
        string company_name = fields[2];
        string requirements = fields[3];
        string location = fields[4];
        string max = fields[5];
        string min = fields[6];
        string status = fields[7];
    
        // Data output
       /* if (show == 0)
        {
            if (status == "recruiting")
                cout << left << setw(6) << id
                     << setw(15) << job_title
                     << setw(15) << company_name
                     << setw(25) << requirements
                     << setw(20) << location
                     << setw(15) << max
                     << setw(15) << min
                     << setw(12) << status << endl;
        }*/
        job n1(stoi(id), job_title, company_name, location, stod(max), stod(min), status, requirements);
        if (status == "recruiting")
        one.insertNode(n1, sort); // ยัดเข้าbst
        n1.clear_vector();
    }
    one.displayInOrder();
    file.close();
    return 0;
}

vector<string> extractFields(const string &line)
{
    vector<string> fields;
    bool inQuotes = false;
    string current;

    for (char ch : line)
    {
        if (ch == '"')
        {
            inQuotes = !inQuotes;
            if (!inQuotes)
            {
                fields.push_back(current);
                current.clear();
            }
        }
        else if (inQuotes)
        {
            current += ch;
        }
    }
    return fields;
}