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
        void start();
        const vector<BaseAction*> &getActionsLog() const{
            return actionsLog;
        };
        void addOrder(Order* order){
            pendingOrders.push_back(order);
        };
        void addAction(BaseAction* action){
            actionsLog.push_back(action);
        };
        void printActionsLogs(){
            for (auto* action: actionsLog){
                if ((*action).getStatus()==ActionStatus::COMPLETED){
                    std::cout <<(*action).toString()<<"COMPLETED";}
                else
                    std::cout <<(*action).toString()<<"ERROR";
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

    //return nullptr ;
    // Handle the case where the order is not found
    throw std::runtime_error("Order not found"); // Or return a nullptr if preferred

        };
        int getCustomerCounter() const{
            return customerCounter;}
        int getVolunteerCounter() const{
            return volunteerCounter;}
        int getOrderCounter() const{
            return orderCounter;}
        void close();
        void open(){
            isOpen=true;
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