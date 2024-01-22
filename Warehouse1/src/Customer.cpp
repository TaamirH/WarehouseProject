#pragma once
#include <string>
#include <vector>
using std::string;
using std::vector;
#include "../include/Order.h"
#include "../include/Action.h"


class Customer {
    public:
        Customer(int _id, const string &_name, int _locationDistance, int _maxOrders):
            id{_id},name{_name},locationDistance{_locationDistance},maxOrders{_maxOrders}{
            madeOrders=0;
        };
        const string &getName() const{
            return name; };
        int getId() const{
            return id;};
        int getCustomerDistance() const{
            return locationDistance; };
        int getMaxOrders() const{
            return maxOrders;}; //Returns maxOrders
        int getNumOrders() const{
            return madeOrders; }; //Returns num of orders the customer has made so far
        bool canMakeOrder() const{
            if (madeOrders<maxOrders)
                return true;
            return false;
        }; //Returns true if the customer didn't reach max orders
        const vector<int> &getOrdersIds() const;
        int addOrder(int orderId){
            
        }; //return OrderId if order was added successfully, -1 otherwise

        virtual Customer *clone() const = 0; // Return a copy of the customer

        
    private:
        const int id;
        const string name;
        const int locationDistance;
        const int maxOrders;
        vector<int> ordersId;
        int madeOrders;
};


class SoldierCustomer: public Customer {
    public:
        SoldierCustomer(int id, const string &name, int locationDistance, int maxOrders);
        SoldierCustomer *clone() const override;
    
    private:
        
};

class CivilianCustomer: public Customer {
    public:
        CivilianCustomer(int id, const string &name, int locationDistance, int maxOrders);
        CivilianCustomer *clone() const override;
    
    private:
        
};