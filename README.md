# AntAlgorithm
 
![ant_algorithm](https://github.com/user-attachments/assets/68338588-c9cd-4f84-819c-7fc52fe66351)



## SETTINGS_JSON
### Параметры и их описание

| Параметр                | Значение               | Описание                                                                                      |
|-------------------------|------------------------|----------------------------------------------------------------------------------------------|
| **a**                  | 2                      | Коэффициент влияния феромонов на выбор пути.                                                 |
| **b**                  | 3                      | Коэффициент влияния длины пути на выбор муравья.                                             |
| **p**                  | 0.1                    | Коэффициент испарения феромонов. Чем меньше, тем медленнее исчезают феромоны.                |
| **q**                  | 1.0                    | Количество феромонов, добавляемых муравьем после прохождения пути.                           |
| **start_r**            | 0.1                    | Начальная вероятность случайного выбора пути.                                                |
| **minChanceTurn**      | 0.0                    | Минимальный шанс изменения направления пути.                                                 |
| **work_with_alive_ants** | true                 | Использовать только активных муравьев (`true`) или учитывать всех муравьев (`false`).        |
| **start_node**         | -1                     | Если -1, то каждый муравей начинает с уникальной вершины, распределенной по порядку. Если    |
|                         |                        | муравьев больше, чем вершин, распределение начинается с начала графа.                        |
| **graph**              | graphs\synthetic.txt   | Путь к файлу с описанием графа, по которому движутся муравьи.                                |
| **bidirectional**      | false                  | Определяет, является ли граф двунаправленным (`true`) или односторонним (`false`).           |
| **count_ants**         | 5100                   | Общее количество муравьев, участвующих в алгоритме.                                          |
| **count_repetitions**  | 1000000                | Количество итераций алгоритма.                                                              |
| **logs**               | false                  | Включает (`true`) или отключает (`false`) ведение логов работы алгоритма.                    |
| **async_mode_matrix**  | false                  | Включает асинхронный режим обновления матрицы феромонов.                                     |
| **async_mode_ants**    | false                  | Включает асинхронный режим движения муравьев.                                               |
| **count_checks**       | 51                     | Количество точек данных, собираемых для построения графика.                                  |
| **time_sleep**         | 0.01                   | Временной интервал между сбором данных для графика (в секундах).                             |



## Makefile: Названия и описания команд

### **run**
Запускает основную программу и Python-скрипт.

### **stop**
Останавливает все запущенные процессы программы.

### **refresh**
Выполняет обновление данных или выполняет дополнительный Python-скрипт.

### **restart**
Перезапускает программу, включая остановку процессов, обновление данных и запуск.

### **clean**
Удаляет исполняемые файлы и временные данные.



## Result

![result](https://github.com/user-attachments/assets/81b59253-0816-45bd-8555-1458a41c499f)


