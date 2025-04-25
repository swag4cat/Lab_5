# Название итогового исполняемого файла
TARGET = main

# Компилятор и флаги
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic

# Все исходники проекта
SRCS = main.cpp transport_manager.cpp

# Заголовочные файлы (для отслеживания изменений)
HEADERS = transport_manager.h commands.h

# Правило сборки по умолчанию
all: $(TARGET)

# Сборка исполняемого файла
$(TARGET): $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

# Очистка файлов сборки
clean:
	rm -f $(TARGET)
