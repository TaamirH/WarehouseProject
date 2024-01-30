
#include <string>
#include <vector>
#include "../include/Order.h"
using std::string;
using std::vector;


#define NO_VOLUNTEER -1

        Order::Order(int _id, int _customerId, int _distance):
            id{_id},customerId{_customerId},distance{_distance}{collectorId=NO_VOLUNTEER; driverId=NO_VOLUNTEER;};
        int Order::getId() const{
            return id;}
        int Order::getCustomerId() const{
            return customerId;
        }
        void Order::setStatus(OrderStatus _status){status =_status;}
        void Order::setCollectorId(int _collectorId){collectorId=_collectorId;}
        void Order::setDriverId(int _driverId){driverId = _driverId;}
        int Order::getCollectorId() const{return collectorId;}
        int Order::getDriverId() const{return driverId;}
        OrderStatus Order::getStatus() const{return status;}
        const string Order::toString() const{return "order " +std::to_string(getId());}
        int Order::getDistance() const {return distance;}
        Order* Order::clone() const {return new Order(*this);}

        // const int id;
        // const int customerId;
        // const int distance;
        // OrderStatus status;
        // int collectorId; //Initialized to NO_VOLUNTEER if no collector has been assigned yet
        // int driverId; //Initialized to NO_VOLUNTEER if no driver has been assigned yet
