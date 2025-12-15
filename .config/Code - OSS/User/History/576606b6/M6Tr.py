\documentclass{article}
\usepackage[utf8]{inputenc}
\usepackage{amsmath}
\usepackage{amsfonts}
\usepackage{amssymb}
\usepackage[russian]{babel}
\usepackage{graphicx}

\title{Отчет по компьютерной графике: Нахождение плоскости пересечения двух треугольников}
\author{Студент}
\date{}

\begin{document}

\maketitle

\section{Цель работы}
Найти плоскость, которая пересекает 2 треугольника.

\section{Ход работы}

\subsection{Задание точек треугольников}
\begin{align*}
\text{Треугольник 1:} & \quad a_1 = (x_{a1}, y_{a1}, z_{a1}), \quad a_2 = (x_{a2}, y_{a2}, z_{a2}), \quad a_3 = (x_{a3}, y_{a3}, z_{a3}) \\
\text{Треугольник 2:} & \quad b_1 = (x_{b1}, y_{b1}, z_{b1}), \quad b_2 = (x_{b2}, y_{b2}, z_{b2}), \quad b_3 = (x_{b3}, y_{b3}, z_{b3})
\end{align*}

\subsection{Построение треугольников}
Треугольники задаются вершинами: $(P_1, P_2, P_3)$ и $(Q_1, Q_2, Q_3)$

\subsection{Нахождение уравнений плоскостей}

Для треугольника 1:
\begin{align*}
\vec{t_{11}} &= \vec{b_1} - \vec{a_1} = \begin{pmatrix} x_{b1} - x_{a1} \\ y_{b1} - y_{a1} \\ z_{b1} - z_{a1} \end{pmatrix} \\
\vec{t_{12}} &= \vec{c_1} - \vec{a_1} = \begin{pmatrix} x_{c1} - x_{a1} \\ y_{c1} - y_{a1} \\ z_{c1} - z_{a1} \end{pmatrix} \\
\vec{N_1} &= \vec{t_{11}} \times \vec{t_{12}} = \begin{pmatrix} -2 \\ 12 \\ -8 \end{pmatrix}
\end{align*}

Уравнение плоскости треугольника 1:
\[
\begin{pmatrix} A_1 \\ B_1 \\ C_1 \\ D_1 \end{pmatrix} \approx \begin{pmatrix} N_{10} \\ N_{11} \\ N_{12} \\ -N_{1-4} \end{pmatrix} = \begin{pmatrix} -2 \\ 12 \\ -8 \\ -2 \end{pmatrix}
\]

Для треугольника 2:
\begin{align*}
\vec{t_{21}} &= \vec{b_2} - \vec{a_2} = \begin{pmatrix} x_{b2} - x_{a2} \\ y_{b2} - y_{a2} \\ z_{b2} - z_{a2} \end{pmatrix} \\
\vec{t_{22}} &= \vec{c_2} - \vec{a_2} = \begin{pmatrix} x_{c2} - x_{a2} \\ y_{c2} - y_{a2} \\ z_{c2} - z_{a2} \end{pmatrix} \\
\vec{N_2} &= \vec{t_{21}} \times \vec{t_{22}} = \begin{pmatrix} -4 \\ -9 \\ -7 \end{pmatrix}
\end{align*}

Уравнение плоскости треугольника 2:
\[
\begin{pmatrix} A_2 \\ B_2 \\ C_2 \\ D_2 \end{pmatrix} \approx \begin{pmatrix} N_{20} \\ N_{21} \\ N_{22} \\ -N_{2-2} \end{pmatrix} = \begin{pmatrix} -4 \\ -9 \\ -7 \\ -41 \end{pmatrix}
\]

Общее уравнение плоскости: $f(x,y,z) = Ax + By + Cz + D = 0$

\subsection{Проверка уравнений плоскостей}

Для треугольника 1:
\begin{align*}
A_1 \cdot x_{a1} + B_1 \cdot y_{a1} + C_1 \cdot z_{a1} + D_1 &= 0 \\
A_1 \cdot x_{b1} + B_1 \cdot y_{b1} + C_1 \cdot z_{b1} + D_1 &= 0 \\
A_1 \cdot x_{c1} + B_1 \cdot y_{c1} + C_1 \cdot z_{c1} + D_1 &= 0
\end{align*}

Для треугольника 2:
\begin{align*}
A_2 \cdot x_{a2} + B_2 \cdot y_{a2} + C_2 \cdot z_{a2} + D_2 &= 0 \\
A_2 \cdot x_{b2} + B_2 \cdot y_{b2} + C_2 \cdot z_{b2} + D_2 &= 0 \\
A_2 \cdot x_{c2} + B_2 \cdot y_{c2} + C_2 \cdot z_{c2} + D_2 &= 0
\end{align*}

\subsection{Нахождение точек пересечения}

Решаем систему уравнений:
\[
\begin{cases}
A_1x + B_1y + C_1z + D_1 = 0 \\
A_2x + B_2y + C_2z + D_2 = 0 \\
A_4x + B_4y + C_4z + D_4 = 0
\end{cases}
\]

Где коэффициенты:
\begin{align*}
A_4 &= 1, \quad B_4 = 1, \quad C_4 = 12, \quad D_4 = 2x_2
\end{align*}

Матричная форма системы:
\[
\begin{pmatrix}
A_1 & B_1 & C_1 & | & D_1 \\
A_2 & B_2 & C_2 & | & D_2 \\
A_4 & B_4 & C_4 & | & D_4
\end{pmatrix}
\]

\section{Результаты}

В результате работы получены:
\begin{itemize}
\item Уравнения плоскостей обоих треугольников
\item Линия пересечения плоскостей
\item Визуализация треугольников и их пересечения
\end{itemize}

\end{document}