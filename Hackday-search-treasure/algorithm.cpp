//
//  algorithm.cpp
//  Hackday-search-treasure
//
//  Created by Jang on 2016. 6. 23..
//  Copyright © 2016년 Jang. All rights reserved.
//

#include "algorithm.hpp"

vector<wstring> allterms;

void loadInputData(const char* filename){
    wstring buff;
    wfstream fs(filename);
    
    std::locale::global(std::locale("ko_KR.UTF-8"));
    wcout.imbue(std::locale("ko_KR.UTF-8"));
    
    cout<<"Loading input data"<<endl;
    
    int count = 0;
    while ( getline(fs, buff) ) {
        count++;
        if(count<shiftTerm) continue;
        allterms.push_back(buff);
    }
    
    cout<<allterms.size()<<" all terms push backed"<<endl;
    
    fs.close();
}

vector<wstring> splitedTerms(int id){
    
    int startIndex = id*dataTerm;
    vector<wstring> terms;
    
    for(int i=startIndex+startIndexTerm; i<startIndex + dataTerm; i++){
        terms.push_back(allterms[i]);
    }
    
    cout<<"Thread id: "<<id<<" "<<terms.size()<<" terms push backed"<<endl;
    
    return terms;
}


std::mutex global_post_mutex;
vector<string> buffer;
vector<string> pushDataList;

void insert_combine(vector<int> lists){
    
    string ret = "";
    
    for(int i=0; i<lists.size(); i++){
        ret += buffer[lists[i]];
        ret += " ";
    }
    pushDataList.push_back(ret);
}

void combine(int n, int k){
    vector<int> selected;
    vector<int> selector(n);
    fill(selector.begin(), selector.begin() + k, 1);
    
    do {
        for (int i = 0; i < n; i++) {
            if (selector[i]) {
                selected.push_back(i);
            }
        }
        insert_combine(selected);
        selected.clear();
    }
    while (prev_permutation(selector.begin(), selector.end()));
}


void postQuery(){
    
    for (int i=0; i<pushDataList.size(); i+=5){
        CURL *curl = curl = curl_easy_init();
        if(curl) {
            string postData = "name=ingyure&answer=";
            
            for(int j=i; j<i+5; j++){
                if(j>=pushDataList.size()) return;
                postData += pushDataList[j];
                postData += "\n";
            }
            
            curl_easy_setopt(curl, CURLOPT_URL, "http://treasure.navercorp.com/campus_hackday/evaluation/");
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
            
            CURLcode res = curl_easy_perform(curl);
            
            if(res != CURLE_OK)
                fprintf(stderr, "curl_easy_perform() failed: %s\n",
                        curl_easy_strerror(res));
            
            curl_easy_cleanup(curl);
        }
    }
    
    pushDataList.clear();
}

void makeCombi(){
    
    for(int i=1; i<=3; i++){
        vector<wstring> input;
        combine(combiNum, i);
    }
    
    buffer.clear();
    postQuery();
}

void pushQuery(string query){
    
    global_post_mutex.lock();
    
    buffer.push_back(query);
    if (buffer.size() > combiNum){
        makeCombi();
    }
    
    global_post_mutex.unlock();
}





