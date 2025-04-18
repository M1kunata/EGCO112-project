#ifndef USERGAMYUI_H
#define USERGAMYUI_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm> 
#include <cctype>
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

    user(string u, string p, string n, string e, string ph, string doc, string r, string sk = "")
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

    static void save_user_to_file(const string& userfile, const string& jobfile, const string& companyfile, const user& new_user) {
        // เขียน user.txt
        ofstream ufile(userfile, ios::app);
        ufile << new_user.username << " " << new_user.password << " DummySchool DummyPet DummyColor\n";
        ufile.close();

        // เขียน jobseeker หรือ company
        ofstream outfile;
        if (new_user.role == "jobseeker") {
            outfile.open(jobfile, ios::app);
            outfile << new_user.username << " " << new_user.email << " " << new_user.phone << " "
                    << new_user.name << " " << new_user.document << " " << new_user.skills << "\n";
        
        } else if (new_user.role == "company") {
            outfile.open(companyfile, ios::app);
            outfile << new_user.username << " " << new_user.email << " " << new_user.phone << " "
                    << new_user.name << " " << new_user.document << "\n"; // ใช้ name เป็นชื่อบริษัท
        }
        outfile.close();
    }

    static user* login(const string& filename, const string& uname, const string& pass) {
        ifstream infile(filename);
        string line;
        while (getline(infile, line)) {
            istringstream iss(line);
            string u, p, school, pet, color;
            if (iss >> u >> p >> school >> pet >> color) {
                if (u == uname && p == pass) {
                    cout << "\n✅ Login success! Welcome, " << uname << "\n";
                    infile.close();
    
                    // jobseeker
                    ifstream job("jobseeker.txt");
                    while (getline(job, line)) {
                        istringstream jobiss(line);
                        string ju, email, phone, name, doc, skills;
                        if (jobiss >> ju >> email >> phone >> name >> doc >> skills && ju == uname) {
                            job.close();
                            return new user(ju, p, name, email, phone, doc, "jobseeker", skills);
                        }
                    }
                    job.close();
    
                    // company
                    ifstream comp("company.txt");
                    while (getline(comp, line)) {
                        istringstream compiss(line);
                        string cu, email, phone, compname, compdesc, jobs;
                        if (compiss >> cu >> email >> phone >> compname >> compdesc >> jobs && cu == uname) {
                            comp.close();
                            return new user(cu, p, compname, email, phone, compdesc, "company", jobs);
                        }
                    }
                    comp.close();
                }
            }
        }
    
        cout << "\n❌ Login failed: Invalid username or password.\n";
    
        // forgot password
        char opt;
        cout << "Forgot your password? (y/n): ";
        cin >> opt;
        if (opt == 'y' || opt == 'Y') {
            forget_password("user.txt");
        }
    
        return nullptr;
    }
    

    static void register_user(const string& userfile, const string& jobfile, const string& companyfile) {
        string uname, pass, name, email, phone, doc, role, skills;
        string school, pet, color;
        string role_input;
        display_register();
        getline(cin, role_input);

        if (role_input == "1") {
            role = "jobseeker";
        } else if (role_input == "2") {
            role = "company";
        } else if (role_input == "3") {
            cout << "↩️  Back to main menu.\n";
            return;
        } else {
            cout << "❌ Invalid choice. Registration cancelled.\n";
            return;
        }
    
        cout << "\nUsername: "; cin >> uname;
        cout << "Password: "; cin >> pass;
        if (role == "company") {
            cout << "Company Name: "; cin >> name;
        } else {
            cout << "Full Name: "; cin >> name;
        }
        do {
            cout << "Email: ";
            cin >> email;
        
            if (email.find('@') == string::npos) {
                cout << "❌ Invalid email. Email must contain '@'. Please try again.\n";
            }
        
        } while (email.find('@') == string::npos);
        
        do {
            cout << "Phone: ";
            cin >> phone;
        
            bool valid = (phone.length() == 10);
        
            for (char c : phone) {
                if (!isdigit(c)) {
                    valid = false;
                    break;
                }
            }
        
            if (!valid) {
                cout << "❌ Invalid phone number. Must be 10 digits only.\n";
            }
        
        } while (
            phone.length() != 10 || 
            !all_of(phone.begin(), phone.end(), ::isdigit)
        );

    
    do {
        cout << "Document Filename: ";
        cin >> doc;

        if (
        doc.find(".pdf") == string::npos &&
        doc.find(".jpg") == string::npos &&
        doc.find(".png") == string::npos &&
        doc.find(".jpeg") == string::npos &&
        doc.find(".doc") == string::npos
    ) {
        cout << "❌ Invalid file format. Please use .pdf, .jpg, .png, .jpeg, or .doc\n";
    }

    } while (
        doc.find(".pdf") == string::npos &&
        doc.find(".jpg") == string::npos &&
        doc.find(".png") == string::npos &&
        doc.find(".jpeg") == string::npos &&
        doc.find(".doc") == string::npos
    );

    
        //  Security questions (ทุกคนกรอก)
        cout << "School you graduated from: "; cin >> school;
        cout << "First pet's name: "; cin >> pet;
        cout << "Favorite color: "; cin >> color;
    
        if (role == "jobseeker") {
            cout << "Skills (comma separated): "; cin >> skills;
        }
    
        // บันทึก user.txt
        ofstream ufile(userfile, ios::app);
        ufile << uname << " " << pass << " " << school << " " << pet << " " << color << "\n";
        ufile.close();
    
        if (role == "company") {
            string company_desc, jobs;
            cout << "Company Description (ใช้ _ แทนช่องว่าง): "; cin >> company_desc;
            cout << "Jobs Offered (comma-separated): "; cin >> jobs;
        
            ofstream comp(companyfile, ios::app);
            comp << uname << " " << email << " " << phone << " "
                 << name << " " << company_desc << " " << jobs << "\n";
            comp.close();
        }
        else {
            ofstream job(jobfile, ios::app);
            job << uname << " " << name << " " << email << " " << phone << " "
            << doc << " " << skills << "\n";
            job.close();
        }
    
        cout << "\n✅ Registered successfully as " << role << "!\n";
    }    
};
void forget_password(const string& userfile) {
    string uname;
    cout << "\n== 🔐 Forgot Password ==" << endl;
    cout << "Enter your username: ";
    cin >> uname;

    display_security();
    int q_choice;
    cin >> q_choice;

    string answer;
    cout << "Your answer: ";
    cin >> answer;

    ifstream infile(userfile);
    vector<string> lines;
    string line;
    bool reset_success = false;

    while (getline(infile, line)) {
        istringstream iss(line);
        string u, p, s, pet, color;
        if (iss >> u >> p >> s >> pet >> color) {
            if (u == uname) {
                bool match = false;
                if (q_choice == 1 && answer == s) match = true;
                else if (q_choice == 2 && answer == pet) match = true;
                else if (q_choice == 3 && answer == color) match = true;

                if (match) {
                    string new_pass;
                    cout << "✅ Verified. Enter new password: ";
                    cin >> new_pass;

                    ostringstream updated;
                    updated << u << " " << new_pass << " " << s << " " << pet << " " << color;
                    lines.push_back(updated.str());
                    reset_success = true;
                    continue;
                }
            }
        }
        lines.push_back(line); // keep เดิม
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
    cout << "Password: "; cin >> pass;

    ifstream userfile("user.txt");
    string line;
    while (getline(userfile, line)) {
        istringstream iss(line);
        string u, p, school, pet, color;
        if (iss >> u >> p >> school >> pet >> color) {
            if (u == uname && p == pass) {
                cout << "\n✅ Login success! Welcome, " << uname << "\n";
                userfile.close();
                // ลองหาข้อมูลใน jobseeker
                ifstream job("jobseeker.txt");
                while (getline(job, line)) {
                    istringstream jobiss(line);
                    string ju, email, phone, name, doc, skills;
                    if (jobiss >> ju >> email >> phone >> name >> doc >> skills && ju == uname) {
                        job.close();
                        return new user(ju, p, name, email, phone, doc, "jobseeker", skills);
                    }
                }
                job.close(); 

                // หรือ company
                ifstream comp("company.txt");
                    while (getline(comp, line)) {
                        istringstream compiss(line);
                        string cu, email, phone, compname, compdesc, jobs;
                         if (compiss >> cu >> email >> phone >> compname >> compdesc >> jobs && cu == uname) {
                        comp.close();
                        return new user(cu, p, compname, email, phone, compdesc, "company", jobs);
                    }
                }
                comp.close();
            }
        }
    }

    cout << "\n❌ Login failed: Invalid username or password.\n";


    // เสนอลืมรหัสผ่าน
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

