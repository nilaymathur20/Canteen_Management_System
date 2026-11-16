#ifndef HELPER_H
#define HELPER_H

#include <iostream>
#include <string>
#include <cctype>
#include <cstdlib>
using namespace std;

// Text ko small letters mein convert karta hai
inline string toLower(string s) {
    for (int i = 0; i < s.length(); i++) {
        s[i] = tolower((unsigned char)s[i]);
    }
    return s;
}

// First letter capital karta hai
inline string capitalize(string s) {
    s = toLower(s);

    if (s.length() > 0) {
        s[0] = toupper((unsigned char)s[0]);
    }

    return s;
}

// User se text input lena
inline string readText(string message) {
    string s;

    cout << message;
    getline(cin, s);

    return s;
}

// User se number input lena
inline double readNumber(string message) {
    double n;

    cout << message;

    if (!(cin >> n)) {
        if (cin.eof()) exit(0);   // input band ho gaya to program band
        cin.clear();
        cin.ignore(1000, '\n');
        return -1;
    }

    cin.ignore(1000, '\n');
    return n;
}

#endif