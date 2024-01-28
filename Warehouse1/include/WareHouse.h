#pragma once
#include <string>
#include <vector>

#include "Order.h"
#include "Customer.h"

class BaseAction;
class Volunteer;
class Customer;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.


class WareHouse {

    public:
        std::string trimLeadingWhitespace(const std::string &str);
        void parseFile(const string &filePath);
        WareHouse(const string &configFilePath);
        WareHouse(WareHouse &other);
        WareHouse &operator=(const WareHouse &other);
        ~WareHouse();
        WareHouse(WareHouse &&other) noexcept;
        WareHouse& operator=(WareHouse &&other) noexcept;
        void start();
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        void addCustomer(Customer* customer);
        void addVolunteer (Volunteer* volunteer);
        void printActionsLogs();
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        const vector<BaseAction*> &getActions() const;
        int getCustomerCounter() const;
        int getVolunteerCounter() const;
        int getOrderCounter() const;
        vector <Order*> &getPendingOrders () const;
        vector <Order*> &getProcessingOrders () const;
        vector <Order*> &getCompletedOrders () const;
        vector <Volunteer*> &getVolunteers () const;

        void close();
        void open();

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

};