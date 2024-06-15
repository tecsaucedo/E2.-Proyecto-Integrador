// =========================================================
// File: main.cpp
// Author: César Ignacio Saucedo Rodríguez A01712245
// Date: 14/jun/2024
// Description:
// To compile: g++ -std=c++11 main.cpp -o app
// To execute: ./app input_file output_file
// =========================================================
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include "customer.h"
#include "operator.h"
#include "vox.h"
#include "internet.h"

int numCustomers, numOperators, numEvents;
std::vector<Customer*> customers;
std::vector<Operator*> operators;

Operator* createOperator(int type, double talkingCharge, double messageCost, double networkCharge, int discountRate) {
    if (type == 1) {  
        return new VoxOperator(type, talkingCharge, messageCost, networkCharge, discountRate, VOX);
    } else if (type == 2) {  
        return new InternetOperator(type, talkingCharge, messageCost, networkCharge, discountRate, INTERNET);
    } else {
        std::cerr << "Error: Tipo de operador inválido: " << type << std::endl;
        return nullptr; 
    }
}

Customer* createCustomer(int id, const std::string& name, int age, Operator* op, double creditLimit) {
    return new Customer(id, name, age, op, creditLimit);
}

void handleTalk(Customer* caller, Customer* receiver, int minutes) {
    if (caller && receiver) {
        caller->talk(minutes, *receiver);
    }
}

void handleMessage(Customer* sender, Customer* receiver, int quantity) {
    if (sender && receiver) {
        sender->message(quantity, *receiver);
    }
}

void handleConnection(Customer* customer, double amount) {
    if (customer) {
        customer->connection(amount);
    }
}

void handlePayment(Customer* customer, double amount) {
    if (customer) {
        customer->pay(amount);
    }
}

void handleChangeOperator(Customer* customer, Operator* newOperator) {
    if (customer && newOperator) {
        customer->setOperator(newOperator);
    }
}

void handleChangeCreditLimit(Customer* customer, double newLimit) {
    if (customer) {
        customer->getBill()->changeTheLimit(newLimit);
    }
}

void generateOperatorReports(const std::vector<Operator*>& operators, std::ofstream& outputFile) {
    outputFile << "Operator Reports:\n";
    for (const auto& op : operators) {
        if (op) { 
            outputFile << "Operator " << op->getId() << ": "
                       << op->getTotalSpentTalkingTime() << " mins, "
                       << op->getTotalMessageSent() << " messages, "
                       << op->getTotalInternetUsage() << " GBs\n";
        }
    }
}

void generateCustomerReports(const std::vector<Customer*>& customers, std::ofstream& outputFile) {
    outputFile << "Customer Reports:\n";
    for (const auto& customer : customers) {
        if (customer) {  
            outputFile << "Customer " << customer->getId() << ": "
                       << customer->getBill()->getTotalMoneySpent() << " spent, "
                       << customer->getBill()->getCurrentDebt() << " debt\n";
        }
    }
}

void generateSpecialReports(const std::vector<Customer*>& customers, std::ofstream& outputFile) {
    // Encontrar y reportar el cliente que más habló
    auto maxTalk = std::max_element(customers.begin(), customers.end(),
        [](const Customer* a, const Customer* b) {
            return a->getTotalSpentTalkingTime() < b->getTotalSpentTalkingTime();
        });
    if (maxTalk != customers.end() && *maxTalk) {  
        outputFile << "Most talkative customer: " << (*maxTalk)->getName()
                   << " with " << (*maxTalk)->getTotalSpentTalkingTime() << " minutes\n";
    }

    // Encontrar y reportar el cliente que más mensajes envió
    auto maxMessage = std::max_element(customers.begin(), customers.end(),
        [](const Customer* a, const Customer* b) {
            return a->getTotalMessageSent() < b->getTotalMessageSent();
        });
    if (maxMessage != customers.end() && *maxMessage) {  
        outputFile << "Most messages sent by: " << (*maxMessage)->getName()
                   << " with " << (*maxMessage)->getTotalMessageSent() << " messages\n";
    }

    auto maxData = std::max_element(customers.begin(), customers.end(),
        [](const Customer* a, const Customer* b) {
            return a->getTotalInternetUsage() < b->getTotalInternetUsage();
        });
    if (maxData != customers.end() && *maxData) {  
        outputFile << "Most data used by: " << (*maxData)->getName()
                   << " with " << (*maxData)->getTotalInternetUsage() << " GB\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <inputfile> <outputfile>" << std::endl;
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    std::ofstream outputFile(argv[2]);

    if (!inputFile) {
        std::cerr << "Error: Could not open input file." << std::endl;
        return 1;
    }
    if (!outputFile) {
        std::cerr << "Error: Could not open output file." << std::endl;
        return 1;
    }

    inputFile >> numCustomers >> numOperators >> numEvents;

    customers.reserve(numCustomers);
    operators.reserve(numOperators);

    int type, age, operatorId;
    double talkingCharge, messageCost, networkCharge, creditLimit;
    int discountRate;
    std::string name;

    for (int i = 0; i < numOperators; ++i) {
        inputFile >> type >> talkingCharge >> messageCost >> networkCharge >> discountRate;
        Operator* op = createOperator(type, talkingCharge, messageCost, networkCharge, discountRate);
        if (!op) {
            std::cerr << "Error: No se pudo crear el operador con tipo: " << type << " en índice: " << i << "\n";
            continue;  
        }
        operators.push_back(op);
    }


    for (int i = 0; i < numCustomers; ++i) {
        inputFile >> name >> age >> operatorId >> creditLimit;

        if (operatorId < 0 || operatorId >= operators.size()) {
            std::cerr << "Error: Índice de operador fuera de límites: " << operatorId << "\n";
            continue;  
        }

        Operator* op = operators[operatorId]; 
        if (!op) {
            std::cerr << "Error: Operador no inicializado en el índice: " << operatorId << "\n";
            continue;  
        }

        Customer* customer = createCustomer(i, name, age, op, creditLimit);
        if (!customer) {
            std::cerr << "Error: No se pudo crear el cliente con ID: " << i << "\n";
            continue;  
        }

        customers.push_back(customer);
    }

    int eventType, customerId1, customerId2, quantity;
    double amount;

    for (int i = 0; i < numEvents; ++i) {
        inputFile >> eventType;
        switch (eventType) {
            case 3: 
                inputFile >> customerId1 >> customerId2 >> quantity;
                if (customerId1 >= 0 && customerId1 < customers.size() &&
                    customerId2 >= 0 && customerId2 < customers.size()) {
                    handleTalk(customers[customerId1], customers[customerId2], quantity);
                } else {
                    std::cerr << "Error: Índice de cliente fuera de límites.\n";
                }
                break;
            case 4: 
                inputFile >> customerId1 >> customerId2 >> quantity;
                if (customerId1 >= 0 && customerId1 < customers.size() &&
                    customerId2 >= 0 && customerId2 < customers.size()) {
                    handleMessage(customers[customerId1], customers[customerId2], quantity);
                } else {
                    std::cerr << "Error: Índice de cliente fuera de límites.\n";
                }
                break;
            case 5: 
                inputFile >> customerId1 >> amount;
                if (customerId1 >= 0 && customerId1 < customers.size()) {
                    handleConnection(customers[customerId1], amount);
                } else {
                    std::cerr << "Error: Índice de cliente fuera de límites.\n";
                }
                break;
            case 6:
                inputFile >> customerId1 >> amount;
                if (customerId1 >= 0 && customerId1 < customers.size()) {
                    handlePayment(customers[customerId1], amount);
                } else {
                    std::cerr << "Error: Índice de cliente fuera de límites.\n";
                }
                break;
            case 7: 
                inputFile >> customerId1 >> customerId2; 
                if (customerId1 >= 0 && customerId1 < customers.size() &&
                    customerId2 >= 0 && customerId2 < operators.size()) {
                    handleChangeOperator(customers[customerId1], operators[customerId2]);
                } else {
                    std::cerr << "Error: Índice de cliente u operador fuera de límites.\n";
                }
                break;
            case 8: 
                inputFile >> customerId1 >> amount;
                if (customerId1 >= 0 && customerId1 < customers.size()) {
                    handleChangeCreditLimit(customers[customerId1], amount);
                } else {
                    std::cerr << "Error: Índice de cliente fuera de límites.\n";
                }
                break;
        }
    }

    // Generar reportes
    generateOperatorReports(operators, outputFile);
    generateCustomerReports(customers, outputFile);
    generateSpecialReports(customers, outputFile);

    l
    inputFile.close();
    outputFile.close();

    for (auto& customer : customers) delete customer;
    customers.clear();

    for (auto& op : operators) delete op;
    operators.clear();

    return 0;
}
