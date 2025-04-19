#ifndef USERGAMYUI_H
#define USERGAMYUI_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm> 
#include <cctype>

#include "pass.h"
//#include "../edit.h"
#include "../display.h"    
using namespace std;

class user;
void display_register();
void display_security();
void display_user_dashboard(user* currentUser);
void clear_screen();
void editUserInfo(user* currentUser);
void display_jobseeker_dashboard(user* currentUser);
void forget_password(const string& userfile);
void apply_job(string user_id, string job_id);
void display_userInfo(user* u, const string& warning = "");
bool is_username_taken(const string& uname);
void display_regisInfo(user* u, const string& warning);

class user {
private:
    string username;
    string password;
    string name;
    string email;
    string phone;
    string document;
    string role;
    string skills; // เก็บเฉพาะฝั่ง jobseeker

public:
    user() = default;

    user(string u, string p, string n, string e, string ph, string doc, string r, string sk)
    : username(u), password(p), name(n), email(e), phone(ph), document(doc), role(r), skills(sk) {}

        string getEmail() const { return email; }
        string getPhone() const { return phone; }
        string getDocument() const { return document; }
        string getName() const { return name; }
        void setPassword(const string& newpass) { password = newpass; }
        void setUsername(const string& newuser) { username = newuser; }
        void setName(const string& newname) { name = newname; }
        void setEmail(const string& e) { email = e; }
        void setPhone(const string& p) { phone = p; }
        void setDocument(const string& d) { document = d; }
        void setSkills(const string& s) { skills = s; }
        
        void display() const {
            cout << "Username: " << username << "\nName: " << name
                 << "\nEmail: " << email << "\nPhone: " << phone
                 << "\nRole: " << role;
        
            if (role == "jobseeker") {
                cout << "\nResume: " << document;
                cout << "\nSkills: " << skills;
            } else if (role == "company") {
                cout << "\nDescription: " << document;
                cout << "\nJobs Offered: " << skills;
            }
        
            cout << endl;
        }

    string getRole() const { return role; }
    string getUsername() const { return username; }
    string getPassword() const { return password; }
    string getSkills() const { return skills; }

    static void save_user_to_file(const string& userfile,
        const string& jobfile,
        const string& companyfile,
        const user& new_user,
        const string& school,
        const string& pet,
        const string& color){
        // เขียน user.txt (ไม่มีช่องว่างในฟิลด์นี้)
        ofstream ufile(userfile, ios::app);
        ufile << new_user.getUsername() << "|"<< new_user.getPassword() << "|"<< school << "|" << pet << "|" << color << "\n";

    ufile.close();
    
        // jobseeker ใช้ | คั่น
        if (new_user.getRole() == "jobseeker") {
            ofstream outfile(jobfile, ios::app);
            outfile << new_user.getUsername() << "|"
                    << new_user.getEmail() << "|"
                    << new_user.getPhone() << "|"
                    << new_user.getName() << "|"
                    << new_user.getDocument() << "|"
                    << new_user.getSkills() << "\n";
            outfile.close();
        }
    
        // company ใช้ | คั่น
        else if (new_user.getRole() == "company") {
            ofstream outfile(companyfile, ios::app);
            outfile << new_user.getUsername() << "|"
                    << new_user.getEmail() << "|"
                    << new_user.getPhone() << "|"
                    << new_user.getName() << "|"  // ใช้ name เป็น company name
                    << new_user.getDocument() << "|"  // ใช้ document เก็บ description
                    << new_user.getSkills() << "\n";  // skills คือ jobs_offered
            outfile.close();
        }
    }
    
    static void register_user(const string& userfile, const string& jobfile, const string& companyfile) {
        string uname, pass, name, email, phone, doc, role, skills;
        string school, pet, color;
        string role_input;

        display_register();
        getline(cin, role_input);
        if (role_input == "1") role = "jobseeker";
        else if (role_input == "2") role = "company";
        else return;

        while (true) {
            cout << "Username (or type 'cc' to cancel): ";
            getline(cin, uname);
            if (uname == "cc" || uname == "cancel") return;
            if (uname.empty()) {
                cout << "❌ Username cannot be empty.\n";
                continue;
            }
            if (is_username_taken(uname)) {
                cout << "❌ Username already taken. Please try another.\n";
                continue;
            }
            break;
        }

        do {
            pass = getPasswordMaskedPreview("Password (or type 'cc' to cancel): ");
            if (pass == "cc" || pass == "cancel") return;
            if (pass.empty()) cout << "❌ Password cannot be empty.\n";
        } while (pass.empty());

        do {
            cout << (role == "company" ? "Company Name" : "Full Name") << " (or type 'cc' to cancel): ";
            getline(cin, name);
            if (name == "cc" || name == "cancel") return;
            if (name.empty()) cout << "❌ Name cannot be empty.\n";
        } while (name.empty());

        do {
            cout << "Email (or type 'cc' to cancel): ";
            getline(cin, email);
            if (email == "cc" || email == "cancel") return;
            if (email.empty() || email.find('@') == string::npos)
                cout << "❌ Invalid email. Must contain '@' and not be empty.\n";
        } while (email.empty() || email.find('@') == string::npos);

        do {
            cout << "Phone (or type 'cc' to cancel): ";
            getline(cin, phone);
            if (phone == "cc" || phone == "cancel") return;
            if (phone.empty() || phone.length() != 10 || !all_of(phone.begin(), phone.end(), ::isdigit))
                cout << "❌ Invalid phone number. Must be 10 digits.\n";
        } while (phone.empty() || phone.length() != 10 || !all_of(phone.begin(), phone.end(), ::isdigit));

        do {
            cout << "Document Filename (or type 'cc' to cancel): ";
            getline(cin, doc);
            if (doc == "cc" || doc == "cancel") return;
            if (doc.empty() || (
                doc.find(".pdf") == string::npos &&
                doc.find(".jpg") == string::npos &&
                doc.find(".png") == string::npos &&
                doc.find(".jpeg") == string::npos &&
                doc.find(".doc") == string::npos)) {
                cout << "❌ Invalid or empty file. Use .pdf, .jpg, .png, .jpeg, or .doc only.\n";
            }
        } while (doc.empty() || (
            doc.find(".pdf") == string::npos &&
            doc.find(".jpg") == string::npos &&
            doc.find(".png") == string::npos &&
            doc.find(".jpeg") == string::npos &&
            doc.find(".doc") == string::npos));

        cout << "School you graduated from: "; getline(cin, school);
        cout << "First pet's name: "; getline(cin, pet);
        cout << "Favorite color: "; getline(cin, color);

        if (role == "jobseeker") {
            vector<string> skill_list;
            while (true) {
                string s;
                cout << "Skill (or type 'cc' to cancel, 'cf' to confirm): ";
                getline(cin, s);
                if (s == "cc" || s == "cancel") return;
                if (s == "cf" || s == "confirm") {
                    if (skill_list.empty()) {
                        cout << "❌ You must enter at least one skill.\n";
                        continue;
                    }
                    break;
                } if (s.empty()) {
                    cout << "❌ Skill cannot be empty.\n";
                    continue;
                }
                if (find(skill_list.begin(), skill_list.end(), s) != skill_list.end()) {
                    cout << "⚠️  You already entered \"" << s << "\". Please enter a different skill.\n";
                    continue;
                }
                skill_list.push_back(s);
            }

            skills = "";
            for (size_t i = 0; i < skill_list.size(); ++i) {
                skills += skill_list[i];
                if (i < skill_list.size() - 1) skills += ",";
            }
        } else {
            cout << "Company Description: "; getline(cin, doc);
            vector<string> job_list;
            cout << "Enter jobs offered one by one. Type 'cf' to confirm, 'cc' to cancel:\n";
            while (true) {
                string job;
                cout << "Job (or type 'cc' to cancel, 'cf' to confirm): ";
                getline(cin, job);

                if (job == "cc" || job == "cancel") return;

                if (job == "cf" || job == "confirm") {
                    if (job_list.empty()) {
                        cout << "❌ You must enter at least one job before confirming.\n";
                        continue;
                    }
                    break;
                }

                if (job.empty()) {
                    cout << "❌ Job cannot be empty.\n";
                    continue;
                }

                if (find(job_list.begin(), job_list.end(), job) != job_list.end()) {
                    cout << "⚠️  You already entered \"" << job << "\". Please enter a different job.\n";
                    continue;
                }

                job_list.push_back(job);
            }
            skills = "";
            for (size_t i = 0; i < job_list.size(); ++i) {
                skills += job_list[i];
                if (i < job_list.size() - 1) skills += ",";
            }
        }

        
        string confirm,warning="";
        while (true) {
            user temp(uname, pass, name, email, phone, doc, role, skills);
            display_regisInfo(&temp, warning);
            getline(cin, confirm);
            if (confirm == "s" || confirm == "save") break;
            if (confirm == "cc" || confirm == "cancel") {
                cout << "❌ Registration cancelled.\n";
                return;
            }
            if (confirm == "1") {
                while (true) {
                    cout << "New Username (or type 'cc' to cancel): ";
                    getline(cin, uname);
                    if (uname == "cc" || uname == "cancel") break;
                    if (uname.empty()) {
                        cout << "❌ Username cannot be empty.\n";
                        continue;
                    }
                    if (is_username_taken(uname)) {
                        cout << "❌ Username already taken.\n";
                        continue;
                    }
                    break;
                }
            }
            else if (confirm == "2") {
                do {
                    cout << "New Name (or type 'cc' to cancel): ";
                    getline(cin, name);
                    if (name == "cc" || name == "cancel") break;
                    if (name.empty()) cout << "❌ Name cannot be empty.\n";
                } while (name.empty());                
            }
            else if (confirm == "3") {
                do {
                    string new_pass = getPasswordMaskedPreview("Enter new password (or type 'cc' to cancel): ");
                    if (new_pass == "cc" || new_pass == "cancel") break;
                    if (new_pass.empty()) {
                        cout << "❌ Password cannot be empty.\n";
                        continue;
                    }
                    pass = new_pass;
                    break;
                } while (true);
                
            }
            else if (confirm == "4") {
                do {
                    cout << "New Email (or type 'cc' to cancel): ";
                    getline(cin, email);
                    if (email == "cc" || email == "cancel") break;
                    if (email.empty() || email.find('@') == string::npos)
                        cout << "❌ Invalid email. Must contain '@' and not be empty.\n";
                } while (email.empty() || email.find('@') == string::npos);
            }
            else if (confirm == "5") {
                do {
                    cout << "New Phone (or type 'cc' to cancel): ";
                    getline(cin, phone);
                    if (phone == "cc" || phone == "cancel") break;
                    if (phone.empty() || phone.length() != 10 || !all_of(phone.begin(), phone.end(), ::isdigit))
                        cout << "❌ Invalid phone number. Must be 10 digits.\n";
                } while (phone.empty() || phone.length() != 10 || !all_of(phone.begin(), phone.end(), ::isdigit));
            }
            else if (confirm == "6") {
                do {
                    cout << (role == "jobseeker" ? "New Resume (or type 'cc' to cancel): " : "New Description (or type 'cc' to cancel): ");
                    getline(cin, doc);
                    if (doc == "cc" || doc == "cancel") break;
                    if (role == "jobseeker" && (
                        doc.empty() ||
                        (doc.find(".pdf") == string::npos &&
                         doc.find(".jpg") == string::npos &&
                         doc.find(".png") == string::npos &&
                         doc.find(".jpeg") == string::npos &&
                         doc.find(".doc") == string::npos))) {
                        cout << "❌ Invalid or empty file. Must be .pdf, .jpg, .png, .jpeg, or .doc only.\n";
                    } else break;
                } while (true);
            }
            else if (confirm == "7") {
                vector<string> items;
                cout << (role == "company" ? "Enter new jobs offered" : "Enter new skills")
                     << " one by one. Type 'cf' to confirm, 'cc' to cancel:\n";
            
                while (true) {
                    string input;
                    cout << (role == "company" ? "Job: " : "Skill: ");
                    getline(cin, input);
            
                    if (input == "cc" || input == "cancel") break;
            
                    if (input == "cf" || input == "confirm") {
                        if (items.empty()) {
                            cout << "❌ You must enter at least one item before confirming.\n";
                            continue;
                        }
                        // รวมข้อมูลเข้า skills
                        skills = "";
                        for (size_t i = 0; i < items.size(); ++i) {
                            skills += items[i];
                            if (i < items.size() - 1) skills += ",";
                        }
                        break;
                    }
            
                    if (input.empty()) {
                        cout << "❌ This field cannot be empty.\n";
                        continue;
                    }
            
                    if (find(items.begin(), items.end(), input) != items.end()) {
                        cout << "⚠️  \"" << input << "\" is already added. Please enter something new.\n";
                        continue;
                    }
            
                    items.push_back(input);
                }
            }else {
                warning = "❌ Invalid choice. Please try again.";
                continue;
            }
            
        }

        user temp(uname, pass, name, email, phone, doc, role, skills);
        save_user_to_file(userfile, jobfile, companyfile, temp, school, pet, color);        
        cout << "\n✅ Registered successfully as " << role << "!\n";
    }
};
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}
void forget_password(const string& userfile) {
    string uname;
    cout << "\n== 🔐 Forgot Password ==" << endl;
    cout << "Enter your username: ";
    cin >> uname;

    display_security();
    int q_choice;
    cin >> q_choice;
    cin.ignore();

    string answer;
    cout << "Your answer: ";
    getline(cin, answer);

    ifstream infile(userfile);
    vector<string> lines;
    string line;
    bool reset_success = false;

    while (getline(infile, line)) {
        
        istringstream iss(line);
        string u, p, school, pet, color;

        getline(iss, u, '|');       u = trim(u);
        getline(iss, p, '|');       p = trim(p);
        getline(iss, school, '|');  school = trim(school);
        getline(iss, pet, '|');     pet = trim(pet);
        getline(iss, color);        color = trim(color);

        if (u == uname) {
            bool match = false;
            if (q_choice == 1 && answer == school) match = true;
            else if (q_choice == 2 && answer == pet) match = true;
            else if (q_choice == 3 && answer == color) match = true;

            if (match) {
                string new_pass;
                cout << "✅ Verified. Enter new password: ";
                cin >> new_pass;

                ostringstream updated;
                updated << u << "|" << new_pass << "|" << school << "|" << pet << "|" << color;
                lines.push_back(updated.str());
                reset_success = true;
                continue;
            }
        }

        lines.push_back(line);
    }
    infile.close();

    if (reset_success) {
        ofstream outfile(userfile);
        for (const auto& l : lines) {
            outfile << l << '\n';
        }
        outfile.close();
        cout << "🔁 Password reset successfully!\n";
    } else {
        cout << "❌ Verification failed. Cannot reset password.\n";
    }
}



void user_register() {
    user::register_user("user.txt", "jobseeker.txt", "company.txt");
}

user* user_login() {
    string uname, pass;
    cout << "== Login ==" << endl;
    cout << "Username: "; cin >> uname;
    cin.ignore();
    pass = getPasswordMaskedPreview("Password: ");

    ifstream userfile("user.txt");
    string line;
    while (getline(userfile, line)) {
        istringstream iss(line);
        string u, p, school, pet, color;
        getline(iss, u, '|');
        getline(iss, p, '|');
        getline(iss, school, '|');
        getline(iss, pet, '|');
        getline(iss, color);

        if (u == uname && p == pass) {
            userfile.close();
            cout << "\n✅ Login success! Welcome, " << uname << "\n";

            // 👉 ลองหาใน jobseeker
            ifstream job("jobseeker.txt");
            string line_job;
            while (getline(job, line_job)) {
                istringstream jobiss(line_job);
                string ju, email, phone, name, doc, skills;

                getline(jobiss, ju, '|');
                getline(jobiss, email, '|');
                getline(jobiss, phone, '|');
                getline(jobiss, name, '|');
                getline(jobiss, doc, '|');
                getline(jobiss, skills);

                if (ju == uname) {
                    job.close();
                    return new user(ju, p, name, email, phone, doc, "jobseeker", skills);
                }
            }
            job.close();

            // 👉 หรือเป็น company
            ifstream comp("company.txt");
            while (getline(comp, line)) {
                istringstream compiss(line);
                string cu, email, phone, cname, desc, jobs;

                getline(compiss, cu, '|');
                getline(compiss, email, '|');
                getline(compiss, phone, '|');
                getline(compiss, cname, '|');
                getline(compiss, desc, '|');
                getline(compiss, jobs);

                if (cu == uname) {
                    comp.close();
                    return new user(cu, p, cname, email, phone, desc, "company", jobs);
                }
            }
            comp.close();

            // 👉 ถ้าหาใน jobseeker/company ไม่เจอ
            return new user(u, p, "Unknown", "N/A", "N/A", "N/A", "unknown","none");
        }
    }

    cout << "\n❌ Login failed: Invalid username or password.\n";

    char opt;
    cout << "Forgot your password? (y/n): ";
    cin >> opt;
    if (opt == 'y' || opt == 'Y') {
        forget_password("user.txt");
    }

    return nullptr;
}


void jobseeker_dashboard(user* currentUser) {
    int choice;
    do {
        display_jobseeker_dashboard(currentUser);
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            clear_screen();
            currentUser->display();
            cout << "\n[Press Enter to go back to the menu]";
            cin.get();
        }
        else if (choice == 2) {
            editUserInfo(currentUser);
            cin.get();
        }
        else if (choice == 3) {
            
            apply_job(currentUser->getUsername(), "");
            cin.get();
        }
        else if (choice != 4) {
            cout << "Invalid choice.\n";
            cin.get();
        }
        clear_screen();
    } while (choice != 4);
}
/*void company_dashboard(user* currentUser) {
    string use=currentUser->getUsername();
    company_menu(use);
    /*int choice;
    do {
        cout << "\n=== Company Dashboard ===\n";
        cout << "Welcome, " << currentUser->getUsername() << " (" << currentUser->getRole() << ")\n";
        cout << "1. View Company Profile\n";
        cout << "2. Post Job \n";
        cout << "3. View Applicants\n";
        cout << "4. Logout\n";
        cout << "Choose: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            clear_screen();
            currentUser->display();
            cout << "\n[Press Enter to go back to the menu]";
            cin.get();
        }
        else if (choice == 2) {
            
            cin.get();
        }
        else if (choice == 3) {
            
            cin.get();
        }
        else if (choice != 4) {
            cout << "Invalid choice.\n";
            cin.get();
        }
        clear_screen();
    } while (choice != 4);
}*/
#endif

