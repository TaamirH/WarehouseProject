#pragma once
#include <string>
#include <vector>
#include "WareHouse.h"
class WareHouse;
using std::string;
using std::vector;
extern WareHouse* backup;

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
        virtual std::string toString() const=0;
        virtual BaseAction* clone() const=0;

    protected:
        void complete();
        void error(std::string errorMsg);
        std::string getErrorMsg() const;

    private:
        std::string errorMsg;
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
        AddOrder(int id);
        void act(WareHouse &wareHouse) override;
        std::string toString() const override;
        AddOrder *clone() const override;
    private:
        const int customerId;
};


class AddCustomer : public BaseAction {
    public:
        CustomerType toCustomerType(const std::string& str);
        std::string CTToString(CustomerType _customerType) const;
        AddCustomer(std::string customerName, std::string customerType, int distance, int maxOrders);
        void act(WareHouse &wareHouse) override;
        AddCustomer *clone() const override;
        std::string toString() const override;
    private:
        const std::string customerName;
        const CustomerType customerType;
        const int distance;
        const int maxOrders;
};

class AddVolunteer : public BaseAction {
    public:
        AddVolunteer(std::string name, std::string volunteerType, int coolDown,  int maxOrders, int maxDistance, int distancePerStep);
        void act(WareHouse &wareHouse) override;
        AddVolunteer *clone() const override;
        std::string toString() const override;
        std::string OSToString(OrderStatus _orderStatus) const;
    private:
        const std::string name;
        const std::string volunteerType;
        const int cooldown;
        const int maxOrders;
        const int maxDistance;
        const int distancePerStep;
};



class PrintOrderStatus : public BaseAction {
    public:
        std::string OSToString(OrderStatus _orderStatus);
        PrintOrderStatus(int id);
        void act(WareHouse &wareHouse) override;
        PrintOrderStatus *clone() const override;
        std::string toString() const override;
    private:
        const int orderId;
};

class PrintCustomerStatus: public BaseAction {
    public:
        PrintCustomerStatus(int customerId);
        void act(WareHouse &wareHouse) override;
        PrintCustomerStatus *clone() const override;
        std::string toString() const override;
    private:
        const int customerId;
};


class PrintVolunteerStatus : public BaseAction {
    public:
        PrintVolunteerStatus(int id);
        void act(WareHouse &wareHouse) override;
        PrintVolunteerStatus *clone() const override;
        std::string toString() const override;
    private:
        const int volunteerId;
};


class PrintActionsLog : public BaseAction {
    public:
        PrintActionsLog();
        void act(WareHouse &wareHouse) override;
        PrintActionsLog *clone() const override;
        std::string toString() const override;
    private:
};

class Close : public BaseAction {
    public:
        Close();
        void act(WareHouse &wareHouse) override;
        Close *clone() const override;
        std::string toString() const override;
    private:
};

class BackupWareHouse : public BaseAction {
    public:
        BackupWareHouse();
        void act(WareHouse &wareHouse) override;
        BackupWareHouse *clone() const override;
        std::string toString() const override;
    private:
};


class RestoreWareHouse : public BaseAction {
    public:
        RestoreWareHouse();
        void act(WareHouse &wareHouse) override;
        RestoreWareHouse *clone() const override;
        std::string toString() const override;
    private:
};
