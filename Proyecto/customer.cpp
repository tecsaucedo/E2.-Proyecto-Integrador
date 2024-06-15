// =========================================================
// File: customer.cpp
// Author: César Ignacio Saucedo Rodríguez A01712245
// Date: 12/06/2024
// Description: Customer
// =========================================================


#include "customer.h"

// Constructor 
Customer::Customer(int id, std::string name, int age, Operator* op, double creditLimit)
: id(id), name(name), age(age), op(op), totalSpentTalkingTime(0), totalMessageSent(0), totalInternetUsage(0) {
    bill = new Bill(creditLimit);
}


Customer::Customer(const Customer& other)
: id(other.id), name(other.name), age(other.age), totalSpentTalkingTime(other.totalSpentTalkingTime),
  totalMessageSent(other.totalMessageSent), totalInternetUsage(other.totalInternetUsage) {
    op = other.op;  // Shallow copy, might need deep copy based on ownership semantics
    bill = new Bill(*other.bill);
}

Customer::~Customer() {
    delete bill;
    bill = nullptr;
    op = nullptr;  
}


int Customer::getId() const { return id; }
int Customer::getAge() const { return age; }
std::string Customer::getName() const { return name; }
Operator* Customer::getOperator() const { return op; }
Bill* Customer::getBill() const { return bill; }
int Customer::getTotalSpentTalkingTime() const { return totalSpentTalkingTime; }
int Customer::getTotalMessageSent() const { return totalMessageSent; }
double Customer::getTotalInternetUsage() const { return totalInternetUsage; }


void Customer::setOperator(Operator* newOp) {
    op = newOp;
}


void Customer::talk(int minutes, Customer& other) {
    if (minutes > 0 && this != &other) {
        double cost = op->calculateTalkingCost(minutes, age);
        bill->add(cost);
        totalSpentTalkingTime += minutes;
        op->addTalkingTime(minutes);
    }
}

void Customer::message(int quantity, const Customer& other) {
    if (quantity > 0 && this != &other) {
        double cost = op->calculateMessageCost(quantity, this->getId(), other.getId());
        bill->add(cost);
        totalMessageSent += quantity;
        op->addTotalMessageSent(quantity);
    }
}

void Customer::connection(double amount) {
    if (amount > 0) {
        double cost = op->calculateNetworkCost(amount);
        bill->add(cost);
        totalInternetUsage += amount;
        op->addTotalInternetUsage(amount);
    }
}

void Customer::pay(double amount) {
    bill->pay(amount);
}

std::string Customer::toString() const {
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(2);
    stream << "Customer #" << id << ": " << bill->getTotalMoneySpent() << " spent, " << bill->getCurrentDebt() << " current debt.";
    return stream.str();
}
