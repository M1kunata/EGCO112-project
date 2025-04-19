#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif
#include <limits>

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
            // ✅ แสดงทั้งหมดเป็น *
            cout << "\r" << prompt;
            for (size_t i = 0; i < password.length(); ++i) cout << "*";
            cout << endl;
            break;
        } else if (ch == 127 || ch == '\b') {
            if (!password.empty()) {
                password.pop_back();
            }
        } else {
            password += ch;
        }

        // ✅ แสดงผลใหม่: *...*C
        cout << "\r" << prompt;
        if (!password.empty()) {
            for (size_t i = 0; i < password.length() - 1; ++i) cout << "*";
            cout << password.back();
        }
        else {
            // เคลียร์ถ้าลบจนหมด
            cout << " ";
        }

        cout.flush();
    }

    return password;
}

