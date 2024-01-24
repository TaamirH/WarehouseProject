#pragma once
#include <string>
#include <vector>
#include "../include/Order.h"
#include <iostream>
using std::string;
using std::vector;

#define NO_ORDER -1

class Volunteer {
    public:
        Volunteer(int _id, const string &_name):id{_id}, name{_name}{};
        int getId() const{return id;}
        const string &getName() const{return name;}
        int getActiveOrderId() const{return activeOrderId;}
        int getCompletedOrderId() const{return completedOrderId;}
        bool isBusy() const {return !activeOrderId==NO_ORDER;} // Signal whether the volunteer is currently processing an order    
        virtual bool hasOrdersLeft() const = 0; // Signal whether the volunteer didn't reach orders limit,Always true for CollectorVolunteer and DriverVolunteer
        virtual bool canTakeOrder(const Order &order) const = 0; // Signal if the volunteer can take the order.      
        virtual void acceptOrder(const Order &order) = 0; // Prepare for new order(Reset activeOrderId,TimeLeft,DistanceLeft,OrdersLeft depends on the volunteer type)
                
        virtual void step() = 0; //Simulate volunteer step,if the volunteer finished the order, transfer activeOrderId to completedOrderId

        virtual string toString() const = 0;
        virtual Volunteer* clone() const = 0; //Return a copy of the volunteer

    protected:
        int completedOrderId; //Initialized to NO_ORDER if no order has been completed yet
        int activeOrderId; //Initialized to NO_ORDER if no order is being processed
    
    private:
        const int id;
        const string name;

};


class CollectorVolunteer: public Volunteer {

    public:
        CollectorVolunteer(int _id, const string &_name, int _coolDown):
        Volunteer(_id, _name),coolDown{_coolDown}{};
        CollectorVolunteer *clone() const override{return new CollectorVolunteer(*this);}
        void step() override;
        int getCoolDown() const{return coolDown;}
        int getTimeLeft() const{return timeLeft;}
        bool decreaseCoolDown(){timeLeft--;
        return getTimeLeft==0;}        //Decrease timeLeft by 1,return true if timeLeft=0,false otherwise
        bool hasOrdersLeft() const override {return true;}
        bool canTakeOrder(const Order &order) const override {return !this->isBusy();}
        void acceptOrder(const Order &order) override;
        string toString() const override {int cd = getCoolDown();
            return "volunteer " + getName() + " collector " + std::to_string(cd);}
    
    private:
        const int coolDown; // The time it takes the volunteer to process an order
        int timeLeft; // Time left until the volunteer finishes his current order
};

class LimitedCollectorVolunteer: public CollectorVolunteer {

    public:
        LimitedCollectorVolunteer(int _id, const string &_name, int _coolDown ,int _maxOrders):
        CollectorVolunteer(_id, _name, _coolDown), maxOrders{_maxOrders}{};
        LimitedCollectorVolunteer *clone() const override {return new LimitedCollectorVolunteer(*this);}
        bool hasOrdersLeft() const override{return ordersLeft!=0;}
        bool canTakeOrder(const Order &order) const override{return !this->isBusy() && hasOrdersLeft;};
        void acceptOrder(const Order &order) override;

        int getMaxOrders() const{return maxOrders;}
        int getNumOrdersLeft() const{return ordersLeft;}
        string toString() const override {int cd = getCoolDown();int mo = getMaxOrders();
            return "volunteer " + getName() + " limited_collector " + std::to_string(cd) +std::to_string(mo);}
    
    
    private:
        const int maxOrders; // The number of orders the volunteer can process in the whole simulation
        int ordersLeft; // The number of orders the volunteer can still take
};

class DriverVolunteer: public Volunteer {

    public:
        DriverVolunteer(int _id, const string &_name, int _maxDistance, int _distancePerStep): 
        Volunteer(_id, _name), maxDistance{_maxDistance}, distancePerStep{_distancePerStep}{};
        DriverVolunteer *clone() const override{return new DriverVolunteer (*this);}

        int getDistanceLeft() const{return distanceLeft;}
        int getMaxDistance() const{return maxDistance;}
        int getDistancePerStep() const{return distancePerStep;}  
        bool decreaseDistanceLeft(){   
            distanceLeft = getDistanceLeft() - getDistancePerStep();
            return distanceLeft<=0;}
                //Decrease distanceLeft by distancePerStep,return true if distanceLeft<=0,false otherwise
        bool hasOrdersLeft() const override {return true;}
        bool canTakeOrder(const Order &order) const override
        {return !this->isBusy() && order.getDistance()<=getMaxDistance();}; // Signal if the volunteer is not busy and the order is within the maxDistance
        void acceptOrder(const Order &order) override; // Assign distanceLeft to order's distance
        void step() override; // Decrease distanceLeft by distancePerStep
        string toString() const override {int md = getMaxDistance();int dps = getDistancePerStep();
            return "volunteer " + getName() + " driver " + std::to_string(md) +std::to_string(dps);}
    

    private:
        const int maxDistance; // The maximum distance of ANY order the volunteer can take
        const int distancePerStep; // The distance the volunteer does in one step
        int distanceLeft; // Distance left until the volunteer finishes his current order
};

class LimitedDriverVolunteer: public DriverVolunteer {

    public:
        LimitedDriverVolunteer(int _id, const string &_name, int _maxDistance, int _distancePerStep,int _maxOrders)
        :DriverVolunteer(_id, _name, _maxDistance, _distancePerStep), maxOrders{_maxOrders}{};
        LimitedDriverVolunteer *clone() const override{return new LimitedCollectorVolunteer(*this);}
        int getMaxOrders() const {return maxOrders;}
        int getNumOrdersLeft() const {return ordersLeft;}
        bool hasOrdersLeft() const override{return ordersLeft>0;}
        bool canTakeOrder(const Order &order) const override
         {return !this->isBusy() && order.getDistance()<=getMaxDistance();}
        // Signal if the volunteer is not busy, the order is within the maxDistance.
        void acceptOrder(const Order &order) override; // Assign distanceLeft to order's distance and decrease ordersLeft
        string toString() const override {int md = getMaxDistance();int dps = getDistancePerStep(); int mo = getMaxOrders();
            return "volunteer " + getName() + " driver_limited " + std::to_string(md) +std::to_string(dps)+std::to_string(mo);}
    

    private:
        const int maxOrders; // The number of orders the volunteer can process in the whole simulation
        int ordersLeft; // The number of orders the volunteer can still take
};