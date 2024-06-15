// =========================================================
// File: internet.cpp
// Author: César Ignacio Saucedo Rodríguez A01712245
// Date: 12/06/2024
// Description: internet
// =========================================================


#include "internet.h"

InternetOperator::InternetOperator(int id, double talkingCharge, double messageCost, double networkCharge, int discountRate, OperatorType type)
: Operator(id, talkingCharge, messageCost, networkCharge, discountRate, type) {}


InternetOperator::InternetOperator(const InternetOperator& other) : Operator(other) {}

double InternetOperator::calculateTalkingCost(int minutes, int age) {
    if (minutes <= 0 || age <= 0) return 0.0;

    double cost = minutes * getTalkingCharge();
    if (minutes < 2) {
        cost *= (1 - getDiscountRate() / 100.0);
    }
    return cost;
}


double InternetOperator::calculateMessageCost(int quantity, int thisOpId, int otherOpId) {
    if (quantity <= 0) return 0.0;

    double cost = quantity * getMessageCost();
    if (quantity < 3 && thisOpId == otherOpId) {
        cost *= (1 - getDiscountRate() / 100.0);
    }
    return cost;
}


double InternetOperator::calculateNetworkCost(double amount) {
    if (amount <= 0) return 0.0;

    if (amount > LIMITGB) {
        return (amount - LIMITGB) * getNetworkCharge();
    } else {
        return 0.0;
    }
}
