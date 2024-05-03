#include "../include/WareHouse.h"
#include "../include/Parser.h"

#include <iostream>
#include <vector>
#include <fstream>



using std::cout;

WareHouse:: WareHouse(const string &configFilePath): isOpen(false), actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), 
    completedOrders(), customers(), customerCounter(0), volunteerCounter(0), totalOrders(0), currentAmountOfVolunters(0) {

    //load a saved state from the config file
    Parser pars = Parser(*this);
    pars.ParseFromConfigFile(configFilePath);

};


void WareHouse:: start(){

    WareHouse::open();
    Parser pars = Parser(*this);

    //start the program loop

    while(isOpen){
        //get the next user input
        string input ;
        getline(cin, input);

        //parse it
        vector<string> words = pars.seperateBySpace(input);

        //check what instruction was received and act accordingly
        pars.actBasedOnInput(words);
    }


    //free all memory by end of AF through the destructor 
}


void WareHouse:: open (){
    isOpen = true;
    cout << "Warehouse is open! \n";
}

void WareHouse::close(){
    //print all orders with their status
    for(Order* i : pendingOrders) 
        cout << "OrderID: " + std::to_string((*i).getId())+", "+ "CustomerID: "+ std::to_string((*i).getCustomerId())+ 
            ", Status: "+ (*i).getStatusAsString() + "\n";

    for(Order* i : inProcessOrders) 
        cout << "OrderID: " + std::to_string((*i).getId())+", "+ "CustomerID: "+ std::to_string((*i).getCustomerId())+ 
            ", Status: "+ (*i).getStatusAsString() + "\n";

    for(Order* i : completedOrders) 
        cout << "OrderID: " + std::to_string((*i).getId())+", "+ "CustomerID: "+ std::to_string((*i).getCustomerId())+ 
            ", Status: "+ (*i).getStatusAsString() + "\n";

    //change isOpen to close to exit the loop
    isOpen = false;

}

void WareHouse::addOrder(Order* order){
    pendingOrders.push_back(order);
    totalOrders++;
}

void WareHouse::addAction(BaseAction* action){
    actionsLog.push_back(action);
}

Customer& WareHouse::getCustomer(int customerId) const{
    Customer& temp =  (*customers[customerId]);
    return temp;
}

//this function is not used by the program and is kept for archiving's sake  
Volunteer& WareHouse::getVolunteer(int volunteerId) const{
    for(int i=0; (unsigned)i < volunteers.size(); i++){
        if(volunteers[i]->getId() == volunteerId)
            return *volunteers[i];
    }

    //if we didn't find any,return a dummy object
    // ID: -774     NAME: ERROR_ON_getVolunteer    COOLDOWN: 1000
    CollectorVolunteer* errorVol = new CollectorVolunteer(-774, "ERROR_ON_getVolunteer", 1000);
    return (*errorVol);
}

Order& WareHouse::getOrder(int orderId) const{
    for(int i=0; (unsigned)i < pendingOrders.size(); i++)
        if(pendingOrders[i]->getId() == orderId)
            return *pendingOrders[i];

    for(int i=0; (unsigned)i < inProcessOrders.size(); i++)
        if(inProcessOrders[i]->getId() == orderId)
            return *inProcessOrders[i];

    for(int i=0; (unsigned)i < completedOrders.size(); i++)
        if(completedOrders[i]->getId() == orderId)
            return *completedOrders[i];
    
    
    //if we didn't find any,return a dummy object
    // ID: -774     CUSTOMERID: -774     DISTANCE: 1000
    Order* errorOrd = new Order(-774, -774 , 1000);
    return (*errorOrd);
}



const vector<BaseAction*>& WareHouse::getActions() const{
    return actionsLog;
}

//---------- Rule of 5 ----------
//Destructor
WareHouse::~WareHouse(){

    for (BaseAction* action : actionsLog){
        delete action;
    }
    actionsLog.clear();

    for(Volunteer* vol : volunteers){
        delete vol;
    }
    volunteers.clear();

    for(Order* pending : pendingOrders){
        delete pending;
    }
    pendingOrders.clear();

    for(Order* processing : inProcessOrders){
        delete processing; 
    }
    inProcessOrders.clear();

    for(Order* comp : completedOrders){
        delete comp;
    }
    completedOrders.clear();

    for(Customer* cust : customers){
        delete cust;
    }
    customers.clear();

}    

//Copy Constructor
WareHouse::WareHouse(const WareHouse& other): isOpen(other.isOpen), actionsLog(deepCloneVector(other.actionsLog)), 
    volunteers(deepCloneVector(other.volunteers)), pendingOrders(deepCloneVector(other.pendingOrders)),
    inProcessOrders(deepCloneVector(other.inProcessOrders)), completedOrders(deepCloneVector(other.completedOrders)),
    customers(deepCloneVector(other.customers)), customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter),
    totalOrders(other.totalOrders), currentAmountOfVolunters(other.currentAmountOfVolunters) {
    
}   

//Copy Assignment Operator
WareHouse& WareHouse::operator=(const WareHouse& other) {
    if(&other != this){
        isOpen = other.isOpen;

        for(BaseAction* ba : actionsLog)
            delete ba;
        actionsLog.clear();
        actionsLog = deepCloneVector(other.actionsLog);

        for(Volunteer* vol : volunteers)
            delete vol;
        volunteers.clear();
        volunteers = deepCloneVector(other.volunteers);

        for(Order* ord : pendingOrders)
            delete ord;
        pendingOrders.clear();
        pendingOrders =  deepCloneVector(other.pendingOrders);

        for(Order* ord : inProcessOrders)
            delete ord;
        inProcessOrders.clear();
        inProcessOrders =  deepCloneVector(other.inProcessOrders);

        for(Order* ord : completedOrders)
            delete ord;
        completedOrders.clear();
        completedOrders =  deepCloneVector(other.completedOrders);

        for(Customer* cust : customers)
            delete cust;
        customers.clear();
        customers =  deepCloneVector(other.customers);

        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        totalOrders = other.totalOrders;
        currentAmountOfVolunters = other.currentAmountOfVolunters;
    }

    return *this;
} 

//Move Costructor
WareHouse::WareHouse(WareHouse&& other) noexcept: isOpen(other.isOpen), actionsLog(std::move(other.actionsLog)),volunteers(std::move(other.volunteers)),
    pendingOrders(std::move(other.pendingOrders)), inProcessOrders(std::move(other.inProcessOrders)) ,completedOrders(std::move(other.completedOrders)), 
    customers(std::move(other.customers)), customerCounter(other.customerCounter) ,volunteerCounter(other.volunteerCounter), totalOrders(other.totalOrders), currentAmountOfVolunters(other.currentAmountOfVolunters){
}   

//Move Assignment Operator
WareHouse& WareHouse::operator=(WareHouse&& other) noexcept{
    if(&other != this){

        isOpen = other.isOpen;

        actionsLog=std::move(other.actionsLog);
        
        volunteers=std::move(other.volunteers);
        pendingOrders=std::move(other.pendingOrders);
        inProcessOrders=std::move(other.inProcessOrders);
        completedOrders=std::move(other.completedOrders);
        customers = std::move(other.customers);

        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        totalOrders = other.totalOrders;
        currentAmountOfVolunters = other.currentAmountOfVolunters;
    }

    return *this;
}


//perform deep copy of vector of pointers --- BaseAction ---
vector<BaseAction*> WareHouse::deepCloneVector(const vector<BaseAction*>& originalVector) {
    vector<BaseAction*> clonedVector;

    for (BaseAction* element : originalVector) {
        clonedVector.push_back((*element).clone());
    }

    return clonedVector;
}

//perform deep copy of vector of pointers --- Volunteer ---
vector<Volunteer*> WareHouse::deepCloneVector(const vector<Volunteer*>& originalVector) {
    vector<Volunteer*> clonedVector;

    for (Volunteer* element : originalVector) {
        clonedVector.push_back((*element).clone());
    }

    return clonedVector;
}

//perform deep copy of vector of pointers --- Order ---
vector<Order*> WareHouse::deepCloneVector(const vector<Order*>& originalVector) {
    vector<Order*> clonedVector;

    for (Order* element : originalVector) {
        clonedVector.push_back((*element).clone());
    }

    return clonedVector;
}

//perform deep copy of vector of pointers --- Customers ---
vector<Customer*> WareHouse::deepCloneVector(const vector<Customer*>& originalVector) {
    vector<Customer*> clonedVector;

    for (Customer* element : originalVector) {
        clonedVector.push_back((*element).clone());
    }

    return clonedVector;
}


//---------- added functions ----------

int WareHouse::getCustomerCounter(){
    return customerCounter;
}

int WareHouse::getVolunteerCounter(){
    return volunteerCounter;
}

void WareHouse::createCivilianCustomer(string name, int locationDistance, int maxOrders){
    //create a new customer of type civilian
    CivilianCustomer* civCust = new CivilianCustomer(customerCounter, name, locationDistance, maxOrders);
    customerCounter ++;

    //add it to the data structure
    customers.push_back(std::move(&(*civCust)));   

}

void WareHouse::createSoldierCustomer(string name, int locationDistance, int maxOrders){
    //create a new customer of type soldier
    SoldierCustomer* civSol = new SoldierCustomer(customerCounter, name, locationDistance, maxOrders);
    customerCounter ++;

    //add it to the data structure
    customers.push_back(std::move(&(*civSol)));
}

void WareHouse::createCollectorVolunteer(string name, int coolDown){
    //create a new Volunteer of type collector (unlimited)
    CollectorVolunteer* volcol = new CollectorVolunteer(volunteerCounter, name, coolDown);
    volunteerCounter ++;
    currentAmountOfVolunters++;

    //add it to the data structure
    volunteers.push_back(std::move(&(*volcol)));
}

void WareHouse::createLimitedCollectorVolunteer(string name, int coolDown ,int maxOrders){
    //create a new Volunteer of type collector (limited)
    LimitedCollectorVolunteer* volcollimit = new LimitedCollectorVolunteer(volunteerCounter, name, coolDown, maxOrders);

    volunteerCounter ++;
    currentAmountOfVolunters++;

    //add it to the data structure
    volunteers.push_back(volcollimit);
    
}

void WareHouse::createDriverVolunteer(string name, int maxDistance, int distancePerStep){
    //create a new Volunteer of type driver (unlimited)
    DriverVolunteer* drivol = new DriverVolunteer(volunteerCounter, name, maxDistance, distancePerStep);
    volunteerCounter ++;
    currentAmountOfVolunters++;

    //add it to the data structure
    volunteers.push_back(std::move(&(*drivol)));
}

void WareHouse::createLimitedDriverVolunteer(const string &name, int maxDistance, int distancePerStep,int maxOrders){
    //create a new Volunteer of type driver (limited)
    LimitedDriverVolunteer* limdrivol = new LimitedDriverVolunteer(volunteerCounter, name, maxDistance, distancePerStep, maxOrders);
    volunteerCounter ++;
    currentAmountOfVolunters++;

    //add it to the data structure
    volunteers.push_back(std::move(&(*limdrivol)));
}

vector<Order*>& WareHouse::getPendingOrders(){
    return pendingOrders;
}

vector<Order*>& WareHouse::getInProcessOrders(){
    return inProcessOrders;
}

vector<Order*>& WareHouse::getCompletedOrders(){
    return completedOrders;
}

vector<BaseAction*>& WareHouse::getActions() {
    return actionsLog;
}

vector<Volunteer*>& WareHouse::getVolunteers() {
    return volunteers;
}

vector<Customer*>& WareHouse::getCustomers(){
    return customers;
}

int WareHouse::getCurrentAmountOfVolunters(){
    return currentAmountOfVolunters;
}

void WareHouse::decreaseCurrentAmountOfVolunters(){
    currentAmountOfVolunters --;
}

int WareHouse::getTotalOrders(){
    return totalOrders;
}

void WareHouse::increaseTotalOrders(){
    totalOrders ++;
}

Customer* WareHouse::getCustomerById(int id){
    for(Customer* curr : customers){
        if(curr->getId() == id)
            return curr;
    }

    //if no such customer exist, return null
    return NULL;
}

Volunteer* WareHouse::getVolunteerById(int id){
    for(Volunteer* curr : volunteers){
        if(curr->getId() == id)
            return curr;
    }

    //if no such customer exist, return null
    return NULL;
}

int WareHouse::getInProcessOrdersIndex(Order* order){
     for(int i=0; (unsigned)i < inProcessOrders.size(); i++)
        if((*order).getId() == (*inProcessOrders[i]).getId())
            return i;
     
     return -1; //if this order is not in the pending orders vetor
}

int WareHouse::getNextAvailableVolunteer(int startIndex, Order* order, classType volType){
    if((unsigned)startIndex >= volunteers.size())
        return -1;

    Volunteer* tempVolunteer = volunteers[startIndex];      
    while(tempVolunteer->getType() != volType ||                                                //wrong type
        (tempVolunteer->getType() == volType && (! tempVolunteer->canTakeOrder((*order)))))  //right type but can't take the order
        {
        startIndex ++;

        if((unsigned)startIndex >= volunteers.size())
            return -1;

        tempVolunteer = volunteers[startIndex];
    }

    if((unsigned)startIndex >= volunteers.size())
        return -1;
    else 
        return startIndex;
}

void WareHouse::cleanUselessVolunteers(){
    for(int i=0; (unsigned)i < volunteers.size(); i++){
        if(! (*volunteers[i]).hasOrdersLeft()){
            delete (volunteers[i]);
            volunteers.erase(volunteers.begin() + i);
            i --;
        }
    }
}

bool WareHouse::getIsOpen(){
    return isOpen;
}


///Adders
void WareHouse::addPendingOrder(Order* order){
    pendingOrders.push_back(order);
}

void WareHouse::addInProcessOrder(Order* order){
    inProcessOrders.push_back(order);
}

void WareHouse::addCompletedOrder(Order* order){
    completedOrders.push_back(order);
}

void WareHouse::addCustomer(Customer* customer){
    customers.push_back(customer);
}   

void WareHouse::addVolunteer(Volunteer* volunteer){
    volunteers.push_back(volunteer);
}