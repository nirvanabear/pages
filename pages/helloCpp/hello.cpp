// #include <stdio.h>
#include <iostream>

using namespace std;

int main() {
    string something = "";
    cout << "Say something: " << endl;
    cin >> something;
    cout << "You said: " << something << endl;
    // printf("Hello World\n");
    return 0;
}


// emcc -o helloCpp.html hello.cpp -O3 --shell-file html_template/shell_minimal.html