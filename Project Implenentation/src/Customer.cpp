#include "../include/Customer.h"
#include <iostream>

using std::string;

Customer::Customer(int id, const string &name, int locationDistance, int maxOrders): id(id), name(name), locationDistance(locationDistance),
     maxOrders(maxOrders), ordersId()  {
}

Customer::~Customer(){};

const string& Customer::getName() const{
    return name;
}

int Customer::getId() const{
    return id;
}

int Customer::getCustomerDistance() const{
    return locationDistance;
}

int Customer::getMaxOrders() const{
    return maxOrders;
}

int Customer::getNumOrders() const{
    return ordersId.size();
}

bool Customer::canMakeOrder() const{
    return (getNumOrdersLeft() > 0);
}

const vector<int>& Customer::getOrdersIds() const {
    return ordersId;
}

int Customer::getNumOrdersLeft() const{
    return (maxOrders - getNumOrders());
}

int Customer::addOrder(int orderId){
    //check if an order can be made 
    if (!canMakeOrder())
        return -1;
    
    //add the order to the customer's list
    ordersId.push_back(orderId);
    return orderId;
}

const string Customer::toString() const{
    string output = "";
    output = output + "CustomerID: " + std::to_string(getId()) +"\n";
    output = output + "numOrdersLeft: " + std::to_string(getNumOrdersLeft()) + "\n";

    return output;
}

void Customer::getReceipt(int order_id){
    ordersId.push_back(order_id);
}



//-----------------  SoldierCustomer  -----------------
SoldierCustomer::SoldierCustomer(int id, string name, int locationDistance, int maxOrders): Customer(id, name, locationDistance, maxOrders){

}

SoldierCustomer* SoldierCustomer::clone() const {
    return new SoldierCustomer(*this);
}

//-----------------  CivilianCustomer  -----------------
CivilianCustomer::CivilianCustomer(int id, string name, int locationDistance, int maxOrders): Customer(id, name, locationDistance, maxOrders){

}

CivilianCustomer* CivilianCustomer::clone() const {
    return new CivilianCustomer(*this);
}
