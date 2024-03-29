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
#include "../include/WareHouse.h"




using namespace std;

class BaseAction;
class Volunteer;
class Customer;
class Order;

// Warehouse responsible for Volunteers, Customers and Actions.
//finish


        
        std::string WareHouse:: trimLeadingWhitespace(const std::string &str)
{
        size_t firstNonSpace = str.find_first_not_of(" \t");
            if (firstNonSpace == std::string::npos)
    {
        // empty string
                return "";
    }
    return str.substr(firstNonSpace);
}
        void WareHouse::parseFile(const string &filePath)
    {
        std::fstream inputFile(filePath);

        if (!inputFile.is_open())
        {
            std::cerr << "Error opening the file." << std::endl;
            return;
        }

    // line var
        std::string line;

    // read file and parse
        while (std::getline(inputFile, line))
        {
            if (!line.empty())
            {
            // Find #
                size_t posHash = line.find('#');
                std::string partBeforeHash;
            // Extract #
                if (posHash != std::string::npos)
                    partBeforeHash = line.substr(0, posHash);

            // Trim first backspace
                std::string trimmedLine = trimLeadingWhitespace(partBeforeHash);
                if (!trimmedLine.empty())
                {
                    std::cout << trimmedLine << std::endl;

                // parse  line and insert into database
                    std::istringstream iss(trimmedLine);
                    std::vector<std::string> words;
                    std::string word;
                // breaking down line into words.
                    while (std::getline(iss, word, ' '))
                    {
                        words.push_back(word);
                    }
                // Add the last word 


                    if (words[0] == "customer")
                    {
                   
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
                            if (words[2] == "collector") // 4 word input
                            {
                                volunteers.push_back(new CollectorVolunteer(getVolunteerCounter(), words[1], std::stoi(words[3])));
                                ++volunteerCounter;
                            }
                            else if (words[2] == "limited_collector") //  5 word input
                            {
                                volunteers.push_back(new LimitedCollectorVolunteer(getVolunteerCounter(), words[1], std::stoi(words[3]), std::stoi(words[4])));
                                ++volunteerCounter;
                            }
                            else if (words[2] == "driver") // 5 word input
                            {
                                volunteers.push_back(new DriverVolunteer(getVolunteerCounter(), words[1], std::stoi(words[3]), std::stoi(words[4])));
                                ++volunteerCounter;
                            }
                            else if (words[2] == "limited_driver") //  6 word input
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
        WareHouse::WareHouse(const string &configFilePath):
            isOpen(false),
            actionsLog(),  // Default initialization for vector
            volunteers(),  // Default initialization for vector
            pendingOrders(),
            inProcessOrders(),
            completedOrders(),
            customers(),
            customerCounter(0),  // Assuming starting value is 0
            volunteerCounter(0),
            orderCounter(0),
            deletedVolunteers() {
            open();
            parseFile(configFilePath);
        };
        WareHouse::WareHouse(WareHouse &other):
            isOpen(other.isOpen),actionsLog(),volunteers(),pendingOrders(),inProcessOrders(),
            completedOrders(),customers(),customerCounter(other.customerCounter),
            volunteerCounter(other.volunteerCounter),orderCounter(other.orderCounter),
            deletedVolunteers(){
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
            for (int deleted:other.deletedVolunteers){
                deletedVolunteers.push_back(deleted);
            }
        }
        WareHouse& WareHouse:: operator=(const WareHouse &other){
            if (this!= &other){
                isOpen=other.isOpen;
                customerCounter=other.customerCounter;
                volunteerCounter=other.volunteerCounter;
                orderCounter=other.orderCounter;
                
                for(BaseAction* action : actionsLog){
                    delete action;
                }
                for(auto vol : volunteers){
                    delete vol;
                }
                for (Order* ord : pendingOrders){
                    delete ord;
                }
                for (Order* ord : inProcessOrders){
                    delete ord;
                }
                for (Order* ord : completedOrders){
                    delete ord;
                }
                for (auto cost : customers){
                    delete cost;
                }

                
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
            return *this;
        }
        WareHouse:: ~WareHouse (){
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
         WareHouse::WareHouse(WareHouse &&other) :
            isOpen(other.isOpen),
            actionsLog(std::move(other.actionsLog)),
            volunteers(std::move(other.volunteers)),
            pendingOrders(std::move(other.pendingOrders)),
            inProcessOrders(std::move(other.inProcessOrders)),
            completedOrders(std::move(other.completedOrders)),
            customers(std::move(other.customers)),
            customerCounter(other.customerCounter),
            volunteerCounter(other.volunteerCounter),
            orderCounter(other.orderCounter),
            deletedVolunteers(std::move(other.deletedVolunteers)) {}

    WareHouse& WareHouse:: operator=(WareHouse &&other) {  // Correct declaration
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
        void WareHouse::start(){
            open();
            while (isOpen){
                std::cout << "\nWhat do you want to do? ";
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

                else if (tokens.size()==2 && tokens[0] == "step") {
                    int numberOfSteps = std::stoi(tokens[1]);
                    BaseAction* action = new SimulateStep(numberOfSteps);  // Create using new
                    action->act(*this);
                    actionsLog.push_back(action);
                    
                } else if (tokens.size()==2 && tokens[0] == "order") {
                    int customerId = std::stoi(tokens[1]);
                    BaseAction* action = new AddOrder(customerId);  // Create using new
                    action->act(*this);
                    actionsLog.push_back(action);
                    
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
                            actionsLog.push_back(action);
                               
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
                            action->act(*this);
                            actionsLog.push_back(action);

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
                            action->act(*this);
                            actionsLog.push_back(action);

                        } else {
                            std::cout << "Invalid volunteer type: " << volunteerType << "\n";
                        }
                    }
                }

                else if (tokens.size()==2 && tokens[0]=="orderStatus")  {
                    int id= std::stoi(tokens[1]);
                    BaseAction* action = new PrintOrderStatus(id);
                    action ->act(*this);
                    actionsLog.push_back(action);
                    
                }
                else if (tokens.size()==2 && tokens[0]=="customerStatus"){
                    int id =std::stoi(tokens[1]);
                    BaseAction* action= new PrintCustomerStatus(id);
                    action ->act(*this);
                    actionsLog.push_back(action);
                    
                }
                else if (tokens.size()==2 && tokens[0]=="volunteerStatus"){
                    int id =std::stoi(tokens[1]);
                    BaseAction* action= new PrintVolunteerStatus(id);
                    action ->act(*this);
                    actionsLog.push_back(action);
                    
                }
                else if (tokens.size()==1 && tokens[0]=="log"){
                    BaseAction* action = new PrintActionsLog();
                    action ->act(*this);
                    actionsLog.push_back(action);
                    
                }
                else if (tokens.size()==1 && tokens[0]=="close"){
                    BaseAction* action = new Close();
                    action ->act(*this);
                    actionsLog.push_back(action);
                    
                }
                else if (tokens.size()==1 && tokens[0]=="backup"){
                    BaseAction* action = new BackupWareHouse();
                    action ->act(*this);
                    actionsLog.push_back(action);
                    
                }
                else if (tokens.size()==1 && tokens[0]=="restore"){
                    BaseAction* action = new RestoreWareHouse();
                    action ->act(*this);
                    actionsLog.push_back(action);
                    
                }


                else {
                    std::cout << "Invalid command: " << tokens[0] << "\n";
                }
            }   

        };

        const vector<BaseAction*> &WareHouse::getActionsLog() const{
            return actionsLog;
        };
        void WareHouse::addOrder(Order* order){
            pendingOrders.push_back(order);
            orderCounter++;
        };
         
        void WareHouse::addAction(BaseAction* action){
            actionsLog.push_back(action);
        };
        void WareHouse::addCustomer(Customer* customer){
            customers.push_back(customer);
            customerCounter++;
        };
        void WareHouse::addVolunteer(Volunteer* volunteer){
            volunteers.push_back(volunteer);
            volunteerCounter++;
        };


        void WareHouse::printActionsLogs(){
            for (auto* action: actionsLog){
                if ((*action).getStatus()==ActionStatus::COMPLETED){
                    std::cout <<(*action).toString() +" COMPLETED\n";}
                else
                    std::cout <<(*action).toString() +" ERROR\n";
            }
        };
        Customer& WareHouse::getCustomer(int customerId) const{
            for (const auto& customer : customers) {
                if ((*customer).getId() == customerId) {
                    return *customer;
            }
        }
        // If no matching customer is found, throw an exception or handle it accordingly
        throw std::out_of_range("Customer with the specified ID not found");
        };
        Volunteer& WareHouse::getVolunteer(int volunteerId) const{
            for (const auto& volunteer : volunteers){
                if ((*volunteer).getId()==volunteerId){
                    return *volunteer;
                }
            }
        throw std::out_of_range("Volunteer with the specified ID not found");

        };
        Order& WareHouse::getOrder(int orderId) const{
            for (const auto& order : pendingOrders) {
                    if (order->getId() == orderId) {
                        return *order;
            }
        }
            for (const auto& order : inProcessOrders) {
                    if (order->getId() == orderId) {
                        return *order;
    }
            }
            for (const auto& order : completedOrders) {
                    if (order->getId() == orderId) {
                        return *order;}}

    throw std::runtime_error("Order not found"); // Or return a nullptr if preferred

        };
        int WareHouse::getCustomerCounter() const{
            return customerCounter;}
        int WareHouse::getVolunteerCounter() const{
            return volunteerCounter;}
        int WareHouse::getOrderCounter() const{
            return orderCounter;}

        const vector <Order*>& WareHouse::getPendingOrders () const{return pendingOrders;}
        const vector <Order*>& WareHouse:: getProcessingOrders () const {return inProcessOrders;}
        const vector <Order*>& WareHouse::getCompletedOrders () const{return completedOrders;}
        const vector <Volunteer*>& WareHouse::getVolunteers () const{return volunteers;}
        
        void WareHouse::close(){
            isOpen=false;
            cout <<"Warehouse is Closed!";
        };
        void WareHouse::open(){
            isOpen=true;
            cout <<"Warehouse is Open!";
        };
        void WareHouse::moveOrder(Order *order,int id){
            OrderStatus status=order->getStatus(); 
            if (status==OrderStatus::PENDING){      // order taken by collector - move to in proccess
                order->setStatus(OrderStatus::COLLECTING);
                order->setCollectorId(id);
                auto it = std::find(pendingOrders.begin(), pendingOrders.end(), order);
                pendingOrders.erase(it);
                inProcessOrders.push_back(order);
            }
            else if (status==OrderStatus::COLLECTING){   // order finished - move to pending
                 auto it = std::find(inProcessOrders.begin(), inProcessOrders.end(), order);
                if (it != inProcessOrders.end()) {
                    inProcessOrders.erase(it);
                    pendingOrders.push_back(order);}
                else{
                    order->setDriverId(id);     //order taken by drive - move to in process
                    auto it = std::find(pendingOrders.begin(), pendingOrders.end(), order);
                    pendingOrders.erase(it);
                    inProcessOrders.push_back(order);
                    order->setStatus(OrderStatus::DELIVERING);
                }
            }
            else if (status==OrderStatus::DELIVERING){      //order finished
                order->setStatus(OrderStatus::COMPLETED);
                auto it = std::find(inProcessOrders.begin(), inProcessOrders.end(), order);
                inProcessOrders.erase(it);
                completedOrders.push_back(order);
            }


        }
        void WareHouse::deleteVol(Volunteer* vol){
            auto it = std::find(volunteers.begin(), volunteers.end(),vol);
            int deletedid = vol->getId();
            delete vol;
            volunteers.erase(it);
            deletedVolunteers.push_back(deletedid);

        }
        const vector <int>& WareHouse::getDeletedVolunteers() const{ return deletedVolunteers;}

        

        
    // private:
    //     bool isOpen;
    //     vector<BaseAction*> actionsLog;
    //     vector<Volunteer*> volunteers;
    //     vector<Order*> pendingOrders;
    //     vector<Order*> inProcessOrders;
    //     vector<Order*> completedOrders;
    //     vector<Customer*> customers;
    //     int customerCounter; //For assigning unique customer IDs
    //     int volunteerCounter; //For assigning unique volunteer IDs
    //     int orderCounter;

    
