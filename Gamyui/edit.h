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
        if (line.empty()) continue;
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
        cout << flush;
        warning = "";
        getline(cin, input);

        if (input == "save" || input == "s" || input == "S" || input == "Save") {
            if (username == original_username && password == currentUser->getPassword() &&
                name == currentUser->getName() && email == currentUser->getEmail() &&
                phone == currentUser->getPhone() && document == currentUser->getDocument() &&
                skills == currentUser->getSkills()) {
                warning = "⚠️ No changes made. Nothing to save.";
                continue;
            }
            break;
        }

        if (input == "back" || input == "b" || input == "B" || input == "Back") {
            cout<<"[Enter to comfirm]";
            return;
        }

        if (input == "1") {
            warning="❌cannot edit";
    
        } else if (input == "2") {
            string temp_input;
            cout << "New Full Name (or type 'cc' to cancel): ";
            getline(cin, temp_input);
            if (temp_input != "cc" && temp_input != "CC") name = temp_input;
        } else if (input == "3") {
            while (true) {
                string old_pass = getPasswordMaskedPreview("Enter old password (or type 'cc' to cancel): ");
                if (old_pass == "cc" || old_pass == "CC") break;
                if (old_pass != password) {
                    cout << "❌ Incorrect password. Please try again.\n";
                    continue;
                }
                string new_pass = getPasswordMaskedPreview("Enter new password (or type 'cc' to cancel): ");
                    password = new_pass;
                
                break;}
            }
         else if (input == "4") {
            while (true) {
                string temp_input;
                cout << "New Email (or type 'cc' to cancel): ";
                getline(cin, temp_input);
                if (temp_input == "cc" || temp_input == "CC") break;
                if (temp_input.find('@') == string::npos) {
                    cout << "❌ Invalid email. Email must contain '@'. Please try again.\n";
                } else { email = temp_input; break; }
            }
        } else if (input == "5") {
            while (true) {
                string temp_input;
                cout << "New Phone (10 digits, or type 'cc' to cancel): ";
                getline(cin, temp_input);
                bool valid = (temp_input.length() == 10 && all_of(temp_input.begin(), temp_input.end(), ::isdigit));
                if (temp_input == "cc" || temp_input == "CC") break;
                else { phone = temp_input; break; }
            }
        } else if (input == "6") {
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
        } else if (input == "7") {
            vector<string> items;
            while (true) {
                string input_item;
                cout << (role == "company" ? "Enter new jobs offered (or type 'cc' to cancel, 'cf' to confirm): " : "Enter new skills (or type 'cc' to cancel, 'cf' to confirm): ");
                getline(cin, input_item);
                if (input_item == "cc" || input_item == "cancel") break;
                if (input_item == "cf" || input_item == "confirm") {
                    if (items.empty()) {
                        cout << "❌ You must enter at least one item before confirming.\n";
                        continue;
                    }
                    skills = "";
                    for (size_t i = 0; i < items.size(); ++i) {
                        skills += items[i];
                        if (i < items.size() - 1) skills += ",";
                    }
                    break;
                }
                if (input_item.empty()) {
                    cout << "❌ This field cannot be empty.\n";
                    continue;
                }
                if (find(items.begin(), items.end(), input_item) != items.end()) {
                    cout << "⚠️  \"" << input_item << "\" is already added.\n";
                    continue;
                }
                items.push_back(input_item);
            }
        } else {
            warning = "❌ Invalid choice. Please try again.";
            continue;
        }
    }

    // Save user.txt
    ifstream userfile("user.txt");
    vector<string> userlines;
    string line;
    while (getline(userfile, line)) {
        stringstream ss(line);
        string u, p, s, pet, color;

        if (!getline(ss, u, '|') || !getline(ss, p, '|') || !getline(ss, s, '|') ||
            !getline(ss, pet, '|') || !getline(ss, color)) {
            cout << "❌Failed to parse user line: " << line << endl;
            userlines.push_back(line); // หรือข้ามบรรทัดนี้ก็ได้
            continue;
        }

        if (u == original_username) {
            ostringstream updated;
            updated << username << "|" << password << "|" << s << "|" << pet << "|" << color;
            userlines.push_back(updated.str());
        } else {
            userlines.push_back(line);
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
            stringstream ss(line);
            string u, e, ph, n, docx, sk;

            if (!getline(ss, u, '|') || !getline(ss, e, '|') || !getline(ss, ph, '|') ||
                !getline(ss, n, '|') || !getline(ss, docx, '|') || !getline(ss, sk)) {
                cout << "❌Failed to parse line: " << line << endl;
                lines.push_back(line);
                continue;
            }

            if (u == original_username) {
                ostringstream updated;
                updated << username << "|" << email << "|" << phone << "|" << name << "|" << document << "|" << skills;
                lines.push_back(updated.str());
            } else {
                lines.push_back(line);
            }
        }
        fin.close();
        ofstream fout("jobseeker.txt");
        for (const auto& l : userlines) userout << l << '\n';

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

    cout << "\n✅ [SUCCESS] User info updated successfully!\n";
}




#endif
