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
        if np.linalg.norm(normal) > 0:
            normal_norm = normal / np.linalg.norm(normal)
        else:
            normal_norm = normal
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
    # Пробуем разные комбинации координат
    solutions = []
    
    # Попробуем z = 0
    try:
        A = np.array([[n1[0], n1[1]], [n2[0], n2[1]]])
        b = np.array([-D1, -D2])
        if np.linalg.det(A) != 0:
            xy_solution = np.linalg.solve(A, b)
            solutions.append(np.array([xy_solution[0], xy_solution[1], 0]))
    except:
        pass
    
    # Попробуем y = 0
    try:
        A = np.array([[n1[0], n1[2]], [n2[0], n2[2]]])
        b = np.array([-D1, -D2])
        if np.linalg.det(A) != 0:
            xz_solution = np.linalg.solve(A, b)
            solutions.append(np.array([xz_solution[0], 0, xz_solution[1]]))
    except:
        pass
    
    # Попробуем x = 0
    try:
        A = np.array([[n1[1], n1[2]], [n2[1], n2[2]]])
        b = np.array([-D1, -D2])
        if np.linalg.det(A) != 0:
            yz_solution = np.linalg.solve(A, b)
            solutions.append(np.array([0, yz_solution[0], yz_solution[1]]))
    except:
        pass
    
    if solutions:
        intersection_point = solutions[0]
    else:
        # Если все системы вырождены, используем метод наименьших квадратов
        A = np.array([n1, n2])
        b = np.array([-D1, -D2])
        solution = np.linalg.lstsq(A.T, b, rcond=None)[0]
        intersection_point = solution
    
    return intersection_point, direction

# Задаем точки треугольников на основе ваших данных
# Треугольник 1 (P1, P2, P3)
P1 = np.array([1, 0, 0])   # a1
P2 = np.array([0, 2, 0])   # b1  
P3 = np.array([0, 0, 3])   # c1

# Треугольник 2 (Q1, Q2, Q3)
Q1 = np.array([2, 1, 1])   # a2
Q2 = np.array([1, 3, 1])   # b2
Q3 = np.array([1, 1, 4])   # c2

# Создаем треугольники
triangle1 = Triangle3D([P1, P2, P3])
triangle2 = Triangle3D([Q1, Q2, Q3])

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
all_points = np.vstack([triangle1.points, triangle2.points])
max_range = np.max(np.ptp(all_points, axis=0))
mid_point = np.mean(all_points, axis=0)

ax.set_xlim(mid_point[0] - max_range/2, mid_point[0] + max_range/2)
ax.set_ylim(mid_point[1] - max_range/2, mid_point[1] + max_range/2)
ax.set_zlim(mid_point[2] - max_range/2, mid_point[2] + max_range/2)

plt.tight_layout()
plt.show()

# Вывод уравнений плоскостей
n1, D1 = triangle1.get_plane_equation()
n2, D2 = triangle2.get_plane_equation()

print("=" * 60)
print("РЕЗУЛЬТАТЫ РАСЧЕТОВ")
print("=" * 60)

print("\nКоординаты точек:")
print(f"Треугольник 1: P1{P1}, P2{P2}, P3{P3}")
print(f"Треугольник 2: Q1{Q1}, Q2{Q2}, Q3{Q3}")

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
print("\n" + "=" * 60)
print("ПРОВЕРКА УРАВНЕНИЙ ПЛОСКОСТЕЙ")
print("=" * 60)

print("\nПроверка для треугольника 1:")
points1 = [P1, P2, P3]
for i, point in enumerate(points1):
    result = np.dot(n1, point) + D1
    print(f"Точка P{i+1}{point}: {result:.8f} (должно быть ≈ 0)")

print("\nПроверка для треугольника 2:")
points2 = [Q1, Q2, Q3]
for i, point in enumerate(points2):
    result = np.dot(n2, point) + D2
    print(f"Точка Q{i+1}{point}: {result:.8f} (должно быть ≈ 0)")