#include "transport_manager.h"
#include <iostream>

void TransportManager::CreateTrolleybus(const std::string& name, const std::vector<std::string>& stops) {
    // Проверка: троллейбус с таким именем уже существует
    if (trls.count(name)) {
        std::cout << "Error: Trolleybus " << name << " already exists.\n";
        return;
    }

    // Проверка: маршрут должен содержать минимум две остановки
    if (stops.size() < 2) {
        std::cout << "Error: Trolleybus route must contain at least two stops.\n";
        return;
    }

    // Проверка: наличие одинаковых остановок
    std::set<std::string> unique_stops(stops.begin(), stops.end());
    if (unique_stops.size() != stops.size()) {
        std::cout << "Error: Route contains duplicate stops.\n";
        return;
    }

    // Сохраняем маршрут
    trls[name] = stops;

    // Обновляем словарь остановок
    for (const auto& stop : stops) {
        stop_to_trls[stop].insert(name);
    }
}


void TransportManager::TrolleybusesInStop(const std::string& stop) const {
    std::cout << "Stop " << stop << ":\n";
    if (stop_to_trls.count(stop) == 0) {
        std::cout << "  no trolleybuses\n";
        return;
    }
    // Выводим все троллейбусы, проходящие через остановку
    for (const auto& trl : stop_to_trls.at(stop)) {
        std::cout << "  " << trl << "\n";
    }
}

void TransportManager::StopsInTrolleybus(const std::string& name) const {
    if (trls.count(name) == 0) {
        std::cout << "No trolleybus\n";
        return;
    }

    std::cout << "Trolleybus " << name << ":\n";
    for (const auto& stop : trls.at(name)) {
        std::cout << "  Stop " << stop << ": ";
        bool has_interchange = false;

        // Проверяем, есть ли другие троллейбусы на этой остановке
        for (const auto& other_trl : stop_to_trls.at(stop)) {
            if (other_trl != name) {
                std::cout << other_trl << " ";
                has_interchange = true;
            }
        }

        if (!has_interchange) {
            std::cout << "no interchange";
        }
        std::cout << "\n";
    }
}


void TransportManager::AllTrolleybuses() const {
    if (trls.empty()) {
        std::cout << "No trolleybuses\n";
        return;
    }

    for (const auto& [name, stops] : trls) {
        std::cout << "Trolleybus " << name << ":\n";
        for (const auto& stop : stops) {
            std::cout << "  " << stop << "\n";
        }
    }
}
