#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

class BaseAction;
class Volunteer;

// Warehouse responsible for Volunteers, Customers and Actions.
//finish

class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        int i =1;
        int j=3;
        int l=5;
        void start();
        const vector<BaseAction*> &getActionsLog() const;
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        void printActionsLogs();
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        void close();
        void open();

    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> vol;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs

    
};