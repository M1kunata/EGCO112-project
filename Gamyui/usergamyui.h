#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

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
                    << new_user.document << " " << new_user.skills << "\n";
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
                    return new user(u, p, "N/A", "N/A", "N/A", "N/A", "unknown");
                }
            }
        }
        cout << "\n❌ Login failed: Invalid username or password.\n";
        return nullptr;
    }

    static void register_user(const string& userfile, const string& jobfile, const string& companyfile) {
        string uname, pass, name, email, phone, doc, role, skills;
        string school, pet, color;
        int role_choice;
    
        cout << "\n== Register ==" << endl;
        cout << "1. Job Seeker" << endl;
        cout << "2. Company" << endl;
        cout << "Choose role: ";
        cin >> role_choice;
    
        if (role_choice == 1) {
            role = "jobseeker";
        } else if (role_choice == 2) {
            role = "company";
        } else {
            cout << "❌ Invalid role. Registration cancelled.\n";
            return;
        }
    
        cout << "\nUsername: "; cin >> uname;
        cout << "Password: "; cin >> pass;
        if (role == "company") {
            cout << "Company Name: "; cin >> name;
        } else {
            cout << "Full Name: "; cin >> name;
        }
        cout << "Email: "; cin >> email;
        cout << "Phone: "; cin >> phone;
        cout << "Document Filename: "; cin >> doc;
    
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
    
        cout << "\n✅ Registered successfully as " << role << "!\n";
    }    
};
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

                // ลองหาข้อมูลใน jobseeker
                ifstream job("jobseeker.txt");
                while (getline(job, line)) {
                    istringstream jobiss(line);
                    string ju, email, phone, doc, skills;
                    if (jobiss >> ju >> email >> phone >> doc >> skills && ju == uname) {
                        job.close();
                        return new user(ju, p, ju, email, phone, doc, "jobseeker", skills);
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
    return nullptr;
}
void user_dashboard(user* currentUser) {
    int choice;
    do {
        cout << "\n=== User Dashboard ===\n";
        cout << "Welcome, " << currentUser->getUsername() << " (" << currentUser->getRole() << ")\n";
        cout << "1. View My Profile\n";
        cout << "2. Edit My Info (ยังไม่ทำ)\n";
        cout << "3. Logout\n";
        cout << "Choose: ";
        cin >> choice;
        cin.ignore(); // เคลียร์ '\n'

        if (choice == 1) {
            system("clear");
            currentUser->display();
            cout << "\n[Press Enter to go back to the menu]";
            cin.get(); // รอกด Enter ก่อนกลับ
        }
        else if (choice == 2) {
            cout << "🛠 ฟังก์ชันแก้ไขข้อมูลอยู่ระหว่างพัฒนา\n";
            cout << "[Press Enter to return to the menu]";
            cin.get();
        }
        else if (choice != 3) {
            cout << "Invalid choice.\n";
            cout << "[Press Enter to return to the menu]";
            cin.get();
        }
        system("clear");
    } while (choice != 3);
}

