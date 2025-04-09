#include "usergamyui.h"
int main() {
    string filename = "user.txt";
    user* currentUser = nullptr;
    int choice;

    do {
        cout << "\n======= MENU =======\n";
        cout << "1. Register\n2. Login\n3. View My Profile\n4. Logout\n0. Exit\nChoose: ";
        cin >> choice;

        if (choice == 1) {
            user::register_user(filename);
        } else if (choice == 2) {
            if (currentUser != nullptr) {
                cout << "⚠️ You're already logged in as " << currentUser->getUsername() << "\n";
            } else {
                string uname, pass;
                cout << "\n=== Login ===\n";
                cout << "Username: "; cin >> uname;
                cout << "Password: "; cin >> pass;
                currentUser = user::login(filename, uname, pass);
            }
        } else if (choice == 3) {
            if (currentUser != nullptr) {
                currentUser->display();
            } else {
                cout << "❗ Please login first to view your profile.\n";
            }
        } else if (choice == 4) {
            if (currentUser != nullptr) {
                cout << "👋 Logged out: " << currentUser->getUsername() << "\n";
                delete currentUser;
                currentUser = nullptr;
            } else {
                cout << "❗ You are not logged in.\n";
            }
        }

    } while (choice != 0);

    // Cleanup
    if (currentUser != nullptr) {
        delete currentUser;
    }

    cout << "\n✅ Program exited.\n";
    return 0;
}