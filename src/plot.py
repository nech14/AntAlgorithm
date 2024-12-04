import matplotlib.pyplot as plt
import numpy as np
import json
import os

filename = "result/output.txt"
last_result = 1000
best_result = 10000000
way = ""
best_way = ""
best_way_len = 100000
chance_way = 0.
amount_pheromone_on_way = 0.

count_checks = 51
time_sleep = 0.1

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
    global last_result
    global amount_pheromone_on_way
    global chance_way

    for line in lines:
        try:
            row = list(map(float, line.split(',')))  # Разделяем строку на числа
            all_data.append(row)
            data.append(row[-1])
        except:
            continue

    
    try:
        min_data = min(data)
        min_index = data.index(min_data)
        way = all_data[min_index][3:-1]
        amount_pheromone_on_way = all_data[min_index][1]
        chance_way = all_data[min_index][0]
        return min_data
    except:
        return last_result


def plot_data(data):
    plt.ion()  # Включаем интерактивный режим
    fig, (ax1, ax2, ax3) = plt.subplots(3, 1)

    data_chance_way = []
    data_amount_pheromone_on_way = []

    global count_checks
    global time_sleep

    while True:
        lines = read_and_delete_lines(filename, count_checks)

        # Проверяем, есть ли новые данные
        if not lines:
            plt.pause(time_sleep)  # Ждём немного перед следующим циклом
            continue

        global last_result
        last_result = read_data(lines)
        global best_result
        global best_way
        global way
        global amount_pheromone_on_way
        global chance_way
        
        if best_result > last_result:
            best_result = last_result
            best_way = way
            print(best_result)
            print(best_way)

        data.append(last_result)
        data_chance_way.append(chance_way)
        data_amount_pheromone_on_way.append(amount_pheromone_on_way)

        fig.suptitle(f"best_way_len: {best_result}")
        ax1.clear()
        ax1.set_title("len_way")
        ax1.plot(data)

        ax2.clear()
        ax2.set_title("chance_way")
        ax2.plot(data_chance_way)

        ax3.clear()
        ax3.set_title("amount_pheromone_on_way")
        ax3.plot(data_amount_pheromone_on_way)

        # ax.legend()
        plt.draw()
        plt.pause(time_sleep)  # Обновляем график каждые 1 секунду


def udpdate_settings(file_path = "settings.json"):
    with open(file_path, 'r', encoding='utf-8') as file:
        data = json.load(file)  # Загружаем JSON-данные
    
    global count_checks
    global time_sleep

    count_checks = int(data["count_checks"])
    time_sleep = float(data["time_sleep"])




if __name__ == "__main__":
    udpdate_settings()
    data = []
    plot_data(data)
