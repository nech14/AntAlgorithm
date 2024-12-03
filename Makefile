
# Путь к заголовочным файлам
INCLUDE = -Isrc/../dependencies/include

# Имя выходного файла
TARGET = my_program

# Исходные файлы
SRC = AntAlgorithm.cpp

# Компилятор
CXX = g++

# Флаги компиляции
CXXFLAGS = -std=c++17 -Wall $(INCLUDE)

# Имя Python-скрипта
PYTHON_SCRIPT = src/plot.py
PYTHON_SCRIPT_REFRESH = src/refresh.py

.PHONY: stop

all: $(TARGET)
	@echo "Building and running C++ project with MSVC..."
# Правило для компиляции
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Команда для запуска
run: $(TARGET)
	./$(TARGET) &	
	python $(PYTHON_SCRIPT) 

# Очистка
clean:
	rm -f $(TARGET)
	rm -f $(TARGET).exe


stop:
	@echo "Stopping all processes..."
	killall my_program || true  # Для Linux/macOS
	taskkill /IM my_program.exe /F || true  # Для Windows


refresh:
	@echo "Start refresh"
	python $(PYTHON_SCRIPT_REFRESH)


restart: stop refresh run
	@echo "restart"
