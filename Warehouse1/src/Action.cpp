#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "../include/WareHouse.h"
#include "../include/Customer.h"
class customer;
using std::string;
using std::vector;
#define NULL ((void*)0)

enum class ActionStatus{
    COMPLETED, ERROR
};

enum class CustomerType{
    Soldier, Civilian   
};


class BaseAction{
    public:
        BaseAction();
        ActionStatus getStatus() const;
        virtual void act(WareHouse& wareHouse)=0;
        virtual string toString() const=0;
        virtual BaseAction* clone() const=0;

    protected:
        void complete(){
            status = ActionStatus::COMPLETED;
        };
        void error(string _errorMsg){
            status = ActionStatus::ERROR;
            errorMsg=_errorMsg;
            std::cout <<"Error:"<<errorMsg;
        };
        string getErrorMsg() const{
            return errorMsg;}

    private:
        string errorMsg;
        ActionStatus status;
};

class SimulateStep : public BaseAction {

    public:
        SimulateStep(int _numOfSteps):numOfSteps{_numOfSteps}{}//need to complete;
        void act(WareHouse &wareHouse) override;
        
    
        std::string toString() const override{return "step " + std::to_string(numOfSteps);}
        SimulateStep *clone() const override{return new SimulateStep(*this);}

    private:
        const int numOfSteps;
};

class AddOrder : public BaseAction {
    public:
        AddOrder(int id): customerId{id}{};
        void act(WareHouse &wareHouse) override;
        string toString() const override{return "order " + std::to_string(customerId);}
        AddOrder *clone() const override{return new AddOrder(*this);}
    private:
        const int customerId;
};

class AddCustomer : public BaseAction {
    public:
    CustomerType toCustomerType(const std::string& str) {
        if (str == "Soldier") {
            return CustomerType::Soldier;
    }   else if (str == "Civilian") {
            return CustomerType::Civilian;
    }   else {
        // Handle invalid input
            throw std::invalid_argument("Invalid customer type string");
    }
}
    string CTToString(CustomerType _customerType) const{
        if (_customerType== CustomerType::Soldier)
            return "Soldier";
        else if (_customerType==CustomerType::Civilian)
            return "Civilian";
        else
            throw std::invalid_argument("Invalid customer type string");
    }



        AddCustomer(string _customerName, string _customerType, int _distance, int _maxOrders):
        customerName{_customerName}, customerType{toCustomerType(_customerType)}, distance{_distance}, 
        maxOrders{_maxOrders}{};
        void act(WareHouse &wareHouse) override;
        AddCustomer *clone() const override{return new AddCustomer(*this);}
        string toString() const override {return "customer "+ customerName + CTToString(customerType)
         + std::to_string(distance) + std::to_string(maxOrders);}
    private:
        const string customerName;
        const CustomerType customerType;
        const int distance;
        const int maxOrders;
};



class PrintOrderStatus : public BaseAction {
    public:
            string OSToString(OrderStatus _orderStatus) const{
        if (_orderStatus== OrderStatus::PENDING)
            return "Pending";
        else if (_orderStatus==OrderStatus::COLLECTING)
            return "Collecting";
        else if (_orderStatus==OrderStatus::DELIVERING)
            return "Delivering";
        else if (_orderStatus==OrderStatus::COMPLETED)
            return "Completed";
        else
            throw std::invalid_argument("Invalid customer type string");
    }

        PrintOrderStatus(int id):orderId(id){};

        void act(WareHouse &wareHouse) override {
        Order _order = wareHouse.getOrder(orderId); 
        // if (_order==nullptr) 
        //   add error ""Order doesn't exist";
        // else{
        std::cout<< "OrderId: " + std::to_string(orderId) +
        "\nOrderStatus: " + OSToString(_order.getStatus());
        if (_order.getCollectorId() == NO_VOLUNTEER)
            std::cout<<"\nNone";
        else
        "\nCostumerID: " +std::to_string (_order.getCustomerId()) ;
        if (_order.getDriverId() == NO_VOLUNTEER)
            std::cout<<"\nNone";
        else
        "\nDriverID: " +std::to_string (_order.getDriverId());
        }
        PrintOrderStatus *clone() const override{return new PrintOrderStatus(*this);}
        string toString() const override{return "orderStatus" + std::to_string(orderId);}
    private:
        const int orderId;
};

class PrintCustomerStatus: public BaseAction {
    public:
        PrintCustomerStatus(int _customerId):customerId{_customerId}{};
        void act(WareHouse &wareHouse) override{


        Customer _customer = wareHouse.getCustomer(customerId); 
        // if (_customer==nullptr) 
        //   add error ""Order doesn't exist";
        // else{
        std::cout<< "CustomerID: " + std::to_string(customerId);
        vector<int> &const orders = _customer.getOrdersIds());

        "\nOrderID: " +  std::to_string(_customer.getOrdersIds());
        if (_order.getCollectorId() == NO_VOLUNTEER)
            std::cout<<"\nNone";
        else
        "\nCostumerID: " +std::to_string (_order.getCustomerId()) ;
        if (_order.getDriverId() == NO_VOLUNTEER)
            std::cout<<"\nNone";
        else
        "\nDriverID: " +std::to_string (_order.getDriverId());
        }
        


        PrintCustomerStatus *clone() const override{return new PrintCustomerStatus(*this);}
        string toString() const override{return "customerStatus " + std::to_string(customerId);}
    private:
        const int customerId;
};


class PrintVolunteerStatus : public BaseAction {
    public:
        PrintVolunteerStatus(int id):volunteerId{id}{};
        void act(WareHouse &wareHouse) override;


        PrintVolunteerStatus *clone() const override{return new PrintVolunteerStatus(*this);}
        string toString() const override{return "volunteerStatus " + std::to_string(volunteerId);}
    private:
        const int volunteerId;
};


class PrintActionsLog : public BaseAction {
    public:
        PrintActionsLog();
        void act(WareHouse &wareHouse) override;
        PrintActionsLog *clone() const override;
        string toString() const override;
    private:
};

class Close : public BaseAction {
    public:
        Close();
        void act(WareHouse &wareHouse) override;
        Close *clone() const override;
        string toString() const override;
    private:
};

class BackupWareHouse : public BaseAction {
    public:
        BackupWareHouse();
        void act(WareHouse &wareHouse) override;
        BackupWareHouse *clone() const override;
        string toString() const override;
    private:
};


class RestoreWareHouse : public BaseAction {
    public:
        RestoreWareHouse();
        void act(WareHouse &wareHouse) override;
        RestoreWareHouse *clone() const override;
        string toString() const override;
    private:
};