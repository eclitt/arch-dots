import numpy as np

def find_plane_equation(a, b, c):
    """
    Находит уравнение плоскости по трем точкам
    Возвращает коэффициенты A, B, C, D уравнения Ax + By + Cz + D = 0
    """
    # Векторы в плоскости
    t1 = b - a
    t2 = c - a
    
    # Нормаль к плоскости (векторное произведение)
    N = np.cross(t1, t2)
    
    # Коэффициент D
    D = -np.dot(N, a)
    
    return np.array([N[0], N[1], N[2], D])

def find_line_equation(p1, p2):
    """
    Находит уравнение прямой по двум точкам в параметрическом виде
    Возвращает коэффициенты для канонического уравнения
    """
    x1, y1, z1 = p1
    x2, y2, z2 = p2
    
    # Направляющие коэффициенты
    if x2 - x1 != 0:
        A = 1 / (x2 - x1)
    else:
        A = 0
        
    if y2 - y1 != 0:
        B = 1 / (y2 - y1)
    else:
        B = 0
        
    if z2 - z1 != 0:
        C = -2 / (z2 - z1)
    else:
        C = 0
    
    # Свободный член
    D = (2*x1/(z2 - z1) if z2 != z1 else 0) * \
        (x1/(y2 - y1) if y2 != y1 else 0) * \
        (x1/(x2 - x1) if x2 != x1 else 0)
    
    return A, B, C, D

def solve_intersection(A1, B1, C1, D1, A2, B2, C2, D2, A3, B3, C3, D3):
    """
    Решает систему трех уравнений для нахождения точки пересечения
    """
    # Матрица коэффициентов
    A = np.array([
        [A1, B1, C1],
        [A2, B2, C2],
        [A3, B3, C3]
    ])
    
    # Вектор правой части
    b = np.array([-D1, -D2, -D3])
    
    try:
        # Решение системы
        solution = np.linalg.solve(A, b)
        return solution
    except np.linalg.LinAlgError:
        print("Система не имеет единственного решения")
        return None

# Пример использования
def main():
    # Точки первого треугольника (примерные значения)
    a1 = np.array([1, 0, 0])
    b1 = np.array([0, 1, 0])
    c1 = np.array([0, 0, 1])
    
    # Точки второго треугольника (примерные значения)
    a2 = np.array([2, 0, 0])
    b2 = np.array([0, 2, 0])
    c2 = np.array([0, 0, 2])
    
    # Уравнения плоскостей
    A1, B1, C1, D1 = find_plane_equation(a1, b1, c1)
    A2, B2, C2, D2 = find_plane_equation(a2, b2, c2)
    
    print("Уравнение плоскости 1:")
    print(f"{A1:.2f}x + {B1:.2f}y + {C1:.2f}z + {D1:.2f} = 0")
    
    print("\nУравнение плоскости 2:")
    print(f"{A2:.2f}x + {B2:.2f}y + {C2:.2f}z + {D2:.2f} = 0")
    
    # Уравнения прямых для сторон второго треугольника
    A3, B3, C3, D3 = find_line_equation(a2, b2)  # сторона a2-b2
    A4, B4, C4, D4 = find_line_equation(a2, c2)  # сторона a2-c2
    
    # Точки пересечения
    point1 = solve_intersection(A1, B1, C1, D1, A2, B2, C2, D2, A3, B3, C3, D3)
    point2 = solve_intersection(A1, B1, C1, D1, A2, B2, C2, D2, A4, B4, C4, D4)
    
    if point1 is not None and point2 is not None:
        print(f"\nПервая точка пересечения: ({point1[0]:.3f}, {point1[1]:.3f}, {point1[2]:.3f})")
        print(f"Вторая точка пересечения: ({point2[0]:.3f}, {point2[1]:.3f}, {point2[2]:.3f})")
        
        # Линия пересечения
        print("\nЛиния пересечения проходит через точки:")
        print(f"O1 = ({point1[0]:.3f}, {point2[0]:.3f})")
        print(f"O2 = ({point1[1]:.3f}, {point2[1]:.3f})")
        print(f"O3 = ({point1[2]:.3f}, {point2[2]:.3f})")

if __name__ == "__main__":
    main()