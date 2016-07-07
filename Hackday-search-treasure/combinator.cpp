//
//  combinator.cpp
//  Hackday-search-treasure
//
//  Created by Jang on 2016. 6. 24..
//  Copyright © 2016년 Jang. All rights reserved.
//


#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <locale>
using namespace std;

const int startFileIdx = 4;
const int endFileIdx = 5;

const string fileFormat = ".txt";
const string inputFile = "/Users/jang/Desktop/hackday/lowTermlistResult";
const string outputFile = "/Users/jang/Desktop/hackday/combineResult.txt";
const string space = " ";

vector<string> terms;
vector<string> combines;

vector<string> readTextfile(const char* filename){
    
    string buff;
    fstream fs(filename);
    
    std::locale::global(std::locale("ko_KR.UTF-8")); // 맨 처음 한번 실행
    wcout.imbue(std::locale("ko_KR.UTF-8"));
    
    while ( getline(fs, buff) ){
        terms.push_back(buff);
    }
    
    cout<<terms.size()<<" terms push backed"<<endl;
    
    fs.close();
    
    return terms;
}

void insertCombine(vector<int> lists){
    
    ofstream outFile(outputFile.c_str(), std::ios_base::out | std::ios_base::app);
    
    string ret = "";
    
    for(int i=0; i<lists.size(); i++){
        ret += terms[lists[i]];
        ret += space;
    }
    
    outFile << ret.c_str() << endl;
    outFile.close();
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

void makeCombi(){
    
    for(int i=1; i<= 5; i++){
        vector<wstring> input;
        
        combine((int)terms.size(), i);
        
        cout<<"Finished "<<i<<" combi"<<endl;
    }
}

int main(){
    
    std::locale::global(std::locale("ko_KR.UTF-8"));
    
    for(int i=startFileIdx; i<=endFileIdx; i++){
        string inputDir = inputFile.c_str()+to_string(i)+fileFormat;
        
        readTextfile(inputDir.c_str());
    }
    makeCombi();
    
    return 0;
}
