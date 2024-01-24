#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <memory>
#include "../include/Customer.h"
#include "../include/Volunteer.h"

using namespace std;

class BaseAction;
class Volunteer;
class Customer;
class Order;

// Warehouse responsible for Volunteers, Customers and Actions.
//finish

class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        void start();
        const vector<BaseAction*> &getActionsLog() const;
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        void printActionsLogs();
        Customer &getCustomer(int customerId) const{
            for (const auto& customer : customers) {
                if ((*customer).getId() == customerId) {
                    return *customer;
            }
        }
        // If no matching customer is found, throw an exception or handle it accordingly
        throw std::out_of_range("Customer with the specified ID not found");
        };
        Volunteer &getVolunteer(int volunteerId) const{
            for (const auto& volunteer : volunteers){
                if ((*volunteer).getId()==volunteerId){
                    return *volunteer;
                }
            }
        throw std::out_of_range("Volunteer with the specified ID not found");

        };
        Order &getOrder(int orderId) const{
            for (const auto& orders : {&pendingOrders, &inProcessOrders, &completedOrders}) {
                for (const auto& orderPtr : *orders) {
                    if (orderPtr->getId() == orderId) {
                        return *orderPtr;
            }
        }
    }

    // Handle the case where the order is not found
    throw std::runtime_error("Order not found"); // Or return a nullptr if preferred

        };

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

    
};