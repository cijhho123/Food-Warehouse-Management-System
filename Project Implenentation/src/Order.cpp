#include "../include/Order.h"
#include <iostream>

using std::string;

// 0 - PENDING   1 - COLLECTING  2 - DELIVERING  3 - COMPLETED

Order::Order(int id, int customerId, int distance): id(id), customerId(customerId), distance(distance), status(OrderStatus::PENDING),
    collectorId(NO_VOLUNTEER),driverId(NO_VOLUNTEER) {

};

int Order::getId() const {
    return id;
}

int Order::getCustomerId() const{
    return customerId;
}

void Order::setStatus(OrderStatus updatedStatus){
    status = updatedStatus;
}

void Order::setCollectorId(int updatedCollectorId){
    collectorId = updatedCollectorId;
}

void Order::setDriverId(int updatedDriverId){
    driverId = updatedDriverId;
}

int Order::getCollectorId() const {
    return collectorId;
}

int Order::getDriverId() const {
    return driverId;
}

OrderStatus Order::getStatus() const {
    return status;
}

int Order::getDistance()  const{
    return distance;
}


const string Order::toString() const {
    
    string output = "";
    output = output + "OrderId: " + std::to_string(getId()) +"\n";
    output = output + "OrderStatus: " + getStatusAsString() + "\n";
    output = output + "CustomerID: " + std::to_string(getCustomerId()) + "\n";

    //add collector and driver data
    std::string current_collector = "None", current_drive = "None";
    
    if (getStatus() >= OrderStatus::COLLECTING)
        current_collector = std::to_string(getCollectorId());
    output = output + "Collector: " + current_collector + "\n";

    if (getStatus() >= OrderStatus::DELIVERING)
        current_drive = std::to_string(getDriverId());
    output = output + "Driver: " + current_drive + "\n";

    return output;
}

string Order::getStatusAsString() const{
    int value = static_cast<int> (status);

    switch(value){
        case 0:
            return "Pending";
        case 1:
            return "Collecting";
        case 2:
            return "Delivering";
        case 3:
            return "Completed";
        default:
            return "N/A";
    }
}

Order* Order::clone() const {
    return new Order(*this);
}