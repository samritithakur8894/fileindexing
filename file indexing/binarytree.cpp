// Set UNICODE to enable Unicode support on Windows
#define UNICODE
#include <iostream> // Provides functions for input and output
#include <string>   // Provides string-related functions
#include <chrono>   // Provides time-related functions
#include <unordered_map> // Provides an unordered associative container
#include <Windows.h> // Provides Windows-specific functions and data types

using namespace std;
class CAbstract
{
    public:
    virtual void traverse()=0;
    virtual void insert(wstring filename)=0;

};
// Define a class for a node in a binary tree
class CNode : public CAbstract
{
    public:
    wstring filename; // The name of the file represented by this node
    CNode* left;      // Pointer to the left child node
    CNode* right;     // Pointer to the right child node

    // Constructor to create a new node with the given filename
    CNode(wstring name) 
    {
        filename = name;
        left = NULL;
        right = NULL;
    }

    // Method to traverse the tree in-order and print the filenames
    void traverse() 
    {
        if (left != NULL) 
            left->traverse();
        
        wcout << filename << endl;
        
        if (right != NULL) 
            right->traverse();
    }

    // Method to insert a new filename into the tree
    void insert(wstring filename)
    {
        if (filename < this->filename) {
            if (left == NULL) 
                left = new CNode(filename);
            else 
                left->insert(filename);
        } else {
            if (right == NULL) 
                right = new CNode(filename);
            else 
                right->insert(filename);
        }
    }
};

// Define a class for a binary  tree
class CBinaryTree : public CAbstract
{
public:
    CNode* root; // Pointer to the root node of the tree

    // Constructor to create a new empty tree
    CBinaryTree() 
    {
        root = NULL;
    }

    // Method to traverse the tree in-order and print the filenames
    void traverse() 
    {
        if (root != NULL) 
            root->traverse();
    }

    // Method to insert a new filename into the tree
    void insert(wstring filename) 
    {
        if (root == NULL) 
            root = new CNode(filename);
        else 
            root->insert(filename);
    }
};

// Function to list all files in a directory and its subdirectories
void vListFilesInDirectory(const wstring& directory, int& fileCount, CBinaryTree& bst)
{
    WIN32_FIND_DATA findFileData; // Structure to hold information about a file
    HANDLE hFind; // Handle to the search operation

    // Search for all files and directories in the given directory
    wstring searchPath = directory + L"\\*";
    hFind = FindFirstFile(searchPath.c_str(), &findFileData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            // Ignore "." and ".." directories
            if (wcscmp(findFileData.cFileName, L".") == 0 ||
                wcscmp(findFileData.cFileName, L"..") == 0) {
                continue;
            }

            // Check if the file is a directory
            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
            {
                // Recursively list files in subdirectory
                wstring subdirectory = directory + L"\\" + findFileData.cFileName;
                vListFilesInDirectory(subdirectory, fileCount, bst);
            } 
            else 
            {
                // Increment file count and add file to binary search tree
                fileCount++;
                bst.insert(directory + L"\\" + findFileData.cFileName);
            }
        } while (FindNextFile(hFind, &findFileData) != 0);

        FindClose(hFind);
    }
}