import matplotlib.pyplot as plt
import networkx as nx
from matplotlib.animation import FuncAnimation
import numpy as np

# Создадим граф
G = nx.Graph()
edges = [(1, 2), (2, 3), (3, 4), (4, 5), (5, 6), (1, 6), (2, 4), (3, 5)]
G.add_edges_from(edges)

# Устанавливаем начальные параметры
paths = [[1, 2, 3, 4], [1, 6, 5, 4], [2, 4, 5, 6]]  # Пример разных путей
path_colors = ["red", "blue", "green"]
pos = nx.spring_layout(G, seed=42)  # Фиксируем расположение узлов
fig, ax = plt.subplots(figsize=(8, 6))

# Функция обновления анимации
def update(num):
    ax.clear()  # Очищаем рисунок на каждом кадре
    nx.draw(G, pos, ax=ax, with_labels=True, node_color="lightblue", edge_color="gray", node_size=500, font_size=10)

    # Получаем текущий путь и выделяем его
    path = paths[num % len(paths)]
    edges_in_path = [(path[i], path[i + 1]) for i in range(len(path) - 1)]
    nx.draw_networkx_edges(G, pos, edgelist=edges_in_path, edge_color=path_colors[num % len(path_colors)], width=2.5)
    ax.set_title(f"Путь {num % len(paths) + 1}", fontsize=16)

# Настраиваем анимацию
ani = FuncAnimation(fig, update, frames=len(paths) * 10, interval=500, repeat=True)

plt.show()
