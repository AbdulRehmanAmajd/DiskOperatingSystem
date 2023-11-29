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
#include<conio.h>
#include<unordered_map>
#include<thread>
#include<chrono>
#include <windows.h>
#include <sstream>

void setclr(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}
class prompt {
    struct directory;

    struct line {
        deque<char>c;
        int max_size = 100;
        bool isfull() {
            if (c.size() - 1 == max_size) {
                return true;
            }
            return false;
        }
        int my_size() {
            return c.size();
        }
    };
    struct document {
        vector<line>Vs;
        int ri = 0, ci = 0;
    };

    struct file {
        std::string name;
        int size = 0;
        int id;
        int number = 0;
        directory* parent;
        document* Ds;
        string password = "";
        file(string _name, directory* _parent = nullptr, int _n = 0) {
            name = _name;
            parent = _parent;
            Ds = new document();
        }
        friend bool operator>(const file& lhs, const file& rhs) {
            return lhs.number > rhs.number;
        }
    };
    queue<directory*> Q;
    priority_queue<file*>files;
    struct directory {
        std::string name;
        std::list<directory*> dir;
        std::list<file*> f;
        directory* parent;
        bool readonly = false;
        bool hidden = false;
        directory(string _name, directory* _parent = nullptr) {
            name = _name;
            parent = _parent;
        }
    };

    directory* root;
    directory* current;
    void print_directory(directory* D) {
        auto iter = D->dir.begin();
        while (iter != D->dir.end()) {
            cout << "\t\tDir-->\t\t\t\t\t" << (*iter)->name << endl;
            iter++;
        }
        if (D->dir.empty())
        {
            cout << "The current DIR is empty" << endl;
        }
        auto fiter = D->f.begin();
        while (fiter != D->f.end()) {
            cout << "\t\tfile-->\t\t\t\t\t" << (*fiter)->size << "\t\t" << (*fiter)->name << endl;
            fiter++;
        }
    }
    void change_directory(directory* D, string n) {
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
        D->f.push_back(new file(n, D));
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
                cout << "file is present in  " << D->name << endl;
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
    bool find_string_in_file(string n, string word) {
        if (!find_file(n, current)) { return false; }
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
                    return false;
                }
                if (word[j] != fileContent[num]) {
                    found = false;
                    break;
                }
                num++;
            }
            if (found == true) {
                cout << "String is present is current file...\n";
                return true;
            }
        }
        cout << "String is not present is current file...\n";
        return false;
    }
    void copy_file(string n, vector<string>store, int number) {
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
    void help_save(ofstream& Wtr, directory* currentDirectory, string indent = "") {
        if (currentDirectory != root) {
            Wtr << "(" << currentDirectory->parent->name << ")" << currentDirectory->name << endl;
        }
        else {
            Wtr << currentDirectory->name << endl;
        }

        for (const auto& file : currentDirectory->f) {
            Wtr << "(" << currentDirectory->name << ")" << "+" << file->name << endl;
        }

        for (const auto& subdirectory : currentDirectory->dir) {
            help_save(Wtr, subdirectory, indent + "  ");
        }
    }
    void save() {
        ofstream Wtr("Save.txt");
        help_save(Wtr, root);
        Wtr.close();
    }
    void printDirectoryStructure(directory* currentDirectory, int depth = 0) {
        for (int i = 0; i < depth; ++i) {
            cout << "  ";
        }

        cout << "|-- " << currentDirectory->name << endl;

        for (const auto& subdirectory : currentDirectory->dir) {
            printDirectoryStructure(subdirectory, depth + 1);
        }

        for (const auto& file : currentDirectory->f) {
            for (int i = 0; i < depth + 1; ++i) {
                cout << "  ";
            }

            cout << "|-- " << file->name << endl;
        }
    }
    void file_storer(directory* currentDirectory, vector<string>& store, string word) {
        for (const auto& file : currentDirectory->f) {
            if (find_string_in_file(file->name, word)) {
                store.push_back(file->name);
            }
        }
        for (const auto& subdirectory : currentDirectory->dir) {
            file_storer(subdirectory, store, word);
        }
    }
    vector<string>find_str_in_all_files(string word) {
        vector<string>store;
        file_storer(root, store, word);
        return store;
    }
    file* findFile(const std::string& fileName, directory* currentDirectory) {
        if (currentDirectory == nullptr) {
            return nullptr;
        }

        for (file* f : currentDirectory->f) {
            if (f->name == fileName) {
                return f;
            }
        }
        for (directory* subdir : currentDirectory->dir) {
            file* foundFile = findFile(fileName, subdir);
            if (foundFile != nullptr) {
                return foundFile;
            }
        }
        return nullptr;
    }
    void display_file(file* F) {
        for (int i = 0; i < F->Ds->Vs.size(); i++) {
            for (int j = 0; j < F->Ds->Vs[i].my_size(); j++) {
                if (i == F->Ds->ri && j == F->Ds->ci) {
                    setclr(FOREGROUND_GREEN);
                    cout << "|";
                    setclr(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                }
                else {
                    setclr(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                }
                cout << F->Ds->Vs[i].c[j];
            }
            cout << endl;
        }
    }
    void enter_text_before(file* F, string text) {
        int r = F->Ds->ri;
        int c = F->Ds->ci;
        if (text == " /p") {
            F->Ds->Vs.push_back(line());
            F->Ds->ci = 0;
            F->Ds->ri = F->Ds->Vs.size() - 1;
            return;
        }
        if (F->Ds->Vs.size() <= r) {
            F->Ds->Vs.push_back(line());
        }
        auto iter = F->Ds->Vs[r].c.begin();
        iter = next(iter, F->Ds->ci);
        for (int i = text.size() - 1, j = 0; i >= 0; i--, j++)
        {
            iter = F->Ds->Vs[r].c.insert(iter, text[i]);
        }
        while (F->Ds->Vs[r].c.size() > 100) {
            r++;
            if (F->Ds->Vs.size() <= r) {
                F->Ds->Vs.push_back(line());
                F->Ds->Vs[r].c.insert(
                    F->Ds->Vs[r].c.begin(),
                    std::make_move_iterator(F->Ds->Vs[r - 1].c.begin() + 100),
                    std::make_move_iterator(F->Ds->Vs[r - 1].c.end())
                );
                F->Ds->Vs[r - 1].c.erase(
                    F->Ds->Vs[r - 1].c.begin() + 100,
                    F->Ds->Vs[r - 1].c.end()
                );
            }
            else {
                F->Ds->Vs[r].c.insert(
                    F->Ds->Vs[r].c.begin(),
                    std::make_move_iterator(F->Ds->Vs[r - 1].c.begin() + 100),
                    std::make_move_iterator(F->Ds->Vs[r - 1].c.end())
                );
                F->Ds->Vs[r - 1].c.erase(
                    F->Ds->Vs[r - 1].c.begin() + 100,
                    F->Ds->Vs[r - 1].c.end()
                );
            }
        }
        F->Ds->ci += text.size();
        while (F->Ds->ci > 99) {
            F->Ds->ri += 1;
            int cc = F->Ds->ci - 99;
            F->Ds->ci = cc;
        }
    }
    void enter_text(file* F, string text) {
        int r = F->Ds->ri;
        int c = F->Ds->ci;
        if (text == " /p") {
            F->Ds->Vs.push_back(line());
            F->Ds->ci = 0;
            F->Ds->ri = F->Ds->Vs.size() - 1;
            return;
        }
        if (F->Ds->Vs.size() <= r) {
            F->Ds->Vs.push_back(line());
        }
        auto iter = F->Ds->Vs[r].c.begin();
        iter = next(iter, F->Ds->ci);
        for (int i = text.size() - 1, j = 0; i >= 0; i--, j++)
        {
            iter = F->Ds->Vs[r].c.insert(iter, text[i]);
        }
        while (F->Ds->Vs[r].c.size() > 100) {
            r++;
            if (F->Ds->Vs.size() <= r) {
                F->Ds->Vs.push_back(line());
                F->Ds->Vs[r].c.insert(
                    F->Ds->Vs[r].c.begin(),
                    std::make_move_iterator(F->Ds->Vs[r - 1].c.begin() + 100),
                    std::make_move_iterator(F->Ds->Vs[r - 1].c.end())
                );
                F->Ds->Vs[r - 1].c.erase(
                    F->Ds->Vs[r - 1].c.begin() + 100,
                    F->Ds->Vs[r - 1].c.end()
                );
            }
            else {
                F->Ds->Vs[r].c.insert(
                    F->Ds->Vs[r].c.begin(),
                    std::make_move_iterator(F->Ds->Vs[r - 1].c.begin() + 100),
                    std::make_move_iterator(F->Ds->Vs[r - 1].c.end())
                );
                F->Ds->Vs[r - 1].c.erase(
                    F->Ds->Vs[r - 1].c.begin() + 100,
                    F->Ds->Vs[r - 1].c.end()
                );
            }
        }

    }
    int count_paragraph(file* F) {
        int ans = 0;
        for (int i = 0; i < F->Ds->Vs.size(); i++) {
            if (F->Ds->Vs[i].c.size() < 100) {
                ans++;
            }
        }
        if (ans == 0) {
            ans++;
        }
        return ans;
    }
    bool isAlphaNumeric(char ch) {
        if (ch >= 'A' && ch <= 'Z') {
            return true;
        }
        if (ch >= 'a' && ch <= 'z') {
            return true;
        }
        if (ch >= '1' && ch <= '9') {
            return true;
        }
        if (ch == ' ') {
            return true;
        }
        return false;
    }
    int special_characters(file* F) {
        int ans = 0;
        for (int i = 0; i < F->Ds->Vs.size(); i++) {
            for (int j = 0; j < F->Ds->Vs[i].my_size(); j++) {
                if (!isAlphaNumeric(F->Ds->Vs[i].c[j])) {
                    ans++;
                }
            }
        }
        return ans;
    }
    void merge_files(file* F1, file* F2) {
        for (int i = 0; i < F2->Ds->Vs.size(); i++) {
            F1->Ds->Vs.push_back(F2->Ds->Vs[i]);
        }
    }
    int largest_word(file* F) {
        int large = 0;
        int current = 0;
        for (int i = 0; i < F->Ds->Vs.size(); i++) {
            current = 0;
            for (int j = 0; j < F->Ds->Vs[i].my_size(); j++) {
                if (F->Ds->Vs[i].c[j] == ' ') {
                    large = max(large, current);
                    current = 0;
                }
                else {
                    current++;
                }
            }
            large = max(large, current);
        }
        return large;
    }
    int minimum_word(file* F) {
        int large = 0; bool found = false;
        for (int i = 0; i < F->Ds->Vs.size(); i++) {
            for (int j = 0; j < F->Ds->Vs[i].my_size(); j++) {
                if (F->Ds->Vs[i].c[j] == ' ') {
                    if (large == 0) {
                        continue;
                    }
                    else {
                        found = true;
                        break;
                    }
                }
                else {
                    large++;
                }
            }
            if (found) {
                break;
            }
        }
        int current = 0;
        for (int i = 0; i < F->Ds->Vs.size(); i++) {
            current = 0;
            for (int j = 0; j < F->Ds->Vs[i].my_size(); j++) {
                if (F->Ds->Vs[i].c[j] == ' ') {
                    if (current != 0)
                    {
                        large = min(large, current);
                    }
                    current = 0;
                }
                else {
                    current++;
                }
            }
            if (current != 0)
            {
                large = min(large, current);
            }
        }
        return large;
    }
    int number_of_lines(file* F) {
        return F->Ds->Vs.size();
    }
    void replace_next(file* F, string old, string replace) {
        vector<pair<int, int>>store;
        for (int i = F->Ds->ri; i < F->Ds->Vs.size(); i++) {
            int j = 0;
            if (i == F->Ds->ri) {
                j = F->Ds->ci;
            }
            else {
                j = 0;
            }
            for (; j < F->Ds->Vs[i].c.size(); j++) {
                if (F->Ds->Vs[i].c[j] == old[0]) {
                    store.push_back(make_pair(i, j));
                }
            }
        }
        for (int i = 0; i < store.size(); i++) {
            int old_index = store[i].second;
            int r = store[i].first;
            int c = store[i].second;
            bool found = true;;
            for (int j = 0; j < old.size(); j++) {
                if (old[j] != F->Ds->Vs[r].c[c]) {
                    found = false;
                    break;
                }
                c++;
                if (c >= 100) {
                    found = false;
                    break;
                }
            }
            if (found) {
                auto iter = F->Ds->Vs[r].c.begin();
                iter = next(iter, old_index);
                for (int i = 0; i < old.size(); i++) {
                    iter = F->Ds->Vs[r].c.erase(iter);
                }
                for (int i = replace.size() - 1; i >= 0; i--) {
                    iter = F->Ds->Vs[r].c.insert(iter, replace[i]);
                }
                break;
            }
        }
    }
    void replace_previous(file* F, string old, string replace) {
        vector<pair<int, int>>store;
        for (int i = 0; i <= F->Ds->ri; i++) {
            int end = F->Ds->Vs[i].c.size();;
            if (i == F->Ds->ri) {
                end = F->Ds->ci;
            }
            for (int j = 0; j < end; j++) {
                if (F->Ds->Vs[i].c[j] == old[0]) {
                    store.push_back(make_pair(i, j));
                }
            }
        }
        for (int i = 0; i < store.size(); i++) {
            int old_index = store[i].second;
            int r = store[i].first;
            int c = store[i].second;
            bool found = true;;
            for (int j = 0; j < old.size(); j++) {
                if (old[j] != F->Ds->Vs[r].c[c]) {
                    found = false;
                    break;
                }
                c++;
                if (c >= 100) {
                    found = false;
                    break;
                }
            }
            if (found) {
                auto iter = F->Ds->Vs[r].c.begin();
                iter = next(iter, old_index);
                for (int i = 0; i < old.size(); i++) {
                    iter = F->Ds->Vs[r].c.erase(iter);
                }
                for (int i = replace.size() - 1; i >= 0; i--) {
                    iter = F->Ds->Vs[r].c.insert(iter, replace[i]);
                }
                break;
            }
        }
    }
    void encode_file(file* F) {
        for (int i = 0; i < F->Ds->Vs.size(); i++) {
            for (int j = 0; j < F->Ds->Vs[i].my_size(); j++) {
                if (F->Ds->Vs[i].c[j] == '%') {
                    cout << " -oo- ";
                }
                else if (F->Ds->Vs[i].c[j] == '-') {
                    cout << " Note:Must Read this :) ";
                }
                else
                {
                    cout << F->Ds->Vs[i].c[j];
                }
            }
            cout << endl;
        }
    }
    string findWordWithMaxOccurrences(file* F) {
        vector<string>store;
        for (int i = 0; i < F->Ds->Vs.size(); i++) {
            string ans = "";
            for (int j = 0; j < F->Ds->Vs[i].my_size(); j++) {
                if (F->Ds->Vs[i].c[j] == ' ') {
                    store.push_back(ans);
                    ans = "";
                }
                else {
                    ans += F->Ds->Vs[i].c[j];
                }
                if (F->Ds->Vs[i].c.size() == j + 1) {
                    store.push_back(ans);
                    ans = "";
                }
            }
        }

        std::unordered_map<std::string, int> wordCount;
        for (const auto& word : store) {
            wordCount[word]++;
        }

        int firstMaxCount = 0;
        int secondMaxCount = 0;
        std::string firstMaxString, secondMaxString;

        for (const auto& pair : wordCount) {
            int count = pair.second;

            if (count > firstMaxCount) {
                secondMaxCount = firstMaxCount;
                firstMaxCount = count;
                secondMaxString = firstMaxString;
                firstMaxString = pair.first;
            }
            else if (count > secondMaxCount && count < firstMaxCount) {
                secondMaxCount = count;
                secondMaxString = pair.first;
            }
        }

        if (firstMaxString != "") {
            return firstMaxString;
        }
        else {
            return secondMaxString;
        }
    }

    // HASSAN DOCUMENT (TEXT-EDITOR)
    void delay(int seconds) {
        std::this_thread::sleep_for(std::chrono::seconds(seconds));
    }
    int avrg_word_length(file* F)
    {
        vector<string>store;
        for (int i = 0; i < F->Ds->Vs.size(); i++) {
            string ans = "";
            for (int j = 0; j < F->Ds->Vs[i].my_size(); j++) {
                if (F->Ds->Vs[i].c[j] == ' ') {
                    store.push_back(ans);
                    ans = "";
                }
                else {
                    ans += F->Ds->Vs[i].c[j];
                }
                if (F->Ds->Vs[i].c.size() == j + 1) {
                    store.push_back(ans);
                    ans = "";
                }
            }
        }
        int count_word_length = 0;
        int count_words = 0;
        string word = "";
        for (int i = 0; i < store.size(); i++)
        {
            word = store[i];
            count_word_length += word.size();
            if (store[i].size() > 1)
            {
                count_words++;
            }
        }
        int avg = count_word_length / count_words;
        return avg;
    }
    void replace_first_word(file* F, char word)
    {
        deque<string>store;
        for (int i = 0; i < F->Ds->Vs.size(); i++) {
            string ans = "";
            for (int j = 0; j < F->Ds->Vs[i].my_size(); j++) {
                if (F->Ds->Vs[i].c[j] == ' ') {
                    store.push_back(ans);
                    ans = "";
                }
                else {
                    ans += F->Ds->Vs[i].c[j];
                }
                if (F->Ds->Vs[i].c.size() == j + 1) {
                    store.push_back(ans);
                    ans = "";
                }
            }
        }
        int i = 0;
        if (F->Ds->Vs[0].c.front() == ' ')
        {
            i++;
        }
        F->Ds->Vs[0].c[i] = word;
    }
    void replace_whole_word(file* F, string word)
    {
        deque<string>store;
        for (int i = 0; i < F->Ds->Vs.size(); i++) {
            string ans = "";
            for (int j = 0; j < F->Ds->Vs[i].my_size(); j++) {
                if (F->Ds->Vs[i].c[j] == ' ') {
                    store.push_back(ans);
                    ans = "";
                }
                else {
                    ans += F->Ds->Vs[i].c[j];
                }
                if (F->Ds->Vs[i].c.size() == j + 1) {
                    store.push_back(ans);
                    ans = "";
                }
            }
        }
        int i = 0;
        int startword = 0;
        int endword = 0;
        if (F->Ds->Vs[0].c.front() == ' ')
        {
            i++;
        }
        string fword = store[i];
        for (int j = i; j < fword.size() + i; j++)
        {
            F->Ds->Vs[0].c[j] = ' ';
        }
        int k = 0;
        int st = 0;
        for (k = i, st = 0; k < word.size() + i; k++, st++)
        {
            F->Ds->Vs[0].c[k] = word[st];
        }
        /*int spaces = fword.size() - word.size();
        while (spaces != 0)
        {

        }*/
    }
    void Toupper(file* F)
    {
        int r = F->Ds->ri;
        int c = F->Ds->ci;
        deque<char> word = F->Ds->Vs[r].c;
        int i = word.size() - 1;
        while (word[i] != ' ')
        {
            i--;
        }
        for (int j = i; j < word.size(); j++)
        {
            word[j] = toupper(word[j]);
        }
        F->Ds->Vs[r].c = word;
        return;
    }
    void Tolower(file* F)
    {
        int r = F->Ds->ri;
        int c = F->Ds->ci;
        deque<char> word = F->Ds->Vs[r].c;
        int i = word.size() - 1;
        while (word[i] != ' ')
        {
            i--;
        }
        for (int j = i; j < word.size(); j++)
        {
            word[j] = tolower(word[j]);
        }
        F->Ds->Vs[r].c = word;
        return;
    }
    int substring_count(file* F, string sub)
    {
        vector<string>store;
        for (int i = 0; i < F->Ds->Vs.size(); i++) {
            string ans = "";
            for (int j = 0; j < F->Ds->Vs[i].my_size(); j++) {
                if (F->Ds->Vs[i].c[j] == ' ') {
                    store.push_back(ans);
                    ans = "";
                }
                else {
                    ans += F->Ds->Vs[i].c[j];
                }
                if (F->Ds->Vs[i].c.size() == j + 1) {
                    store.push_back(ans);
                    ans = "";
                }
            }
        }
        int sub_count = 0;
        int count = 0;
        string A;
        for (int i = 0; i < store.size(); i++)
        {
            count = 0;
            A = store[i];
            for (int j = 0, k = 0; j < A.size(); j++)
            {
                if (sub[k] == A[j])
                {
                    k++;
                    count++;
                }
            }
            if (count == sub.size() && A.size() > 1)
            {
                sub_count++;
            }
        }
        return sub_count;
    }
    bool find_word_s(file* F, string word)
    {
        vector<string>store;
        for (int i = 0; i < F->Ds->Vs.size(); i++) {
            string ans = "";
            for (int j = 0; j < F->Ds->Vs[i].my_size(); j++) {
                if (F->Ds->Vs[i].c[j] == ' ') {
                    store.push_back(ans);
                    ans = "";
                }
                else {
                    ans += F->Ds->Vs[i].c[j];
                }
                if (F->Ds->Vs[i].c.size() == j + 1) {
                    store.push_back(ans);
                    ans = "";
                }
            }
        }
        string A;
        int count = 0;
        for (int i = 0; i < store.size(); i++)
        {
            A = store[i];
            count = 0;
            for (int j = 0; j < A.size(); j++)
            {
                if (A[j] == word[j])
                {
                    count++;
                }
                else if (A[j] != word[j])
                {
                    break;
                }
            }
            if (count == A.size() && A.size() > 0)
                return true;
        }
        return false;
    }
    bool find_word_ns(file* F, string word)
    {
        vector<string>store;
        for (int i = 0; i < F->Ds->Vs.size(); i++) {
            string ans = "";
            for (int j = 0; j < F->Ds->Vs[i].my_size(); j++) {
                if (F->Ds->Vs[i].c[j] == ' ') {
                    store.push_back(ans);
                    ans = "";
                }
                else {
                    ans += F->Ds->Vs[i].c[j];
                }
                if (F->Ds->Vs[i].c.size() == j + 1) {
                    store.push_back(ans);
                    ans = "";
                }
            }
        }
        string A;
        int count = 0;
        for (int i = 0; i < store.size(); i++)
        {
            A = store[i];
            count = 0;
            for (int j = 0; j < A.size(); j++)
            {
                if (A[j] == word[j] || A[j] == tolower(word[j]) || A[j] == toupper(word[j]))
                {
                    count++;
                }
                else if (A[j] != word[j])
                {
                    break;
                }
            }
            if (count == A.size() && A.size() > 0)
                return true;
        }
        return false;
    }
    bool find_sentence(file* F, vector<string>& V)
    {
        vector<string>store;
        for (int i = 0; i < F->Ds->Vs.size(); i++) {
            string ans = "";
            for (int j = 0; j < F->Ds->Vs[i].my_size(); j++) {
                if (F->Ds->Vs[i].c[j] == ' ') {
                    store.push_back(ans);
                    ans = "";
                }
                else {
                    ans += F->Ds->Vs[i].c[j];
                }
                if (F->Ds->Vs[i].c.size() == j + 1) {
                    store.push_back(ans);
                    ans = "";
                }
            }
        }
        int count = 0;
        int k = 0;
        int last = 0;
        for (int i = 0; i < store.size(); i++)
        {
            if (k < V.size())
            {
                if (store[i] == V[k])
                {
                    count++;
                    last = k;
                    k++;
                }
                if (store[i] == V[last])
                {
                    if (i + 1 < store.size() && last + 1 < V.size() && store[i + 1] != V[last + 1])
                    {
                        count--;
                        k--;
                    }
                    if (i + 1 >= store.size() && last + 1 >= V.size() && store[i + 1] != V[last + 1])
                        break;
                }
            }
            else
                break;
        }
        if (count == V.size())
            return true;
        return false;
    }

    // HASSAN DOCUMENT (DOS)
    bool isPromptmode(string command)
    {
        return (command == "PROMPT");
    }
    vector<directory*> DIR(directory* D)
    {
        vector<directory*> dirs;
        vector<file*> files;
        auto iter = D->dir.begin();
        while (iter != D->dir.end())
        {
            dirs.push_back(*iter);
            iter++;
        }
        auto fiter = D->f.begin();
        while (fiter != D->f.end())
        {
            files.push_back(*fiter);
            fiter++;
        }
        return dirs;
    }
    void formatsD(directory* D)
    {
        for (auto iter = D->dir.begin(); iter != D->dir.end(); iter++)
        {
            formatsD(*iter);
        }
        D->dir.clear();
    }
    void formatD(directory* D)
    {
        string n;
        if (D->parent)
        {
            n = D->name;
            D = D->parent;
            for (auto iter = D->dir.begin(); iter != D->dir.end(); iter++)
            {
                if ((*iter)->name == n)
                {
                    D->dir.erase(iter);
                    break;
                }
            }
        }
        current = current->parent;
    }
    void print(vector<directory*>& V)
    {
        cout << "\t\t";
        for (int i = 0; i < V.size(); i++)
        {
            cout << V[i]->name << "-->";
        }
        cout << endl;
    }
    void removesD(directory* D)
    {
        for (auto iter = D->dir.begin(); iter != D->dir.end(); iter++)
        {
            (*iter)->f.clear();
            removesD(*iter);
        }
        D->dir.clear();
    }
    void removeD(directory* D)
    {
        string n;
        if (D->parent)
        {
            n = D->name;
            D = D->parent;
            for (auto iter = D->dir.begin(); iter != D->dir.end(); iter++)
            {
                if ((*iter)->name == n)
                {
                    (*iter)->f.clear();
                    D->dir.erase(iter);
                    break;
                }
            }
        }
        current = current->parent;
    }
    bool findsubDIR(directory* D, string n)
    {
        for (auto iter = D->dir.begin(); iter != D->dir.end(); iter++)
        {
            if ((*iter)->name == n)
                return true;
            findsubDIR(*iter, n);
        }
        return false;
    }
    directory* returnfindsubDIR(directory* D, string n)
    {
        for (auto iter = D->dir.begin(); iter != D->dir.end(); iter++)
        {
            if ((*iter)->name == n)
                return *iter;
            returnfindsubDIR(*iter, n);
        }
    }
    file* foundfile(directory* D, string n)
    {
        for (auto fiter = D->f.begin(); fiter != D->f.end(); fiter++)
        {
            if ((*fiter)->name == n)
                return *fiter;
        }
    }
    void move_directory(string sd, string dd, string f)
    {
        directory* S = returnfindsubDIR(root, sd);
        directory* D = returnfindsubDIR(root, dd);
        file* F = foundfile(S, f);
        S->f.remove(F);
        D->f.push_back(F);
    }
    void printQ(directory* D)
    {
        // cout << D->name;
        Q.push(D);
    }
    void printqueue()
    {
        queue<directory*> q = Q;
        while (!q.empty())
        {
            cout << q.front()->name << "->";
            q.pop();
        }
    }
    /* void printpriority_queue()
     {
         priority_queue<directory*> PQ = ;
     }*/
    bool isfound(directory* d, string oldname)
    {
        for (auto fiter = d->f.begin(); fiter != d->f.end(); fiter++)
        {
            if ((*fiter)->name == oldname)
            {
                return true;
            }
        }
        return false;
    }
    void convert(directory* d, string oldname, string newname)
    {
        for (auto fiter = d->f.begin(); fiter != d->f.end(); fiter++)
        {
            if ((*fiter)->name == oldname)
            {
                (*fiter)->name = newname;
                break;
            }
        }
    }

public:
    prompt() {
        root = new directory("Dell");
        root->dir.push_back(new directory("C", root));
        root->dir.push_back(new directory("D", root));
        root->dir.push_back(new directory("E", root));
        root->dir.push_back(new directory("F", root));
        current = root;
    }
    string cmd;
    string c;
    void run() {
        while (true) {
            if (isPromptmode(cmd) || c == "Y") {
                cout << " " << current->name << "$  ";
            }
            else if (!isPromptmode(cmd)) {
                cout << " " << current->name << ":\>  ";
            }
            cin >> cmd;
            if (cmd == "CD.") {
                print_directory(current);
            }
            else if (cmd == "CD") {
                string filename;
                cin >> filename;
                change_directory(current, filename);
            }
            else if (cmd == "CD..") {
                if (current != root)
                {
                    current = current->parent;
                }
                else
                    cout << "The current DIR is the root directory" << endl;
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
                delete_file(filename, current);
            }
            else if (cmd == "find") {
                string filename;
                cin >> filename;
                find_file(filename, current);
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
            else if (cmd == "DIR") {
                vector<directory*> V;
                V = DIR(current);
                print(V);
            }
            else if (cmd == "PROMPT") {
                cin >> c;
                isPromptmode(cmd);
            }
            else if (cmd == "FORMAT") {
                formatsD(current);
                formatD(current);
            }
            else if (cmd == "RMDIR") {
                removesD(current);
                removeD(current);
            }
            else if (cmd == "printQ")
            {
                printQ(current);
            }
            else if (cmd == "QUEUE")
            {
                cout << "The current state of queue is: \t\t";
                printqueue();
                cout << "\n";
            }
            else if (cmd == "CONVERT")
            {
                cout << "Enter the file old name:\n";
                string fname1;
                cin >> fname1;
                if (!isfound(current, fname1))
                    cout << "The file could not found in current directory!!!\n";
                string fname2;
                cout << "Enter the file new name:\n";
                cin >> fname2;
                convert(current, fname1, fname2);
            }
            else if (cmd == "MOVE")
            {
                string sd, dd, f;
                cout << "Give the source DIR..." << endl;
                do
                {
                    cin >> sd;
                    if (!findsubDIR(root, sd))
                        cout << "The selected source does not exist plz try again!!!\n";
                } while (!findsubDIR(root, sd));
                cout << "Give the destination DIR..." << endl;
                cin >> dd;
                cout << "Give the file name you wanna move" << endl;
                cin >> f;
                move_directory(sd, dd, f);
                cout << "The file has been moved successfully\n";
            }
            else if (cmd == "EXIT")
            {
                system("cls");
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
            else if (cmd == "help") {
                help();
            }
            else if (cmd == "strfind") {
                vector<string>result;
                string word;
                cin >> word;
                result = find_str_in_all_files(word);
                if (result.empty()) {
                    cout << "word not found...";
                }
                else {
                    for (int i = 0; i < result.size(); i++) {
                        cout << word << " is present in " << result[i] << endl;
                    }
                }
            }
            else if (cmd == "save") {
                save();
            }

            else if (cmd == "pwd") {
                cout << "\t\t" << current->name << endl;
            }
            else if (cmd == "pprint") {
                string filename;
                cin >> filename;
                int priority;
                cin >> priority;
                files.push(new file(filename, nullptr, priority));
                std::ofstream outputFile(filename);
                if (!outputFile.is_open()) {
                    std::cerr << "Error opening file: " << std::endl;
                    throw exception("Error...");
                }
                outputFile.close();
                std::cout << "File created successfully: " << std::endl;
                priority_queue<file*>copy = files;
                while (!copy.empty()) {
                    cout << "  " << copy.top()->name << endl;
                    copy.pop();
                }
            }
            else if (cmd == "edit") {
                cout << "Enter the file name:";
                string filename;
                cin >> filename;
                file* F = findFile(filename, root);
                document* C = nullptr;
                if (F) {
                    C = F->Ds;
                }
                else {
                    cout << "\n no such file exists\n";
                    continue;
                }
                string password;
                while (password != F->password) {
                    if (F->password == "") {
                        break;
                    }
                    system("cls");
                    cout << "Enter the password:";
                    cin >> password;
                }
                while (true) {
                    system("cls");
                    display_file(F);

                    cout << "\n\n";
                    cout << "Enter command:";
                    string command;
                    cin >> command;
                    if (command == "enter") {
                        string text;
                        cout << "Enter text:";
                        getline(cin, text);
                        enter_text(F, text);
                        system("cls");
                        display_file(F);
                    }
                    else if (command == "enterb") {
                        string text;
                        cout << "Enter text:";
                        getline(cin, text);
                        enter_text_before(F, text);
                        system("cls");
                        display_file(F);
                    }
                    else if (command == "count") {
                        cout << "\n number of paragraphs are:";
                        cout << count_paragraph(F);
                        cout << "\npress any key to continue...";
                        _getch();
                    }
                    else if (command == "special") {
                        cout << "\n number of special words are:";
                        cout << special_characters(F);
                        cout << "\npress any key to continue...";
                        _getch();
                    }
                    else if (command == "merge") {
                        cout << "Enter file name:";
                        cin >> filename;
                        file* F2 = findFile(filename, root);
                        if (!F2) {
                            cout << "file not found";
                            continue;
                        }
                        else {
                            merge_files(F, F2);
                        }
                    }
                    else if (command == "exit") {
                        system("cls");
                        break;
                    }
                    else if (command == "large") {
                        cout << "\n largest number length is::";
                        cout << largest_word(F);
                        cout << "\npress any key to continue...";
                        _getch();
                    }
                    else if (command == "small") {
                        cout << "\n smallest number length is::";
                        cout << minimum_word(F);
                        cout << "\npress any key to continue...";
                        _getch();
                    }
                    else if (command == "lines") {
                        cout << "\n smallest number length is::";
                        cout << number_of_lines(F);
                        cout << "\npress any key to continue...";
                        _getch();
                    }
                    else if (command == "password") {
                        cout << "enter the pssword:";
                        cin >> F->password;
                    }
                    else if (command == "replace") {
                        string W1;
                        string W2;
                        cout << "Enter the old word:";
                        cin >> W1;
                        cout << "\nEnter the new word:";
                        cin >> W2;
                        replace_next(F, W1, W2);
                    }
                    else if (command == "replacepre") {
                        string W1;
                        string W2;
                        cout << "Enter the old word:";
                        cin >> W1;
                        cout << "\nEnter the new word:";
                        cin >> W2;
                        replace_previous(F, W1, W2);
                    }
                    else if (command == "encode") {
                        cout << "\n\n";
                        encode_file(F);
                        cout << "\n\nEnter any key to continue";
                        _getch();
                    }
                    else if (command == "max") {
                        cout << "The word that came maximum time is:";
                        cout << findWordWithMaxOccurrences(F);
                        cout << "\nEnter any key to continue...";
                        _getch();
                    }
                    //Hassan Additions

                    else if (command == "wordavg")
                    {
                        cout << "Enter the file name\n";
                        string fname;
                        cin >> fname;
                        file* F2 = findFile(fname, root);
                        if (!F2) {
                            cout << "file not found";
                            continue;
                        }
                        else
                        {
                            cout << "The Average worf length for this file is: ";
                            cout << avrg_word_length(F2);
                        }
                        delay(2);
                    }
                    else if (command == "fword")
                    {
                        cout << "Enter the file name\n";
                        string fname;
                        cin >> fname;
                        file* F2 = findFile(fname, root);
                        cout << "Enter the char you wanna replace\n";
                        char data;
                        cin >> data;
                        cout << "The modified file data after replacing with first word only is:\n";
                        replace_first_word(F2, data);
                        display_file(F2);
                        delay(2);
                    }
                    else if (command == "wholeword")
                    {
                        cout << "Enter the file name\n";
                        string fname;
                        cin >> fname;
                        file* F2 = findFile(fname, root);
                        cout << "Enter the char you wanna replace\n";
                        string data;
                        cin >> data;
                        cout << "The modified file data after replacing with first word only is:\n";
                        replace_whole_word(F2, data);
                        display_file(F2);
                        delay(2);
                    }
                    else if (command == "upper")
                    {
                        cout << "Enter the file name\n";
                        string fname;
                        cin >> fname;
                        file* F2 = findFile(fname, root);
                        if (!F2) {
                            cout << "file not found";
                            continue;
                        }
                        else
                        {
                            Toupper(F2);
                            cout << "The upper case string is: ";
                            display_file(F2);
                            delay(3);
                        }
                    }
                    else if (command == "lower") {
                        cout << "Enter the file name\n";
                        string fname;
                        cin >> fname;
                        file* F2 = findFile(fname, root);
                        if (!F2) {
                            cout << "file not found";
                            continue;
                        }
                        else
                        {
                            Tolower(F2);
                            cout << "The lower case string is: ";
                            display_file(F2);
                            delay(3);
                        }
                    }
                    else if (command == "subcount")
                    {
                        cout << "Enter the file name\n";
                        string fname;
                        cin >> fname;
                        file* F2 = findFile(fname, root);
                        if (!F2) {
                            cout << "file not found";
                            continue;
                        }
                        else
                        {
                            cout << "Enter the sub-string:\n";
                            cin >> fname;
                            cout << "The number of sub strings are: ";
                            cout << substring_count(F2, fname);
                            delay(3);
                        }
                    }
                    else if (command == "findwords")
                    {
                        cout << "Enter the file name\n";
                        string fname;
                        cin >> fname;
                        file* F2 = findFile(fname, root);
                        if (!F2) {
                            cout << "file not found";
                            continue;
                        }
                        else
                        {
                            cout << "Enter the word:\n";
                            cin >> fname;
                            if (find_word_s(F2, fname))
                                cout << "The word is found!!!";
                            else
                                cout << "The word could not found!!!";
                            delay(3);
                        }
                    }
                    else if (command == "findwordns")
                    {
                        cout << "Enter the file name\n";
                        string fname;
                        cin >> fname;
                        file* F2 = findFile(fname, root);
                        if (!F2) {
                            cout << "file not found";
                            continue;
                        }
                        else
                        {
                            cout << "Enter the word:\n";
                            cin >> fname;
                            if (find_word_ns(F2, fname))
                                cout << "The word is found!!!";
                            else
                                cout << "The word could not found!!!";
                            delay(3);
                        }
                    }
                    else if (command == "findsent")
                    {
                        cout << "Enter the file name\n";
                        string fname;
                        cin >> fname;
                        file* F2 = findFile(fname, root);
                        if (!F2) {
                            cout << "file not found";
                            continue;
                        }
                        else
                        {
                            vector<string> A;
                            A.push_back("Hello");
                            A.push_back("Hassan");
                            if (find_sentence(F2, A))
                            {
                                cout << "The sentence is found!!!";
                            }
                            else if (!find_sentence(F2, A))
                            {
                                cout << "The sentence could not found!!!";
                            }
                            delay(3);
                        }
                    }
                    else if (command == "move") {
                        system("cls");
                        display_file(F);
                        while (true) {
                            char key;
                            bool done = false;
                            if (_kbhit()) {
                                key = _getch();


                                key = _getch();
                                switch (key) {
                                case 77:
                                    F->Ds->ci++;
                                    F->Ds->ci %= F->Ds->Vs[F->Ds->ri].c.size();
                                    system("cls");
                                    display_file(F);
                                    break;
                                case 75:
                                    F->Ds->ci--;
                                    F->Ds->ci %= F->Ds->Vs[F->Ds->ri].c.size();
                                    system("cls");
                                    display_file(F);
                                    break;
                                case 80:
                                    F->Ds->ri++;
                                    F->Ds->ri %= F->Ds->Vs.size();
                                    F->Ds->ci %= F->Ds->Vs[F->Ds->ri].c.size();
                                    system("cls");
                                    display_file(F);
                                    break;
                                case 72:
                                    F->Ds->ri--;
                                    F->Ds->ri %= F->Ds->Vs.size();
                                    F->Ds->ci %= F->Ds->Vs[F->Ds->ri].c.size();
                                    system("cls");
                                    display_file(F);
                                    break;
                                default:
                                    done = true;
                                    break;
                                }
                            }
                            if (done) {
                                break;
                            }
                        }
                    }
                }
            }
            else if (cmd == "tree") {
                printDirectoryStructure(root);
            }
            else {
                cout << "\n\t\tNo such command exist...";
            }
        }
    }
};

