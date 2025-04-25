#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <sstream>
#include <algorithm>
#include <set>
#include <cstdlib>
#include <ctime>

using namespace std;

// Структура для хранения информации о посетителе
struct Visitor {
    string ticket;    // Номер талона
    int duration;     // Время обслуживания в минутах
};

// Структура для хранения информации об окне обслуживания
struct Window {
    int id;           // Номер окна
    int totalTime;    // Общее время обслуживания всех посетителей
    vector<string> tickets;  // Список талонов посетителей

    // Перегрузка оператора для приоритетной очереди)
    bool operator>(const Window& other) const {
        return totalTime > other.totalTime;
    }
};

int main() {
    // Инициализация генератора случайных чисел
    srand(static_cast<unsigned>(time(0)));

    int numWindows;
    cout << "Введите кол-во окон\n>>> ";
    cin >> numWindows;

    vector<Visitor> queue;    // Очередь посетителей
    string command;          // Команда от пользователя
    int duration;            // Время обслуживания для нового посетителя
    set<int> usedNumbers;    // Множество использованных номеров талонов

    // Лямбда-функция для генерации уникального номера талона
    auto generateTicket = [&usedNumbers]() {
        int number;
        do {
            number = rand() % 900 + 100;
        } while (usedNumbers.count(number));  // Проверка на уникальность
        usedNumbers.insert(number);
        stringstream ss;
        ss << 'T' << number;
        return ss.str();
    };

    // Основной цикл обработки команд
    while (true) {
        cout << "<<< ";
        cin >> command;

        if (command == "ENQUEUE") {
            // Добавление нового посетителя в очередь
            cin >> duration;
            string ticket = generateTicket();
            queue.push_back({ticket, duration});
            cout << ">>> " << ticket << endl;
        } else if (command == "DISTRIBUTE") {
            // Сортировка очереди по убыванию времени обслуживания
            sort(queue.begin(), queue.end(), [](const Visitor& a, const Visitor& b) {
                return a.duration > b.duration;
            });

            // Приоритетная очередь для распределения посетителей по окнам
            priority_queue<Window, vector<Window>, greater<Window>> windows;

            // Инициализация окон
            for (int i = 0; i < numWindows; ++i) {
                windows.push({i + 1, 0, {}});
            }

            // Распределение посетителей по окнам
            for (const auto& visitor : queue) {
                Window win = windows.top();  // Берем окно с наименьшей загрузкой
                windows.pop();
                win.totalTime += visitor.duration;  // Увеличиваем общее время
                win.tickets.push_back(visitor.ticket);  // Добавляем талон
                windows.push(win);  // Возвращаем окно обратно в очередь
            }

            // Извлечение окон из приоритетной очереди для вывода
            vector<Window> result;
            while (!windows.empty()) {
                result.push_back(windows.top());
                windows.pop();
            }

            // Сортировка окон по их номеру для удобного вывода
            sort(result.begin(), result.end(), [](const Window& a, const Window& b) {
                return a.id < b.id;
            });

            // Вывод результатов
            for (const auto& win : result) {
                cout << ">>> Окно " << win.id << " (" << win.totalTime << " минут): ";
                for (size_t i = 0; i < win.tickets.size(); ++i) {
                    cout << win.tickets[i];
                    if (i < win.tickets.size() - 1) cout << ", ";
                }
                cout << endl;
            }

            break;
        }
    }

    return 0;
}
