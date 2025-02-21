#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <algorithm>

using namespace std;

const int MAX_FILES = 100;  // Increased array size for up to 100,000,000 values

// Global variables
int TotalFound = 0;
int AboveThreshold = 0;
int EqualsThreshold = 0;
int BelowThreshold = 0;

// Function to process a file by a thread
void processFile(int threadID, int start, int end, char CHAR, int TH, string* files, int N , int *counts) {
    if (start > end) {
        cout << "TID" << threadID << " has no work to do." << endl;
        return;
    }
    int endStart = end-start;
    int localTotal[endStart];
    int localTotalCounter = 0;
    for (int i = start; i < end ; ++i) {
    	localTotal[localTotalCounter]=0;
        ifstream file(files[i]);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                localTotal[localTotalCounter] += count(line.begin(), line.end(), CHAR);
            }
            localTotalCounter++;
            file.close();
        }
    }

    // Update global variables within the critical section
    localTotalCounter = 0;
    
    for (int i=0;i<endStart;i++){{
    
    TotalFound += localTotal[i];
    }
    if (localTotal[i] > TH) {
        
        AboveThreshold++;
    } else if (localTotal[i] == TH) {
        
        EqualsThreshold++;
    } else {
        
        BelowThreshold++;
    }

}
    // Output thread information
    cout << "TID" << threadID << " --> Starting thread firstItem=" << start << ", lastItem=" << end << endl;
    localTotalCounter = 0;
    
    for (int i = start; i < end && i < N; ++i) {
        cout << "TID" << threadID << " --> File: " << files[i] << ", (" << CHAR << ") found=" << localTotal[localTotalCounter] << endl;
        counts[i]=localTotal[localTotalCounter++];
    }
    cout << "TID" << threadID << " --> Ending thread firstItem=" << start << ", lastItem=" << end << endl;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <T> <CHAR> " << endl;
        return 1;
    }

    int T = stoi(argv[1]);
    char CHAR = argv[2][0];
    ifstream inputFile("in.txt");

    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open input file." << endl;
        return 1;
    }

    int N, TH;
    inputFile >> N >> TH;

    cout<< "Main --> Search Engine searching for ("<<CHAR<<") in "<<N<<" files, using "<<T<<" threads (with threshold="<<TH<<")"<<endl;
    string* files = new string[N];
    for (int i = 0; i < N; ++i) {
        inputFile >> files[i];
    }

    inputFile.close();
    int* counts = new int[N];
    // Create threads
    thread threads[T];
    for (int i = 0; i < T; ++i) {
    int start = (T>=N) ? i : (N / T) * (i);
    int end = (T>=N) ? i+1 : (N / T) * (i + 1);
    if (T>=N && i<N || T<N){
    threads[i] = thread(processFile, i, start, end, CHAR, TH, files, N, counts);
    }else {
    cout << "Skipping thread " << i << " as it has no work to do." << endl;
    }
}


    // Join threads
    if (T>=N)
    for (int i = 0; i < N; ++i) {
        threads[i].join();
    }
    else 
    for (int i = 0; i < T; ++i) {
        threads[i].join();
    }
    
    
    // Output global statistics
    cout << "Main --> TotalFound=" << TotalFound << ", AboveThreshold=" << AboveThreshold
         << ", EqualsThreshold=" << EqualsThreshold << ", BelowThreshold=" << BelowThreshold << endl;
    
    ofstream outputFile("out.txt");
    bool checkOrNot[N];
    for (int i=0;i<N;i++)
    checkOrNot[i]=false;
    outputFile <<"Sorted list of files:"<<endl;
    for (int i=0;i<N;i++){
    	outputFile << i + 1 <<".[ ";
    	int max=-1;
    	int saveIndex=-1;
    for(int j=0;j<N;j++)
    	if (counts[j]>=max && checkOrNot[j]==false){
    		max=counts[j];
    		saveIndex=j;}
    	checkOrNot[saveIndex]=true;
    	outputFile << files[saveIndex] << " ]" << "(found = "<<counts[saveIndex]<<")"<<endl;
    }    
    // Cleanup dynamic memory
    delete[] files;

    return 0;
}

