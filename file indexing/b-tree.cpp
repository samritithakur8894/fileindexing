// Enable support for Unicode characters
#define UNICODE
// Include necessary headers
#include <Windows.h> // Provides functions for working with files and directories on Windows
#include <iostream> // Provides functions for input and output
#include <string> // Provides functions for working with strings
#include <unordered_map> // Provides an unordered map data structure
#include <locale> // Provides localization utilities
#include <codecvt> // Provides utilities for working with character encodings
#include <chrono> // Provides time utilities

using namespace std;

// Abstract base class for directory indexing
class CDirectoryIndexer 
{
    public:
    virtual void vIndexDirectory(const wstring& directory, int& fileCount, int& subdirectoryCount, unordered_map<string, wstring>& fileIndex) = 0;
};

// Derived class implementing btree method for directory indexing
class CBtreeIndexer : public CDirectoryIndexer 
{
    public:
    void vIndexDirectory(const wstring& directory, int& fileCount, int& subdirectoryCount, unordered_map<string, wstring>& fileIndex)
    {
        WIN32_FIND_DATA findFileData;
        HANDLE hFind;
        // Create a search path for all files and directories in the given directory
        wstring searchPath = directory + L"\\*";

        // Start searching for files and directories
        hFind = FindFirstFile(searchPath.c_str(), &findFileData);

        // Check if the search was successful
        if (INVALID_HANDLE_VALUE == hFind)
        {
            // Print an error message if the search was not successful
            cout << "Error in Finding File" << endl;
            cout << "Error" << endl;
        }

        // If the search was successful, continue searching until no more files or directories are found
        if (hFind != INVALID_HANDLE_VALUE)
        {
            do
            {
                // Ignore "." and ".." directories
                if (wcscmp(findFileData.cFileName, L".") == 0 ||
                    wcscmp(findFileData.cFileName, L"..") == 0)
                {
                    continue;
                }

                // Check if the file is a directory
                if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    // Recursively list all files and directories in the subdirectory
                    wstring subdirectory = directory + L"\\" + findFileData.cFileName;
                    subdirectoryCount++;
                    vIndexDirectory(subdirectory, fileCount, subdirectoryCount, fileIndex);
                }
                else
                {
                    // Increment file count
                    fileCount++;

                    // Generate a simple hash code for the file by summing the Unicode code point values of each character in the file name
                    wstring fileName = wstring(findFileData.cFileName);
                    int hashCode = 0;
                    for (int i = 0; i < fileName.length(); i++)
                    {
                        hashCode += (int)fileName[i];
                    }

                    // Add the file to the index using the hash code as the key
                    wstring filePath = directory + L"\\" + findFileData.cFileName;
                    fileIndex[std::to_string(hashCode)] = filePath;
                }

            } while (FindNextFile(hFind, &findFileData) != 0); // Keep searching until no more files or directories are found
            // Close the handle to the file search
            FindClose(hFind);
        }
    }
};