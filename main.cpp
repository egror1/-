#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>

namespace {
const int kMaxCount = 1000000;
const int kDivisor = 5;
const int kFirstEven = 2;
const int kLastEven = 8;
const int kFirstTriple = 3;
const int kLastTriple = 9;
const int kFirstCheckpoint = 3;
const int kSecondCheckpoint = 5;
const int kThirdCheckpoint = 10;
const int kTableSteps = 5;
const int kWidth = 17;
const int kPrecision = 9;
const double kTolerance = 1e-6;
const double kEvenMultiplier = 2.0;
const double kMaxMagnitude = 1e100;

[[nodiscard]] bool FinishLine() {
    int symbol = std::cin.get();
    bool valid = true;
    while (symbol != '\n' && symbol != std::char_traits<char>::eof()) {
        if (symbol != ' ' && symbol != '\t' && symbol != '\r') {
            valid = false;
        }
        symbol = std::cin.get();
    }
    return valid;
}

void RecoverInput() {
    if (!std::cin.eof()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

[[nodiscard]] bool ReadInteger(int& value) {
    if (!(std::cin >> value)) {
        RecoverInput();
        return false;
    }
    return FinishLine();
}

[[nodiscard]] bool ReadReal(double& value) {
    if (!(std::cin >> value)) {
        RecoverInput();
        return false;
    }
    const bool validLine = FinishLine();
    return validLine && std::isfinite(value) && std::abs(value) <= kMaxMagnitude;
}

[[nodiscard]] long long SumMultiples(const int upper, const int excludedDivisor) {
    long long sum = 0;
    for (int number = kDivisor; number <= upper; number += kDivisor) {
        if (number % excludedDivisor != 0) {
            sum += number;
        }
    }
    return sum;
}

void RunSum() {
    int upper = 0;
    int excludedDivisor = 0;
    std::cout << "Введите n (2..1000000): ";
    if (!ReadInteger(upper) || upper < 2 || upper > kMaxCount) {
        std::cout << "Ошибка: недопустимое n.\n";
        return;
    }
    std::cout << "Введите m (1 <= m < n): ";
    if (!ReadInteger(excludedDivisor) || excludedDivisor < 1 || excludedDivisor >= upper) {
        std::cout << "Ошибка: требуется 1 <= m < n.\n";
        return;
    }
    std::cout << "Сумма = " << SumMultiples(upper, excludedDivisor) << '\n';
}

[[nodiscard]] double CalculateProduct(const double parameter) {
    double product = 1.0;
    if (parameter >= 0.0) {
        for (int index = kFirstEven; index <= kLastEven; index += kFirstEven) {
            product *= index * index;
        }
        return product - parameter;
    }
    for (int index = kFirstTriple; index <= kLastTriple; index += kFirstTriple) {
        product *= index - kFirstEven;
    }
    return product;
}

void RunProduct() {
    double parameter = 0.0;
    std::cout << "Введите a (|a| <= 1e100): ";
    if (!ReadReal(parameter)) {
        std::cout << "Ошибка: требуется конечное вещественное число.\n";
        return;
    }
    std::cout << "Произведение S = " << CalculateProduct(parameter) << '\n';
}

[[nodiscard]] double CalculateSeries(const double x, int& lastIndex) {
    double term = 1.0;
    double sum = term;
    lastIndex = 0;
    while (std::abs(term) >= kTolerance) {
        ++lastIndex;
        const double index = lastIndex;
        const double previousIndex = index - 1.0;
        const double previousTerm = term;
        term = previousTerm * (-x * x) * (kEvenMultiplier * index * index + 1.0) /
               ((kEvenMultiplier * index - 1.0) * (kEvenMultiplier * index) * (kEvenMultiplier * previousIndex * previousIndex + 1.0));
        sum += term;
    }
    return sum;
}

void RunTable() {
    std::cout << std::setw(kWidth) << "x" << std::setw(kWidth) << "Y(x)" << std::setw(kWidth) << "S(x)" << std::setw(kWidth) << "N" << '\n';
    for (int step = 0; step <= kTableSteps; ++step) {
        const double x = static_cast<double>(step) / kTableSteps;
        const double reference = (1.0 - x * x / 2.0) * std::cos(x) - x / 2.0 * std::sin(x);
        int lastIndex = 0;
        const double sum = CalculateSeries(x, lastIndex);
        std::cout << std::setw(kWidth) << x << std::setw(kWidth) << reference << std::setw(kWidth) << sum << std::setw(kWidth) << lastIndex << '\n';
    }
}

void RunRadical() {
    int count = 0;
    std::cout << "Введите n (1..1000000): ";
    if (!ReadInteger(count) || count < 1 || count > kMaxCount) {
        std::cout << "Ошибка: недопустимое n.\n";
        return;
    }
    double value = 0.0;
    for (int index = 1; index <= count; ++index) {
        value = std::sqrt(kEvenMultiplier * index + value);
        if (index == kFirstCheckpoint || index == kSecondCheckpoint || index == kThirdCheckpoint) {
            std::cout << "Итерация " << index << ": y = " << value << '\n';
        }
    }
    std::cout << "Результат y = " << value << '\n';
}

[[nodiscard]] bool AskContinue() {
    while (!std::cin.eof()) {
        std::cout << "Продолжить работу? (y/n): ";
        char answer = '\0';
        if (!(std::cin >> answer)) {
            return false;
        }
        const bool validLine = FinishLine();
        if (validLine && (answer == 'n' || answer == 'N')) {
            return false;
        }
        if (validLine && (answer == 'y' || answer == 'Y')) {
            return true;
        }
        std::cout << "Ошибка: введите y или n.\n";
    }
    return false;
}

void ShowMenu() {
    std::cout << "\nЛабораторная 2. Вариант 11\n"
                 "1 - Сумма натуральных чисел\n2 - Произведения\n"
                 "3 - Таблица ряда Тейлора\n4 - Вложенные корни\n0 - Выход\nВыбор: ";
}

void RunApplication() {
    std::cout << std::fixed << std::setprecision(kPrecision);
    while (true) {
        ShowMenu();
        int selection = 0;
        if (!ReadInteger(selection)) {
            if (std::cin.eof()) {
                return;
            }
            std::cout << "Ошибка: требуется целый номер задания.\n";
        } else {
            switch (selection) {
                case 0:
                    return;
                case 1:
                    RunSum();
                    break;
                case 2:
                    RunProduct();
                    break;
                case 3:
                    RunTable();
                    break;
                case 4:
                    RunRadical();
                    break;
                default:
                    std::cout << "Ошибка: неизвестный пункт меню.\n";
                    break;
            }
        }
        if (!AskContinue()) {
            return;
        }
    }
}
}  // namespace

int main(int, char**) {
    RunApplication();
    return 0;
}
