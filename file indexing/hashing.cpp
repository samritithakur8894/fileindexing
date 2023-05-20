#define UNICODE
#include <iostream> //provides input/output operations in C++.
#include <fstream>//provides file input/output operations.
#include <string>// Provides functions for working with strings
#include <vector>//provides a dynamic array-like data structure.
#include <map>//provides an associative array data structure.
#include <cstring>// provides functions for manipulating C-style strings
#include <chrono>//provides functionality for measuring time durations.
#include <algorithm>//provides a collection of functions for performing operations on containers.
#include <windows.h>//provides access to the Windows API.
#include <locale>  // to std::wstring_convert
#include <codecvt>//provides a codecvt facet that can convert between different character encodings.

using namespace std;

class CFileIndexer
{
public:
    virtual size_t hash_filename(const wstring& filename) = 0;
    virtual void vListFilesInDirectory(const wstring& directory, map<size_t, vector<wstring>>& index, int& file_count) = 0;
    virtual void print_index(const map<size_t, vector<wstring>>& index, int file_count) = 0;
};

class CHashing : public CFileIndexer
{
    public:
    // Hash function that returns an index for a given filename
    size_t hash_filename(const wstring& filename)
    {
        const size_t p = 31; // A prime number used in the hash function
        const size_t m = 1e9 + 9; // A large prime number used in the hash function
        size_t hash_value = 0;
        size_t p_pow = 1;
        for (wchar_t c : filename)
        {
            hash_value = (hash_value + (size_t(c) * p_pow) % m) % m; // The hash function
            p_pow = (p_pow * p) % m;
        }
        return hash_value;
    }

    void vListFilesInDirectory(const wstring& directory, map<size_t, vector<wstring>>& index, int& file_count)
    {
        WIN32_FIND_DATA findFileData;
        HANDLE hFind;

        // for all files and directories in the given directory
        wstring searchPath = directory + L"\\*";
        hFind = FindFirstFile(searchPath.c_str(), &findFileData);

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
                    // Recursively list files in subdirectory
                    wstring subdirectory = directory + L"\\" + findFileData.cFileName;
                    vListFilesInDirectory(subdirectory, index, file_count);
                }
                else
                {
                    // Hash filename to get the index key
                    size_t index_key = hash_filename(findFileData.cFileName);
                    wstring file_path = directory + L"\\" + findFileData.cFileName;
                    index[index_key].push_back(file_path);
                    file_count++;
                }
            } while (FindNextFile(hFind, &findFileData) != 0);

            FindClose(hFind);
        }
    }

    void print_index(const map<size_t, vector<wstring>>& index, int file_count)
    {
        cout << "Indexed " << file_count << " files." << endl;

        // Print index
        for (auto it = index.begin(); it != index.end(); it++)
        {
            cout << "Index key " << it->first << " : " << endl;
            for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++)
            {
                wcout << "  " << *it2 << endl;
            }   
        }
    }
};

