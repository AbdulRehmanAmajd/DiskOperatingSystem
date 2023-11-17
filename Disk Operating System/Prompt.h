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
        int number = 0;
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
        bool readonly = false;
        bool hidden = false;
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
    void create_file_two(string n, directory* D) {
        D->f.push_back(new file(n,D));
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
    int get_file_number(string n, directory* D) {
        auto iter = D->f.begin();
        while (iter != D->f.end()) {
            if ((*iter)->name == n) {
                return (*iter)->number;
            }
            iter++;
        }
        return 0;
    }
    bool find_directory(string n, directory* D) {
        auto iter = D->dir.begin();
        while (iter != D->dir.end()) {
            if ((*iter)->name == n) {
                return true;
            }
            iter++;
        }
        cout << "file is not present in  " << D->name << endl;
        return false;
    }
    directory* get_directory(string n, directory* D) {
        auto iter = D->dir.begin();
        while (iter != D->dir.end()) {
            if ((*iter)->name == n) {
                return *iter;
            }
            iter++;
        }
        cout << "file is not present in  " << D->name << endl;
        return nullptr;
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
    void copy_file(string n,vector<string>store,int number) {
        directory* mycurrent = root;
        for (int i = 0; i < store.size(); i++) {
            if (!find_directory(store[i], mycurrent)) {
                cout << "\n directory not found...\n";
                return;
            }
            mycurrent = get_directory(store[i], mycurrent);
        }
        create_file_two(n + to_string(number + 1), mycurrent);
    }
    void rename_file(string old_name, string new_name) {
        auto iter = current->f.begin();
        while (iter != current->f.end()) {
            if ((*iter)->name == old_name) {
                (*iter)->name = new_name;
                rename(old_name.c_str(), new_name.c_str());
            }
            iter++;
        }
    }
    void help() {
        std::cout << "ATTRIB: Displays or changes file attributes." << std::endl;
        std::cout << "CD: Changes the current directory." << std::endl;
        std::cout << "CD.: Prints the working directory." << std::endl;
        std::cout << "CD..: Changes directory to the previous directory." << std::endl;
        std::cout << "CD\\: Changes directory to the root directory." << std::endl;
        std::cout << "CONVERT: Converts file extensions of specified type." << std::endl;
        std::cout << "COPY: Copies a file to another location." << std::endl;
        std::cout << "CREATE: Creates a file with user-entered contents." << std::endl;
        std::cout << "DEL: Deletes a file." << std::endl;
        std::cout << "DIR: Displays files and subdirectories in a directory." << std::endl;
        std::cout << "EDIT: Opens and allows editing of a file." << std::endl;
        std::cout << "EXIT: Quits the program." << std::endl;
        std::cout << "FIND: Searches for a file in the current directory." << std::endl;
        std::cout << "FINDF: Searches for a text string in the currently open file." << std::endl;
        std::cout << "FINDSTR: Searches for strings in files, prints names of files with the string." << std::endl;
        std::cout << "FORMAT: Empties the current directory and subdirectories." << std::endl;
        std::cout << "HELP: Provides help information for all commands." << std::endl;
        std::cout << "LOADTREE: Loads a tree structure from a file." << std::endl;
        std::cout << "MKDIR: Creates a virtual directory." << std::endl;
        std::cout << "MOVE: Moves a file from one directory to another." << std::endl;
        std::cout << "PPRINT: Adds a text file to the priority-based print queue." << std::endl;
        std::cout << "PROMPT: Changes the command prompt." << std::endl;
        std::cout << "PRINT: Adds a text file to the print queue." << std::endl;
        std::cout << "PQUEUE: Displays the priority-based print queue." << std::endl;
        std::cout << "PWD: Prints the working directory." << std::endl;
        std::cout << "QUEUE: Displays the print queue." << std::endl;
        std::cout << "RENAME: Renames a file." << std::endl;
        std::cout << "RMDIR: Removes a directory and its contents." << std::endl;
        std::cout << "SAVE: Saves the currently open file to disk." << std::endl;
        std::cout << "TREE: Displays the complete directory structure." << std::endl;
        std::cout << "VER: Displays the version of the program." << std::endl;

    }
    void help_save(ofstream&Wtr, directory* currentDirectory, string indent = "") {
        if (currentDirectory != root)
        {
            Wtr << "(" << currentDirectory->parent->name << ")" << currentDirectory->name << endl;
        }
        else {
            Wtr << currentDirectory->name << endl;
        }
        for (const auto& file : currentDirectory->f) {
            Wtr << "(" << currentDirectory->parent->name << ")" <<"+" << file->name << endl;
        }
        for (const auto& subdirectory : currentDirectory->dir) {
            help_save(Wtr,subdirectory, indent + "  ");
        }
    }
    void save() {
        ofstream Wtr("Save.txt");
        help_save(Wtr,root);
    }
    //deefft5
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
            else if (cmd == "ATTRIB") {
                string command;
                cin >> command;
                if (command == "R") {
                    current->readonly = true;
                }
                else if (command == "H") {
                    current->hidden = true;
                }
                else {
                    cout << "\n wrong command...";
                }
            }
            else if (cmd == "copy") {
                string filename;
                cin >> filename;
                if (!find_file(filename, current)) {
                    cout << "file not found...";
                    continue;
                }
                else {
                    int n = get_file_number(filename, current);
                    vector<string>store;
                    string getter;
                    cin >> getter;
                    store.push_back(getter);
                    while (getter[getter.size() - 1] != '-') {
                        cin >> getter;
                        store.push_back(getter);
                    }
                    store[store.size() - 1].pop_back();
                    copy_file(filename, store, n);
                }
            }
            else if (cmd == "rename") {
                string old_name;
                cin >> old_name;
                if (!find_file(old_name, current)) {
                    cout << "\n not found\n";
                }
                else {
                    string new_name;
                    cin >> new_name;
                    rename_file(old_name, new_name);
                }
            }
            else if (cmd == "help"){
                help();
            }
            else if (cmd == "save") {
                save();
            }
            else {
                cout << "\n\t\tNo such command exist...";
            }
        }
    }
};
