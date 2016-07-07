//
//  treasureResult.cpp
//  Hackday-search-treasure
//
//  Created by Jang on 2016. 6. 22..
//  Copyright © 2016년 Jang. All rights reserved.
//


#include "treasureResult.hpp"


KINSEARCH parseTreasureResult(string contents) {
    KINSEARCH retVal = KINSEARCH();
    
    XMLDocument xmlDoc;
    
//    cout<<"Parsing contents length : " << contents.size() <<endl;
//    XMLError eResult = xmlDoc.LoadFile(contents.c_str());
    
    XMLError eResult = xmlDoc.Parse(contents.c_str());
    XMLCheckResult(eResult);
    
    XMLNode* pRoot = xmlDoc.FirstChild();
    if (pRoot == nullptr) return retVal;
    
    pRoot = pRoot->NextSibling();
    
    XMLElement * pElement = pRoot->FirstChildElement("query");
    
    if (pElement != nullptr){
        retVal.query = pElement->GetText();
    }
    
    pElement = pRoot->FirstChildElement("section");
    
    while (pElement != nullptr)
    {
        SECTION section;
        
        XMLElement* cElement = pElement->FirstChildElement("total");
        
        if (cElement != nullptr){
            XMLError eResult = cElement->QueryIntText(&section.total);
            XMLCheckResult(eResult);
        }
        
        cElement = pElement->FirstChildElement("item");
        while (cElement != nullptr){
            ITEM item;
            
            XMLElement* iElement = cElement->FirstChildElement("key");
            if (iElement != nullptr){
                item.key = iElement->GetText();
            }
            iElement = cElement->FirstChildElement("relevance");
            if (iElement != nullptr){
                eResult = iElement->QueryFloatText(&item.relevance);
                XMLCheckResult(eResult);
            }
            iElement = cElement->FirstChildElement("treasure");
            if (iElement != nullptr){
                eResult = iElement->QueryIntText(&item.treasure);
                
                retVal.totalTreasure += item.treasure;
                XMLCheckResult(eResult);
            }
            
//            SETStringValue(item.q_title, "q_title");
//            SETStringValue(item.q_content, "q_content");
//            SETStringValue(item.b_content, "b_content");
//            SETStringValue(item.a_content, "a_content");
            
            cElement = cElement->NextSiblingElement("item");
            section.items.push_back(item);
        }
        
        
        pElement = pElement->NextSiblingElement("section");
        retVal.sections.push_back(section);
    }
    
    return retVal;
}




