#include "../include/Action.h"


extern WareHouse* backup;

//---------------------------- BaseAction ----------------------------
BaseAction::BaseAction(): errorMsg("ERROR: "), status(ActionStatus::COMPLETED){
    
};

ActionStatus BaseAction::getStatus() const{
    return status;
}

void BaseAction::complete(){
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string newErrorMsg){
    status = ActionStatus::ERROR;
    errorMsg = newErrorMsg;

    cout << "ERROR: " + errorMsg + "\n";
}

string BaseAction::getErrorMsg() const{
    return errorMsg;
}

BaseAction::~BaseAction(){
    
}


//---------------------------- SimulateStep ----------------------------
SimulateStep::SimulateStep(int numOfSteps): numOfSteps(numOfSteps){

}

void SimulateStep::act(WareHouse &wareHouse) {

    for(int i=0; i < numOfSteps; i++)
        singleStep(wareHouse);

    complete();
}

void SimulateStep::singleStep(WareHouse &wareHouse){

    //step 1 - proceed with the handling of each pending order
    int collectorIndex = 0, driverIndex = 0;

    for(int i=0; (unsigned)i < wareHouse.getPendingOrders().size(); i++){
        Order* currentOrder = (wareHouse.getPendingOrders()[i]);        
        
        if((*currentOrder).getStatus() == OrderStatus::PENDING){ //if the order is waiting for a collector

            //go to the next collector volunteer  who can handle the order   
            collectorIndex =  wareHouse.getNextAvailableVolunteer(collectorIndex, currentOrder, classType::COLLECTOR);                                                                


            //check if the a volunteer with such ID exist
            if(collectorIndex == -1){
                collectorIndex = wareHouse.getVolunteers().size() + 1;  //ensure we won't look for collectors in this step anymore
                continue;
            }
            
            Volunteer& tempVolunteer = *(wareHouse.getVolunteers()[collectorIndex]);

            //check if the current collector can take the current order
            if(tempVolunteer.getType() == classType::COLLECTOR && tempVolunteer.canTakeOrder((*currentOrder))){
                
                (*currentOrder).setStatus(OrderStatus::COLLECTING);
                (*currentOrder).setCollectorId(tempVolunteer.getId());

                tempVolunteer.acceptOrder(((*currentOrder)));
                wareHouse.addInProcessOrder((currentOrder));

                wareHouse.getPendingOrders().erase((wareHouse.getPendingOrders().begin() + i));

                //adjust for the next order (because we've deleted one order from the process vector)
                i --;
            }

            collectorIndex ++;

        } else {    //if the order is waiting for a driver

            //go to the next collector volunteer  who can handle the order   
            driverIndex =  wareHouse.getNextAvailableVolunteer(driverIndex, currentOrder, classType::DRIVER);                                                                


            //check if the a volunteer with such ID exist
            if(driverIndex == -1){
                driverIndex = wareHouse.getVolunteers().size() + 1;  //ensure we won't look for drivers in this step anymore
                continue;
            }

            Volunteer& tempVolunteer = *(wareHouse.getVolunteers()[driverIndex]);   // check!

            //check if the current collector can take the current order
            if(tempVolunteer.getType() == classType::DRIVER && tempVolunteer.canTakeOrder((*currentOrder))){
                
                (*currentOrder).setStatus(OrderStatus::DELIVERING);
                (*currentOrder).setDriverId(tempVolunteer.getId());

                tempVolunteer.acceptOrder(((*currentOrder)));
                wareHouse.addInProcessOrder((currentOrder));

                wareHouse.getPendingOrders().erase((wareHouse.getPendingOrders().begin() + i));

                //adjust for the next order (because we've deleted one order from the process vector)
                i --;
                
            }

            driverIndex ++;
        }
    }

    //step 2 – decrease the timeLeft by one for each collector, and decrease the distanceLeft of a driver by distancePerStep
    for(Volunteer* vol : wareHouse.getVolunteers()){
        vol->step();
    }

    //step 3 - check whether jobs are done 
    for(int i=0; (unsigned)i < wareHouse.getVolunteers().size(); i++){   
        Volunteer* vol = (wareHouse.getVolunteers()[i]);
        if((*vol).actUponFinish()){ //return true if the current task is finished

            //mark the order as complete its current stage
            Order& currentOrder = wareHouse.getOrder((*vol).getCompletedOrderId());

            if(currentOrder.getStatus() == OrderStatus::COLLECTING){         //if collection have ended

                wareHouse.addPendingOrder(&currentOrder);
                wareHouse.getInProcessOrders().erase(wareHouse.getInProcessOrders().begin() + wareHouse.getInProcessOrdersIndex(&currentOrder));   

            }else {                                          //if delivery have ended

                currentOrder.setStatus(OrderStatus::COMPLETED);
                wareHouse.addCompletedOrder(&currentOrder);

                wareHouse.getInProcessOrders().erase(wareHouse.getInProcessOrders().begin() + wareHouse.getInProcessOrdersIndex(&currentOrder));   
            }
        }
    }

    ///new step 4
    wareHouse.cleanUselessVolunteers();
}

std::string SimulateStep::toString() const {
    string output = "";
    output += "simulationStep " + std::to_string(numOfSteps)+ " ";

    if(getStatus() == ActionStatus::COMPLETED)
        output += " COMPLETED";
    else 
        output += " ERROR";

    return output;
}

SimulateStep* SimulateStep::clone() const {
    return new SimulateStep(*this);
}

SimulateStep::~SimulateStep(){
}


//---------------------------- AddOrder ----------------------------
AddOrder::AddOrder(int id): customerId(id) {

}

void AddOrder::act(WareHouse &wareHouse) {
    Customer* tempCustomer = wareHouse.getCustomerById(customerId);

    if(tempCustomer != NULL && tempCustomer->canMakeOrder()){   

        (*tempCustomer).getReceipt(wareHouse.getTotalOrders());    

        wareHouse.addPendingOrder(new Order(wareHouse.getTotalOrders(), customerId, ((*tempCustomer).getCustomerDistance())));   
        wareHouse.increaseTotalOrders();
        
        complete();
        
    } else {
        error("Cannot place this order");
    }
    
}

string AddOrder::toString() const {
    string output = "order " + std::to_string(customerId) + " ";
    if(getStatus() == ActionStatus::COMPLETED){
        output += "COMPLETED";
    } else {
        output += "ERROR";
    }
    return output;
}

AddOrder* AddOrder::clone() const {
    return new AddOrder(*this);
}

AddOrder::~AddOrder(){

}

//---------------------------- AddCustomer ----------------------------
AddCustomer::AddCustomer(string customerName, string providedCustomerType, int distance, int maxOrders): customerName(customerName),
    customerType(chooseType(providedCustomerType)),distance(distance), maxOrders(maxOrders), customerId(-1),customerTypeString(providedCustomerType){

}

CustomerType AddCustomer::chooseType(string str) const {
    if(str == "soldier" || str == "Soldier")
        return CustomerType::Soldier;
    else   
        return CustomerType::Civilian;
}

void AddCustomer::act(WareHouse &wareHouse) {
    if(customerType == CustomerType::Civilian)
        wareHouse.createCivilianCustomer(customerName, distance, maxOrders);
    else
        wareHouse.createSoldierCustomer(customerName, distance, maxOrders);

    customerId = wareHouse.getCustomerCounter() - 1;

    complete();
}

AddCustomer* AddCustomer::clone() const {
    return new AddCustomer(*this);
}

string AddCustomer::toString() const {
    string output = "";
    output += "customer " + customerName + " " + customerTypeString  + " " + std::to_string(distance) + " " + std::to_string(maxOrders) + " ";

    if(getStatus() == ActionStatus::COMPLETED){
        output += "COMPLETED";
    } else {
        output += "ERROR";
    }

    return output;
}

AddCustomer::~AddCustomer(){

}

//---------------------------- PrintOrderStatus ----------------------------
PrintOrderStatus::PrintOrderStatus(int id): orderId(id){

}

void PrintOrderStatus::act(WareHouse &wareHouse) {
    if( 0 <= orderId && orderId < wareHouse.getTotalOrders()){
        string output = wareHouse.getOrder(orderId).toString();
        cout << output;

        complete();
    } else {
        this->error("Order Doesn't exist");
    }
}

PrintOrderStatus* PrintOrderStatus::clone() const {
    return new PrintOrderStatus(*this);
}

string PrintOrderStatus::toString() const {
    string output = "orderStatus " + std::to_string(orderId) + " ";

    if(getStatus() == ActionStatus::COMPLETED){
        output += "COMPLETED";
    } else {
        output += "ERROR";
    }

    return output;
}

PrintOrderStatus::~PrintOrderStatus(){

}

//---------------------------- PrintCustomerStatus ----------------------------
PrintCustomerStatus::PrintCustomerStatus(int customerId): customerId(customerId) {

}

void PrintCustomerStatus::act(WareHouse &wareHouse) {
    if(customerId >= wareHouse.getCustomerCounter() || customerId < 0){
        error("Customer doesn’t exist");
        return;
    } else {
        string output = "";
        output += "CustomerID: " + std::to_string(customerId) + "\n";

        for (int order: wareHouse.getCustomer(customerId).getOrdersIds()){
            output += "OrderID: " + std::to_string(order) + "\n";
            output += "OrderStatus: " + wareHouse.getOrder(order).getStatusAsString() + "\n";
        }

        output += "numOrdersLeft: " + std::to_string(wareHouse.getCustomer(customerId).getNumOrdersLeft()) + "\n";

        cout << output;
        complete();
    }
}

PrintCustomerStatus* PrintCustomerStatus::clone() const {
    return new PrintCustomerStatus(*this);
}

string PrintCustomerStatus::toString() const {
    string output = "";
    output += "customerStatus " + std::to_string(customerId) + " ";

    if(getStatus() == ActionStatus::COMPLETED){
        output += "COMPLETED";
    } else {
        output += "ERROR";
    }

    return output;

}

PrintCustomerStatus::~PrintCustomerStatus(){

}

//---------------------------- PrintVolunteerStatus ----------------------------
PrintVolunteerStatus::PrintVolunteerStatus(int id): volunteerId(id){

}

void PrintVolunteerStatus::act(WareHouse &wareHouse) {
    string output = "";
    Volunteer* tempVolunteer = wareHouse.getVolunteerById(volunteerId);

    if(tempVolunteer != NULL){
        output += "VolunteerID: " + std::to_string(volunteerId) + "\n";

        output += "isBusy: ";
        if((*tempVolunteer).isBusy()){
            output += "True \n";
            output += "OrderID: " + std::to_string((*tempVolunteer).getActiveOrderId()) + "\n";
            output += "timeLeft: " + std::to_string((*tempVolunteer).taskProgress()) + "\n";
        }else {
            output += "False \n";
            output += "OrderID: None \n";
            output += "timeLeft: None \n" ;
        }

        int numOfOders = (*tempVolunteer).getLimitInfo(); // (-1) means no limit
        if (numOfOders != (-1))
            output += "OrdersLeft: " + std::to_string(numOfOders) + "\n";
        else
            output += "OrdersLeft: No Limit \n";
        
        
        cout << output;
        
        complete();
    } else {
        error("Volunteer doesn’t exist");
    }
}

PrintVolunteerStatus* PrintVolunteerStatus::clone() const {
    return new PrintVolunteerStatus(*this);
}

string PrintVolunteerStatus::toString() const {
    string output = "";
    output += "volunteerStatus " + std::to_string(volunteerId) +" ";

    if(getStatus() == ActionStatus::COMPLETED){
        output += "COMPLETED";
    } else {
        output += "ERROR";
    }

    return output;   
}

PrintVolunteerStatus::~PrintVolunteerStatus(){

}

//---------------------------- PrintActionsLog ----------------------------
PrintActionsLog::PrintActionsLog(){

}

void PrintActionsLog::act(WareHouse &wareHouse) {
    vector<BaseAction*> pastActions = wareHouse.getActions();

    for(BaseAction* action : pastActions){
        cout << (*action).toString() + "\n";
    }
}

PrintActionsLog* PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}

string PrintActionsLog::toString() const {
    return "log COMPLETED";
}

PrintActionsLog::~PrintActionsLog(){

}

//---------------------------- Close ----------------------------
Close::Close(){

}

void Close::act(WareHouse &wareHouse) {
    wareHouse.close();
}

Close* Close::clone() const {
    return new Close(*this);
}

string Close::toString() const {
    return "close COMPLETED";
}

Close::~Close(){

}

//---------------------------- BackupWarehouse ----------------------------
BackupWareHouse::BackupWareHouse(){

}

void BackupWareHouse::act(WareHouse &wareHouse) {
    //delete old backup (if exist)
    if(backup != nullptr){
    	delete backup;
    	backup = nullptr;
    }

    WareHouse* temp = new WareHouse(wareHouse);  //calling the copy constructor
    backup = temp;
}

BackupWareHouse* BackupWareHouse::clone() const {
    return new BackupWareHouse(*this);
}

string BackupWareHouse::toString() const {
    return "Backup COMPLETED";
}

BackupWareHouse::~BackupWareHouse(){

}

//---------------------------- RestoreWarehouse ----------------------------
RestoreWareHouse::RestoreWareHouse(){

}

void RestoreWareHouse::act(WareHouse &wareHouse) {
    if(backup != nullptr){
        wareHouse = (*backup);

        complete();
    } else {
        error("No backup available");
    }
}

RestoreWareHouse* RestoreWareHouse::clone() const {
    return new RestoreWareHouse(*this);
}

string RestoreWareHouse::toString() const {
    string output = "Restore Backup ";

    if(getStatus() == ActionStatus::COMPLETED){
        output += "COMPLETED";
    } else {
        output += "ERROR";
    }

    return output;  
}

RestoreWareHouse::~RestoreWareHouse(){

}