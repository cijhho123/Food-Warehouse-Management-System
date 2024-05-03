#include "../include/Volunteer.h"
#include <iostream>

using std::string;

//-------------------------- Volunteer --------------------------

Volunteer::Volunteer(int id, const string &name): completedOrderId(NO_ORDER), activeOrderId(NO_ORDER), id(id), name(name){

}

Volunteer::~Volunteer(){};

int Volunteer::getId() const {
    return id;
}

const string& Volunteer::getName() const{
    return name;
}

int Volunteer::getActiveOrderId() const{
    return activeOrderId;
}

int Volunteer::getCompletedOrderId() const{
    return completedOrderId;
}

bool Volunteer::isBusy() const{
    return activeOrderId != NO_ORDER;
}


//-------------------------- CollectorVolunteer --------------------------
CollectorVolunteer::CollectorVolunteer(int id, string name, int coolDown): Volunteer(id, name), coolDown(coolDown), timeLeft(0), type(classType::COLLECTOR){

}

CollectorVolunteer* CollectorVolunteer::clone()const{
    return new CollectorVolunteer(*this);
}

//Simulate volunteer step,if the volunteer finished the order, transfer activeOrderId to completedOrderId
void CollectorVolunteer::step() {
    if(!isBusy())
        return;

    decreaseCoolDown();
}

int CollectorVolunteer::getCoolDown() const {
    return coolDown;
}

int CollectorVolunteer::getTimeLeft() const{
    return timeLeft;
}

void CollectorVolunteer::setTimeLeft(int time){
    timeLeft = time;
}

bool CollectorVolunteer::decreaseCoolDown(){
    timeLeft--;

    return (timeLeft == 0);
}

bool CollectorVolunteer::hasOrdersLeft() const {
    return true;
}

bool CollectorVolunteer::canTakeOrder(const Order &order) const {
    if(isBusy())
        return false;
    if(!hasOrdersLeft())
        return false;
    
    return true;
}

// Prepare for new order(Reset activeOrderId,TimeLeft,DistanceLeft,OrdersLeft depends on the volunteer type)
void CollectorVolunteer::acceptOrder(const Order &order) {
    activeOrderId = order.getId();
    timeLeft = coolDown;
}

string CollectorVolunteer::toString() const {
    string output = "";
    output = output + "VolunteerID: " + std::to_string(getId()) +"\n";

    if(isBusy()){
        output = output + "isBusy: True" + "\n";
        output = output + "OrderId: " + std::to_string(getActiveOrderId()) + "\n";
        output = output + "Time Left: " + std::to_string(getTimeLeft()) + "\n";
    } else {
        output = output + "isBusy: False" + std::to_string(isBusy()) + "\n";
    }
    
    output = output + "ordersLeft: No Limit" + "\n";

    return output;
}

classType CollectorVolunteer::getType() const {
    return type;
}

bool CollectorVolunteer::actUponFinish(){
    if (timeLeft == 0 && isBusy()){
        completedOrderId = activeOrderId;
        activeOrderId = NO_ORDER;
        timeLeft = NO_ORDER;

        return true;   
    }

    return false;
}

int CollectorVolunteer::taskProgress() const {
    return timeLeft;
}

int CollectorVolunteer::getLimitInfo(){
    return -1;
}

//-------------------------- LimitedCollectorVolunteer --------------------------

LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, string name, int coolDown ,int maxOrders): CollectorVolunteer(id, name, coolDown), maxOrders(maxOrders), ordersLeft(maxOrders){

}

LimitedCollectorVolunteer* LimitedCollectorVolunteer::clone() const {
    return new LimitedCollectorVolunteer(*this);
}

bool LimitedCollectorVolunteer::hasOrdersLeft() const {
    if(isBusy())
        return true;

    return ordersLeft > 0;
}

bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const {
    if(isBusy())
        return false;
    if(!hasOrdersLeft())
        return false;
    
    return true;
}

void LimitedCollectorVolunteer::acceptOrder(const Order &order) {
    activeOrderId = order.getId();
    setTimeLeft(getCoolDown());

    ordersLeft--;
}

int LimitedCollectorVolunteer::getMaxOrders() const{
    return maxOrders;
}

int LimitedCollectorVolunteer::getNumOrdersLeft() const{
    return ordersLeft;
}

string LimitedCollectorVolunteer::toString() const {
    string output = "";
    output = output + "VolunteerID: " + std::to_string(getId()) +"\n";

    if(isBusy()){
        output = output + "isBusy: True" + "\n";
        output = output + "OrderId: " + std::to_string(getActiveOrderId()) + "\n";
        output = output + "Time Left: " + std::to_string(getTimeLeft()) + "\n";
    } else {
        output = output + "isBusy: False" + std::to_string(isBusy()) + "\n";
    }
    
    output = output + "ordersLeft: No Limit" + "\n";

    return output;
}

bool LimitedCollectorVolunteer::actUponFinish(){
    if (getTimeLeft() == 0 && isBusy()){
        completedOrderId = activeOrderId;
        activeOrderId = NO_ORDER;
        setTimeLeft(NO_ORDER);
        

        return true;   
    }

    return false;
}

int LimitedCollectorVolunteer::getLimitInfo(){
    return ordersLeft;
}


//-------------------------- DriverVolunteer --------------------------
DriverVolunteer::DriverVolunteer(int id, string name, int maxDistance, int distancePerStep): Volunteer(id, name), type(classType::DRIVER), maxDistance(maxDistance), distancePerStep(distancePerStep), distanceLeft(0){

}

DriverVolunteer* DriverVolunteer::clone() const {
    return new DriverVolunteer(*this);
}

int DriverVolunteer::getDistanceLeft() const{
    return distanceLeft;
}

int DriverVolunteer::getMaxDistance() const{
    return maxDistance;
}

int DriverVolunteer::getDistancePerStep() const{
    return distancePerStep;
}

void DriverVolunteer::setDistanceLeft(int distance){
    distanceLeft = distance;
}

//Decrease distanceLeft by distancePerStep,return true if distanceLeft<=0,false otherwise
bool DriverVolunteer::decreaseDistanceLeft(){
    
    distanceLeft = std::max(distanceLeft - distancePerStep,0);

    return (distanceLeft == 0);
}

bool DriverVolunteer::hasOrdersLeft() const {
    return true;
}

// Signal if the volunteer is not busy and the order is within the maxDistance
bool DriverVolunteer::canTakeOrder(const Order &order) const {
    if(isBusy())
        return false;
    if(!hasOrdersLeft())
        return false;
    if(order.getDistance() > maxDistance)
        return false;
    
    return true;
} 

// Reset distanceLeft to maxDistance
void DriverVolunteer::acceptOrder(const Order &order) {
    activeOrderId = order.getId();
    distanceLeft = order.getDistance(); 

} 

// Decrease distanceLeft by distancePerStep
void DriverVolunteer::step() {
    if(!isBusy())
        return;

    decreaseDistanceLeft();
} 

string DriverVolunteer::toString() const{
    string output = "";
    output = output + "VolunteerID: " + std::to_string(getId()) +"\n";

    if(isBusy()){
        output = output + "isBusy: True" + "\n";
        output = output + "OrderId: " + std::to_string(getActiveOrderId()) + "\n";
        output = output + "DistanceLeft: " + std::to_string(getDistanceLeft()) + "\n";
    } else {
        output = output + "isBusy: False" + std::to_string(isBusy()) + "\n";
    }
    
    output = output + "ordersLeft: No Limit" + "\n";

    return output;
}

classType DriverVolunteer::getType() const {
    return type;
}

bool DriverVolunteer::actUponFinish(){
    if (distanceLeft == 0 && isBusy()){
        completedOrderId = activeOrderId;
        activeOrderId = NO_ORDER;
        distanceLeft = NO_ORDER;

        return true;   
    }

    return false;
}

int DriverVolunteer::taskProgress() const {
    return distanceLeft;
}

int DriverVolunteer::getLimitInfo(){
    return -1;
}


//-------------------------- LimitedDriverVolunteer --------------------------
LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep,int maxOrders):DriverVolunteer(id, name, maxDistance, distancePerStep), maxOrders(maxOrders), ordersLeft(maxOrders){

}

LimitedDriverVolunteer* LimitedDriverVolunteer::clone() const {
    return new LimitedDriverVolunteer(*this);
}

int LimitedDriverVolunteer::getMaxOrders() const{
    return maxOrders;
}

int LimitedDriverVolunteer::getNumOrdersLeft() const{
    return ordersLeft;
}

bool LimitedDriverVolunteer::hasOrdersLeft() const {
    if(isBusy())
        return true;
    
    return ordersLeft>0;
}



// Signal if the volunteer is not busy, the order is within the maxDistance and have orders left
bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const {
    if(isBusy())
        return false;
    if(!hasOrdersLeft())
        return false;
    if(order.getDistance() > getMaxDistance())
        return false;
    
    return true;
}

// Reset distanceLeft to maxDistance and decrease ordersLeft
void LimitedDriverVolunteer::acceptOrder(const Order &order) {
    activeOrderId = order.getId();
    setDistanceLeft(order.getDistance()); 

    ordersLeft--;
} 

string LimitedDriverVolunteer::toString() const{
    string output = "";
    output = output + "VolunteerID: " + std::to_string(getId()) +"\n";

    if(isBusy()){
        output = output + "isBusy: True" + "\n";
        output = output + "OrderId: " + std::to_string(getActiveOrderId()) + "\n";
        output = output + "DistanceLeft: " + std::to_string(getDistanceLeft()) + "\n";
    } else {
        output = output + "isBusy: False" + std::to_string(isBusy()) + "\n";
    }
    
    output = output + "ordersLeft: " + std::to_string(getNumOrdersLeft()) + "\n";

    return output;
}

bool LimitedDriverVolunteer::actUponFinish(){
    if (getDistanceLeft() == 0 && isBusy()){
        completedOrderId = activeOrderId;
        activeOrderId = NO_ORDER;
        setDistanceLeft(NO_ORDER);

        return true;   
    }

    return false;
}

int LimitedDriverVolunteer::getLimitInfo(){
    return ordersLeft;
}