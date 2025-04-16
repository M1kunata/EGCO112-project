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
    string name = currentUser->getName();
    string email = currentUser->getEmail();
    string phone = currentUser->getPhone();
    string document = currentUser->getDocument();
    string skills = currentUser->getSkills();

    string input;
    bool editing = true;
    string warning = "";

    while (editing) {
        clear_screen();
        user tempUser(username, password, name, email, phone, document, role, skills);
        display_userInfo(&tempUser, warning);
        warning = "";
        getline(cin, input);

        if (input == "cc" || input == "CC" || input == "cancel" || input == "Cancel") {
            char confirm;
            cout << "⚠️  Cancel all edits? (y/n): ";
            cin >> confirm; cin.ignore();
            if (confirm == 'y' || confirm == 'Y') {
                cout << "❎ Edit canceled.\n";
                cout << "🔄 Reverting to original data...\n";
                display_userInfo(currentUser);
                return;
            } else continue;
        }
        if (input == "save" || input == "s" || input == "S" || input == "Save") {
            break;
        }
        if (input == "back" || input == "b" || input == "B" || input == "Back") {
            return;
        }

        if (input == "1") {
            while (true) {
                string temp_input;
                cout << "New Username (or type 'cc' to cancel): ";
                getline(cin, temp_input);
                if (temp_input == "cc" || temp_input == "CC") break;
                if (temp_input == username) cout << "⚠️  Same as old username. Please try again.\n";
                else if (is_username_taken(temp_input)) cout << "❌ Username already taken. Please try another.\n";
                else { username = temp_input; break; }
            }
        } else if (input == "2") {
            string temp_input;
            cout << "New Full Name (or type 'cc' to cancel): ";
            getline(cin, temp_input);
            if (temp_input != "cc" && temp_input != "CC") name = temp_input;
        } else if (input == "3") {
            while (true) {
                string temp_input;
                cout << "New Email (or type 'cc' to cancel): ";
                getline(cin, temp_input);
                if (temp_input == "cc" || temp_input == "CC") break;
                if (temp_input.find('@') == string::npos) cout << "❌ Invalid email. Email must contain '@'. Please try again.\n";
                else { email = temp_input; break; }
            }
        } else if (input == "4") {
            while (true) {
                string temp_input;
                cout << "New Phone (10 digits, or type 'cc' to cancel): ";
                getline(cin, temp_input);
                if (temp_input == "cc" || temp_input == "CC") break;
                bool valid = (temp_input.length() == 10 && all_of(temp_input.begin(), temp_input.end(), ::isdigit));
                if (!valid) cout << "❌ Invalid phone number. Must be 10 digits only.\n";
                else { phone = temp_input; break; }
            }
        } else if (input == "5") {
            string temp_input;
            cout << (role == "company" ? "New Description" : "New Resume") << " (or type 'cc' to cancel): ";
            getline(cin, temp_input);
            if (temp_input != "cc" && temp_input != "CC") {
                if (role == "jobseeker") {
                    if (temp_input.find(".pdf") == string::npos && temp_input.find(".jpg") == string::npos &&
                        temp_input.find(".png") == string::npos && temp_input.find(".jpeg") == string::npos &&
                        temp_input.find(".doc") == string::npos) {
                        cout << "❌ Invalid file format. Please use .pdf, .jpg, .png, .jpeg, or .doc\n";
                    } else document = temp_input;
                } else {
                    document = temp_input;
                }
            }
        } else if (input == "6") {
            string temp_input;
            cout << (role == "company" ? "New Jobs Offered" : "New Skills") << " (or type 'cc' to cancel): ";
            getline(cin, temp_input);
            if (temp_input != "cc" && temp_input != "CC") skills = temp_input;
        } else if (input == "7") {
            while (true) {
                string old_pass;
                cout << "Enter old password (or type 'cc' to cancel): ";
                getline(cin, old_pass);
                if (old_pass == "cc" || old_pass == "CC") break;
                if (old_pass != password) {
                    cout << "❌ Incorrect password. Please try again.\n";
                    continue;
                }
                cout << "Enter new password (or type 'cc' to cancel): ";
                string new_pass;
                getline(cin, new_pass);
                if (new_pass != "cc" && new_pass != "CC") password = new_pass;
                break;
            }
        } else {
            warning = "❌ Invalid choice. Please try again.";
            continue;
        }
    }

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
    ofstream userout("user.txt");
    for (const auto& l : userlines) userout << l << '\n';
    userout.close();

    if (role == "jobseeker") {
        ifstream fin("jobseeker.txt");
        vector<string> lines;
        while (getline(fin, line)) {
            istringstream iss(line);
            string u, n, e, ph, docx;
            string sk;
            if (!(iss >> u >> n >> e >> ph >> docx)) {
                cout << "❌Failed to parse line: " << line << endl;
                lines.push_back(line);
                continue;
            }
            getline(iss, sk);
            size_t start = sk.find_first_not_of(" \t");
            if (start != string::npos) sk = sk.substr(start);
            else sk = "";

            if (u == original_username) {
                ostringstream updated;
                updated << username << " " << name << " " << email << " " << phone << " " << document << " " << skills;
                lines.push_back(updated.str());
            } else lines.push_back(line);
        }
        fin.close();
        ofstream fout("jobseeker.txt");
        for (const auto& l : lines) fout << l << '\n';
        fout.close();
    } else if (role == "company") {
        ifstream fin("company.txt");
        vector<string> lines;
        while (getline(fin, line)) {
            istringstream iss(line);
            string u, n, e, ph, desc, jobs;
            if (!(iss >> u >> n >> e >> ph >> desc >> jobs)) {
                cout << "❌Failed to parse line: " << line << endl;
                lines.push_back(line);
                continue;
            }
            if (u == original_username) {
                ostringstream updated;
                updated << username << " " << name << " " << email << " " << phone << " " << document << " " << skills;
                lines.push_back(updated.str());
            } else lines.push_back(line);
        }
        fin.close();
        ofstream fout("company.txt");
        for (const auto& l : lines) fout << l << '\n';
        fout.close();
    }

    currentUser->setUsername(username);
    currentUser->setPassword(password);
    currentUser->setName(name);
    currentUser->setEmail(email);
    currentUser->setPhone(phone);
    currentUser->setDocument(document);
    currentUser->setSkills(skills);

    cout << "\n[SUCCESS] User info updated successfully!\n";
}



#endif
