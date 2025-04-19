#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif
#include <limits>
#include <iostream>
using namespace std;

inline string getPasswordMaskedPreview(const string& prompt = "") {
    cout << prompt;
    string password;
    char ch;

    while (true) {
#ifdef _WIN32
        ch = _getch();
#else
        struct termios oldt, newt;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif

        if (ch == '\n' || ch == '\r') {
            // Enter: แสดงเป็น * แล้วขึ้นบรรทัดใหม่
            cout << "\r" << string(80, ' ') << "\r" << prompt;
            for (size_t i = 0; i < password.length(); ++i) cout << "*";
            cout << endl;
            break;
        } else if ((ch == 127 || ch == '\b')) {
            if (!password.empty()) {
                password.pop_back();
            }
        } else if (!iscntrl(ch)) {
            password += ch;
        }

        // เคลียร์บรรทัดก่อนแสดงใหม่ (ล้างคราบเก่า)
        cout << "\r" << string(80, ' ') << "\r" << prompt;

        if (!password.empty()) {
            for (size_t i = 0; i < password.length() - 1; ++i)
                cout << "*";
            cout << password.back(); // ตัวล่าสุดแสดงจริง
        }

        cout.flush();
    }

    return password;
}
