package main

import (
	"container/heap"
	"fmt"
	"math/rand"
	"sort"
	"strconv"
	"time"
)

// Структура посетителя
type Visitor struct {
	Ticket   string
	Duration int
}

// Структура окна обслуживания
type Window struct {
	ID        int
	TotalTime int
	Tickets   []string
}

// Для приоритетной очереди
type WindowPQ []Window

func (pq WindowPQ) Len() int { return len(pq) }
func (pq WindowPQ) Less(i, j int) bool {
	return pq[i].TotalTime < pq[j].TotalTime
}
func (pq WindowPQ) Swap(i, j int) {
	pq[i], pq[j] = pq[j], pq[i]
}
func (pq *WindowPQ) Push(x interface{}) {
	*pq = append(*pq, x.(Window))
}
func (pq *WindowPQ) Pop() interface{} {
	old := *pq
	n := len(old)
	item := old[n-1]
	*pq = old[0 : n-1]
	return item
}

func generateTicket(used map[int]bool) string {
	var number int
	for {
		number = rand.Intn(900) + 100
		if !used[number] {
			used[number] = true
			break
		}
	}
	return "T" + strconv.Itoa(number)
}

func main() {
	rand.Seed(time.Now().UnixNano())

	var numWindows int
	fmt.Print("Введите кол-во окон\n>>> ")
	fmt.Scan(&numWindows)

	var queue []Visitor
	usedNumbers := make(map[int]bool)
	var command string

	for {
		fmt.Print("<<< ")
		fmt.Scan(&command)

		if command == "ENQUEUE" {
			var duration int
			fmt.Scan(&duration)
			ticket := generateTicket(usedNumbers)
			queue = append(queue, Visitor{Ticket: ticket, Duration: duration})
			fmt.Println(">>>", ticket)

		} else if command == "DISTRIBUTE" {
			// Сортировка по убыванию времени обслуживания
			sort.Slice(queue, func(i, j int) bool {
				return queue[i].Duration > queue[j].Duration
			})

			// Инициализация окон
			pq := &WindowPQ{}
			heap.Init(pq)
			for i := 1; i <= numWindows; i++ {
				heap.Push(pq, Window{ID: i})
			}

			// Распределение по окнам
			for _, visitor := range queue {
				win := heap.Pop(pq).(Window)
				win.TotalTime += visitor.Duration
				win.Tickets = append(win.Tickets, visitor.Ticket)
				heap.Push(pq, win)
			}

			// Извлечение всех окон из очереди
			var result []Window
			for pq.Len() > 0 {
				result = append(result, heap.Pop(pq).(Window))
			}

			// Сортировка окон по ID
			sort.Slice(result, func(i, j int) bool {
				return result[i].ID < result[j].ID
			})

			// Вывод результатов
			for _, win := range result {
				fmt.Printf(">>> Окно %d (%d минут): ", win.ID, win.TotalTime)
				for i, t := range win.Tickets {
					fmt.Print(t)
					if i < len(win.Tickets)-1 {
						fmt.Print(", ")
					}
				}
				fmt.Println()
			}
			break
		}
	}
}
