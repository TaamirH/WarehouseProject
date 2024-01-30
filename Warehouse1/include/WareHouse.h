#pragma once
#include <string>
#include <vector>

#include "Order.h"
#include "Customer.h"

class BaseAction;
class Volunteer;
class Customer;
class  Action;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.


class WareHouse {

    public:
        std::string trimLeadingWhitespace(const std::string &str);
        void parseFile(const string &filePath);
        WareHouse(const string &configFilePath);
        WareHouse(WareHouse &other);
        WareHouse &operator=(const WareHouse &other);
        ~WareHouse();
        WareHouse(WareHouse &&other) ;
        WareHouse& operator=(WareHouse &&other) ;
        void start();
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        void addCustomer(Customer* customer);
        void addVolunteer (Volunteer* volunteer);
        void printActionsLogs();
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        const vector<BaseAction*> &getActionsLog() const;
        int getCustomerCounter() const;
        int getVolunteerCounter() const;
        int getOrderCounter() const;
        const vector <Order*> &getPendingOrders () const;
        const vector <Order*> &getProcessingOrders () const;
        const vector <Order*> &getCompletedOrders () const;
        const vector <Volunteer*> &getVolunteers () const;
        void close();
        void open();
        void moveOrder(Order *order,int id);
        void deleteVol(Volunteer* vol);
        const vector <int>& getDeletedVolunteers() const;
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
        int orderCounter;
        vector <int> deletedVolunteers;

};