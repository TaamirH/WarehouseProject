#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "../include/WareHouse.h"
#include "../include/Customer.h"
class volunteer;
#include "../include/Volunteer.h"
#include "../include/Action.h"
class customer;
using std::string;
using std::vector;
extern WareHouse* backup;
#include <stack>;
using namespace std;
//
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
        SimulateStep(int _numOfSteps):numOfSteps{_numOfSteps}{}

        void act(WareHouse &wareHouse) override{
            stack<Order> orders;
            
            for (auto* vol : wareHouse.getVolunteers()){
                for (Order* ord : wareHouse.getPendingOrders()){
                    if(vol->canTakeOrder(*ord)){
                    
                        vol->acceptOrder( *ord);
                        ord->setCollectorId(vol->getId());
                        ord->setStatus(OrderStatus::COLLECTING);
                        wareHouse.moveOrder(*ord, vol->getId());
                    }
                }}  
                for (auto* vol : wareHouse.getVolunteers()){
                    int activeO = vol->getActiveOrderId();
                    vol->step();
                    if (activeO==vol->getCompletedOrderId() ){
                        wareHouse.moveOrder(*wareHouse.getOrder(activeO), 0);
                        // if (!vol->canTakeOrder())
                        //     wareHouse.deleteVol(vol);
                    }
                }
        }
        
    
        std::string toString() const override{return "step " + std::to_string(numOfSteps);}
        SimulateStep *clone() const override{return new SimulateStep(*this);}

    private:
        const int numOfSteps;
};

class AddOrder : public BaseAction {
    public:
        AddOrder(int id): customerId{id}{};
        void act(WareHouse &wareHouse) override{
            if (customerId>wareHouse.getCustomerCounter() || customerId<0)
                error("Cannot place this order");
            else if (wareHouse.getCustomer(customerId).canMakeOrder()){

            Order* newOrd = new Order (wareHouse.getOrderCounter(), customerId,
             wareHouse.getCustomer(customerId).getCustomerDistance());
            wareHouse.addOrder(newOrd);
            complete();
            }else
            error("Cannot place this order");

            wareHouse.addAction(this);

            }
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
        void act(WareHouse &wareHouse) override{ 
            CivilianCustomer* cos = new CivilianCustomer(wareHouse.getCustomerCounter(),
                 customerName, distance, maxOrders);
            if (customerType==CustomerType::Soldier){
                delete cos;
                SoldierCustomer* cos = new SoldierCustomer(wareHouse.getCustomerCounter(),
                 customerName, distance, maxOrders);}
            wareHouse.addCustomer(cos);
            complete();
            wareHouse.addAction(this);
        }
        AddCustomer *clone() const override{return new AddCustomer(*this);}
        string toString() const override {return "customer "+ customerName + CTToString(customerType)
         + std::to_string(distance) + std::to_string(maxOrders);}
    private:
        const string customerName;
        const CustomerType customerType;
        const int distance;
        const int maxOrders;
};


class AddVolunteer : public BaseAction {

public:
        AddVolunteer(string _name, string _volunteerType, int _coolDown,  int _maxOrders, int _maxDistance, int _distancePerStep):
        name{name}, volunteerType{_volunteerType}, cooldown{_coolDown}, maxOrders{_maxOrders}, 
        maxDistance{_maxDistance}, distancePerStep{_distancePerStep}{};
        void act(WareHouse &wareHouse) override{ 
            CollectorVolunteer* vol =new CollectorVolunteer(wareHouse.getVolunteerCounter()
            , name, cooldown);
        
         if (volunteerType=="limited_collector"){
            delete vol;
            LimitedCollectorVolunteer* vol = new LimitedCollectorVolunteer(wareHouse.getVolunteerCounter()
            , name, cooldown, maxOrders);
         }
        
        if (volunteerType=="driver"){
            delete vol;
            DriverVolunteer* vol = new DriverVolunteer(wareHouse.getVolunteerCounter(),
             name, maxDistance, distancePerStep);

        }
        
        if (volunteerType=="limited_ driver"){
            delete vol;
            LimitedDriverVolunteer* vol = new LimitedDriverVolunteer(wareHouse.getVolunteerCounter(),
             name, maxDistance, distancePerStep, maxOrders);}
             
            wareHouse.addVolunteer(vol);
            complete();
            wareHouse.addAction(this);

            
        }

        AddVolunteer *clone() const override {return new AddVolunteer(*this);}
        string toString() const override {string s = "volunteer " +name + " "+ volunteerType + " ";
        if (volunteerType=="collector")
            s= s + std::to_string(cooldown);
        
         if (volunteerType=="limited_collector")
            s= s + std::to_string(cooldown) +" "+ std::to_string(maxOrders);
        
        if (volunteerType=="driver")
            s= s + std::to_string(maxDistance) +" "+ std::to_string(distancePerStep);
        
        if (volunteerType=="limited_ driver")
            s= s + std::to_string(maxDistance) +" "+ std::to_string( distancePerStep) 
            +" "+ std::to_string(maxOrders);
        
        }

    private:
        const string name;
        const string volunteerType;
        const int cooldown;
        const int maxOrders;
        const int maxDistance;
        const int distancePerStep;
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

        if (orderId>wareHouse.getOrderCounter() || orderId<0)
            error("Order doesn't exist");
        else{
        Order _order = wareHouse.getOrder(orderId); 
        
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
        complete();}
        wareHouse.addAction(this);
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

        if (customerId>wareHouse.getCustomerCounter() || customerId<0){
            error("Customer doesn't exist");
        }else{
        Customer &_customer = wareHouse.getCustomer(customerId); 
        
        std::cout<< "CustomerID: " + std::to_string(customerId);
        vector<int> const orders = _customer.getOrdersIds();

        PrintOrderStatus* pos = new PrintOrderStatus(420);
        for (const int ord: orders){
            std::cout<< "\nOrderID: " + std::to_string(ord);
            std::cout<< "\nOrderStatus: " + pos->OSToString(wareHouse.getOrder(ord).getStatus());
        }
        int numLeft = _customer.getMaxOrders() - _customer.getNumOrders();
            std::cout<< "\nnumOrdersLeft: " + std::to_string(numLeft);
        complete();}
        wareHouse.addAction(this);
        }

        PrintCustomerStatus *clone() const override{return new PrintCustomerStatus(*this);}
        string toString() const override{return "customerStatus " + std::to_string(customerId);}
    private:
        const int customerId;
};


class PrintVolunteerStatus : public BaseAction {
    public:
        PrintVolunteerStatus(int id):volunteerId{id}{};
        void act(WareHouse &_wareHouse) override{
            if (volunteerId>_wareHouse.getVolunteerCounter() || volunteerId<0){
                error("Volunteer doesn't exist");
            }else{
            Volunteer &vol =_wareHouse.getVolunteer(volunteerId);

            std::cout<<"voluteerID: " + std::to_string(volunteerId);
            bool busy = vol.isBusy();
            if (!busy)
                std::cout<< "\n isBusy: False \n None \n None";
            else std::cout<<"\n isBusy: True \nOrderId: " + std::to_string(vol.getActiveOrderId());
            
             if (vol.whoAmI()==0){         //is a collector volunteer
                 CollectorVolunteer* cv = dynamic_cast <CollectorVolunteer*>(&vol);
                 if (busy)
                   std::cout<<"\nTimeLeft: "+  std::to_string(cv->getTimeLeft()) + "OrdersLeft: No Limit";
                delete cv;
             }
            else if (vol.whoAmI()==1){         //is a limited collector volunteer
                    LimitedCollectorVolunteer* lcv = dynamic_cast <LimitedCollectorVolunteer*>(&vol);
                        if(busy)
                    std::cout<<"\nTimeLeft: "+  std::to_string(lcv->getTimeLeft());
                    std::cout<<"\nOrdersleft: " + std::to_string(lcv->getActiveOrderId());
                    delete lcv;
             }
                
             else if (vol.whoAmI()==2){//is a driver volunteer
                DriverVolunteer* dv = dynamic_cast <DriverVolunteer*>(&vol);
                if(busy)
                    std::cout<<"\nDistanceLeft: "+  std::to_string(dv->getDistanceLeft());
                delete dv;
                std::cout<<"\nOrders Left: No Limit";
             }
            else {         //is a limited Driver volunteer
                    LimitedDriverVolunteer* ldv = dynamic_cast <LimitedDriverVolunteer*>(&vol);
                        if(busy)
                    std::cout<<"\nDistanceLeft: "+  std::to_string(ldv->getDistanceLeft());
                    std::cout<<"\nOrdersleft: " + std::to_string(ldv->getActiveOrderId());
                    delete ldv;
                } 
            
                complete();
            }
                _wareHouse.addAction(this);
}

        PrintVolunteerStatus *clone() const override{return new PrintVolunteerStatus(*this);}
        string toString() const override{return "volunteerStatus " + std::to_string(volunteerId);}
    private:
        const int volunteerId;
};


class PrintActionsLog : public BaseAction {
    public:
        PrintActionsLog();
        void act(WareHouse &wareHouse) override{wareHouse.printActionsLogs();
        complete();
        wareHouse.addAction(this);
        }
        PrintActionsLog *clone() const override{return new PrintActionsLog(*this);}
        string toString() const override{return "log";}
    private:
};

class Close : public BaseAction {
    public:
        Close();
        void act(WareHouse &wareHouse) override;
        Close *clone() const override{return new Close(*this);};
        string toString() const override{return "close";};
    private:
};

class BackupWareHouse : public BaseAction {
    public:
        BackupWareHouse();
        void act(WareHouse &wareHouse) override{
            backup = &wareHouse;
        };
        BackupWareHouse *clone() const override{return new BackupWareHouse(*this);};
        string toString() const override{return "backup";};
    private:
};


class RestoreWareHouse : public BaseAction {
    public:
        RestoreWareHouse();
        void act(WareHouse &wareHouse) override{
            if (backup== nullptr){
                error("No backup available");
            }
            else{
                wareHouse=*backup;
                        }
        };
        RestoreWareHouse *clone() const override{return new RestoreWareHouse(*this);};
        string toString() const override{return "restore";};
    private:
};