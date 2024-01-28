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
#include "../include/Action.h"
#define NULL __null
#define NULL ((void*)0)



using namespace std;

class BaseAction;
class Volunteer;
class Customer;
class Order;

// Warehouse responsible for Volunteers, Customers and Actions.
//finish

class WareHouse {

    public:
        
        std::string trimLeadingWhitespace(const std::string &str)
{
        size_t firstNonSpace = str.find_first_not_of(" \t");
            if (firstNonSpace == std::string::npos)
    {
        // The string is all spaces, return an empty string
                return "";
    }
    return str.substr(firstNonSpace);
}
        void parseFile(const string &filePath)
    {
        std::fstream inputFile(filePath);

        if (!inputFile.is_open())
        {
            std::cerr << "Error opening the file." << std::endl;
            return;
        }

    // Variable to store each line
        std::string line;

    // Read and parse the file content
        while (std::getline(inputFile, line))
        {
            if (!line.empty())
            {
            // Find the position of #
                size_t posHash = line.find('#');
                std::string partBeforeHash;
            // Extract the part before #
                if (posHash != std::string::npos)
                    partBeforeHash = line.substr(0, posHash);

            // Trim leading whitespace from the line
                std::string trimmedLine = trimLeadingWhitespace(partBeforeHash);
                if (!trimmedLine.empty())
                {
                    std::cout << trimmedLine << std::endl;

                // Now parse each line and insert into database
                // Vector to store parsed words
                    std::istringstream iss(trimmedLine);
                    std::vector<std::string> words;
                    std::string word;
                // breaking down the line into words.
                    while (std::getline(iss, word, ' '))
                    {
                        words.push_back(word);
                    }
                // Add the last word (or the only word if no space is found)
                // words.push_back(trimmedLine);

                // Now the line is stored word by word in words variable.
                // for customer

                // Inset data
                    if (words[0] == "customer")
                    {
                    // words[0] contains the role
                    // words[1] contains the name
                    // words[2] contains the type
                    // words[3] contains the distance
                    // words[4] contains the max orders number
                        try
                        {
                            if (words[2] == "soldier")
                            {
                                customers.push_back(new SoldierCustomer(getCustomerCounter(), words[1], std::stoi(words[3]), std::stoi(words[4])));
                                ++customerCounter;
                            }
                            else if (words[2] == "civilian")
                            {
                                customers.push_back(new CivilianCustomer(getCustomerCounter(), words[1], std::stoi(words[3]), std::stoi(words[4])));
                                ++customerCounter;
                            }
                        }
                        catch (const std::exception &e)
                        {
                            std::cerr << "Wrong syntax, please check configuration file." << '\n';
                        }
                    }
                    else if (words[0] == "volunteer")
                    {
                        try
                        {
                            if (words[2] == "collector") // Recieves a 4 word input
                            {
                                volunteers.push_back(new CollectorVolunteer(getVolunteerCounter(), words[1], std::stoi(words[3])));
                                ++volunteerCounter;
                            }
                            else if (words[2] == "limited_collector") // Recieves a 5 word input
                            {
                                volunteers.push_back(new LimitedCollectorVolunteer(getVolunteerCounter(), words[1], std::stoi(words[3]), std::stoi(words[4])));
                                ++volunteerCounter;
                            }
                            else if (words[2] == "driver") // Recieves a 5 word input
                            {
                                volunteers.push_back(new DriverVolunteer(getVolunteerCounter(), words[1], std::stoi(words[3]), std::stoi(words[4])));
                                ++volunteerCounter;
                            }
                            else if (words[2] == "limited_driver") // Recieves a 6 word input
                            {
                                volunteers.push_back(new LimitedDriverVolunteer(getVolunteerCounter(), words[1], std::stoi(words[3]), std::stoi(words[4]), std::stoi(words[5])));
                                ++volunteerCounter;
                            }
                        }
                        catch (const std::exception &e)
                        {
                            std::cerr << "Wrong syntax, please check configuration file." << '\n';
                        }
                    }
                    else
                    {
                    // Code to execute if none of the conditions match
                        std::cout << "Wrong syntax, please check configuration file." << std::endl;
                    }
                }
            }
        }

    // Close the file
    inputFile.close();
}
        WareHouse(const string &configFilePath){
            customerCounter,volunteerCounter,orderCounter=0;
            open();
            parseFile(configFilePath);
        };
        WareHouse(WareHouse &other):
            isOpen(other.isOpen),customerCounter(other.customerCounter),
            volunteerCounter(other.volunteerCounter),orderCounter(other.orderCounter){
            for (auto* action:other.actionsLog){
                actionsLog.push_back(action->clone());
            }    
            for (auto* volunteer:other.volunteers){
                volunteers.push_back(volunteer->clone());
            }
            for (auto* pendingorder:other.pendingOrders){
                pendingOrders.push_back(pendingorder->clone());
            }
            for (auto* inproccesorder:other.inProcessOrders){
                inProcessOrders.push_back(inproccesorder->clone());
            }
            for (auto* completedorder:other.completedOrders){
                completedOrders.push_back(completedorder->clone());
            }
            for (auto* customer:other.customers){
                customers.push_back(customer->clone());
            }
        }
        WareHouse &operator=(const WareHouse &other){
            if (this!= &other){
                isOpen=other.isOpen;
                customerCounter=other.customerCounter;
                volunteerCounter=other.volunteerCounter;
                orderCounter=other.orderCounter;
                actionsLog.clear();
                volunteers.clear();
                pendingOrders.clear();
                inProcessOrders.clear();
                completedOrders.clear();
                customers.clear();
                for (auto* actionlog:other.actionsLog){
                    actionsLog.push_back(actionlog->clone());
                }
                for (auto* volunteer:other.volunteers){
                    volunteers.push_back(volunteer->clone());
            }
                for (auto* pendingorder:other.pendingOrders){
                    pendingOrders.push_back(pendingorder->clone());
            }
                for (auto* inproccesorder:other.inProcessOrders){
                    inProcessOrders.push_back(inproccesorder->clone());
            }
                for (auto* completedorder:other.completedOrders){
                    completedOrders.push_back(completedorder->clone());
            }
                for (auto* customer:other.customers){
                    customers.push_back(customer->clone());
            }
            }
        }
        ~WareHouse(){
            for (auto* actionlog:actionsLog){
                delete actionlog;
            }
            for (auto* volunteer:volunteers){
                delete volunteer;
            }
            for (auto* pendingorder:pendingOrders){
                delete pendingorder;
            }
            for (auto* inproccessorder:inProcessOrders){
                delete inproccessorder;
            }
            for (auto* completedorder:completedOrders){
                delete completedorder;
            }
            for (auto* customer:customers){
                delete customer;
            }        
            }
        WareHouse(WareHouse &&other) noexcept :
            isOpen(other.isOpen),
            customerCounter(other.customerCounter),
            volunteerCounter(other.volunteerCounter),
            orderCounter(other.orderCounter),
            actionsLog(std::move(other.actionsLog)),  // Move containers directly
            volunteers(std::move(other.volunteers)),
            pendingOrders(std::move(other.pendingOrders)),
            inProcessOrders(std::move(other.inProcessOrders)),
            completedOrders(std::move(other.completedOrders)),
            customers(std::move(other.customers)) {}

    WareHouse& operator=(WareHouse &&other) noexcept {  // Correct declaration
        if (this != &other) {
        //// Move basic members
            isOpen = other.isOpen;
            customerCounter = other.customerCounter;
            volunteerCounter = other.volunteerCounter;
            orderCounter = other.orderCounter;
        // Move containers directly
            actionsLog = std::move(other.actionsLog);
            volunteers = std::move(other.volunteers);
            pendingOrders = std::move(other.pendingOrders);
            inProcessOrders = std::move(other.inProcessOrders);
            completedOrders = std::move(other.completedOrders);
            customers = std::move(other.customers);
        }
        return *this;
        }
        void start(){
            open();
            while (isOpen){
                std::cout << "What do you want to do? ";
                std::string input;
                std::getline(std::cin, input);  // Read entire line of input    
                std::stringstream ss(input);
                std::vector<std::string> tokens;
                std::string token;
                while (std::getline(ss, token, ' ')) {
                    tokens.push_back(token);
                }

                if (tokens.empty()) {
                    std::cout << "Invalid input: no command specified\n";
                    continue;
                }

                if (tokens[0] == "step") {
                    int numberOfSteps = std::stoi(tokens[1]);
                    BaseAction* action = new SimulateStep(numberOfSteps);  // Create using new
                    action->act(*this);
                    delete action;  // Manually delete after use
                } else if (tokens[0] == "order") {
                    int customerId = std::stoi(tokens[1]);
                    BaseAction* action = new AddOrder(customerId);  // Create using new
                    action->act(*this);
                    delete action;  // Manually delete after use
                }
                else if (tokens[0] == "customer") {
                    if (tokens.size() < 5) {
                        std::cout << "Invalid input: missing customer arguments\n";
                    } else {
                        try {
                            std::string customerName = tokens[1];
                            std::string customerType = tokens[2];
                            int customerDistance = std::stoi(tokens[3]);
                            int maxOrders = std::stoi(tokens[4]);
                            BaseAction* action = new AddCustomer(customerName, customerType, customerDistance, maxOrders);
                            action->act(*this);
                            delete action;    
                        } catch (std::invalid_argument& e) {
                            std::cout << "Invalid input: customer arguments must be strings and integers\n";
                        }
                    }
                }
                else if (tokens[0] == "volunteer") {
                    if (tokens.size() < 3) {
                        std::cout << "Invalid input: missing volunteer name and type\n";
                    } else {
                        std::string volunteerName = tokens[1];
                        std::string volunteerType = tokens[2];

                        // Determine argument parsing based on volunteer type
                        if (volunteerType == "collector" || volunteerType == "limited_collector") {
                            int coolDown = 0;
                            int maxOrders = 0;
                            if (tokens.size() >= 4) {
                                try {
                                    coolDown = std::stoi(tokens[3]);
                                } catch (std::invalid_argument& e) {
                                    std::cout << "Invalid input: cooldown must be an integer\n";
                                }
                            }
                            if (tokens.size() >= 5 && volunteerType == "limited_collector") {
                                try {
                                    maxOrders = std::stoi(tokens[4]);
                                } catch (std::invalid_argument& e) {
                                    std::cout << "Invalid input: maxOrders must be an integer\n";
                                }
                            }
                            BaseAction* action = new AddVolunteer(volunteerName, volunteerType, coolDown, maxOrders, 0, 0);  // Set maxDistance and distancePerStep to 0 for collectors
            // ...
                        } else if (volunteerType == "driver" || volunteerType == "limited_driver") {
                            int maxDistance = 0;
                            int distancePerStep = 0;
                            int maxOrders = 0;
                            if (tokens.size() >= 4) {
                                try {
                                    maxDistance = std::stoi(tokens[3]);
                                } catch (std::invalid_argument& e) {
                                    std::cout << "Invalid input: maxDistance must be an integer\n";
                                }
                            }
                            if (tokens.size() >= 5) {
                                try {
                                    distancePerStep = std::stoi(tokens[4]);
                                } catch (std::invalid_argument& e) {
                                    std::cout << "Invalid input: distancePerStep must be an integer\n";
                                }
                            }
                            if (tokens.size() >= 6 && volunteerType == "limited_driver") {
                                try {
                                    maxOrders = std::stoi(tokens[5]);
                                } catch (std::invalid_argument& e) {
                                    std::cout << "Invalid input: maxOrders must be an integer\n";
                                }
                            }
                            BaseAction* action = new AddVolunteer(volunteerName, volunteerType, 0, maxOrders, maxDistance, distancePerStep);  // Set coolDown to 0 for drivers
            // ...
                        } else {
                            std::cout << "Invalid volunteer type: " << volunteerType << "\n";
                        }
                    }
                }

                else if (tokens[0]=="orderStatus")  {
                    int id= std::stoi(tokens[1]);
                    BaseAction* action = new PrintOrderStatus(id);
                    action ->act(*this);
                    delete action;
                }
                else if (tokens[0]=="customerStatus"){
                    int id =std::stoi(tokens[1]);
                    BaseAction* action= new PrintCustomerStatus(id);
                    action ->act(*this);
                    delete action;
                }
                else if (tokens[0]=="volunteerStatus"){
                    int id =std::stoi(tokens[1]);
                    BaseAction* action= new PrintVolunteerStatus(id);
                    action ->act(*this);
                    delete action;
                }
                else if (tokens[0]=="log"){
                    BaseAction* action = new PrintActionsLog();
                    action ->act(*this);
                    delete action;
                }
                else if (tokens[0]=="close"){
                    BaseAction* action = new Close();
                    action ->act(*this);
                    delete action;
                }
                else if (tokens[0]=="backup"){
                    BaseAction* action = new BackupWareHouse();
                    action ->act(*this);
                    delete action;
                }
                else if (tokens[0]=="restore"){
                    BaseAction* action = new RestoreWareHouse();
                    action ->act(*this);
                    delete action;
                }


                else {
                    std::cout << "Invalid command: " << tokens[0] << "\n";
                }
            }   

        };
        const vector<BaseAction*> &getActionsLog() const{
            return actionsLog;
        };
        void addOrder(Order* order){
            pendingOrders.push_back(order);
            orderCounter++;
        };
         
        void addAction(BaseAction* action){
            actionsLog.push_back(action);
        };
        void addCustomer(Customer* customer){
            customers.push_back(customer);
            customerCounter++;
        };
        void addVolunteer(Volunteer* volunteer){
            volunteers.push_back(volunteer);
            volunteerCounter++;
        };


        void printActionsLogs(){
            for (auto* action: actionsLog){
                if ((*action).getStatus()==ActionStatus::COMPLETED){
                    std::cout <<(*action).toString()<<" COMPLETED";}
                else
                    std::cout <<(*action).toString()<<" ERROR";
            }
        };
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

    throw std::runtime_error("Order not found"); // Or return a nullptr if preferred

        };
        int getCustomerCounter() const{
            return customerCounter;}
        int getVolunteerCounter() const{
            return volunteerCounter;}
        int getOrderCounter() const{
            return orderCounter;}

        vector <Order*> getPendingOrders () const{return pendingOrders;}
        vector <Order*> getProcessingOrders () const {return inProcessOrders;}
        vector <Order*> getCompletedOrders () const{return completedOrders;}
        vector <Volunteer*> getVolunteers () const{return volunteers;}
        
        void close(){
            isOpen=false;
            cout <<"Warehouse is Closed!";
        };
        void open(){
            isOpen=true;
            cout <<"Warehouse is Open!";
        };

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