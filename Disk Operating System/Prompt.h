#pragma once
#include<iostream>
using namespace std;
#include<stdexcept>
#include<vector>
#include<list>
#include<string>
#include<map>
#include<queue>
#include<fstream>


class prompt {
    struct directory;

    struct file {
        std::string name;
        int size=0;
        int id;
        directory* parent;
        file(string _name, directory* _parent = nullptr) {
            name = _name;
            parent = _parent;
        }
    };

    struct directory {
        std::string name;
        std::list<directory*> dir;
        std::list<file*> f;
        directory* parent;
        directory(string _name,directory*_parent=nullptr) {
            name = _name;
            parent = _parent;
        }
    };

    directory* root;
    directory* current;
    void print_directory(directory*D) {
        auto iter = D->dir.begin();
        while (iter != D->dir.end()) {
            cout << "\t\tDir-->\t\t\t\t\t" << (*iter)->name << endl;
            iter++;
        }
        auto fiter = D->f.begin();
        while (fiter != D->f.end()) {
            cout << "\t\tfile-->\t\t\t\t\t"<< (*fiter)->size<<"\t\t" << (*fiter)->name << endl;
            fiter++;
        }
    }
    void change_directory(directory* D,string n) {
        auto iter = D->dir.begin();
        while (iter != D->dir.end()) {
            if ((*iter)->name == n) {
                break;
            }
            iter++;
        }
        if (iter == D->dir.end()) {
            cout << "\nNo such file exist...";
        }
        else {
            current = *iter;
        }
    }
    void create_file(string n) {
        current->f.push_back(new file(n, current));
        std::ofstream outputFile(n);
        if (!outputFile.is_open()) {
            std::cerr << "Error opening file: " << n << std::endl;
            throw exception("Error...");
        }
        outputFile.close();
        std::cout << "File created successfully: " << n << std::endl;
    }
    bool find_file(string n, directory* D) {
        auto iter = D->f.begin();
        while (iter != D->f.end()) {
            if ((*iter)->name == n) {
                cout << "file is present in  " << D->name<<endl;
                return true;
            }
            iter++;
        }
        cout << "file is not present in  " << D->name << endl;
        return false;
    }
    void delete_file(string n, directory* D) {
        auto iter = D->f.begin();
        while (iter != D->f.end()) {
            if ((*iter)->name == n) {
                D->f.erase(iter);
                return;
            }
            iter++;
        }
        cout << "file is not present in  " << D->name << endl;
    }
    void create_directory(string n) {
        current->dir.push_back(new directory(n, current));
    }
    void find_string_in_file(string n,string word) {
        if (!find_file(n, current)) { return; }
        ifstream inputFile(n);
        if (!inputFile.is_open()) {
            std::cerr << "Error opening file: " << std::endl;
        }
        std::vector<char> fileContent((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
        inputFile.close();
        vector<int>index;
        for (int i = 0; i < fileContent.size(); i++) {
            if (word[0] == fileContent[i]) {
                index.push_back(i);
            }
        }
        for (int i = 0; i < index.size(); i++) {
            int num = index[i];
            bool found = true;
            for (int j = 0; j < word.size(); j++) {
                if (num >= fileContent.size()) {
                    return;
                }
                if (word[j] != fileContent[num]) {
                    found = false;
                    break;
                }
                num++;
            }
            if (found == true) {
                cout << "String is present is current file...\n";
                return;
            }
        }
        cout << "String is not present is current file...\n";
    }
public:
    prompt() {
        root = new directory("Dell");
        root->dir.push_back(new directory("C",root));
        root->dir.push_back(new directory("D",root));
        root->dir.push_back(new directory("E",root));
        root->dir.push_back(new directory("F",root));
        current = root;
    }

    void run() {
        while (true) {
            cout <<" " << current->name << ":\\>  ";
            string cmd;
            cin >> cmd;
            if (cmd == "CD.") {
                print_directory(current);
            }
            else if (cmd=="CD") {
                string filename;
                cin >> filename;
                change_directory(current,filename);
            }
            else if (cmd == "CD..") {
                if (current != root)
                {
                    current = current->parent;
                }
            }
            else if (cmd == "CD/") {
                current = root;
            }
            else if (cmd == "create") {
                string filename;
                cin >> filename;
                create_file(filename);
            }
            else if (cmd == "delete") {
                string filename;
                cin >> filename;
                delete_file(filename,current);
            }
            else if (cmd == "find") {
                string filename;
                cin >> filename;
                find_file(filename,current);
            }
            else if (cmd == "findf") {
                string filename;
                cin >> filename;
                string word;
                cin >> word;
                find_string_in_file(filename, word);
            }
            else if (cmd == "MKDIR") {
                string cmd;
                cin >> cmd;
                create_directory(cmd);
            }
            else if (cmd == "VER") {
                cout << "\nVersion 0.0.000.0001\n";
            }
            else {
                cout << "\n\t\tNo such command exist...";
            }
        }
    }
};