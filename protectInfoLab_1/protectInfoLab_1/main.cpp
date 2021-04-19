#include <fstream>
#include <iostream>
#include <map>
#include <stdlib.h>
#include <ctime>
#include <vector>
using namespace std;

// all paths
char pathFile[] = "/Users/ortin/Universe/6_semester/protectInfo/protectInfoLab_1/protectInfoLab_1/testFile.doc";
char pathKey[] = "/Users/ortin/Universe/6_semester/protectInfo/protectInfoLab_1/protectInfoLab_1/key.txt";

void mainLoop(void);
void task1(void);
void task2(void);
void task3(void);
void putKey(void);
void getKey(vector<int> *);
void decryption(vector<int> *);
void encryption(vector<int> *);

int main(void) {
    mainLoop();
    return 0;
}

void mainLoop(void) {
    char enter;
    do {
        cout << "Enter number of task or \"e\" to get out: ";
        cin >> enter;
        
        switch(enter) {
            case '1': {
                task1();
                getchar();
                break;
            }
             
            case '2': {
                task2();
                getchar();
                break;
            }
                
            case '3': {
                task3();
                getchar();
                break;
            }
            
            case 'e': {
                puts("Goodbay...\n");
                getchar();
                break;
            }
                
            default: {
                puts("There is no such task...\n");
                getchar();
                break;
            }
        }
    } while(enter != 'e');
}

void task1(void) {
    FILE *file;
    
    if ((file = fopen(pathFile, "r")) == NULL) {
        puts("Could not open file...");
        getchar();
    } else {
        fseek(file, 0, SEEK_END); // set at the end of the file
        long int size = ftell(file); // ftell return position in file
        cout << "The size of testFile.doc is " << size << " bytes\n\nPress any key to continue...\n\n";
        getchar();
        fclose(file);
    }
}
    
void task2(void) {
    ifstream file(pathFile, ios::binary);
    
    map<char, int> bytes;
    char byte;
    
    while(file.get(byte))
        bytes[byte]++;
    
    int a = 0;
    
    for (auto i : bytes)
        cout << ++a  << ": " << i.second << endl;
    
    file.close();
}

void task3(void) {
    vector<int> *key = new vector<int>();
    
    cout << "1. Generate key\n2. Encoding\n3. Decoding\nEnter: ";
    int a;
    cin >> a;
    
    switch(a) {
        case 1: {
            putKey();
            break;
        }
            
        case 2: {
            getKey(key);
            encryption(key);
            break;
        }
            
        case 3: {
            getKey(key);
            decryption(key);
            break;
        }
    }
    
    cout << "\nPress any key to continue...\n\n";
    getchar();
}


void encryption(vector<int> *key) {
    fstream file;
    file.open(pathFile, ios::in);
    
    vector<unsigned char> bytes;
    char byte;
    
    while (file.get(byte))
        bytes.push_back((unsigned char)byte);

    file.close();

    file.open(pathFile, ios::out);
    
    for (unsigned char i : bytes)
        file.put(key->at((int)i));

    file.close();
}

void decryption(vector<int> *key) {
    fstream file;
    file.open(pathFile, ios::in);
    
    vector<unsigned char> encrypted_bytes;
    char byte;
    
    while (file.get(byte))
        encrypted_bytes.push_back((unsigned char)byte);

    file.close();

    file.open(pathFile, ios::out);

    for (unsigned char i : encrypted_bytes)
        for (int j = 0; j < key->size(); j++)
            if (i == key->at(j))
                file.put((char)j);

    file.close();
}

void getKey(vector<int> *key) {
    ifstream file(pathKey, ios::binary);
    char byte;
    
    while (file.get(byte))
        key->push_back((int)(unsigned char)byte);

    file.close();
}

void putKey(void) {
    srand(static_cast<unsigned int>(time(nullptr)));
    
    ofstream file(pathKey, ios::binary);
    int alphabet[256], i, j;

    for (int i = 0; i < 256; i++)
        alphabet[i] = i;

    for (int k = 0; k < 10000; k++) {
        i = rand() % 256;
        j = rand() % 256;
        int temp = alphabet[i];
        alphabet[i] = alphabet[j];
        alphabet[j] = temp;
    }

    for (int i = 0; i < 256; i++)
        file.put((char)alphabet[i]);

    file.close();
}
