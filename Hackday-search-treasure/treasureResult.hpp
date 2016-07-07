//
//  treasureResult.hpp
//  Hackday-search-treasure
//
//  Created by Jang on 2016. 6. 22..
//  Copyright © 2016년 Jang. All rights reserved.
//

#ifndef treasureResult_hpp
#define treasureResult_hpp

#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return retVal; }

#define SETStringValue(variable, element) { iElement = cElement->FirstChildElement(element); if (iElement != nullptr){ if (iElement->GetText() != nullptr){ XMLPrinter printer;iElement->Accept(&printer); variable = printer.CStr(); } XMLCheckResult(eResult); }}

#include "tinyxml2.h"
using namespace tinyxml2;

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>


using namespace std;

typedef struct __item{
    
    string key;
    float relevance;
    int treasure;
    string q_title;
    string q_content;
    string b_content;
    string a_content;
} ITEM;


typedef struct __section{
    
    int total;
    vector<ITEM> items;
} SECTION;


typedef struct __kinsearch{
    
    int totalTreasure;
    string query;
    vector<SECTION> sections;
    
} KINSEARCH;


KINSEARCH parseTreasureResult(string contents);

#endif /* treasureResult_hpp */
