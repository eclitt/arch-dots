import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

class Triangle3D:
    def __init__(self, points):
        self.points = np.array(points)
        self.A, self.B, self.C = points
        
    def get_plane_equation(self):
        """Найти уравнение плоскости треугольника"""
        AB = self.B - self.A
        AC = self.C - self.A
        normal = np.cross(AB, AC)
        # Нормализуем вектор нормали
        normal_norm = normal / np.linalg.norm(normal)
        D = -np.dot(normal_norm, self.A)
        return normal_norm, D
    
    def plot(self, ax, color='blue', alpha=0.5, label=""):
        """Отрисовать треугольник"""
        vertices = np.array([self.A, self.B, self.C, self.A])
        ax.plot(vertices[:, 0], vertices[:, 1], vertices[:, 2], 
                color=color, linewidth=2, label=label)
        ax.scatter(vertices[:, 0], vertices[:, 1], vertices[:, 2], 
                  color=color, s=50)
        
        # Заполнение треугольника
        from mpl_toolkits.mplot3d.art3d import Poly3DCollection
        triangle = Poly3DCollection([self.points], alpha=alpha, color=color)
        ax.add_collection3d(triangle)

def find_intersection_line(triangle1, triangle2):
    """Найти линию пересечения двух плоскостей"""
    n1, D1 = triangle1.get_plane_equation()
    n2, D2 = triangle2.get_plane_equation()
    
    # Проверка на параллельность плоскостей
    cross_product = np.cross(n1, n2)
    if np.linalg.norm(cross_product) < 1e-10:
        print("Плоскости параллельны или совпадают")
        return None, None
    
    # Направляющий вектор линии пересечения
    direction = cross_product / np.linalg.norm(cross_product)
    
    # Находим точку на линии пересечения
    # Решаем систему: n1·p = -D1, n2·p = -D2
    # Выбираем координату z = 0 и решаем для x, y
    A = np.array([n1[:2], n2[:2]])
    b = np.array([-D1, -D2])
    
    try:
        xy_solution = np.linalg.solve(A, b)
        intersection_point = np.array([xy_solution[0], xy_solution[1], 0])
    except np.linalg.LinAlgError:
        # Если система вырождена, пробуем другую координату
        try:
            A = np.array([n1[0:3:2], n2[0:3:2]])  # x, z
            b = np.array([-D1, -D2])
            xz_solution = np.linalg.solve(A, b)
            intersection_point = np.array([xz_solution[0], 0, xz_solution[1]])
        except np.linalg.LinAlgError:
            # Пробуем y, z
            A = np.array([n1[1:], n2[1:]])
            b = np.array([-D1, -D2])
            yz_solution = np.linalg.solve(A, b)
            intersection_point = np.array([0, yz_solution[0], yz_solution[1]])
    
    return intersection_point, direction

def point_in_triangle(point, triangle):
    """Проверить, находится ли точка внутри треугольника (2D проекция)"""
    # Проецируем на плоскость треугольника
    A, B, C = triangle.points
    
    # Векторы из вершин треугольника к точке
    v0 = C - A
    v1 = B - A
    v2 = point - A
    
    # Вычисляем барицентрические координаты
    dot00 = np.dot(v0, v0)
    dot01 = np.dot(v0, v1)
    dot02 = np.dot(v0, v2)
    dot11 = np.dot(v1, v1)
    dot12 = np.dot(v1, v2)
    
    inv_denom = 1 / (dot00 * dot11 - dot01 * dot01)
    u = (dot11 * dot02 - dot01 * dot12) * inv_denom
    v = (dot00 * dot12 - dot01 * dot02) * inv_denom
    
    # Точка внутри треугольника, если u >= 0, v >= 0, u + v <= 1
    return (u >= 0) and (v >= 0) and (u + v <= 1)

# Задаем точки треугольников (используем ваши обозначения)
# Треугольник 1
a1 = np.array([0, 0, 0])
a2 = np.array([4, 0, 0]) 
a3 = np.array([0, 3, 0])

# Треугольник 2
b1 = np.array([1, 1, 2])
b2 = np.array([5, 1, 2])
b3 = np.array([1, 4, 2])

# Создаем треугольники
triangle1 = Triangle3D([a1, a2, a3])
triangle2 = Triangle3D([b1, b2, b3])

# Находим линию пересечения плоскостей
intersection_point, direction = find_intersection_line(triangle1, triangle2)

# Визуализация
fig = plt.figure(figsize=(12, 10))
ax = fig.add_subplot(111, projection='3d')

# Рисуем треугольники
triangle1.plot(ax, color='blue', alpha=0.3, label="Треугольник 1")
triangle2.plot(ax, color='red', alpha=0.3, label="Треугольник 2")

# Рисуем линию пересечения плоскостей
if intersection_point is not None:
    t = np.linspace(-5, 5, 100)
    line_points = np.array([intersection_point + t_i * direction for t_i in t])
    ax.plot(line_points[:, 0], line_points[:, 1], line_points[:, 2], 
            'g-', linewidth=3, label='Линия пересечения плоскостей')
    
    # Отмечаем точку пересечения
    ax.scatter(*intersection_point, color='green', s=100, marker='o', 
               label='Точка на линии пересечения')

# Настройка графика
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
ax.legend()
ax.set_title('Пересечение двух треугольников')

# Устанавливаем равные масштабы по осям
max_range = max(np.ptp([a1, a2, a3, b1, b2, b3], axis=0))
mid_x = np.mean([a1[0], a2[0], a3[0], b1[0], b2[0], b3[0]])
mid_y = np.mean([a1[1], a2[1], a3[1], b1[1], b2[1], b3[1]])
mid_z = np.mean([a1[2], a2[2], a3[2], b1[2], b2[2], b3[2]])

ax.set_xlim(mid_x - max_range/2, mid_x + max_range/2)
ax.set_ylim(mid_y - max_range/2, mid_y + max_range/2)
ax.set_zlim(mid_z - max_range/2, mid_z + max_range/2)

plt.tight_layout()
plt.show()

# Вывод уравнений плоскостей
n1, D1 = triangle1.get_plane_equation()
n2, D2 = triangle2.get_plane_equation()

print("=" * 50)
print("РЕЗУЛЬТАТЫ РАСЧЕТОВ")
print("=" * 50)

print("\nУравнение плоскости треугольника 1:")
print(f"{n1[0]:.4f}x + {n1[1]:.4f}y + {n1[2]:.4f}z + {D1:.4f} = 0")

print("\nУравнение плоскости треугольника 2:")
print(f"{n2[0]:.4f}x + {n2[1]:.4f}y + {n2[2]:.4f}z + {D2:.4f} = 0")

if intersection_point is not None:
    print(f"\nТочка на линии пересечения: ({intersection_point[0]:.4f}, {intersection_point[1]:.4f}, {intersection_point[2]:.4f})")
    print(f"Направляющий вектор линии: ({direction[0]:.4f}, {direction[1]:.4f}, {direction[2]:.4f})")
    
    # Параметрическое уравнение линии пересечения
    print(f"\nПараметрическое уравнение линии пересечения:")
    print(f"x = {intersection_point[0]:.4f} + {direction[0]:.4f}t")
    print(f"y = {intersection_point[1]:.4f} + {direction[1]:.4f}t") 
    print(f"z = {intersection_point[2]:.4f} + {direction[2]:.4f}t")
else:
    print("\nПлоскости параллельны или совпадают")

# Проверка принадлежности точек плоскостям
print("\n" + "=" * 50)
print("ПРОВЕРКА УРАВНЕНИЙ ПЛОСКОСТЕЙ")
print("=" * 50)

print("\nПроверка для треугольника 1:")
for i, point in enumerate([a1, a2, a3]):
    result = np.dot(n1, point) + D1
    print(f"Точка a{i+1}: {result:.6f} (должно быть ≈ 0)")

print("\nПроверка для треугольника 2:")
for i, point in enumerate([b1, b2, b3]):
    result = np.dot(n2, point) + D2
    print(f"Точка b{i+1}: {result:.6f} (должно быть ≈ 0)")