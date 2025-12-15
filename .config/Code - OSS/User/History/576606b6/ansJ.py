import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

class TriangleAnalyzer:
    def __init__(self):
        pass
    
    def calculate_plane_equation(self, a, b, c):
        """
        Вычисляет уравнение плоскости по трем точкам
        """
        t1 = b - a
        t2 = c - a
        
        # Векторное произведение для нахождения нормали
        N = np.cross(t1, t2)
        
        # Коэффициенты уравнения плоскости Ax + By + Cz + D = 0
        A, B, C = N
        D = -np.dot(N, a)
        
        return np.array([A, B, C, D])
    
    def check_point_in_plane(self, plane_coeffs, point):
        """
        Проверяет принадлежность точки плоскости
        """
        A, B, C, D = plane_coeffs
        x, y, z = point
        return A*x + B*y + C*z + D
    
    def find_line_equation(self, x, y):
        """
        Находит уравнение прямой по двум точкам
        """
        dx = y[0] - x[0]
        dy = y[1] - x[1]
        dz = y[2] - x[2]
        
        A = 1/dx if dx != 0 else 0
        B = 1/dy if dy != 0 else 0
        C = -2/dz if dz != 0 else 0
        
        # Вычисление D (упрощенная версия)
        D = (2*x[2]/dz) * (x[1]/dy) * (x[0]/dx) if all([dz != 0, dy != 0, dx != 0]) else 0
        
        return np.array([A, B, C, D])
    
    def find_intersection_point(self, plane1, plane2, line):
        """
        Находит точку пересечения двух плоскостей и прямой
        """
        A1, B1, C1, D1 = plane1
        A2, B2, C2, D2 = plane2
        A3, B3, C3, D3 = line
        
        # Матрица коэффициентов
        A_matrix = np.array([
            [A1, B1, C1],
            [A2, B2, C2],
            [A3, B3, C3]
        ])
        
        # Вектор правой части
        b_vector = np.array([-D1, -D2, -D3])
        
        try:
            # Решение системы уравнений
            intersection_point = np.linalg.solve(A_matrix, b_vector)
            return intersection_point
        except np.linalg.LinAlgError:
            print("Система уравнений не имеет решения")
            return None
    
    def visualize_triangles(self, triangle1, triangle2, intersection_points=None):
        """
        Визуализация треугольников и точек пересечения
        """
        fig = plt.figure(figsize=(12, 10))
        ax = fig.add_subplot(111, projection='3d')
        
        # Первый треугольник
        tri1 = np.vstack([triangle1, triangle1[0]])
        ax.plot(tri1[:, 0], tri1[:, 1], tri1[:, 2], 'b-', linewidth=2, label='Треугольник 1')
        ax.scatter(triangle1[:, 0], triangle1[:, 1], triangle1[:, 2], c='blue', s=100)
        
        # Второй треугольник
        tri2 = np.vstack([triangle2, triangle2[0]])
        ax.plot(tri2[:, 0], tri2[:, 1], tri2[:, 2], 'r-', linewidth=2, label='Треугольник 2')
        ax.scatter(triangle2[:, 0], triangle2[:, 1], triangle2[:, 2], c='red', s=100)
        
        # Точки пересечения
        if intersection_points is not None:
            for i, point in enumerate(intersection_points):
                ax.scatter(point[0], point[1], point[2], c='green', s=150, marker='*', 
                          label=f'Точка пересечения {i+1}')
        
        ax.set_xlabel('X')
        ax.set_ylabel('Y')
        ax.set_zlabel('Z')
        ax.legend()
        ax.set_title('Визуализация треугольников и точек пересечения')
        plt.show()

# Пример использования
def main():
    analyzer = TriangleAnalyzer()
    
    # Пример данных (замените на ваши реальные точки)
    # Треугольник 1
    a1 = np.array([1, 0, 0])
    b1 = np.array([0, 1, 0])
    c1 = np.array([0, 0, 1])
    triangle1 = np.array([a1, b1, c1])
    
    # Треугольник 2
    a2 = np.array([0, 0, 0])
    b2 = np.array([1, 0, 0])
    c2 = np.array([0, 1, 0])
    triangle2 = np.array([a2, b2, c2])
    
    # Вычисление уравнений плоскостей
    plane1_eq = analyzer.calculate_plane_equation(a1, b1, c1)
    plane2_eq = analyzer.calculate_plane_equation(a2, b2, c2)
    
    print("Уравнение плоскости 1:", plane1_eq)
    print("Уравнение плоскости 2:", plane2_eq)
    
    # Проверка принадлежности точек плоскостям
    print("\nПроверка принадлежности точек плоскости 1:")
    for point in triangle1:
        result = analyzer.check_point_in_plane(plane1_eq, point)
        print(f"Точка {point}: {result:.6f}")
    
    # Нахождение уравнений прямых
    line1_eq = analyzer.find_line_equation(a1, b1)
    line2_eq = analyzer.find_line_equation(a1, c1)
    
    print(f"\nУравнение прямой a1-b1: {line1_eq}")
    print(f"Уравнение прямой a1-c1: {line2_eq}")
    
    # Нахождение точек пересечения
    intersection1 = analyzer.find_intersection_point(plane1_eq, plane2_eq, line1_eq)
    intersection2 = analyzer.find_intersection_point(plane1_eq, plane2_eq, line2_eq)
    
    if intersection1 is not None:
        print(f"\nПервая точка пересечения: {intersection1}")
    if intersection2 is not None:
        print(f"Вторая точка пересечения: {intersection2}")
    
    # Визуализация
    intersection_points = []
    if intersection1 is not None:
        intersection_points.append(intersection1)
    if intersection2 is not None:
        intersection_points.append(intersection2)
    
    analyzer.visualize_triangles(triangle1, triangle2, intersection_points)

if __name__ == "__main__":
    main()