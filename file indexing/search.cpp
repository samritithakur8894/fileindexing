#include<iostream>// Provides functions for input and output
#include<chrono>//header file to calculate time 
#include<dirent.h> // header file for directory operations
#include<cstring> // header file for string operations

using namespace std;
using namespace std::chrono;

// Base class for file search
class CFileSearch {
protected:
    DIR *directory; // pointer to directory stream
    struct dirent *entry; // structure representing directory entry
    string searchString, directoryPath; // string for search and directory path
    bool resultFound; // flag for search result
    int entryCount; // number of entries searched

public:
    CFileSearch() {
        entryCount = 0;
        resultFound = false;
    }

    virtual void getInput() = 0; // pure virtual method for getting user input
    virtual void searchFiles() = 0; // pure virtual method for searching files
    virtual void printResult() = 0; // pure virtual method for printing search result
};

// Derived class for searching files in a directory
class CDirectorySearch : public CFileSearch {
public:
    // method to get user input
    void getInput() {
        cout << "Please enter the path to search: "<<endl;
        cin >> directoryPath;

        cout << "Please enter the string to search: "<<endl;
        cin >> searchString;
    }

    // method to search files in directory
    void searchFiles() {
        auto startTime = high_resolution_clock::now(); // start time

        // check if directory can be opened
        if ((directory = opendir(directoryPath.c_str())) == NULL) {
            cout << "Error: Cannot open directory!" << endl;
            return;
        }

        // read directory entries until end or error
        while ((entry = readdir(directory)) != NULL) {
            entryCount++; // increment number of entries searched
            if (strstr(entry->d_name, searchString.c_str())) {
                cout << entry->d_name << endl; // print file name if search string found
                resultFound = true; // set flag for search result
            }
        }

        // check if any entries were searched
        if (entryCount == 0) {
            cout << "Error: No entries found in directory!" << endl;
            closedir(directory); // close directory stream
            return;
        }

        // check if search string was found in any entries
        if (!resultFound) {
            cout << "Error: Search string not found in any entries!" << endl;
        }

        closedir(directory); // close directory stream

        auto stopTime = high_resolution_clock::now(); // stop time
        auto duration = duration_cast<nanoseconds>(stopTime - startTime); // calculate duration in nanoseconds

        cout << "\nSearching time: " << duration.count() << " nanoseconds" << endl;
    }

    // method to print search result
    void printResult() {
        if (resultFound) {
            cout << "\nFile(s) is present" << endl;
        }
        else {
            cout << "\nFile(s) is not present" << endl;
        }
    }
};
