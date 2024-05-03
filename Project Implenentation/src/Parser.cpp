#include "../include/Parser.h"


//Text file parser
Parser::Parser(WareHouse& wh): wh(wh){

}

void Parser::ParseFromConfigFile(const string &path){
    std::ifstream file(path);

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            //std::cout << line + "\n"; -- print each line, for debug only

            //seperate line be space delimiter
            std::vector<std::string> words = seperateBySpace(line);
            if(words.size() == 0)   // if the line is empty
                continue;
            
            //check on which case we are
            actBasedOnInputConfig(words);
        }
        file.close();
    }
}

vector<string>  Parser::seperateBySpace (string& str){
    std::stringstream ss(str);       // Insert the string into a stream
    std::string buf;    //buffer 
    std::vector<std::string> tokens; // Create vector to hold our words

    while (ss >> buf)
        tokens.push_back(buf);

    return tokens;
}

void Parser::actBasedOnInputConfig(vector<std::string> words){
    //check for empty input
    if(words.size() == 0)
        return;

    //customer
    else if(words[0] == "customer"){

        if(words[2] == "soldier" || words[2] == "Soldier")
            wh.createSoldierCustomer(words[1], stoi(words[3]), stoi(words[4]));
        else
            wh.createCivilianCustomer(words[1], stoi(words[3]), stoi(words[4]));
    }
    
    //volunteer (only in the setup stage)
    else if(words[0] == "volunteer"){
        if(words[2] == "collector")
            wh.createCollectorVolunteer(words[1], stoi(words[3]));
        else if(words[2] == "limited_collector")
            wh.createLimitedCollectorVolunteer(words[1], stoi(words[3]), stoi(words[4]));
        else if(words[2] == "driver")
            wh.createDriverVolunteer(words[1], stoi(words[3]), stoi(words[4]));
        else if(words[2] == "limited_driver")
            wh.createLimitedDriverVolunteer(words[1], stoi(words[3]), stoi(words[4]), stoi(words[5]));
    }

    
    //illegal input
    else {
        //cout << "Wrong input!";
    }
}

void Parser::actBasedOnInput(vector<std::string> words){
    //check for empty input
    if(words.size() == 0)
        return;

    //order
    else if(words[0] == "order"){
        AddOrder* addorder = new AddOrder(stoi(words[1]));
        addorder->act(wh);

        wh.getActions().push_back(addorder);
    }


    //customer
    else if(words[0] == "customer"){
        AddCustomer* addcustomer = new AddCustomer(words[1], words[2], stoi(words[3]), stoi(words[4]));
        addcustomer->act(wh);

        wh.getActions().push_back(addcustomer);
    }
    
    //volunteer (only in the setup stage)
    else if(words[0] == "volunteer"){
        if(words[2] == "collector")
            wh.createCollectorVolunteer(words[1], stoi(words[3]));
        else if(words[2] == "limited_collector")
            wh.createLimitedCollectorVolunteer(words[1], stoi(words[3]), stoi(words[4]));
        else if(words[2] == "driver")
            wh.createDriverVolunteer(words[1], stoi(words[3]), stoi(words[4]));
        else if(words[2] == "limited_driver")
            wh.createLimitedDriverVolunteer(words[1], stoi(words[3]), stoi(words[4]), stoi(words[5]));
    }

    //step
    else if(words[0] == "step"){
        SimulateStep* simulatestep = new SimulateStep(stoi(words[1]));
        simulatestep->act(wh);

        wh.addAction(simulatestep);
    }

    //orderstatus
    else if(words[0] == "orderStatus"){
        PrintOrderStatus* printorderstatus = new PrintOrderStatus(stoi(words[1]));
        printorderstatus->act(wh);

        wh.getActions().push_back(printorderstatus);
    }
    
    //customerStatus
    else if(words[0] == "customerStatus"){
        PrintCustomerStatus* printcustomerstatus = new PrintCustomerStatus(stoi(words[1]));
        printcustomerstatus->act(wh);

        wh.getActions().push_back(printcustomerstatus);
    }

    //volunteerStatus 
    else if(words[0] == "volunteerStatus"){
        PrintVolunteerStatus* printvolunteerstatus = new PrintVolunteerStatus(stoi(words[1]));
        printvolunteerstatus->act(wh);

        wh.getActions().push_back(printvolunteerstatus);
    }

    //log
    else if(words[0] == "log"){
        PrintActionsLog* printactionslog = new PrintActionsLog();
        printactionslog->act(wh);

        wh.getActions().push_back(printactionslog);
    }

    //close
    else if(words[0] == "close"){
        Close* close = new Close();
        close->act(wh);

        wh.getActions().push_back(close);
    }

    //backup
    else if(words[0] == "backup"){
        BackupWareHouse* backup = new BackupWareHouse();
        backup->act(wh);

        wh.getActions().push_back(backup);
    }

    //restore a backup
    else if(words[0] == "restore"){
        RestoreWareHouse* restore = new RestoreWareHouse();
        restore->act(wh);

        wh.getActions().push_back(restore);
    } 
    
    //illegal input
    else {
        //cout << "Wrong input!";
    }
}

