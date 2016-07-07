//
//  algorithm.hpp
//  Hackday-search-treasure
//
//  Created by Jang on 2016. 6. 23..
//  Copyright © 2016년 Jang. All rights reserved.
//

#ifndef algorithm_hpp
#define algorithm_hpp

#include <curl/curl.h>

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;

#define THREADNUM 14

const int combiNum = 10;

const int shiftTerm = 35000000;
const int searchValue = 15;
const int startIndexTerm = 0;
const int dataTerm = 1000000;

void loadInputData(const char* filename);
vector<wstring> splitedTerms(int id);

void pushQuery(string query);
//void writeTextFile(const char* filename);

#endif /* algorithm_hpp */
