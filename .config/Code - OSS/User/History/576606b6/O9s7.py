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
        D = -np.dot(normal, self.A)
        return normal, D
    
    def plot(self, ax, color='blue', alpha=0.5):
        """Отрисовать треугольник"""
        vertices = np.array([self.A, self.B, self.C, self.A])
        ax.plot(vertices[:, 0], vertices[:, 1], vertices[:, 2], 
                color=color, linewidth=2)
        ax.scatter(vertices[:, 0], vertices[:, 1], vertices[:, 2], 
                  color=color, s=50)
        
        # Заполнение треугольника
        from mpl_toolkits.mplot3d.art3d import Poly3DCollection
        triangle = Poly3DCollection([self.points], alpha=alpha, color=color)
        ax.add_collection3d(triangle)

def find_intersection_plane(triangle1, triangle2):
    """Найти плоскость, пересекающую оба треугольника"""
    # Векторы нормалей для обеих плоскостей
    n1, D1 = triangle1.get_plane_equation()
    n2, D2 = triangle2.get_plane_equation()
    
    # Направляющий вектор линии пересечения плоскостей
    direction = np.cross(n1, n2)
    
    # Находим точку на линии пересечения
    # Решаем систему: n1·p + D1 = 0, n2·p + D2 = 0
    A_system = np.array([n1[:2], n2[:2]])
    b_system = np.array([-D1, -D2])
    
    try:
        xy_solution = np.linalg.solve(A_system, b_system)
        intersection_point = np.array([xy_solution[0], xy_solution[1], 0])
    except:
        # Если система вырождена, выбираем другую координату
        A_system = np.array([n1[1:], n2[1:]])
        b_system = np.array([-D1, -D2])
        yz_solution = np.linalg.solve(A_system, b_system)
        intersection_point = np.array([0, yz_solution[0], yz_solution[1]])
    
    return intersection_point, direction

# Пример данных (замените на ваши реальные координаты)
# Треугольник 1
points1 = [
    np.array([0, 0, 0]),    # a1
    np.array([1, 0, 0]),    # a2  
    np.array([0, 1, 0])     # a3
]

# Треугольник 2
points2 = [
    np.array([0.5, 0.5, 1]),  # b1
    np.array([1.5, 0.5, 1]),  # b2
    np.array([0.5, 1.5, 1])   # b3
]

# Создаем треугольники
triangle1 = Triangle3D(points1)
triangle2 = Triangle3D(points2)

# Находим плоскость пересечения
intersection_point, direction = find_intersection_plane(triangle1, triangle2)

# Визуализация
fig = plt.figure(figsize=(12, 10))
ax = fig.add_subplot(111, projection='3d')

# Рисуем треугольники
triangle1.plot(ax, color='blue', alpha=0.3)
triangle2.plot(ax, color='red', alpha=0.3)

# Рисуем линию пересечения
t = np.linspace(-2, 2, 100)
line_points = np.array([intersection_point + t_i * direction for t_i in t])
ax.plot(line_points[:, 0], line_points[:, 1], line_points[:, 2], 
        'g-', linewidth=2, label='Линия пересечения')

# Настройка графика
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
ax.legend()
ax.set_title('Пересечение двух треугольников')

plt.tight_layout()
plt.show()

# Вывод уравнений плоскостей
n1, D1 = triangle1.get_plane_equation()
n2, D2 = triangle2.get_plane_equation()

print("Уравнение плоскости треугольника 1:")
print(f"{n1[0]:.2f}x + {n1[1]:.2f}y + {n1[2]:.2f}z + {D1:.2f} = 0")

print("\nУравнение плоскости треугольника 2:")
print(f"{n2[0]:.2f}x + {n2[1]:.2f}y + {n2[2]:.2f}z + {D2:.2f} = 0")

print(f"\nТочка пересечения: {intersection_point}")
print(f"Направляющий вектор: {direction}")