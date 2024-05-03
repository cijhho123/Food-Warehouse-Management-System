#pragma once
#include <string>
#include <vector>
using namespace std;

#include "Order.h"
#include "Customer.h"
#include "Volunteer.h"
#include "Parser.h"

class BaseAction;
class Volunteer;
class Customer;
class Parser;


// Warehouse responsible for Volunteers, Customers and Actions.


class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        void start();
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        const vector<BaseAction*> &getActions() const;
        void close();
        void open();

        //Rule of 5
        ~WareHouse();                                   //Destructor
        WareHouse(const WareHouse& other);              //Copy Constructor
        WareHouse& operator=(const WareHouse& other);   //Copy Assignment Operator
        WareHouse(WareHouse&& other) noexcept;          //Move Costructor
        WareHouse& operator=(WareHouse&& other) noexcept;//Move Assignment Operator

        //ceated methods
        int getTotalOrders();
        void increaseTotalOrders();
        int getCustomerCounter();
        int getVolunteerCounter();
        int getCurrentAmountOfVolunters();
        void decreaseCurrentAmountOfVolunters();
        int getNextAvailableVolunteer(int startIndex, Order* order, classType volType);
        
        int getInProcessOrdersIndex(Order* order);
        void cleanUselessVolunteers();
        bool getIsOpen();

        void createCivilianCustomer(string name, int locationDistance, int maxOrders);
        void createSoldierCustomer(string name, int locationDistance, int maxOrders);

        void createCollectorVolunteer(string name, int coolDown);
        void createLimitedCollectorVolunteer(string name, int coolDown ,int maxOrders);
        void createDriverVolunteer(string name, int maxDistance, int distancePerStep);
        void createLimitedDriverVolunteer(const string &name, int maxDistance, int distancePerStep,int maxOrders);

        vector<Order*>& getPendingOrders();
        vector<Order*>& getInProcessOrders();
        vector<Order*>& getCompletedOrders();

        void addPendingOrder(Order* order);
        void addInProcessOrder(Order* order);
        void addCompletedOrder(Order* order);
        void addCustomer(Customer* customer);
        void addVolunteer(Volunteer* volunteer);

        vector<BaseAction*>& getActions();
        vector<Volunteer*>& getVolunteers();
        vector<Customer*>& getCustomers();

        Customer* getCustomerById(int id);
        Volunteer* getVolunteerById(int id);

        vector<BaseAction*> deepCloneVector(const vector<BaseAction*>& originalVector);
        vector<Volunteer*> deepCloneVector(const vector<Volunteer*>& originalVector);
        vector<Order*> deepCloneVector(const vector<Order*>& originalVector);
        vector<Customer*> deepCloneVector(const vector<Customer*>& originalVector);


    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs

        //added members
        int totalOrders;    //the total amount of orders so far
        int currentAmountOfVolunters;

        
};
