import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# 1. Построение треугольника и нормали к нему
print("1. Построение треугольника и нормали к нему")

# Исходные данные
x = np.array([0, 1, 2, 0])
y = np.array([0, 3, 1, 0]) 
z = np.array([0, 3, 2, 0])

# Вершины треугольника
A = np.array([x[0], y[0], z[0]])
B = np.array([x[1], y[1], z[1]]) 
C = np.array([x[2], y[2], z[2]])

print(f"Вершины треугольника:")
print(f"A = {A}")
print(f"B = {B}")
print(f"C = {C}")

# Вычисление нормали через векторное произведение
AB = B - A
BC = C - B
N = np.cross(AB, BC)
print(f"Нормаль N = {N}")

# Точки для отображения нормали
N_points = np.array([
    [N[0], -N[0], 0],
    [N[1], -N[1], 0], 
    [N[2], -N[2], 0]
])

# 2. Поворот треугольника в плоскость XOY
print("\n2. Поворот треугольника в плоскость XOY")

# Матрица поворота вокруг оси Z
alpha = np.arctan2(N[1], -N[0])  # исправленная формула
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

# Проверка нормали
N_check = np.cross(B1 - A1, C1 - B1)
print(f"Проверка нормали: {N_check}")

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
print("Матрица поворота Q_eq2:")
print(Q_eq2)

# Поворот вершин треугольника
A2 = Q_eq2 @ A
B2 = Q_eq2 @ B  
C2 = Q_eq2 @ C

print(f"Вершины после поворота:")
print(f"A2 = {A2}")
print(f"B2 = {B2}")
print(f"C2 = {C2}")

# 4. Поворот треугольника в плоскость YOZ
print("\n4. Поворот треугольника в плоскость YOZ")

# Поворот вокруг оси X
alpha_x = np.arctan2(N[1], N[2])
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
print("Матрица поворота Q_eq3:")
print(Q_eq3)

# Поворот вершин треугольника
A3 = Q_eq3 @ A
B3 = Q_eq3 @ B
C3 = Q_eq3 @ C

print(f"Вершины после поворота:")
print(f"A3 = {A3}")
print(f"B3 = {B3}")
print(f"C3 = {C3}")

# Визуализация
fig = plt.figure(figsize=(20, 15))

# 1. Исходный треугольник и нормаль
ax1 = fig.add_subplot(231, projection='3d')
# Треугольник
ax1.plot(x, y, z, 'b-', linewidth=2, label='Треугольник ABC')
ax1.scatter(x[:3], y[:3], z[:3], c='red', s=50)
# Нормаль
center = np.mean([A, B, C], axis=0)
ax1.quiver(center[0], center[1], center[2], 
           N[0], N[1], N[2], 
           color='green', length=2, label='Нормаль N')
ax1.set_title('Исходный треугольник и нормаль')
ax1.legend()
ax1.set_xlabel('X')
ax1.set_ylabel('Y')
ax1.set_zlabel('Z')

# 2. Поворот в плоскость XOY
ax2 = fig.add_subplot(232, projection='3d')
# Исходный треугольник
ax2.plot(x, y, z, 'b-', linewidth=1, alpha=0.5, label='Исходный')
# Повернутый треугольник
tri1_x = [A1[0], B1[0], C1[0], A1[0]]
tri1_y = [A1[1], B1[1], C1[1], A1[1]] 
tri1_z = [A1[2], B1[2], C1[2], A1[2]]
ax2.plot(tri1_x, tri1_y, tri1_z, 'r-', linewidth=2, label='Повернутый в XOY')
ax2.scatter(tri1_x[:3], tri1_y[:3], tri1_z[:3], c='red', s=50)
ax2.set_title('Поворот в плоскость XOY')
ax2.legend()
ax2.set_xlabel('X')
ax2.set_ylabel('Y')
ax2.set_zlabel('Z')

# 3. Поворот в плоскость XOZ
ax3 = fig.add_subplot(233, projection='3d')
# Исходный треугольник
ax3.plot(x, y, z, 'b-', linewidth=1, alpha=0.5, label='Исходный')
# Повернутый треугольник
tri2_x = [A2[0], B2[0], C2[0], A2[0]]
tri2_y = [A2[1], B2[1], C2[1], A2[1]]
tri2_z = [A2[2], B2[2], C2[2], A2[2]]
ax3.plot(tri2_x, tri2_y, tri2_z, 'g-', linewidth=2, label='Повернутый в XOZ')
ax3.scatter(tri2_x[:3], tri2_y[:3], tri2_z[:3], c='green', s=50)
ax3.set_title('Поворот в плоскость XOZ')
ax3.legend()
ax3.set_xlabel('X')
ax3.set_ylabel('Y')
ax3.set_zlabel('Z')

# 4. Поворот в плоскость YOZ
ax4 = fig.add_subplot(234, projection='3d')
# Исходный треугольник
ax4.plot(x, y, z, 'b-', linewidth=1, alpha=0.5, label='Исходный')
# Повернутый треугольник
tri3_x = [A3[0], B3[0], C3[0], A3[0]]
tri3_y = [A3[1], B3[1], C3[1], A3[1]]
tri3_z = [A3[2], B3[2], C3[2], A3[2]]
ax4.plot(tri3_x, tri3_y, tri3_z, 'm-', linewidth=2, label='Повернутый в YOZ')
ax4.scatter(tri3_x[:3], tri3_y[:3], tri3_z[:3], c='magenta', s=50)
ax4.set_title('Поворот в плоскость YOZ')
ax4.legend()
ax4.set_xlabel('X')
ax4.set_ylabel('Y')
ax4.set_zlabel('Z')

# 5. Все треугольники вместе
ax5 = fig.add_subplot(235, projection='3d')
ax5.plot(x, y, z, 'b-', linewidth=2, label='Исходный ABC')
ax5.plot(tri1_x, tri1_y, tri1_z, 'r-', linewidth=2, label='В XOY')
ax5.plot(tri2_x, tri2_y, tri2_z, 'g-', linewidth=2, label='В XOZ') 
ax5.plot(tri3_x, tri3_y, tri3_z, 'm-', linewidth=2, label='В YOZ')
ax5.set_title('Все треугольники')
ax5.legend()
ax5.set_xlabel('X')
ax5.set_ylabel('Y')
ax5.set_zlabel('Z')

# 6. Проекции на координатные плоскости
ax6 = fig.add_subplot(236)
# Проекция на XY
ax6.plot(x, y, 'b-', linewidth=2, label='Исходный (XY)')
ax6.plot(tri1_x, tri1_y, 'r-', linewidth=2, label='В XOY (XY)')
ax6.plot([A2[0], B2[0], C2[0], A2[0]], 
         [A2[1], B2[1], C2[1], A2[1]], 'g-', linewidth=2, label='В XOZ (XY)')
ax6.plot([A3[0], B3[0], C3[0], A3[0]], 
         [A3[1], B3[1], C3[1], A3[1]], 'm-', linewidth=2, label='В YOZ (XY)')
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