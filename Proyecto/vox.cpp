// =========================================================
// File: vox.cpp
// Author: César Ignacio Saucedo Rodríguez A01712245
// Date: 12/06/2024
// Description: VoxOperator
// =========================================================


#include "vox.h"

// Constructor
VoxOperator::VoxOperator(int id, double talkingCharge, double messageCost, double networkCharge, int discountRate, OperatorType type)
: Operator(id, talkingCharge, messageCost, networkCharge, discountRate, type) {}

VoxOperator::VoxOperator(const VoxOperator& other) : Operator(other) {}


double VoxOperator::calculateTalkingCost(int minutes, int age) {
    if (minutes <= 0 || age <= 0) return 0.0;

    double cost = minutes * getTalkingCharge();
    if (age < 18 || age > 65) {
        cost *= (1 - getDiscountRate() / 100.0);
    }
    return cost;
}


double VoxOperator::calculateMessageCost(int quantity, int thisOpId, int otherOpId) {
    if (quantity <= 0) return 0.0;

    double cost = quantity * getMessageCost();
    if (thisOpId == otherOpId) {
        cost *= (1 - getDiscountRate() / 100.0);
    }
    return cost;
}


double VoxOperator::calculateNetworkCost(double amount) {
    if (amount <= 0) return 0.0;

    return amount * getNetworkCharge();
}
