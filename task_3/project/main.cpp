#include "commands.h"
#include "transport_manager.h"
#include <iostream>
#include <sstream>

int main() {
    TransportManager manager;
    std::string line;

    // Чтение команд из ввода
    while (getline(std::cin, line)) {
        std::istringstream iss(line);
        std::string command;
        iss >> command;

        if (command == "CREATE_TRL") {
            std::string name;
            iss >> name;
            std::vector<std::string> stops;
            std::string stop;
            while (iss >> stop) {
                stops.push_back(stop);
            }
            manager.CreateTrolleybus(name, stops);
        }
        else if (command == "TRL_IN_STOP") {
            std::string stop;
            iss >> stop;
            manager.TrolleybusesInStop(stop);
        }
        else if (command == "STOPS_IN_TRL") {
            std::string name;
            iss >> name;
            manager.StopsInTrolleybus(name);
        }
        else if (command == "TRLS") {
            manager.AllTrolleybuses();
        }
    }

    return 0;
}
