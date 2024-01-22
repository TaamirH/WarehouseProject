#pragma once

#include <string>
#include <vector>
using std::string;
using std::vector;

enum class OrderStatus {
    PENDING,
    COLLECTING,
    DELIVERING,
    COMPLETED,
};

#define NO_VOLUNTEER -1

class Order {

    public:
        Order(int _id, int _customerId, int _distance):
            id{_id},customerId{_customerId},distance{_distance}{};
        int getId() const{
            return id;}
        int getCustomerId() const{
            return customerId;
        };
        void setStatus(OrderStatus status){};
        void setCollectorId(int _collectorId){collectorId=_collectorId;}
        void setDriverId(int _driverId){driverId = _driverId;}
        int getCollectorId() const{return collectorId;}
        int getDriverId() const{return driverId;}
        OrderStatus getStatus() const{return status;}
        const string toString() const;

    private:
        const int id;
        const int customerId;
        const int distance;
        OrderStatus status;
        int collectorId; //Initialized to NO_VOLUNTEER if no collector has been assigned yet
        int driverId; //Initialized to NO_VOLUNTEER if no driver has been assigned yet
};