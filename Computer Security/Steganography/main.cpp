//
//  main.cpp
//  Steganography
//
//  Created by 피수영 on 2021/03/15.
//

#include <iostream>
#include <fstream>
#include "bitmap.h"

using namespace std;

const string bmp_path = "origin.bmp";
const string test_path = "ca.txt";

void read_file() {
    ifstream file("test.txt");
    string s;
    
    if(file.is_open()) {
        file >> s;
        cout << "입력받은 문자열 : " << s << '\n';
    } else
        cout << "파일을 읽을 수 없습니다\n";
}

int main(int argc, const char * argv[]) {
    // insert code here...
    read_file();
    
    return 0;
}
