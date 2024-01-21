#include <iostream>
#include <string>

#ifdef USE_WIDE_STRINGS
typedef std::wstring MyString;
    #define MY_TEXT(str) L##str
#else
typedef std::string MyString;
#define MY_TEXT(str) str
#endif

class ThirdPartyClass {
public:
    ThirdPartyClass(bool value) {
        std::cout << "Bool constructor called with value: " << value << std::endl;
    }

    ThirdPartyClass(const char *value) {
        std::cout << "Const char* constructor called with value: " << value
                  << std::endl;
    }

    // should have a ctor with const wchar_t* parameter but not
    // ThirdPartyClass(const wchar_t *value) {
    //   std::cout << "const wchar_t* constructor called with value: " << value << std::endl;
    // }
};

void thirdPartyFunction(bool value) {
    std::cout << "Bool function called with value: " << value << std::endl;
}

void thirdPartyFunction(const char *value) {
    std::cout << "Const char* function called with value: " << value << std::endl;
}

// should have a function with const wchar_t* parameter but not
// void thirdPartyFunction(const wchar_t *value) {
//   std::cout << "const wchar_t* function called with value: " << value << std::endl;
// }

int main() {
    ThirdPartyClass instance1(true);
    ThirdPartyClass instance2("Hello, World");

    MyString str = MY_TEXT("Hello, World");
    ThirdPartyClass instance3(str.c_str()); // call ThirdPartyClass(bool value) when use wide strings

    thirdPartyFunction(true);
    thirdPartyFunction("Hello, World");
    thirdPartyFunction(str.c_str());// call thirdPartyFunction(bool value) when use wide strings

    return 0;
}