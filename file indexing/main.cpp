#define UNICODE // Set UNICODE to enable Unicode support on Windows
#include <iostream> // Provides functions for input and output
#include <string>   // Provides string-related functions
#include <chrono>   // Provides time-related functions
#include <unordered_map> // Provides an unordered associative container
#include <Windows.h> // Provides Windows-specific functions and data types

// Include the source files for the B-Tree, hashing, and search algorithms
#include "C:\Users\samriti\Desktop\juit\os project\sorting\b-tree.cpp"
#include "C:\Users\samriti\Desktop\juit\os project\sorting\hashing.cpp"
#include "C:\Users\samriti\Desktop\juit\os project\sorting\search.cpp"
#include "C:\Users\samriti\Desktop\juit\os project\sorting\binarytree.cpp"


// This is the main function of the program.
int main()
{
    // Prompt the user to choose between indexing or searching.
    cout << "Press 1 for indexing" << endl;
    cout << "Press 2 for searching" << endl;
    // Read user input for choice
    int choice1;
    cin >> choice1;

    // If the user chooses to index files
    if (choice1 == 1) 
    {
        // Ask user to input the directory path
        wstring directory;
        cout << "Enter directory path: ";

        // Ignore any remaining newline characters from previous input
        cin.ignore();
        // Read directory path from user
        getline(wcin, directory);

        // Declare variables for file and subdirectory count
        int fileCount = 0;
        int subdirectoryCount = 0;

        // Ask user to choose indexing method
        int choice;
        cout << "Enter your choice:" << endl;
        cout << "Press 1 for indexing using binary tree" << endl;
        cout << "Press 2 for indexing using btree" << endl;
        cout << "Press 3 for indexing using hashing" << endl;
        // Switch statement to choose indexing method
        cin >> choice;

        switch (choice)
        {
            // If user chooses binary tree indexing
            case 1:
            {
                CAbstract *ptr;
                // Create a binary tree object
                CBinaryTree bst;
                ptr=&bst;
                // Record start time for indexing
                auto start = chrono::high_resolution_clock::now();

                // Index files in the directory using binary tree method
                vListFilesInDirectory(directory, fileCount, bst);

                // Record end time for indexing
                auto end = chrono::high_resolution_clock::now();

                // Calculate indexing duration in nanoseconds
                auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

                // Print the indexed files using inorder traversal of the binary tree
                ptr->traverse();

                // Print file count and indexing duration
                cout << "Total files: " << fileCount << endl;
                cout << "Time taken to index files: " << duration << " nanoseconds" << endl;
            }
            
            // If user chooses btree-search indexing
            case 2:
            {
                CDirectoryIndexer *ptr;
                // Create a new instance of the Btree Indexer class
                CBtreeIndexer indexer;
                ptr=&indexer;

                // Declare variables to hold the index statistics
                int fileCount = 0;
                int subdirectoryCount = 0;
                unordered_map<string, wstring> fileIndex;

                // Start the timer
                auto start = chrono::high_resolution_clock::now();

                // Index the directory and its subdirectories using the Btree Indexer class
                ptr->vIndexDirectory(directory, fileCount, subdirectoryCount, fileIndex);

                // Stop the timer
                auto stop = chrono::high_resolution_clock::now();

                // Calculate the duration of the indexing process
                auto duration = chrono::duration_cast<chrono::nanoseconds>(stop - start).count();

                // Print the index statistics
                cout << "Indexed " << fileCount << " files in " << subdirectoryCount << " subdirectories." << endl;

                // Print the file index
                for (auto it = fileIndex.begin(); it != fileIndex.end(); it++)
                {
                    string value = wstring_convert<codecvt_utf8<wchar_t>>().to_bytes(it->second);
                    cout << "Key: " << it->first << ", Value: " << value << endl;
                }

                // Print the time taken to index the files
                cout << "Time taken to index files: " << duration << " nanoseconds" << endl;

            }
            // If user chooses hashing indexing
            case 3:   
            { // Create a pointer to a CFileIndexer object
                CFileIndexer* file_indexer;
                // Create a Hasing object and assign its address to the file_indexer pointer
                CHashing hashing;
                file_indexer = &hashing;

                map<size_t, vector<wstring>> index;
                int file_count = 0;

                // Record the starting time of the indexing process
                auto start = chrono::high_resolution_clock::now();
                
                // Use the pointer to call virtual functions of the CFileIndexer class
                file_indexer->vListFilesInDirectory(directory, index, file_count);
                file_indexer->print_index(index, file_count);
                // Record the ending time of the indexing process
                auto end = chrono::high_resolution_clock::now();

                // Calculate the time taken to index the files
                auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

                // Print the time taken to index the files
                cout << "Time taken to index files: " << duration << " nanoseconds" << endl;    
                break;
            }

            //default case if no valid choice is entered
            default:
            {
                cout<<"enter a valid choice"<<endl;
                break;
            }
        }
    }
    // If the user chooses to search files
    else
    {
        // Create a CFileSearch object and call its member functions to 
        CFileSearch* search = new CDirectorySearch();
        search->getInput(); //get user input
        search->searchFiles();// search for files
        search->printResult();//print the search
        // Delete the ListFilesSearch object to free up memory
        delete search;
    }
    return 0;
}