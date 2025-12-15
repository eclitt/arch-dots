import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# 1. Построение треугольника и нормали к нему
print("1. Построение треугольника и нормали к нему")

# Исходные данные - вершины треугольника
A = np.array([0, 0, 0])
B = np.array([1, 3, 3])
C = np.array([2, 1, 2])

print(f"Вершины треугольника:")
print(f"A = {A}")
print(f"B = {B}")
print(f"C = {C}")

# Вычисление нормали через векторное произведение
AB = B - A
AC = C - A
N = np.cross(AB, AC)
print(f"Нормаль N = {N}")

# Координаты для отрисовки треугольника
triangle_x = [A[0], B[0], C[0], A[0]]
triangle_y = [A[1], B[1], C[1], A[1]]
triangle_z = [A[2], B[2], C[2], A[2]]

# Точки для отображения нормали (из центра треугольника)
center = (A + B + C) / 3
normal_end = center + N / np.linalg.norm(N) * 2  # Укорачиваем для визуализации

# 2. Поворот треугольника в плоскость XOY
print("\n2. Поворот треугольника в плоскость XOY")

# Матрица поворота вокруг оси Z
alpha = np.arctan2(N[1], -N[0])
print(f"Угол поворота α = {alpha:.3f} радиан")

Rz = np.array([
    [np.cos(alpha), np.sin(alpha), 0],
    [-np.sin(alpha), np.cos(alpha), 0],
    [0, 0, 1]
])

# Поворот нормали
N1 = Rz @ N
print(f"Нормаль после поворота вокруг Z: N1 = {N1}")

# Поворот вокруг оси X
beta = np.arctan2(N1[1], N1[2])
print(f"Угол поворота β = {beta:.3f} радиан")

Rx = np.array([
    [1, 0, 0],
    [0, np.cos(beta), np.sin(beta)],
    [0, -np.sin(beta), np.cos(beta)]
])

# Полная матрица поворота
Q_eq1 = Rx @ Rz
print("Матрица поворота Q_eq1:")
print(Q_eq1)

# Поворот вершин треугольника
A1 = Q_eq1 @ A
B1 = Q_eq1 @ B
C1 = Q_eq1 @ C

print(f"Вершины после поворота:")
print(f"A1 = {A1}")
print(f"B1 = {B1}") 
print(f"C1 = {C1}")

# Координаты для отрисовки повернутого треугольника
triangle1_x = [A1[0], B1[0], C1[0], A1[0]]
triangle1_y = [A1[1], B1[1], C1[1], A1[1]]
triangle1_z = [A1[2], B1[2], C1[2], A1[2]]

# Нормаль после поворота
N1_rotated = Q_eq1 @ N
center1 = (A1 + B1 + C1) / 3
normal1_end = center1 + N1_rotated / np.linalg.norm(N1_rotated) * 2

# 3. Поворот треугольника в плоскость XOZ
print("\n3. Поворот треугольника в плоскость XOZ")

# Поворот вокруг оси Y
alpha_y = np.arctan2(N[0], N[2])
print(f"Угол поворота α = {alpha_y:.3f} радиан")

Ry = np.array([
    [np.cos(alpha_y), 0, -np.sin(alpha_y)],
    [0, 1, 0],
    [np.sin(alpha_y), 0, np.cos(alpha_y)]
])

N1_y = Ry @ N
print(f"Нормаль после поворота вокруг Y: {N1_y}")

# Поворот вокруг оси X
beta_x = np.arctan2(N1_y[2], N1_y[1])
print(f"Угол поворота β = {beta_x:.3f} радиан")

Rx2 = np.array([
    [1, 0, 0],
    [0, np.cos(beta_x), np.sin(beta_x)],
    [0, -np.sin(beta_x), np.cos(beta_x)]
])

# Полная матрица поворота
Q_eq2 = Rx2 @ Ry

# Поворот вершин треугольника
A2 = Q_eq2 @ A
B2 = Q_eq2 @ B  
C2 = Q_eq2 @ C

print(f"Вершины после поворота:")
print(f"A2 = {A2}")
print(f"B2 = {B2}")
print(f"C2 = {C2}")

# Координаты для отрисовки
triangle2_x = [A2[0], B2[0], C2[0], A2[0]]
triangle2_y = [A2[1], B2[1], C2[1], A2[1]]
triangle2_z = [A2[2], B2[2], C2[2], A2[2]]

# 4. Поворот треугольника в плоскость YOZ
print("\n4. Поворот треугольника в плоскость YOZ")

# Поворот вокруг оси X
alpha_x = np.arctan2(N[2], N[1])
print(f"Угол поворота α = {alpha_x:.3f} радиан")

Rx3 = np.array([
    [1, 0, 0],
    [0, np.cos(alpha_x), np.sin(alpha_x)],
    [0, -np.sin(alpha_x), np.cos(alpha_x)]
])

N1_x = Rx3 @ N
print(f"Нормаль после поворота вокруг X: {N1_x}")

# Поворот вокруг оси Y
beta_y = np.arctan2(N1_x[0], N1_x[2])
print(f"Угол поворота β = {beta_y:.3f} радиан")

Ry2 = np.array([
    [np.cos(beta_y), 0, -np.sin(beta_y)],
    [0, 1, 0],
    [np.sin(beta_y), 0, np.cos(beta_y)]
])

# Полная матрица поворота
Q_eq3 = Ry2 @ Rx3

# Поворот вершин треугольника
A3 = Q_eq3 @ A
B3 = Q_eq3 @ B
C3 = Q_eq3 @ C

print(f"Вершины после поворота:")
print(f"A3 = {A3}")
print(f"B3 = {B3}")
print(f"C3 = {C3}")

# Координаты для отрисовки
triangle3_x = [A3[0], B3[0], C3[0], A3[0]]
triangle3_y = [A3[1], B3[1], C3[1], A3[1]]
triangle3_z = [A3[2], B3[2], C3[2], A3[2]]

# Визуализация
fig = plt.figure(figsize=(18, 12))

# 1. Исходный треугольник и нормаль
ax1 = fig.add_subplot(231, projection='3d')
# Треугольник
ax1.plot(triangle_x, triangle_y, triangle_z, 'b-', linewidth=3, label='Треугольник ABC')
ax1.scatter([A[0], B[0], C[0]], [A[1], B[1], C[1]], [A[2], B[2], C[2]], c='red', s=80)
# Нормаль
ax1.quiver(center[0], center[1], center[2], 
           N[0], N[1], N[2], 
           color='green', length=2, label='Нормаль N', linewidth=3)
ax1.set_title('Исходный треугольник и нормаль')
ax1.legend()
ax1.set_xlabel('X')
ax1.set_ylabel('Y')
ax1.set_zlabel('Z')

# 2. Поворот в плоскость XOY
ax2 = fig.add_subplot(232, projection='3d')
# Исходный треугольник (полупрозрачный)
ax2.plot(triangle_x, triangle_y, triangle_z, 'b-', linewidth=1, alpha=0.3, label='Исходный')
# Повернутый треугольник
ax2.plot(triangle1_x, triangle1_y, triangle1_z, 'r-', linewidth=3, label='Повернутый в XOY')
ax2.scatter([A1[0], B1[0], C1[0]], [A1[1], B1[1], C1[1]], [A1[2], B1[2], C1[2]], c='red', s=80)
# Нормаль повернутого
ax2.quiver(center1[0], center1[1], center1[2], 
           N1_rotated[0], N1_rotated[1], N1_rotated[2], 
           color='orange', length=2, label='Нормаль N1', linewidth=3)
ax2.set_title('Поворот в плоскость XOY')
ax2.legend()
ax2.set_xlabel('X')
ax2.set_ylabel('Y')
ax2.set_zlabel('Z')

# 3. Поворот в плоскость XOZ
ax3 = fig.add_subplot(233, projection='3d')
# Исходный треугольник
ax3.plot(triangle_x, triangle_y, triangle_z, 'b-', linewidth=1, alpha=0.3, label='Исходный')
# Повернутый треугольник
ax3.plot(triangle2_x, triangle2_y, triangle2_z, 'g-', linewidth=3, label='Повернутый в XOZ')
ax3.scatter([A2[0], B2[0], C2[0]], [A2[1], B2[1], C2[1]], [A2[2], B2[2], C2[2]], c='green', s=80)
ax3.set_title('Поворот в плоскость XOZ')
ax3.legend()
ax3.set_xlabel('X')
ax3.set_ylabel('Y')
ax3.set_zlabel('Z')

# 4. Поворот в плоскость YOZ
ax4 = fig.add_subplot(234, projection='3d')
# Исходный треугольник
ax4.plot(triangle_x, triangle_y, triangle_z, 'b-', linewidth=1, alpha=0.3, label='Исходный')
# Повернутый треугольник
ax4.plot(triangle3_x, triangle3_y, triangle3_z, 'm-', linewidth=3, label='Повернутый в YOZ')
ax4.scatter([A3[0], B3[0], C3[0]], [A3[1], B3[1], C3[1]], [A3[2], B3[2], C3[2]], c='magenta', s=80)
ax4.set_title('Поворот в плоскость YOZ')
ax4.legend()
ax4.set_xlabel('X')
ax4.set_ylabel('Y')
ax4.set_zlabel('Z')

# 5. Все треугольники вместе
ax5 = fig.add_subplot(235, projection='3d')
ax5.plot(triangle_x, triangle_y, triangle_z, 'b-', linewidth=3, label='Исходный ABC')
ax5.plot(triangle1_x, triangle1_y, triangle1_z, 'r-', linewidth=2, label='В XOY')
ax5.plot(triangle2_x, triangle2_y, triangle2_z, 'g-', linewidth=2, label='В XOZ') 
ax5.plot(triangle3_x, triangle3_y, triangle3_z, 'm-', linewidth=2, label='В YOZ')
# Вершины
ax5.scatter([A[0], B[0], C[0]], [A[1], B[1], C[1]], [A[2], B[2], C[2]], c='blue', s=50)
ax5.scatter([A1[0], B1[0], C1[0]], [A1[1], B1[1], C1[1]], [A1[2], B1[2], C1[2]], c='red', s=50)
ax5.scatter([A2[0], B2[0], C2[0]], [A2[1], B2[1], C2[1]], [A2[2], B2[2], C2[2]], c='green', s=50)
ax5.scatter([A3[0], B3[0], C3[0]], [A3[1], B3[1], C3[1]], [A3[2], B3[2], C3[2]], c='magenta', s=50)
ax5.set_title('Все треугольники')
ax5.legend()
ax5.set_xlabel('X')
ax5.set_ylabel('Y')
ax5.set_zlabel('Z')

# 6. Проекции на координатные плоскости
ax6 = fig.add_subplot(236)
# Проекция на XY
ax6.plot(triangle_x, triangle_y, 'b-', linewidth=3, label='Исходный (XY)')
ax6.plot(triangle1_x, triangle1_y, 'r-', linewidth=2, label='В XOY (XY)')
ax6.plot(triangle2_x, triangle2_y, 'g-', linewidth=2, label='В XOZ (XY)')
ax6.plot(triangle3_x, triangle3_y, 'm-', linewidth=2, label='В YOZ (XY)')
# Вершины
ax6.scatter([A[0], B[0], C[0]], [A[1], B[1], C[1]], c='blue', s=50)
ax6.scatter([A1[0], B1[0], C1[0]], [A1[1], B1[1], C1[1]], c='red', s=50)
ax6.scatter([A2[0], B2[0], C2[0]], [A2[1], B2[1], C2[1]], c='green', s=50)
ax6.scatter([A3[0], B3[0], C3[0]], [A3[1], B3[1], C3[1]], c='magenta', s=50)
ax6.set_title('Проекции на плоскость XY')
ax6.legend()
ax6.set_xlabel('X')
ax6.set_ylabel('Y')
ax6.grid(True)

plt.tight_layout()
plt.show()

# Вывод результатов
print("\n" + "="*50)
print("РЕЗУЛЬТАТЫ ЛАБОРАТОРНОЙ РАБОТЫ")
print("="*50)

print(f"\nИсходный треугольник:")
print(f"A({A[0]}, {A[1]}, {A[2]})")
print(f"B({B[0]}, {B[1]}, {B[2]})") 
print(f"C({C[0]}, {C[1]}, {C[2]})")

print(f"\nНормаль к треугольнику: N({N[0]:.3f}, {N[1]:.3f}, {N[2]:.3f})")

print(f"\nПосле поворота в плоскость XOY:")
print(f"A1({A1[0]:.3f}, {A1[1]:.3f}, {A1[2]:.3f})")
print(f"B1({B1[0]:.3f}, {B1[1]:.3f}, {B1[2]:.3f})")
print(f"C1({C1[0]:.3f}, {C1[1]:.3f}, {C1[2]:.3f})")

print(f"\nПосле поворота в плоскость XOZ:")
print(f"A2({A2[0]:.3f}, {A2[1]:.3f}, {A2[2]:.3f})")
print(f"B2({B2[0]:.3f}, {B2[1]:.3f}, {B2[2]:.3f})")
print(f"C2({C2[0]:.3f}, {C2[1]:.3f}, {C2[2]:.3f})")

print(f"\nПосле поворота в плоскость YOZ:")
print(f"A3({A3[0]:.3f}, {A3[1]:.3f}, {A3[2]:.3f})")
print(f"B3({B3[0]:.3f}, {B3[1]:.3f}, {B3[2]:.3f})")
print(f"C3({C3[0]:.3f}, {C3[1]:.3f}, {C3[2]:.3f})")