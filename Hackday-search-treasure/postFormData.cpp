//
//  postFormData.cpp
//  Hackday-search-treasure
//
//  Created by Jang on 2016. 6. 24..
//  Copyright © 2016년 Jang. All rights reserved.
//


#include <iostream>
#include <vector>
#include <locale>
#include <string>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <curl/curl.h>

using namespace std;

#define THREADNUM 20

const string inputFile = "/Users/jang/Desktop/hackday/combineResult.txt";

vector<string> queury[THREADNUM];

void readTextfile(const char* filename){
    string buff;
    fstream fs(filename);
    
    std::locale::global(std::locale("ko_KR.UTF-8"));
    wcout.imbue(std::locale("ko_KR.UTF-8"));
    
    int index = 0;
    while ( getline(fs, buff) ) {
        queury[index].push_back(buff);
        index++;
        if(index==THREADNUM) index = 0;
    }
    
    for(int i=0; i<THREADNUM; i++){
        cout<<queury[i].size()<<" queuries push backed"<<endl;
    }
    
    fs.close();
}

void postQuery(int index){
    
    for (int i=0; i<queury[index].size(); i+=5){
        CURL *curl = curl = curl_easy_init();
        if(curl) {
            string postData = "name=ingyure&answer=";
            
            for(int j=i; j<i+5; j++){
                postData += queury[index][j];
                postData += "\n";
            }
            
            //            cout<<postData<<endl;
            
            curl_easy_setopt(curl, CURLOPT_URL, "http://treasure.navercorp.com/campus_hackday/evaluation/");
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
            
            CURLcode res = curl_easy_perform(curl);
            
            if(res != CURLE_OK)
                fprintf(stderr, "curl_easy_perform() failed: %s\n",
                        curl_easy_strerror(res));
            
            curl_easy_cleanup(curl);
        }
    }
}

void *t_function(void* data){
    
    int id = *((int *)data);
    
    postQuery(id);
    while(1);
}

int main(void) {
    
    int status;
    pthread_t p_thread[THREADNUM];
    
    locale::global(std::locale("ko_KR.UTF-8"));
    
    
    readTextfile(inputFile.c_str());
    
    for(int i=0; i<THREADNUM; i++){
        int id  = i;
        
        int thr_id = pthread_create(&p_thread[i], NULL, t_function, (void *)&id);
        if (thr_id < 0) {
            perror("thread create error : ");
            exit(0);
        }
    }
    
    for(int i=0; i<THREADNUM; i++){
        pthread_join(p_thread[i], (void **)&status);
    }
    
    curl_global_cleanup();
    return 0;
}