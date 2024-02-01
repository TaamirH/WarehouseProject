#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
class WareHouse;
#include "../include/Customer.h"
class volunteer;
#include "../include/Volunteer.h"
#include "../include/Action.h"
class customer;
using std::string;
using std::vector;



//



        BaseAction:: BaseAction(){};
        ActionStatus BaseAction::getStatus() const{return status;}
        // virtual void act(WareHouse& wareHouse)=0;
        // virtual string toString() const=0;
        // virtual BaseAction* clone() const=0;

    
        void BaseAction::complete(){
            status = ActionStatus::COMPLETED;
        };
        void BaseAction::error(std::string _errorMsg){
            status = ActionStatus::ERROR;
            errorMsg=_errorMsg;
            std::cout <<"Error:"<<errorMsg;
        };
        std::string BaseAction::getErrorMsg() const{
            return errorMsg;}

    // private:
    //     string errorMsg;
    //     ActionStatus status;



        SimulateStep ::SimulateStep(int _numOfSteps):numOfSteps{_numOfSteps}{};

        void SimulateStep ::act(WareHouse &wareHouse) {

            for (int i = numOfSteps ; i>0 ; i=i-1){
            
            for (auto* vol : wareHouse.getVolunteers()){
                for (Order* ord : wareHouse.getPendingOrders()){
                    if(vol->canTakeOrder(*ord)){
                        std::cout<<"\norder taken by:";
                        std::cout<<vol->toString();
                        vol->acceptOrder( *ord);
                        wareHouse.moveOrder(ord, vol->getId());
                        break;
                    }
                }} 
                for (auto* vol : wareHouse.getVolunteers()){
                    int activeO = vol->getActiveOrderId();
                    std::cout<<"\nactiveO = " +std::to_string(activeO);
                    vol->step();
                    if (activeO==vol->getCompletedOrderId() && !(activeO==NO_ORDER) ){
                        std::cout<<"\ninside loop order" + std::to_string(activeO);
                        wareHouse.moveOrder(&wareHouse.getOrder(activeO), -1);
                        if (!vol->hasOrdersLeft())
                            wareHouse.deleteVol(vol);
                    }
                }
            }

                complete();
        }
        
    
        std::string SimulateStep ::toString() const {return "step " + std::to_string(numOfSteps);}
        SimulateStep * SimulateStep ::clone() const {return new SimulateStep(*this);}

    // private:
        // const int numOfSteps;



        AddOrder ::AddOrder(int id): customerId{id}{};
        void AddOrder ::act(WareHouse &wareHouse) {


    const Customer& customer = wareHouse.getCustomer(customerId);

  if (customerId < 0 || customerId > wareHouse.getCustomerCounter() || 
      !customer.canMakeOrder()) {
    error("Cannot place this order");
    return; // Early return on error
  }

  int orderId = wareHouse.getOrderCounter();

   Order* newOrderPtr = new Order(wareHouse.getCustomer(customerId).addOrder(orderId), customerId, customer.getCustomerDistance());

  wareHouse.addOrder(newOrderPtr);

  complete();
            }
        std::string AddOrder ::toString() const {return "order " + std::to_string(customerId);}
        AddOrder * AddOrder ::clone() const {return new AddOrder(*this);}
    // private:
        // const int customerId;



    CustomerType AddCustomer ::toCustomerType(const std::string& str) {
        if (str == "Soldier" || str == "soldier") {
            return CustomerType::Soldier;
    }   else if (str == "Civilian"|| str == "civilian") {
            return CustomerType::Civilian;
    }   else {
        // Handle invalid input
            throw std::invalid_argument("Invalid customer type string");
    }
}
    std::string AddCustomer ::CTToString(CustomerType _customerType) const{
        if (_customerType== CustomerType::Soldier)
            return "Soldier";
        else if (_customerType==CustomerType::Civilian)
            return "Civilian";
        else
            throw std::invalid_argument("Invalid customer type string");
    }



        AddCustomer ::AddCustomer(std::string _customerName, std::string _customerType, int _distance, int _maxOrders):
        customerName{_customerName}, customerType{toCustomerType(_customerType)}, distance{_distance}, 
        maxOrders{_maxOrders}{};
        void AddCustomer ::act(WareHouse &wareHouse) {     
            CivilianCustomer* cos = new CivilianCustomer(wareHouse.getCustomerCounter(),
                 customerName, distance, maxOrders);
            if (customerType==CustomerType::Soldier){
                SoldierCustomer* cos = new SoldierCustomer(wareHouse.getCustomerCounter(),
                 customerName, distance, maxOrders);}
            wareHouse.addCustomer(cos);
            complete();
        }
        AddCustomer * AddCustomer ::clone() const {return new AddCustomer(*this);}
        std::string AddCustomer ::toString() const {return "customer "+ customerName + " "+ CTToString(customerType)
        + " " + std::to_string(distance) + " "+std::to_string(maxOrders);}
    // private:
    //     const string customerName;
    //     const CustomerType customerType;
    //     const int distance;
    //     const int maxOrders;




        AddVolunteer ::AddVolunteer(std::string _name, std::string _volunteerType, int _coolDown,  int _maxOrders, int _maxDistance, int _distancePerStep):
        name{name}, volunteerType{_volunteerType}, cooldown{_coolDown}, maxOrders{_maxOrders}, 
        maxDistance{_maxDistance}, distancePerStep{_distancePerStep}{};

        void AddVolunteer ::act(WareHouse &wareHouse) { 
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

            
        }

        AddVolunteer * AddVolunteer ::clone() const {return new AddVolunteer(*this);}
        std::string AddVolunteer ::toString() const {std::string s = "volunteer " +name + " "+ volunteerType + " ";
        if (volunteerType=="collector")
            s= s + std::to_string(cooldown);
        
         if (volunteerType=="limited_collector")
            s= s + std::to_string(cooldown) +" "+ std::to_string(maxOrders);
        
        if (volunteerType=="driver")
            s= s + std::to_string(maxDistance) +" "+ std::to_string(distancePerStep);
        
        if (volunteerType=="limited_ driver")
            s= s + std::to_string(maxDistance) +" "+ std::to_string( distancePerStep) 
            +" "+ std::to_string(maxOrders);
         return s;
        }

//     private:
//         const string name;
//         const string volunteerType;
//         const int cooldown;
//         const int maxOrders;
//         const int maxDistance;
//         const int distancePerStep;

        




    std::string PrintOrderStatus ::OSToString(OrderStatus _orderStatus) {
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

        PrintOrderStatus ::PrintOrderStatus(int id):orderId(id){};

        void PrintOrderStatus ::act(WareHouse &wareHouse)  {

        if (orderId>=wareHouse.getOrderCounter() || orderId<0)
            error("Order doesn't exist");
        else{
        Order _order = wareHouse.getOrder(orderId); 
        
        std::cout<< "OrderId: " + std::to_string(orderId) +
        "\nOrderStatus: " + OSToString(_order.getStatus());
        if (_order.getCustomerId() == NO_VOLUNTEER)
            std::cout<<"\nNone";
        else
        std::cout << "\nCostumerID: " +std::to_string (_order.getCustomerId()) ;

        if (_order.getCollectorId() == NO_VOLUNTEER)
            std::cout<<"\nNone";
        else
        std::cout<< "\nCollectorID: " +std::to_string (_order.getCollectorId());

        if (_order.getDriverId() == NO_VOLUNTEER)
            std::cout<<"\nNone";
        else
        std::cout << "\nDriverID: " +std::to_string (_order.getDriverId());

        complete();}
        }
        PrintOrderStatus * PrintOrderStatus ::clone() const {return new PrintOrderStatus(*this);}
        string PrintOrderStatus ::toString() const {return "orderStatus" + std::to_string(orderId);}
    // private:
        // const int orderId;



        PrintCustomerStatus::PrintCustomerStatus(int _customerId):customerId{_customerId}{};
        void PrintCustomerStatus::act(WareHouse &wareHouse) {

        if (customerId>=wareHouse.getCustomerCounter() || customerId<0){
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
        int numLeft = (_customer.getMaxOrders() - _customer.getNumOrders());
            std::cout<< "\nnumOrdersLeft: " + std::to_string(numLeft);
        complete();}
        }

        PrintCustomerStatus * PrintCustomerStatus::clone() const {return new PrintCustomerStatus(*this);}
        string PrintCustomerStatus::toString() const {return "customerStatus " + std::to_string(customerId);}
    // private:
        // const int customerId;


        PrintVolunteerStatus ::PrintVolunteerStatus(int id):volunteerId{id}{};
        void PrintVolunteerStatus ::act(WareHouse &_wareHouse) {
            auto it = std::find(_wareHouse.getDeletedVolunteers().begin(),
             _wareHouse.getDeletedVolunteers().end(), volunteerId);
            if (volunteerId>=_wareHouse.getVolunteerCounter() || volunteerId<0 ||(it != _wareHouse.getDeletedVolunteers().end()) ) {
                error("Volunteer doesn't exist");
            }else{
            Volunteer &vol =_wareHouse.getVolunteer(volunteerId);

            std::cout<<"voluteerID: " + std::to_string(volunteerId);
            bool busy = vol.isBusy();
            if (!busy)
                std::cout<< "\nisBusy: False\nNone \nNone";
            else std::cout<<"\nisBusy: True \nOrderId: " + std::to_string(vol.getActiveOrderId());
            
             if (vol.whoAmI()==0){         //is a collector volunteer
                 CollectorVolunteer* cv = dynamic_cast <CollectorVolunteer*>(&vol);
                 if (busy)
                   std::cout<<"\nTimeLeft: "+  std::to_string(cv->getTimeLeft()) ;
                else
                    std::cout<<"\nNone";
                    std::cout<< "\nOrdersLeft: No Limit";
                
             }
            else if (vol.whoAmI()==1){         //is a limited collector volunteer
                    LimitedCollectorVolunteer* lcv = dynamic_cast <LimitedCollectorVolunteer*>(&vol);
                        if(busy)
                    std::cout<<"\nTimeLeft: "+  std::to_string(lcv->getTimeLeft());
                    else
                    std::cout<<"\nNone";
                    std::cout<<"\nOrdersleft: " + std::to_string(lcv->getNumOrdersLeft());
                    
             }
                
             else if (vol.whoAmI()==2){//is a driver volunteer
                DriverVolunteer* dv = dynamic_cast <DriverVolunteer*>(&vol);
                if(busy)
                    std::cout<<"\nDistanceLeft: "+  std::to_string(dv->getDistanceLeft());
                    else
                    std::cout<<"\nNone";
                
                std::cout<<"\nOrders Left: No Limit";
             }
            else {         //is a limited Driver volunteer
                    LimitedDriverVolunteer* ldv = dynamic_cast <LimitedDriverVolunteer*>(&vol);
                        if(busy)
                    std::cout<<"\nDistanceLeft: "+  std::to_string(ldv->getDistanceLeft());
                    else
                    std::cout<<"\nNone";
                    std::cout<<"\nOrdersleft: " + std::to_string(ldv->getNumOrdersLeft());
                    
                } 
            
                complete();
            }
}

        PrintVolunteerStatus * PrintVolunteerStatus ::clone() const {return new PrintVolunteerStatus(*this);}
        std::string PrintVolunteerStatus ::toString() const {return "volunteerStatus " + std::to_string(volunteerId);}
    // private:
    //     const int volunteerId;


        PrintActionsLog ::PrintActionsLog(){};
        void PrintActionsLog ::act(WareHouse &wareHouse) {wareHouse.printActionsLogs();
        complete();
              }
        PrintActionsLog* PrintActionsLog ::clone() const {return new PrintActionsLog(*this);}
        std::string PrintActionsLog ::toString() const {return "log";}
    // private:



        Close::Close(){};
        void Close::act(WareHouse &wareHouse) {
            PrintOrderStatus print = PrintOrderStatus(410);
            for (int i=0 ; i<wareHouse.getOrderCounter();i++){
                Order &order = wareHouse.getOrder(i);
                std::cout<<"OrderID: " + std::to_string(i) +
                ", CustomerID: " + std::to_string(order.getCustomerId()) + 
                ", OrderStatus: " + print.OSToString(order.getStatus()) +"\n";
            }
            wareHouse.close();
            complete();
        };
        Close * Close::clone() const {return new Close(*this);};
        std::string Close::toString() const {return "close";};
    // private:


        BackupWareHouse ::BackupWareHouse(){}
        void BackupWareHouse ::act(WareHouse &wareHouse) {
            backup = new WareHouse(wareHouse);
            complete();
        };
        BackupWareHouse* BackupWareHouse :: clone() const {return new BackupWareHouse(*this);};
        std::string BackupWareHouse ::toString() const {return "backup";};




        RestoreWareHouse ::RestoreWareHouse(){}
        void RestoreWareHouse ::act(WareHouse &wareHouse) {
            if (backup== nullptr){
                error("No backup available");
            }
            else{
                wareHouse=*backup;
            complete();
            }
                        
        };
        RestoreWareHouse * RestoreWareHouse ::clone() const {return new RestoreWareHouse(*this);};
        std::string RestoreWareHouse ::toString() const{return "restore";};
