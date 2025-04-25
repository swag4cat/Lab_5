package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

const MAX_UNITS_PER_CELL = 10 // Макс. вместимость ячейки

type Item struct { // Данные о товаре
	name     string
	quantity int
}

var warehouse = make(map[string][]Item) // Структура склада

// Генерация адресов ячеек по заданному шаблону
func generateAddresses() []string {
	var addresses []string
	for zone := 'A'; zone <= 'J'; zone++ { // Зоны A-J
		for rack := 1; rack <= 3; rack++ { // Стеллажи 1-3
			section := 1 // Секция всегда 1
			for shelf := 1; shelf <= 5; shelf++ { // Полки 1-5
				// Формат адреса: зона+стеллаж+секция+полка (A131)
				address := string(zone) + strconv.Itoa(rack) + strconv.Itoa(section) + strconv.Itoa(shelf)
				addresses = append(addresses, address)
			}
		}
	}
	return addresses
}

// Инициализация склада (все ячейки пустые)
func initializeWarehouse() {
	for _, address := range generateAddresses() {
		warehouse[address] = []Item{}
	}
}

// Подсчет общего количества товаров в ячейке
func getTotalInCell(items []Item) int {
	total := 0
	for _, item := range items {
		total += item.quantity
	}
	return total
}

// Добавление товара с проверками
func addItem(name string, quantity int, address string) {
	// Проверка корректности количества
	if quantity <= 0 {
		fmt.Println("Ошибка: количество товара должно быть положительным.")
		return
	}

	// Проверка существования ячейки
	if _, exists := warehouse[address]; !exists {
		fmt.Printf("Ошибка: Ячейка %s не существует.\n", address)
		return
	}

	// Проверка свободного места
	current := getTotalInCell(warehouse[address])
	if current+quantity > MAX_UNITS_PER_CELL {
		fmt.Printf("Ошибка: В ячейке %s только %d свободных мест.\n", address, MAX_UNITS_PER_CELL-current)
		return
	}

	// Поиск существующего товара
	found := false
	for i, item := range warehouse[address] {
		if item.name == name {
			warehouse[address][i].quantity += quantity
			found = true
			break
		}
	}

	// Добавление нового товара при необходимости
	if !found {
		warehouse[address] = append(warehouse[address], Item{name, quantity})
	}

	fmt.Printf("Добавлено %d ед. товара '%s' в %s.\n", quantity, name, address)
}

// Удаление товара с проверками
func removeItem(name string, quantity int, address string) {
	// Проверка корректности количества
	if quantity <= 0 {
		fmt.Println("Ошибка: количество товара должно быть положительным.")
		return
	}

	// Проверка существования ячейки
	if _, exists := warehouse[address]; !exists {
		fmt.Printf("Ошибка: Ячейка %s не существует.\n", address)
		return
	}

	items := warehouse[address]
	removed := 0
	var newItems []Item

	// Удаление указанного количества товара
	for _, item := range items {
		if item.name == name && removed < quantity {
			if item.quantity+removed <= quantity {
				removed += item.quantity
				continue // Полное удаление товара
			} else {
				take := quantity - removed
				item.quantity -= take
				removed += take
				newItems = append(newItems, item)
			}
		} else {
			newItems = append(newItems, item)
		}
	}

	// Обработка результата удаления
	if removed < quantity {
		fmt.Printf("Ошибка: Недостаточно товара '%s' в %s. Убрать можно только %d.\n", name, address, removed)
	} else {
		warehouse[address] = newItems
		fmt.Printf("Удалено %d ед. товара '%s' из %s.\n", removed, name, address)
	}
}

// Вывод полной информации о складе
func info() {
	zoneLoad := make(map[rune]int)      // Нагрузка по зонам
	var emptyCells []string             // Список пустых ячеек
	sectionLoad := make(map[string]int) // Загрузка секций

	fmt.Println("\nЗанятые ячейки:")
	for address, items := range warehouse {
		total := getTotalInCell(items)
		if total > 0 {
			fmt.Printf("%s: ", address)
			for _, item := range items {
				fmt.Printf("%s(%d) ", item.name, item.quantity)
				zoneLoad[rune(address[0])] += item.quantity

				// Секция определяется по первым трём символам: зона+стеллаж+секция
				sectionKey := address[:3]
				sectionLoad[sectionKey] += item.quantity
			}
			fmt.Println()
		} else {
			emptyCells = append(emptyCells, address)
		}
	}

	// Вывод загрузки по зонам
	fmt.Println("\nЗагрузка по зонам:")
	for zone, qty := range zoneLoad {
		percent := float64(qty) / 150.0 * 100.0
		fmt.Printf("Зона %c: %d из 150 (%.2f%%)\n", zone, qty, percent)
	}

	// Вывод загрузки секций
	fmt.Println("\nЗаполненность секций:")
	for section, qty := range sectionLoad {
		percent := float64(qty) / 50.0 * 100.0
		fmt.Printf("Секция %s: %d из 50 (%.2f%%)\n", section, qty, percent)
	}

	// Пустые ячейки
	fmt.Println("\nПустые ячейки:")
	for _, address := range emptyCells {
		fmt.Printf("%s ", address)
	}
	fmt.Println()
}

func main() {
	initializeWarehouse()
	scanner := bufio.NewScanner(os.Stdin)

	fmt.Println("Введите команды (ADD, REMOVE, INFO). Для выхода введите EXIT.")

	for {
		fmt.Print("> ")
		if !scanner.Scan() {
			break
		}
		line := scanner.Text()
		if line == "EXIT" {
			break
		}

		parts := strings.Fields(line)
		if len(parts) == 0 {
			continue
		}

		command := parts[0]

		// Обработка команд
		switch command {
			case "ADD":
				if len(parts) != 4 {
					fmt.Println("Неверный формат команды. Используйте: ADD имя количество адрес")
					continue
				}
				name := parts[1]
				qty, err := strconv.Atoi(parts[2])
				if err != nil {
					fmt.Println("Ошибка: количество должно быть числом")
					continue
				}
				address := parts[3]
				addItem(name, qty, address)
			case "REMOVE":
				if len(parts) != 4 {
					fmt.Println("Неверный формат команды. Используйте: REMOVE имя количество адрес")
					continue
				}
				name := parts[1]
				qty, err := strconv.Atoi(parts[2])
				if err != nil {
					fmt.Println("Ошибка: количество должно быть числом")
					continue
				}
				address := parts[3]
				removeItem(name, qty, address)
			case "INFO":
				info()
			default:
				fmt.Println("Неизвестная команда. Используйте ADD, REMOVE или INFO.")
		}
	}
}
