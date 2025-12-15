#include "menu.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

int main() {
    // Инициализация генератора случайных чисел
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    
    // Установка локали для корректного вывода кириллицы в Windows
    #ifdef _WIN32
        system("chcp 65001 > nul");
    #endif
    
    try {
        Menu menu;
        menu.run();
    } catch (const std::exception& e) {
        std::cerr << "Произошла ошибка: " << e.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << "Произошла неизвестная ошибка!\n";
        return 1;
    }
    
    return 0;
}