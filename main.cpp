#include <iostream>

namespace {
const int kMinValue = 0;
const int kHoursPerDay = 24;
const int kMinutesPerHour = 60;
const int kHoursPerHalfDay = 12;
const int kMorningStart = 5;
const int kEveningStart = 18;
const int kBaseTen = 10;
const int kBaseHundred = 100;
const int kOne = 1;
const int kFewStart = 2;
const int kFewEnd = 4;
const int kTeenStart = 11;
const int kTeenEnd = 14;
const int kSuccess = 0;
const int kInputError = 1;
}  // namespace

bool HasValidLineEnding() {
    int nextSymbol = std::cin.get();
    while (nextSymbol != '\n' && nextSymbol != std::char_traits<char>::eof()) {
        if (nextSymbol != ' ' && nextSymbol != '\t' && nextSymbol != '\r') {
            return false;
        }
        nextSymbol = std::cin.get();
    }
    return true;
}

void PrintHours(int hours) {
    std::cout << hours;
    if (hours % kBaseHundred >= kTeenStart && hours % kBaseHundred <= kTeenEnd) {
        std::cout << " часов";
    } else if (hours % kBaseTen == kOne) {
        std::cout << " час";
    } else if (hours % kBaseTen >= kFewStart && hours % kBaseTen <= kFewEnd) {
        std::cout << " часа";
    } else {
        std::cout << " часов";
    }
}

void PrintMinutes(int minutes) {
    std::cout << minutes;
    if (minutes % kBaseHundred >= kTeenStart && minutes % kBaseHundred <= kTeenEnd) {
        std::cout << " минут";
    } else if (minutes % kBaseTen == kOne) {
        std::cout << " минута";
    } else if (minutes % kBaseTen >= kFewStart && minutes % kBaseTen <= kFewEnd) {
        std::cout << " минуты";
    } else {
        std::cout << " минут";
    }
}

void PrintDayPeriod(int hours) {
    if (hours < kMorningStart) {
        std::cout << "ночи";
    } else if (hours < kHoursPerHalfDay) {
        std::cout << "утра";
    } else if (hours < kEveningStart) {
        std::cout << "дня";
    } else {
        std::cout << "вечера";
    }
}

int main() {
    int hours = kMinValue;
    int minutes = kMinValue;
    std::cout << "Введите часы и минуты: ";
    if (!(std::cin >> hours >> minutes) || !HasValidLineEnding()) {
        std::cout << "Ошибка: введите два целых числа.\n";
        return kInputError;
    }
    if (hours < kMinValue || hours >= kHoursPerDay || minutes < kMinValue || minutes >= kMinutesPerHour) {
        std::cout << "Введены недопустимые данные.\n";
        return kInputError;
    }
    if (hours == kHoursPerHalfDay && minutes == kMinValue) {
        std::cout << "полдень\n";
        return kSuccess;
    }
    if (hours == kMinValue && minutes == kMinValue) {
        std::cout << "полночь\n";
        return kSuccess;
    }
    int displayHours = hours % kHoursPerHalfDay;
    if (displayHours == kMinValue) {
        displayHours = kHoursPerHalfDay;
    }
    PrintHours(displayHours);
    std::cout << ' ';
    if (minutes != kMinValue) {
        PrintMinutes(minutes);
        std::cout << ' ';
    }
    PrintDayPeriod(hours);
    if (minutes == kMinValue) {
        std::cout << " ровно";
    }
    std::cout << '\n';
    return kSuccess;
}
