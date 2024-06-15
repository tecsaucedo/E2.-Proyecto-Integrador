// =========================================================
// File: bill.cpp
// Author: César Ignacio Saucedo Rodríguez A01712245
// Date: 12/06/2024
// Description: Client facturation system 
// =========================================================

#include "bill.h"

Bill::Bill() : limitAmount(0.0), currentDebt(0.0), totalMoneySpent(0.0) {}


Bill::Bill(double limit) : limitAmount(limit), currentDebt(0.0), totalMoneySpent(0.0) {}

Bill::Bill(const Bill& other) : limitAmount(other.limitAmount), currentDebt(other.currentDebt), totalMoneySpent(other.totalMoneySpent) {}


double Bill::getLimitAmount() const {
    return limitAmount;
}

double Bill::getCurrentDebt() const {
    return currentDebt;
}


double Bill::getTotalMoneySpent() const {
    return totalMoneySpent;
}


void Bill::add(double amount) {
    if (amount > 0) {
        if (currentDebt + amount <= limitAmount) {
            currentDebt += amount;
            totalMoneySpent += amount;
        }
    }
}


void Bill::pay(double amount) {
    if (amount > 0 && amount <= currentDebt) {
        currentDebt -= amount;
    }
}


void Bill::changeTheLimit(double amount) {
    if (amount > 0 && amount > currentDebt) {
        limitAmount = amount;
    }
}


bool Bill::check(double amount) const {
    return (currentDebt + amount) <= limitAmount;
}
