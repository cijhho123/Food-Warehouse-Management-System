#pragma once

#include "WareHouse.h"
#include "Action.h"

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using std::cout;
using std::string;
using std::back_inserter;

class WareHouse;
class BaseAction;

class Parser {
    public:
    Parser(WareHouse &wh);
    void ParseFromConfigFile(const string &path);
    vector<string>  seperateBySpace (string& str);
    void actBasedOnInput(vector<std::string> words);
    void actBasedOnInputConfig(vector<std::string> words);
    

    private:
        WareHouse& wh;
        
};