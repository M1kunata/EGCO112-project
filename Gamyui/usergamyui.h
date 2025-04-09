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

public:
    user() = default; // constructor เปล่า

    user(string u, string p, string n, string e, string ph, string doc, string r) {
        username = u;
        password = p;
        name = n;
        email = e;
        phone = ph;
        document = doc;
        role = r;
    }

    void display() {
        cout << "Username: " << username << "\nName: " << name
             << "\nEmail: " << email << "\nPhone: " << phone
             << "\nDocument: " << document << "\nRole: " << role << endl;
    }

    string getRole() {
        return role;
    }

    string getUsername() {
        return username;
    }

    string getPassword() {
        return password;
    }

    // ฟังก์ชันโหลดจากไฟล์
    static vector<user> load_users_from_file(const string& filename) {
        vector<user> users;
        ifstream infile(filename);
        string line;
        while (getline(infile, line)) {
            istringstream iss(line);
            string u, p, n, e, ph, doc, r;
            if (iss >> u >> p >> n >> e >> ph >> doc >> r) {
                users.emplace_back(u, p, n, e, ph, doc, r);
            }
        }
        return users;
    }

    static void save_user_to_file(const string& filename, const user& new_user) {
        ifstream checkfile(filename);
        bool need_newline = false;
        if (checkfile.peek() != EOF) { // มีข้อมูลในไฟล์
            checkfile.seekg(-1, ios::end);
            char lastChar;
            checkfile.get(lastChar);
            if (lastChar != '\n') {
                need_newline = true;
            }
        }
        checkfile.close();
    
        ofstream outfile(filename, ios::app); // append mode
        if (need_newline) outfile << '\n'; // ⭐ บรรทัดสำคัญ
    
        outfile << new_user.username << " " << new_user.password << " " << new_user.name << " "
                << new_user.email << " " << new_user.phone << " " << new_user.document << " "
                << new_user.role << "\n";
    
        outfile.close();
    }
    

    static user* login(const string& filename, const string& uname, const string& pass) {
        vector<user> users = load_users_from_file(filename);
        for (auto& u : users) {
            if (u.getUsername() == uname && u.getPassword() == pass) {
                cout << "\n✅ Login success! Welcome, " << uname << " (" << u.getRole() << ")\n";
                return new user(u); // คืน object user ที่ login สำเร็จ
            }
        }
        cout << "\n❌ Login failed: Invalid username or password.\n";
        return nullptr;
    }

    static void register_user(const string& filename) {
        string uname, pass, name, email, phone, doc, role;
        cout << "\n=== Register ===\n";
        cout << "Username: "; cin >> uname;
        cout << "Password: "; cin >> pass;
        cout << "Full Name (use _ instead of space): "; cin >> name;
        cout << "Email: "; cin >> email;
        cout << "Phone: "; cin >> phone;
        cout << "Document Filename: "; cin >> doc;
        cout << "Role (jobseeker/company): "; cin >> role;

        user new_user(uname, pass, name, email, phone, doc, role);
        save_user_to_file(filename, new_user);
        cout << "\n✅ Registered successfully!\n";
    }
};
