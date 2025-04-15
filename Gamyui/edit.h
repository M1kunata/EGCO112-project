#ifndef EDIT_H
#define EDIT_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "usergamyui.h"

using namespace std;

inline bool is_username_taken(const string& uname) {
    ifstream infile("user.txt");
    string line;
    while (getline(infile, line)) {
        istringstream iss(line);
        string u, p, s, pet, color;
        if (iss >> u >> p >> s >> pet >> color && u == uname) {
            return true;
        }
    }
    return false;
}

inline void editUserInfo(user* currentUser) {
    if (!currentUser) {
        cout << "\n❌ No user logged in.\n";
        return;
    }

    string role = currentUser->getRole();
    string original_username = currentUser->getUsername();
    string username = original_username;
    string password = currentUser->getPassword();
    string email = currentUser->getEmail();
    string phone = currentUser->getPhone();
    string document = currentUser->getDocument();
    string skills = currentUser->getSkills();

    string input;
    bool editing = true;

    while (editing) {
        user tempUser(username, password, username, email, phone, document, role, skills);
        display_userInfo(&tempUser);

        cout << "🔧 Enter the number to edit, or type"<<endl<<"   s or save  - to confirm all changes"<<endl<< "   b or back  - to return to menu without saving"<<endl<< "   cc or cancel - to cancel all edits"<<endl<< "Your choice: ";
        getline(cin, input);

        if (input == "cc" || input == "CC" || input == "cancel" || input == "Cancel") {
            char confirm;
            cout << "⚠️  Cancel all edits? (y/n): ";
            cin >> confirm; cin.ignore();
            if (confirm == 'y' || confirm == 'Y') {
                cout << "❎ Edit canceled.\n";
                cout << "🔄 Reverting to original data...\n";
                display_userInfo(currentUser);
                username = original_username;
                password = currentUser->getPassword();
                email = currentUser->getEmail();
                phone = currentUser->getPhone();
                document = currentUser->getDocument();
                skills = currentUser->getSkills();
                continue;
            } else continue;
        }
        if (input == "save" || input == "s" || input == "S"|| input == "Save") {
            break; // ไปบันทึก
        }
        if (input == "back" || input == "b" || input == "B"|| input == "Back") {
            return;
        }

        if (input == "1") {
            while (true) {
                string new_uname;
                cout << "New Username (or type 'cc' to cancel): ";
                getline(cin, new_uname);
                if (new_uname == "cc" || new_uname == "CC") break;
                if (new_uname == username) cout << "⚠️  Same as old username. Try again.\n";
                else if (is_username_taken(new_uname)) cout << "❌ Username taken. Try again.\n";
                else { username = new_uname; break; }
            }
        } else if (input == "2") {
            while (true) {
                string old_pass;
                cout << "Enter old password (or type 'cc' to cancel): ";
                getline(cin, old_pass);
                if (old_pass == "cc" || old_pass == "CC") break;
                if (old_pass != password) {
                    cout << "❌ Incorrect password. Try again.\n";
                    continue;
                }
                cout << "Enter new password (or type 'cc' to cancel): ";
                string new_pass;
                getline(cin, new_pass);
                if (new_pass != "cc" && new_pass != "CC") password = new_pass;
                break;
            }
        } else if (input == "3") {
            do {
                cout << "New Email (or type 'cc' to cancel): ";
                getline(cin, email);
                if (email == "cc" || email == "CC") break;
                if (email.find('@') == string::npos) cout << "❌ Invalid email. Try again.\n";
                else break;
            } while (true);
        } else if (input == "4") {
            do {
                cout << "New Phone (10 digits, or type 'cc' to cancel all edits): ";
                getline(cin, phone);
                if (phone == "cc" || phone == "CC") break;
                bool valid = (phone.length() == 10 && all_of(phone.begin(), phone.end(), ::isdigit));
                if (!valid) cout << "❌ Invalid phone number. Try again.\n";
                else break;
            } while (true);
        } else if (input == "5") {
            do {
                cout << "New Document Filename (or type 'cc' to cancel): ";
                getline(cin, document);
                if (document == "cc" || document == "CC") break;
                if (document.find(".pdf") == string::npos && document.find(".jpg") == string::npos &&
                    document.find(".png") == string::npos && document.find(".jpeg") == string::npos &&
                    document.find(".doc") == string::npos) {
                    cout << "❌ Invalid file format. Try again.\n";
                } else break;
            } while (true);
        } else if (input == "6" && role == "jobseeker") {
            cout << "New Skills (or type 'cc' to cancel): ";
            string s;
            getline(cin, s);
            if (s != "cc" && s != "CC") skills = s;
        } else {
            cout << "❌ Invalid choice.\n";
            continue;
        }
    }

    // Save to user.txt
    ifstream userfile("user.txt");
    vector<string> userlines;
    string line;
    while (getline(userfile, line)) {
        istringstream iss(line);
        string u, p, s, pet, color;
        if (iss >> u >> p >> s >> pet >> color) {
            if (u == original_username) {
                ostringstream updated;
                updated << username << " " << password << " " << s << " " << pet << " " << color;
                userlines.push_back(updated.str());
            } else {
                userlines.push_back(line);
            }
        }
    }
    userfile.close();
    ofstream uout("user.txt");
    for (auto& l : userlines) uout << l << "\n";

    // Save to jobseeker/company
    string filename = (role == "jobseeker") ? "jobseeker.txt" : "company.txt";
    ifstream infile(filename);
    vector<string> lines;
    while (getline(infile, line)) {
        istringstream iss(line);
        string u;
        iss >> u;
        if (u == original_username) {
            ostringstream updated;
            if (role == "jobseeker") {
                updated << username << " " << email << " " << phone << " " << document << " " << skills;
            } else {
                string old_email, old_phone, name, desc, jobs;
                iss >> old_email >> old_phone >> name >> desc >> jobs;
                updated << username << " " << email << " " << phone << " " << name << " " << document << " " << jobs;
            }
            lines.push_back(updated.str());
        } else {
            lines.push_back(line);
        }
    }
    infile.close();
    ofstream outfile(filename);
    for (const string& l : lines) outfile << l << '\n';
    currentUser->setUsername(username);
    currentUser->setPassword(password);
    currentUser->setEmail(email);
    currentUser->setPhone(phone);
    currentUser->setDocument(document);
    if (role == "jobseeker") {
        currentUser->setSkills(skills);
    }

    cout << "\n✅ User info updated successfully!\n";
}

#endif
