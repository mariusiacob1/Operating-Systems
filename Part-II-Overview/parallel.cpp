/*
Parellel
parallel fetching:
the parent will create a child for each location (i.e., without waiting
for a child to complete) and only once all children have been created, the parent will wait
for all of them to terminate.
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
using namespace std;

int main(){
    // Declaring Variables
    pid_t pid;
    int childVal;
    int waitVal;
    int execlpVal;
    int numberOfFile = 0;

    float  latitude;
    float  longitude;
    char   finalURLBuff[1000];	

    // File
    ifstream File;
    File.open("input.txt");

    while(File >> latitude >> longitude){
        numberOfFile++;
        pid = fork();
        
        // if child
        if(pid == 0){
            // construct the URL for the lat and long
            snprintf(finalURLBuff, 1000, 
           "https://api.open-meteo.com/v1/forecast?latitude=%f&longitude=%f&current_weather=True", latitude, longitude); 
           
            // constructing the file name
            string file = "file" + to_string(numberOfFile) + ".json";
            cout << "curl " << "-o " << file << " " << finalURLBuff << endl;
            execlpVal = execlp("/usr/bin/curl", "curl", "-o", file.c_str(), finalURLBuff, (char*)NULL);
            
            if(execlpVal < 0){
			    perror("execlp");
			    exit(1);
		    }
        }
    }

        // if parent
        if(pid > 0){
            int j = 1;
            while((waitVal = wait(NULL)) > 0){
                printf("\n%d child completed\n", j++);
                
                // if wait failed
                if(waitVal < 0){
                    perror("wait");
                    exit(1);
                }
            }
            // in parent now
            printf("all children terminated. in parent now\n");
        }
}