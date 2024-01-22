#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

class BaseAction;
class Volunteer;
class Customer;
class Order;

// Warehouse responsible for Volunteers, Customers and Actions.
//finish

class WareHouse {

    public:
      /*  bool parseFile(const std::string& configFilePath, std::vector<Customer>& customers, std::vector<Volunteer>& volunteers) {
        std::ifstream file(configFilePath);
        if (!file.is_open()) {
         std::cerr << "Error opening file!" << std::endl;
        return false;
                            
        }
        std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "customer") {
            std::string name, typeStr;
            int distance, maxOrders;
            iss >> name >> typeStr >> distance >> maxOrders;
            customers.push_back(Customer(name, typeStr, distance, maxOrders)); // Assuming a constructor for Customer
        } else if (type == "volunteer") {
            std::string name, role;
            int coolDownOrMaxDistance, distancePerStep = 0, maxOrders = -1; // Optional values initialized
            iss >> name >> role >> coolDownOrMaxDistance;
            if (iss >> distancePerStep) { // Check for optional values
                iss >> maxOrders;
            }
            volunteers.push_back(Volunteer(name, role, coolDownOrMaxDistance, distancePerStep, maxOrders)); // Assuming a constructor for Volunteer
        } else {
            // Ignore comment lines
        }
    }

    return true;}*/

        WareHouse(const string &configFilePath);
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