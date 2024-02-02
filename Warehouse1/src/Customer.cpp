#include <string>
#include <vector>
using std::string;
using std::vector;
#include "../include/Order.h"
#include "../include/Action.h"


        Customer::Customer(int _id, const string &_name, int _locationDistance, int _maxOrders):
            id{_id},name{_name},locationDistance{_locationDistance},maxOrders{_maxOrders},
            ordersId(),madeOrders(0){}
        const string& Customer::getName() const{
            return name; };
        int Customer::getId() const{
            return id;};
        int Customer::getCustomerDistance() const{
            return locationDistance; };
        int Customer::getMaxOrders() const{
            return maxOrders;}; //Returns maxOrders
        int Customer::getNumOrders() const{
            return madeOrders; }; //Returns num of orders the customer has made so far
        bool Customer::canMakeOrder() const{
            if (madeOrders<maxOrders)
                return true;
            return false;
        }; //Returns true if the customer didn't reach max orders
        const vector<int> & Customer::getOrdersIds() const{
            return ordersId;}

        int Customer::addOrder(int orderId){
            if (canMakeOrder()){
                ordersId.push_back(orderId);
                madeOrders++;
                return orderId;
                }
            else
                return -1;
        }; //return OrderId if order was added successfully, -1 otherwise

        // virtual Customer *clone() const = 0; // Return a copy of the customer

        
    // private:
    //     const int id;
    //     const string name;
    //     const int locationDistance;
    //     const int maxOrders;
    //     vector<int> ordersId;
    //     int madeOrders;




        SoldierCustomer::SoldierCustomer(int _id, const string &_name, int _locationDistance, int _maxOrders):
          Customer(_id, _name, _locationDistance, _maxOrders){};
        SoldierCustomer* SoldierCustomer:: clone() const {
            return new SoldierCustomer(*this);}
    


        CivilianCustomer::CivilianCustomer(int _id, const string &_name, int _locationDistance, int _maxOrders):
         Customer(_id, _name, _locationDistance, _maxOrders){};
        CivilianCustomer* CivilianCustomer::clone() const {
            return(new CivilianCustomer(*this));

        };
