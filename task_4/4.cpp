#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

int main() {
    // Основная очередь студентов
    vector<int> students_queue;
    // Множество студентов в "топ-листе" для отчисления
    set<int> top_list;
    // Множество "бессмертных" студентов, которых нельзя отчислить
    set<int> immortals;

    // Счетчик для генерации уникальных номеров новых студентов
    int next_student_number = 1;

    string line;
    // Чтение команд построчно из стандартного ввода
    while (getline(cin, line)) {
        istringstream iss(line);
        string command;
        iss >> command;  // Извлекаем команду из строки

        if (command == "NEW_STUDENTS") {
            // Обработка команды добавления/удаления студентов
            int num;
            iss >> num;
            if (num > 0) {
                // Добавление новых студентов
                for (int i = 0; i < num; ++i)
                    students_queue.push_back(next_student_number++);
                cout << "Welcome " << num << " clever students!" << endl;
            } else if (-num <= (int)students_queue.size()) {
                // Удаление студентов с конца очереди
                cout << "GoodBye " << -num << " clever students!" << endl;
                for (int i = 0; i < -num; ++i) {
                    int removed = students_queue.back();
                    students_queue.pop_back();
                    // Удаляем студента из топ-листа и бессмертных, если он там был
                    top_list.erase(removed);
                }
            } else {
                // Некорректное количество студентов для удаления
                cerr << "Incorrect" << endl;
            }

        } else if (command == "SUSPICIOUS") {
            // Помещение студента в подозрительные (в конец очереди и в топ-лист)
            int num;
            iss >> num;
            auto it = find(students_queue.begin(), students_queue.end(), num);
            // Проверяем, что студент есть в очереди и он не бессмертный
            if (it != students_queue.end() && immortals.find(num) == immortals.end()) {
                students_queue.erase(it);
                students_queue.push_back(num);  // переместили в конец очереди
                top_list.insert(num);           // добавили в топ-лист для отчисления
                cout << "The suspected student " << num << endl;
            } else {
                cerr << "Incorrect" << endl;
            }

        } else if (command == "IMMORTIAL" || command == "IMMORTAL") {
            // Сделать студента бессмертным (исключить из топ-листа)
            int num;
            iss >> num;
            auto it = find(students_queue.begin(), students_queue.end(), num);
            if (it != students_queue.end()) {
                top_list.erase(num);      // убираем из списка на отчисление
                immortals.insert(num);    // добавляем в бессмертные
                cout << "Student " << num << " is immortal!" << endl;
            } else {
                cerr << "Incorrect" << endl;
            }

        } else if (command == "TOP-LIST") {
            // Вывод отсортированного списка студентов на отчисление
            cout << "List of students for expulsion:";
            if (top_list.empty()) {
                cout << endl;
            } else {
                // Создаем отсортированный вектор из множества
                vector<int> sorted(top_list.begin(), top_list.end());
                sort(sorted.begin(), sorted.end());
                // Выводим студентов через запятую
                for (size_t i = 0; i < sorted.size(); ++i) {
                    if (i > 0) cout << ",";
                    cout << " Student " << sorted[i];
                }
                cout << endl;
            }

        } else if (command == "SCOUNT") {
            // Вывод количества студентов в топ-листе
            cout << "List of students for expulsion consists of " << top_list.size() << " students" << endl;

        } else {
            // Неизвестная команда
            cerr << "Incorrect" << endl;
        }
    }

    return 0;
}
