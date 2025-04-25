#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

const int MAX_UNITS_PER_CELL = 10;  // Макс. вместимость ячейки

struct Item {  // Данные о товаре
    string name;
    int quantity;
};

map<string, vector<Item>> warehouse;  // Структура склада

// Генерация адресов ячеек по заданному шаблону
vector<string> generateAddresses() {
    vector<string> addresses;
    for (char zone = 'A'; zone <= 'J'; ++zone) {       // Зоны A-J
        for (int rack = 1; rack <= 3; ++rack) {        // Стеллажи 1-3
            int section = 1;                           // Секция всегда 1
            for (int shelf = 1; shelf <= 5; ++shelf) { // Полки 1-5
                // Формат адреса: зона+стеллаж+секция+полка (A131)
                addresses.push_back(zone + to_string(rack) + to_string(section) + to_string(shelf));
            }
        }
    }
    return addresses;
}

// Инициализация склада (все ячейки пустые)
void initializeWarehouse() {
    for (const auto& address : generateAddresses()) {
        warehouse[address] = {};
    }
}

// Подсчет общего количества товаров в ячейке
int getTotalInCell(const vector<Item>& items) {
    int total = 0;
    for (const auto& item : items) {
        total += item.quantity;
    }
    return total;
}

// Добавление товара с проверками
void addItem(string name, int quantity, string address) {
    // Проверка корректности количества
    if (quantity <= 0) {
        cout << "Ошибка: количество товара должно быть положительным.\n";
        return;
    }

    // Проверка существования ячейки
    if (warehouse.find(address) == warehouse.end()) {
        cout << "Ошибка: Ячейка " << address << " не существует.\n";
        return;
    }

    // Проверка свободного места
    int current = getTotalInCell(warehouse[address]);
    if (current + quantity > MAX_UNITS_PER_CELL) {
        cout << "Ошибка: В ячейке " << address << " только " << (MAX_UNITS_PER_CELL - current) << " свободных мест.\n";
        return;
    }

    // Поиск существующего товара
    bool found = false;
    for (auto& item : warehouse[address]) {
        if (item.name == name) {
            item.quantity += quantity;
            found = true;
            break;
        }
    }

    // Добавление нового товара при необходимости
    if (!found) {
        warehouse[address].push_back({name, quantity});
    }

    cout << "Добавлено " << quantity << " ед. товара '" << name << "' в " << address << ".\n";
}

// Удаление товара с проверками
void removeItem(string name, int quantity, string address) {
    // Проверка корректности количества
    if (quantity <= 0) {
        cout << "Ошибка: количество товара должно быть положительным.\n";
        return;
    }

    // Проверка существования ячейки
    if (warehouse.find(address) == warehouse.end()) {
        cout << "Ошибка: Ячейка " << address << " не существует.\n";
        return;
    }

    auto& items = warehouse[address];
    int removed = 0;
    vector<Item> newItems;

    // Удаление указанного количества товара
    for (auto& item : items) {
        if (item.name == name && removed < quantity) {
            if (item.quantity + removed <= quantity) {
                removed += item.quantity;
                continue;  // Полное удаление товара
            } else {
                int take = quantity - removed;
                item.quantity -= take;
                removed += take;
                newItems.push_back(item);
            }
        } else {
            newItems.push_back(item);
        }
    }

    // Обработка результата удаления
    if (removed < quantity) {
        cout << "Ошибка: Недостаточно товара '" << name << "' в " << address << ". Убрать можно только " << removed << ".\n";
    } else {
        warehouse[address] = newItems;
        cout << "Удалено " << removed << " ед. товара '" << name << "' из " << address << ".\n";
    }
}

// Вывод полной информации о складе
void info() {
    map<char, int> zoneLoad;        // Нагрузка по зонам
    vector<string> emptyCells;      // Список пустых ячеек
    map<string, int> sectionLoad;   // Загрузка секций

    cout << "\nЗанятые ячейки:\n";
    for (const auto& [address, items] : warehouse) {
        int total = getTotalInCell(items);
        if (total > 0) {
            cout << address << ": ";
            for (const auto& item : items) {
                cout << item.name << "(" << item.quantity << ") ";
                zoneLoad[address[0]] += item.quantity;

                // Секция определяется по первым трём символам: зона+стеллаж+секция
                string sectionKey = address.substr(0, 3);
                sectionLoad[sectionKey] += item.quantity;
            }
            cout << "\n";
        } else {
            emptyCells.push_back(address);
        }
    }

    // Вывод загрузки по зонам
    cout << "\nЗагрузка по зонам:\n";
    for (auto& [zone, qty] : zoneLoad) {
        double percent = (qty / 150.0) * 100.0;
        cout << "Зона " << zone << ": " << qty << " из 150 (" << percent << "%)\n";
    }

    // Вывод загрузки секций
    cout << "\nЗаполненность секций:\n";
    for (auto& [section, qty] : sectionLoad) {
        double percent = (qty / 50.0) * 100.0;
        cout << "Секция " << section << ": " << qty << " из 50 (" << percent << "%)\n";
    }

    // Пустые ячейки
    cout << "\nПустые ячейки:\n";
    for (const auto& address : emptyCells) {
        cout << address << " ";
    }
    cout << "\n";
}


// Основной цикл программы
int main() {
    initializeWarehouse();
    string line;

    cout << "Введите команды (ADD, REMOVE, INFO). Для выхода введите EXIT.\n";

    while (true) {
        cout << "> ";
        getline(cin, line);
        if (line == "EXIT") break;

        stringstream ss(line);
        string command;
        ss >> command;

        // Обработка команд
        if (command == "ADD") {
            string name, address;
            int qty;
            ss >> name >> qty >> address;
            addItem(name, qty, address);
        } else if (command == "REMOVE") {
            string name, address;
            int qty;
            ss >> name >> qty >> address;
            removeItem(name, qty, address);
        } else if (command == "INFO") {
            info();
        } else {
            cout << "Неизвестная команда. Используйте ADD, REMOVE или INFO.\n";
        }
    }

    return 0;
}
