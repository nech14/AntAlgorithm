import networkx as nx
import matplotlib.pyplot as plt

def create_graph(num_vertices, edges):
     # Создаём граф
    G = nx.Graph()
    
    # Добавляем вершины
    G.add_nodes_from(range(1, num_vertices + 1))
    
    # Добавляем рёбра
    for u, v, weight in edges:
        G.add_edge(u, v, weight=weight)
    
    return G

def read_graph_from_file(filename):
    with open(filename, 'r') as file:
        lines = file.readlines()
    
    # Чтение количества вершин
    num_vertices = int(lines[0].strip())
    
    # Пропуск второй строки
    edges = []
    for line in lines[2:]:
        u, v, weight = map(int, line.split())
        edges.append((u, v, weight))
    
    return num_vertices, edges


def save_graph_to_file(graph, filename, path_info="1 9"):
    with open(filename, 'w') as file:
        # Записываем количество вершин
        file.write(f"{len(graph.nodes())}\n")
        
        # Записываем путь или произвольную строку
        file.write(f"{path_info}\n")
        
        # Записываем рёбра
        for u, v, data in graph.edges(data=True):
            weight = data.get('weight', 1)  # По умолчанию вес = 1, если не задан
            file.write(f"{u} {v} {weight}\n")


def draw_graph(G):
    
    # Получаем позиции для вершин
    pos = nx.spring_layout(G)  # Расположение вершин
    
    # Рисуем вершины и рёбра
    nx.draw(G, pos, with_labels=True, node_color='lightblue', node_size=500, font_size=10)
    
    # Добавляем веса рёбер
    edge_labels = nx.get_edge_attributes(G, 'weight')
    nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels, font_size=8)
    
    # Показать граф
    plt.title("Граф с рёбрами и их весами")
    plt.show()


def check_hamiltonian_ore(G):
    
    
    # Проверяем условие теоремы Оре
    for u in G.nodes:
        for v in G.nodes:
            if u != v and not G.has_edge(u, v):  # Пара несмежных вершин
                if G.degree[u] + G.degree[v] < G.number_of_nodes():
                    return False  # Условие нарушено
    return True  # Условие выполнено для всех пар


def check_posa_theorem(G):
    # Упорядочиваем вершины по степеням
    degrees = sorted([G.degree[v] for v in G.nodes()], reverse=True)
    
    d_k = sum(1 for deg in degrees if deg < 2)
    if d_k > 0 :
        return False

    for k in range(1, G.number_of_nodes() // 2 + 1):
        # Число вершин со степенью >= k
        d_k = sum(1 for deg in degrees if deg >= k+1)
        
        # Проверяем условие
        if d_k < k:
            return False  # Условие нарушено
    
    return True  # Условие выполнено


def minimize_edges_to_make_hamiltonian(G):
    """
    Минимизирует число добавленных рёбер, чтобы граф стал гамильтоновым.
    """
    nodes = list(G.nodes())
    added_edges = 0  # Счётчик добавленных рёбер

    # Шаг 1: Обработка изолированных вершин (степень = 0)
    isolated_nodes = [node for node in nodes if G.degree[node] == 0]
    for node in isolated_nodes:
        # Подключаем изолированную вершину к любой другой (например, с минимальной степенью)
        min_degree_node = min((n for n in nodes if n != node), key=lambda x: G.degree[x])
        G.add_edge(node, min_degree_node, weight=1)
        added_edges += 1

    # Шаг 2: Обеспечение минимальной степени >= 2
    for node in nodes:
        while G.degree[node] < 2:
            # Находим вершину, с которой нет связи
            candidate = min((n for n in nodes if n != node and not G.has_edge(node, n)), key=lambda x: G.degree[x])
            G.add_edge(node, candidate, weight=1)
            added_edges += 1

    # Шаг 2.1: Проверка вершин со степенью ровно 2
    nodes_with_degree_2 = [node for node in nodes if G.degree[node] == 2]
    if len(nodes_with_degree_2) > 1:
        for node in nodes_with_degree_2:
            neighbors = set(G.neighbors(node))
            for candidate in nodes:
                if candidate not in neighbors and G.degree[candidate] > 2:
                    G.add_edge(node, candidate, weight=1)
                    added_edges += 1
                    break

    # Шаг 3: Проверяем, удовлетворяет ли граф теореме Пóса
    for k in range(1, len(nodes) // 2 + 1):
        degrees = sorted((node, G.degree[node]) for node in nodes)

        # Проверяем для всех вершин с недостаточной степенью
        for target_node, d_k in degrees:
            if d_k < k + 1:
                neighbors = set(G.neighbors(target_node))
                
                # Соединяем с вершинами, чтобы минимизировать добавление рёбер
                for candidate in nodes:
                    if candidate != target_node and candidate not in neighbors:
                        G.add_edge(target_node, candidate, weight=1)
                        added_edges += 1

                        # Перепроверяем степень после добавления
                        if G.degree[target_node] >= k + 1:
                            break

                # После обработки одной вершины пересчитываем степени
                break

    print(f"Добавлено рёбер: {added_edges}")
    return G


def ensure_dirac_theorem_optimized(G):
    """
    Модифицирует граф, чтобы он удовлетворял теореме Дирака.
    """
    num_vertices = G.number_of_nodes()
    threshold = num_vertices // 2  # Условие теоремы Дирака
    added_edges = 0  # Счётчик добавленных рёбер

    # Собираем вершины, которые не удовлетворяют условию
    low_degree_nodes = {node for node in G.nodes if G.degree[node] < threshold}

    # Пока есть вершины с недостаточной степенью
    while low_degree_nodes:
        u = low_degree_nodes.pop()  # Берём вершину с низкой степенью

        # Ищем вершину для подключения
        for v in list(low_degree_nodes):  # Итерация по вершинам с низкой степенью
            if not G.has_edge(u, v):
                G.add_edge(u, v, weight=1)  # Добавляем ребро
                added_edges += 1
                if G.degree[v] >= threshold:
                    low_degree_nodes.remove(v)  # Убираем вершину, если она больше не имеет низкую степень
                if G.degree[u] >= threshold:
                    break  # Прекращаем, если u теперь удовлетворяет условию
        # Если u теперь удовлетворяет условию, не нужно больше обрабатывать её
        if G.degree[u] >= threshold:
            continue

        # Если не осталось вершин с низкой степенью для подключения, подключаем к другим
        for v in G.nodes:
            if u != v and not G.has_edge(u, v):
                G.add_edge(u, v, weight=1)  # Добавляем ребро
                added_edges += 1
                if G.degree[v] >= threshold and v in low_degree_nodes:
                    low_degree_nodes.remove(v)
                if G.degree[u] >= threshold:
                    break
    print(f"Добавлено рёбер: {added_edges}")
    return G



# Пример использования
filename = '../graphs/1000.txt'
num_vertices, edges = read_graph_from_file(filename)

G = create_graph(num_vertices, edges)

print("Количество вершин:", num_vertices)
# print("Ребра:")
# for edge in edges:
#     print(edge)

# Проверка гамильтонова цикла
if check_hamiltonian_ore(G):
    print("Граф удовлетворяет условиям теоремы Оре. Гамильтонов цикл существует.")
else:
    print("Условия теоремы Оре не выполнены. Нельзя гарантировать наличие гамильтонова цикла.")


if check_posa_theorem(G):
    print("Граф удовлетворяет теореме По́са. Гамильтонов цикл существует.")
else:
    print("Условия теоремы По́са не выполнены.")


G = create_graph(num_vertices, edges)
print("Граф до добавления рёбер:")
draw_graph(G)

# G = minimize_edges_to_make_hamiltonian(G)
G = ensure_dirac_theorem_optimized(G)
print("Граф после минимизации добавленных рёбер:")

path_info = "1 9"  # Произвольная информация, которую хотите записать
save_graph_to_file(G, '../graphs/1000_test_1.txt', path_info)

draw_graph(G)


