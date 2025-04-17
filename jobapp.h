#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
using namespace std;

void apply_job(string user_id, string job_id);
void addNewApplication(string user_id, string job_id);
int read_jobs();
vector<vector<string>> read_applications();

vector<string> extractFields(const string &line);

void apply_job(string user_id, string job_id)
{
    int selected_job;
    read_jobs();
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

void updateApplicationStatus(string application_id, string status)
{

    ifstream inFile("application_data.txt");
    vector<string> lines;
    string line;
    int targetline = stoi(application_id);
    int currentline = 1;
    while (getline(inFile, line))
    {
        auto fields = extractFields(line);
        if (fields[0] == application_id)
        {
            fields[3] = status;
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
    ifstream file("application_data.txt");
    string line;
    int lineno = 1;
    while (getline(file, line))
    {
        if (!line.empty())
            lineno++;
    }
    file.close();

    string newLine = "\"" + to_string(lineno) + "\" \"" + job_id + "\" \"" + user_id + "\" \"" + "pending" + "\"";
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

    applications = read_applications();
    // Header
    cout << left << setw(15) << "Application ID"
         << setw(15) << "Company ID"
         << setw(15) << "JobSeeker ID" << endl;
    cout << string(55, '-') << endl;
    for (const auto &application : applications)
    {
        if (application[1] == company_id)
        {
            cout << left << setw(15) << application[0]
                 << setw(15) << application[1]
                 << setw(15) << application[2] << endl;
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

int read_jobs()
{
    ifstream file("job_data.txt");

    if (!file)
    {
        cerr << "Failed to open file." << endl;
        return 1;
    }

    string line;
    int lineNumber = 1;

    // Header
    cout << left << setw(6) << "ID"
         << setw(15) << "Company"
         << setw(6) << "C"
         << setw(15) << "Skills"
         << setw(10) << "E"
         << setw(6) << "F"
         << setw(6) << "G"
         << setw(12) << "Status" << endl;
    cout << string(75, '-') << endl;
    while (getline(file, line))
    {
        vector<string> fields = extractFields(line);
        string id = fields[0];
        string company_name = fields[1];
        string c = fields[2];
        string skills = fields[3];
        string e = fields[4];
        string f = fields[5];
        string g = fields[6];
        string status = fields[7];
        // Data output
        cout << left << setw(6) << id
             << setw(15) << company_name
             << setw(6) << c
             << setw(15) << skills
             << setw(10) << e
             << setw(6) << f
             << setw(6) << g
             << setw(12) << status << endl;
    }

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