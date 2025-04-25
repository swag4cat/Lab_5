#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>

// Класс для управления маршрутом троллейбусов и остановками
class TransportManager {
public:
    void CreateTrolleybus(const std::string& name, const std::vector<std::string>& stops);
    void TrolleybusesInStop(const std::string& stop) const;
    void StopsInTrolleybus(const std::string& name) const;
    void AllTrolleybuses() const;

private:
    // Маршрут каждого троллейбуса
    std::map<std::string, std::vector<std::string>> trls;

    // Список троллейбусов для каждой остановки
    std::map<std::string, std::set<std::string>> stop_to_trls;
};
