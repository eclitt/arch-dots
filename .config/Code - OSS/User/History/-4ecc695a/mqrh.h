#ifndef DATE_H
#define DATE_H

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>

class Date {
private:
    int day;
    int month;
    int year;

    bool isValidDate(int d, int m, int y) const {
        if (y < 1900 || y > 2100) return false;
        if (m < 1 || m > 12) return false;
        
        int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        
        // Проверка на високосный год для февраля
        if (m == 2) {
            bool isLeap = (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
            if (isLeap) daysInMonth[1] = 29;
        }
        
        return d >= 1 && d <= daysInMonth[m - 1];
    }

public:
    Date() : day(1), month(1), year(2000) {}
    
    Date(int d, int m, int y) {
        if (isValidDate(d, m, y)) {
            day = d;
            month = m;
            year = y;
        } else {
            day = 1;
            month = 1;
            year = 2000;
        }
    }
    
    Date(const std::string& dateStr) {
        std::istringstream iss(dateStr);
        char delimiter;
        iss >> day >> delimiter >> month >> delimiter >> year;
        
        if (!isValidDate(day, month, year)) {
            day = 1;
            month = 1;
            year = 2000;
        }
    }
    
    // Геттеры
    int getDay() const { return day; }
    int getMonth() const { return month; }
    int getYear() const { return year; }
    
    // Сеттеры
    void setDay(int d) { if (d >= 1 && d <= 31) day = d; }
    void setMonth(int m) { if (m >= 1 && m <= 12) month = m; }
    void setYear(int y) { if (y >= 1900 && y <= 2100) year = y; }
    
    // Операторы сравнения
    bool operator<(const Date& other) const {
        if (year != other.year) return year < other.year;
        if (month != other.month) return month < other.month;
        return day < other.day;
    }
    
    bool operator>(const Date& other) const {
        return other < *this;
    }
    
    bool operator==(const Date& other) const {
        return day == other.day && month == other.month && year == other.year;
    }
    
    bool operator!=(const Date& other) const {
        return !(*this == other);
    }
    
    bool operator<=(const Date& other) const {
        return *this < other || *this == other;
    }
    
    bool operator>=(const Date& other) const {
        return *this > other || *this == other;
    }
    
    // Ввод/вывод
    friend std::ostream& operator<<(std::ostream& os, const Date& date) {
        os << std::setw(2) << std::setfill('0') << date.day << "."
           << std::setw(2) << std::setfill('0') << date.month << "."
           << date.year;
        return os;
    }
    
    friend std::istream& operator>>(std::istream& is, Date& date) {
        std::string input;
        is >> input;
        std::istringstream iss(input);
        char delimiter;
        
        int d, m, y;
        if (iss >> d >> delimiter >> m >> delimiter >> y) {
            if (date.isValidDate(d, m, y)) {
                date.day = d;
                date.month = m;
                date.year = y;
            }
        }
        
        return is;
    }
    
    // Сериализация для бинарного файла
    void serialize(std::ostream& os) const {
        os.write(reinterpret_cast<const char*>(&day), sizeof(day));
        os.write(reinterpret_cast<const char*>(&month), sizeof(month));
        os.write(reinterpret_cast<const char*>(&year), sizeof(year));
    }
    
    void deserialize(std::istream& is) {
        is.read(reinterpret_cast<char*>(&day), sizeof(day));
        is.read(reinterpret_cast<char*>(&month), sizeof(month));
        is.read(reinterpret_cast<char*>(&year), sizeof(year));
    }
    
    // Получение текущей даты
    static Date getCurrentDate() {
        std::time_t t = std::time(nullptr);
        std::tm* now = std::localtime(&t);
        return Date(now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);
    }
    
    // Преобразование в строку
    std::string toString() const {
        std::ostringstream oss;
        oss << *this;
        return oss.str();
    }
    
    // Вычисление возраста
    int getAge() const {
        Date current = getCurrentDate();
        int age = current.year - year;
        if (current.month < month || (current.month == month && current.day < day)) {
            age--;
        }
        return age;
    }
};

#endif // DATE_H