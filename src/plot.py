import matplotlib.pyplot as plt
import numpy as np
import os

filename = "../result/output.txt"
last_result = 1000
best_result = 10000000
way = ""
best_way = ""

def read_and_delete_lines(filename, num_lines):
    """
    Считывает `num_lines` строк из файла и удаляет их.
    :param filename: Имя файла для чтения и изменения.
    :param num_lines: Количество строк для чтения и удаления.
    :return: Прочитанные строки.
    """
    # Имя временного файла
    temp_filename = filename + ".tmp"
    read_lines = []

    with open(filename, "r") as file, open(temp_filename, "w") as temp_file:
        # Читаем строки из файла
        for i, line in enumerate(file):
            if i < num_lines:
                read_lines.append(line.strip())  # Сохраняем строки без \n
            else:
                temp_file.write(line)  # Остальное записываем во временный файл

    # Заменяем оригинальный файл временным
    try:
        os.replace(temp_filename, filename)
    except:
        pass
    return read_lines


def read_data(lines):
    # Чтение данных из файла
    data = []
    all_data = []
    global way
    for line in lines:
        row = list(map(int, line.split(',')))  # Разделяем строку на числа
        all_data.append(row)
        data.append(row[-1])

    global last_result
    try:
        min_data = min(data)
        min_index = data.index(min_data)
        way = all_data[min_index][1:-1]
        return min_data
    except:
        return last_result


def plot_data(data):
    plt.ion()  # Включаем интерактивный режим
    fig, ax = plt.subplots()

    while True:
        lines = read_and_delete_lines(filename, 10)
        global last_result
        last_result = read_data(lines)
        global best_result
        global best_way
        global way
        
        if best_result > last_result:
            best_result = last_result
            best_way = way
            print(best_result)
            print(best_way)
        data.append(last_result)
        ax.clear()
        # for i, series in enumerate(zip(*data)):  # Транспонируем для построения
            # ax.plot(series, label=f"Series {i+1}")
        ax.plot(data)
        # ax.legend()
        plt.draw()
        plt.pause(0.01)  # Обновляем график каждые 1 секунду

if __name__ == "__main__":
    data = []
    plot_data(data)
