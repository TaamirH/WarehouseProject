#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "../include/WareHouse.h"
#include "../include/Customer.h"
class customer;
using std::string;
using std::vector;

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
        SimulateStep(int numOfSteps);
        void act(WareHouse &wareHouse) override;
        std::string toString() const override;
        SimulateStep *clone() const override;

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
        AddCustomer(string _customerName, string _customerType, int _distance, int _maxOrders):
        customerName{_customerName}, customerType{CustomerType::Civilian}, distance{_distance}, 
        maxOrders{_maxOrders}{};
        void act(WareHouse &wareHouse) override;
        AddCustomer *clone() const override{return new AddCustomer(*this);}
        string toString() const override{return "customer "+ customerName //+ CTToString(customerType)
         + std::to_string(distance) + std::to_string(maxOrders);}
    private:
        const string customerName;
        const CustomerType customerType;
        const int distance;
        const int maxOrders;
};



class PrintOrderStatus : public BaseAction {
    public:
        PrintOrderStatus(int id);
        void act(WareHouse &wareHouse) override;
        PrintOrderStatus *clone() const override;
        string toString() const override;
    private:
        const int orderId;
};

class PrintCustomerStatus: public BaseAction {
    public:
        PrintCustomerStatus(int customerId);
        void act(WareHouse &wareHouse) override;
        PrintCustomerStatus *clone() const override;
        string toString() const override;
    private:
        const int customerId;
};


class PrintVolunteerStatus : public BaseAction {
    public:
        PrintVolunteerStatus(int id);
        void act(WareHouse &wareHouse) override;
        PrintVolunteerStatus *clone() const override;
        string toString() const override;
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