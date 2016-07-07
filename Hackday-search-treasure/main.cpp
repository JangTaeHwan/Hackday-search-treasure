//
//  main.cpp
//  Hackday-search-treasure
//
//  Created by Jang on 2016. 6. 22..
//  Copyright © 2016년 Jang. All rights reserved.
//

#include "treasureResult.hpp"
#include "algorithm.hpp"

#include <locale>

const string tab = "\t";
const string space = " ";
const string fileFormat = ".txt";
const string inputFile = "/Users/jang/Desktop/hackday/termlist.txt";
const string highOutputFile = "/Users/jang/Desktop/hackday/highTermlistResult";
const string lowOutputFile = "/Users/jang/Desktop/hackday/lowTermlistResult";
const string singleResultPath = "/Users/jang/Desktop/hackday/single term results/";
wstring requestUrl = L"http://treasure.navercorp.com:8080/nx.search?query=";

std::mutex global_curl_mutex;
string data[THREADNUM];

int threadSafeIndex[THREADNUM];
int singleResultIndex[THREADNUM];
vector<wstring> terms[THREADNUM];

std::mutex g_num_mutex[THREADNUM];

void saveValuableTerm(int id, KINSEARCH term){
    string filename;
    
    if (term.totalTreasure >= searchValue)
        filename = highOutputFile + to_string(id) + fileFormat;
    else if(term.totalTreasure >= 1)
        filename = lowOutputFile + to_string(id) + fileFormat;
    else
        return;
    
    ofstream outFile(filename.c_str(), std::ios_base::out | std::ios_base::app);
    outFile << term.query.c_str();
    outFile <<endl;
    outFile.close();
    
    //    pushQuery(term.query);
}


void saveSingleTermResult(int id, const char* filename, KINSEARCH data){
    //    wofstream outFile(filename);
    //
    //    outFile << data.totalTreasure << endl;
    //
    //    for(int i = 0; i < data.sections.size() ; i++){
    //        SECTION section = data.sections[i];
    //
    //        outFile << section.items.size() << endl;
    //
    //        for(int j=0; j < section.items.size(); j++){
    //            ITEM item = section.items[j];
    //
    //            outFile << item.key.c_str() << endl;
    //            outFile << item.treasure << endl;
    //        }
    //    }
    //
    //    outFile.close();
    saveValuableTerm(id, data);
}


size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up) {
    
    int id = *((int *)up);
    
//    global_curl_mutex.lock();
    
    for (int c = 0; c<size*nmemb; c++){
        data[id].push_back(buf[c]);
    }
    
    KINSEARCH result = parseTreasureResult(data[id]);
    
    if (result.query != ""){
        data[id].clear();
        string path = singleResultPath + to_string(singleResultIndex[id]) + fileFormat;
        saveSingleTermResult(id, path.c_str(), result);
    }
    
    g_num_mutex[id].unlock();
//    global_curl_mutex.unlock();
    
    return size*nmemb;
}

void searchSingletermResult(int id){
    
    while(threadSafeIndex[id]<terms[id].size()){
        
        char str[1000];
        
        g_num_mutex[id].lock();
        
        threadSafeIndex[id]++;
        
        wcout<<"Searching query: "<<terms[id][threadSafeIndex[id]]<<endl;
        
        wstring request = requestUrl + terms[id][threadSafeIndex[id]];
        
        singleResultIndex[id] = threadSafeIndex[id] + id*dataTerm;
        sprintf(str,"%ls", request.c_str());
        
        CURL* curl = curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_URL, str);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &id);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
        CURLcode res = curl_easy_perform(curl);
        
        curl_easy_cleanup(curl);
        
        if (CURLE_OK != res) g_num_mutex[id].unlock();
        
    }
}

void *t_function(void* data){
    
    int id = *((int *)data);
    
    searchSingletermResult(id);
    while(1);
}

int main(int argc, const char * argv[]) {
    // insert code here...
    
    int status;
    pthread_t p_thread[THREADNUM];
    
    std::locale::global(std::locale("ko_KR.UTF-8"));
    
    loadInputData(inputFile.c_str());
    for(int i=0; i<THREADNUM; i++){
        terms[i] = splitedTerms(i);
    }
    
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
    
    return 0;
}
