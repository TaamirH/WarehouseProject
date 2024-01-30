#include <string>
#include <vector>
#include "../include/Order.h"
#include "../include/Volunteer.h"
#include <iostream>
using std::string;
using std::vector;
class order;
#define NO_ORDER -1


        Volunteer::Volunteer(int _id, const string &_name):id{_id}, name{_name}{
            activeOrderId=NO_ORDER; completedOrderId=NO_ORDER;};
        int Volunteer:: getId() const{return id;}
        const string &Volunteer:: getName() const{return name;}
        int Volunteer:: getActiveOrderId() const{return activeOrderId;}
        int Volunteer:: getCompletedOrderId() const{return completedOrderId;}
        bool Volunteer::isBusy() const {return !activeOrderId==NO_ORDER;} // Signal whether the volunteer is currently processing an order    
        // virtual bool Volunteer::hasOrdersLeft() const = 0; // Signal whether the volunteer didn't reach orders limit,Always true for CollectorVolunteer and DriverVolunteer
        // virtual bool Volunteer::canTakeOrder(const Order &order) const = 0; // Signal if the volunteer can take the order.      
        // virtual void Volunteer::acceptOrder(const Order &order) = 0; // Prepare for new order(Reset activeOrderId,TimeLeft,DistanceLeft,OrdersLeft depends on the volunteer type)
                
        // virtual void Volunteer::step() = 0; //Simulate volunteer step,if the volunteer finished the order, transfer activeOrderId to completedOrderId

        // virtual string toString() const = 0;
        // virtual Volunteer* clone() const = 0; //Return a copy of the volunteer
        // virtual int whoAmI () const = 0;

    //    int completedOrderId; //Initialized to NO_ORDER if no order has been completed yet
     //   int activeOrderId; //Initialized to NO_ORDER if no order is being processed

   //     const int id;
 //       const string name;


        CollectorVolunteer::CollectorVolunteer(int _id, const string &_name, int _coolDown):
        Volunteer(_id, _name),coolDown{_coolDown}{};
        CollectorVolunteer* CollectorVolunteer:: clone() const {return new CollectorVolunteer(*this);}
        void CollectorVolunteer:: step() {if(decreaseCoolDown()){
            completedOrderId = activeOrderId;
            activeOrderId = NO_ORDER;
        }}
        
        int CollectorVolunteer:: getCoolDown() const{return coolDown;}
        int CollectorVolunteer:: getTimeLeft() const{return timeLeft;}
        bool CollectorVolunteer:: decreaseCoolDown(){if (isBusy())timeLeft--;
        return getTimeLeft()==0;}        //Decrease timeLeft by 1,return true if timeLeft=0,false otherwise
        bool CollectorVolunteer:: hasOrdersLeft() const {return true;}
        bool CollectorVolunteer:: canTakeOrder(const Order &order) const {return !this->isBusy()
         && order.getStatus()==OrderStatus::PENDING;}
        void CollectorVolunteer:: acceptOrder(const Order &order) {timeLeft= getCoolDown();
        activeOrderId = order.getId();
        }
        string CollectorVolunteer:: toString() const {int cd = getCoolDown();
            return "volunteer " + getName() + " collector " + std::to_string(cd);}
        int CollectorVolunteer:: whoAmI() const {return 0;}
    
    
        // const int coolDown; // The time it takes the volunteer to process an order
        //// int timeLeft; // Time left until the volunteer finishes his current order


        LimitedCollectorVolunteer::LimitedCollectorVolunteer(int _id, const string &_name, int _coolDown ,int _maxOrders):
        CollectorVolunteer(_id, _name, _coolDown), maxOrders{_maxOrders},ordersLeft(0){};
        LimitedCollectorVolunteer* LimitedCollectorVolunteer:: clone() const {return new LimitedCollectorVolunteer(*this);}
        bool LimitedCollectorVolunteer::hasOrdersLeft() const {return ordersLeft!=0;}
        bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const {return !this->isBusy() && hasOrdersLeft() 
        && order.getStatus()==OrderStatus::PENDING;};
        void LimitedCollectorVolunteer::acceptOrder(const Order &order) {ordersLeft--;
        activeOrderId = order.getId();
        }

        int LimitedCollectorVolunteer::getMaxOrders() const{return maxOrders;}
        int LimitedCollectorVolunteer::getNumOrdersLeft() const{return ordersLeft;}
        string LimitedCollectorVolunteer::toString() const {int cd = getCoolDown();int mo = getMaxOrders();
            return "volunteer " + getName() + " limited_collector " + std::to_string(cd) +std::to_string(mo);}
        int LimitedCollectorVolunteer::whoAmI()const {return 1;}
    
    
        // const int maxOrders; // The number of orders the volunteer can process in the whole simulation
        // int ordersLeft; // The number of orders the volunteer can still take



        DriverVolunteer::DriverVolunteer(int _id, const string &_name, int _maxDistance, int _distancePerStep): 
        Volunteer(_id, _name), maxDistance{_maxDistance}, distancePerStep{_distancePerStep}{};
        DriverVolunteer* DriverVolunteer:: clone() const {return new DriverVolunteer (*this);}

        int DriverVolunteer::getDistanceLeft() const{return distanceLeft;}
        int DriverVolunteer::getMaxDistance() const{return maxDistance;}
        int DriverVolunteer::getDistancePerStep() const{return distancePerStep;}  
        bool DriverVolunteer::decreaseDistanceLeft(){if(isBusy())   
            distanceLeft = getDistanceLeft() - getDistancePerStep();
            return distanceLeft<=0;}
                //Decrease distanceLeft by distancePerStep,return true if distanceLeft<=0,false otherwise
        bool DriverVolunteer::hasOrdersLeft() const {return true;}
        bool DriverVolunteer::canTakeOrder(const Order &order) const 
        {return !this->isBusy() && order.getDistance()<=getMaxDistance() &&
         order.getStatus()==OrderStatus::COLLECTING;}; // Signal if the volunteer is not busy and the order is within the maxDistance
        void DriverVolunteer::acceptOrder(const Order &order) {setDistanceLeft (order.getDistance());
        activeOrderId = order.getId();
        } // Assign distanceLeft to order's distance
        void DriverVolunteer::step() {if(decreaseDistanceLeft()){
            completedOrderId = activeOrderId;
            activeOrderId = NO_ORDER;
        }
        } // Decrease distanceLeft by distancePerStep
        string DriverVolunteer::toString() const {int md = getMaxDistance();int dps = getDistancePerStep();
            return "volunteer " + getName() + " driver " + std::to_string(md) +std::to_string(dps);}
         void DriverVolunteer::setDistanceLeft(int i) {distanceLeft=i;}
         int DriverVolunteer::whoAmI()const {return 2;}

        // const int maxDistance; // The maximum distance of ANY order the volunteer can take
        // const int distancePerStep; // The distance the volunteer does in one step
        // int distanceLeft; // Distance left until the volunteer finishes his current order

        LimitedDriverVolunteer::LimitedDriverVolunteer(int _id, const string &_name, int _maxDistance, int _distancePerStep,int _maxOrders)
        :DriverVolunteer(_id, _name, _maxDistance, _distancePerStep), maxOrders{_maxOrders},ordersLeft(0){};
        LimitedDriverVolunteer* LimitedDriverVolunteer:: clone() const {return new LimitedDriverVolunteer(*this);}
        int LimitedDriverVolunteer::getMaxOrders() const {return maxOrders;}
        int LimitedDriverVolunteer::getNumOrdersLeft() const {return ordersLeft;}
        bool LimitedDriverVolunteer::hasOrdersLeft() const {return ordersLeft>0;}
        bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const 
         {return !this->isBusy() && order.getDistance()<=getMaxDistance() && hasOrdersLeft()
         && order.getStatus()==OrderStatus::COLLECTING;}
        // Signal if the volunteer is not busy, the order is within the maxDistance.
        void LimitedDriverVolunteer::acceptOrder(const Order &order) {ordersLeft--;
        setDistanceLeft(order.getDistance());
        activeOrderId = order.getId();}
        // Assign distanceLeft to order's distance and decrease ordersLeft
        string LimitedDriverVolunteer::toString() const {int md = getMaxDistance();int dps = getDistancePerStep(); int mo = getMaxOrders();
            return "volunteer " + getName() + " driver_limited " + std::to_string(md) +std::to_string(dps)+std::to_string(mo);}
        int LimitedDriverVolunteer::whoAmI() const {return 3;}
    

    
        // const int maxOrders; // The number of orders the volunteer can process in the whole simulation
        // int ordersLeft; // The number of orders the volunteer can still take

