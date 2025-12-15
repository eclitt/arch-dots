import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

class Triangle3D:
    def __init__(self, points, name=""):
        self.points = np.array(points)
        self.A, self.B, self.C = points
        self.name = name
        
    def get_plane_equation(self):
        """Найти уравнение плоскости треугольника"""
        AB = self.B - self.A
        AC = self.C - self.A
        normal = np.cross(AB, AC)
        D = -np.dot(normal, self.A)
        return normal, D
    
    def plot(self, ax, color='blue', alpha=0.5):
        """Отрисовать треугольник"""
        vertices = np.vstack([self.points, self.points[0]])
        ax.plot(vertices[:, 0], vertices[:, 1], vertices[:, 2], 
                color=color, linewidth=2, label=self.name)
        ax.scatter(vertices[:, 0], vertices[:, 1], vertices[:, 2], 
                  color=color, s=50)
        
        # Заполнение треугольника
        from mpl_toolkits.mplot3d.art3d import Poly3DCollection
        triangle = Poly3DCollection([self.points], alpha=alpha, color=color)
        ax.add_collection3d(triangle)

def find_intersection_line(n1, D1, n2, D2):
    """Найти линию пересечения двух плоскостей"""
    # Направляющий вектор линии пересечения
    direction = np.cross(n1, n2)
    
    if np.linalg.norm(direction) < 1e-10:
        return None, None
    
    # Нормализуем направляющий вектор
    direction = direction / np.linalg.norm(direction)
    
    # Находим точку на линии пересечения (решаем систему)
    A = np.array([n1, n2])
    b = np.array([-D1, -D2])
    
    # Добавляем условие для однозначного решения
    A_augmented = np.vstack([A.T, [1, 1, 1]])
    b_augmented = np.append(b, 0)
    
    try:
        point = np.linalg.lstsq(A_augmented, b_augmented, rcond=None)[0]
        return point, direction
    except:
        return None, None

# Задаем точки треугольников
a1 = np.array([1, 1, 1])
b1 = np.array([1, 3, 4])
c1 = np.array([5, 1, 0])

a2 = np.array([1, 1, 4])
b2 = np.array([3, 4, 1])
c2 = np.array([4, 2, 1])

# Создаем треугольники
triangle1 = Triangle3D([a1, b1, c1], "Треугольник 1")
triangle2 = Triangle3D([a2, b2, c2], "Треугольник 2")

# Находим уравнения плоскостей
n1, D1 = triangle1.get_plane_equation()
n2, D2 = triangle2.get_plane_equation()

# Находим линию пересечения
intersection_point, direction = find_intersection_line(n1, D1, n2, D2)

# Визуализация
fig = plt.figure(figsize=(12, 10))
ax = fig.add_subplot(111, projection='3d')

# Рисуем треугольники
triangle1.plot(ax, color='blue', alpha=0.3)
triangle2.plot(ax, color='red', alpha=0.3)

# Рисуем линию пересечения
if intersection_point is not None:
    t = np.linspace(-3, 3, 100)
    line_points = np.array([intersection_point + t_i * direction for t_i in t])
    ax.plot(line_points[:, 0], line_points[:, 1], line_points[:, 2], 
            'g-', linewidth=3, label='Линия пересечения')
    
    ax.scatter(*intersection_point, color='green', s=100, marker='o')

# Настройка графика
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
ax.legend()
ax.set_title('Пересечение двух треугольников')
plt.show()

# Вывод результатов
print("Уравнение плоскости треугольника 1:")
print(f"{n1[0]:.2f}x + {n1[1]:.2f}y + {n1[2]:.2f}z + {D1:.2f} = 0")

print("\nУравнение плоскости треугольника 2:")
print(f"{n2[0]:.2f}x + {n2[1]:.2f}y + {n2[2]:.2f}z + {D2:.2f} = 0")

if intersection_point is not None:
    print(f"\nЛиния пересечения:")
    print(f"x = {intersection_point[0]:.2f} + {direction[0]:.2f}t")
    print(f"y = {intersection_point[1]:.2f} + {direction[1]:.2f}t")
    print(f"z = {intersection_point[2]:.2f} + {direction[2]:.2f}t")