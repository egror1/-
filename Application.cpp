#include "Application.hpp"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>

#include "Roots.hpp"

namespace {
const double kMaxMagnitude = 1e6;
const double kMinTolerance = 1e-12;
const double kMaxTolerance = 1e-2;
const int kPrecision = 12;

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

[[nodiscard]] bool ReadReal(double& value) {
    if (!(std::cin >> value)) {
        RecoverInput();
        return false;
    }
    const bool validLine = FinishLine();
    return validLine && std::isfinite(value) && std::abs(value) <= kMaxMagnitude;
}

void PrintResult(const Roots::Result& result, const double coefficient) {
    std::cout << "N = " << result.iterations << '\n';
    if (result.status == Roots::Status::Success) {
        std::cout << "Корень x = " << result.root << "\nНевязка |f(x)| = " << std::scientific << std::abs(Roots::Evaluate(result.root, coefficient))
                  << std::fixed << '\n';
        return;
    }
    switch (result.status) {
        case Roots::Status::IterationLimit:
            std::cout << "Нет сходимости за 99999 итераций.\n";
            break;
        case Roots::Status::ZeroDerivative:
            std::cout << "Ошибка: производная близка к нулю.\n";
            break;
        case Roots::Status::NonFinite:
            std::cout << "Ошибка: переполнение при вычислении.\n";
            break;
        case Roots::Status::InvalidInterval:
            std::cout << "Ошибка: нужен отрезок left < right со сменой знака f(x).\n";
            break;
        case Roots::Status::Stagnation:
            std::cout << "Достигнут предел точности double.\n";
            break;
        default:
            break;
    }
}

void RunMethod(const Roots::Method method) {
    double coefficient = 0.0;
    double tolerance = 0.0;
    std::cout << "Введите коэффициент a (|a| <= 1e6): ";
    if (!ReadReal(coefficient)) {
        std::cout << "Ошибка: недопустимый коэффициент.\n";
        return;
    }
    std::cout << "Введите погрешность (1e-12..1e-2): ";
    if (!ReadReal(tolerance) || tolerance < kMinTolerance || tolerance > kMaxTolerance) {
        std::cout << "Ошибка: недопустимая погрешность.\n";
        return;
    }
    if (method == Roots::Method::Bisection) {
        double left = 0.0;
        double right = 0.0;
        std::cout << "Введите левую границу: ";
        if (!ReadReal(left)) {
            std::cout << "Ошибка: недопустимая граница.\n";
            return;
        }
        std::cout << "Введите правую границу: ";
        if (!ReadReal(right)) {
            std::cout << "Ошибка: недопустимая граница.\n";
            return;
        }
        PrintResult(Roots::Bisect(coefficient, tolerance, left, right), coefficient);
        return;
    }
    double initial = 0.0;
    std::cout << "Введите начальное приближение x0: ";
    if (!ReadReal(initial)) {
        std::cout << "Ошибка: недопустимое приближение.\n";
        return;
    }
    if (method == Roots::Method::Iteration) {
        PrintResult(Roots::Iterate(coefficient, tolerance, initial), coefficient);
    } else {
        PrintResult(Roots::Newton(coefficient, tolerance, initial), coefficient);
    }
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
}  // namespace

namespace Roots {
void ShowMenu() {
    std::cout << "\nЛабораторная 3. Уравнение x - a*cos(x) = 0\n"
                 "1 - Простая итерация\n2 - Половинное деление\n"
                 "3 - Метод Ньютона\n0 - Выход\nВыбор: ";
}

void RunApplication() {
    std::cout << std::setprecision(kPrecision) << std::fixed;
    while (true) {
        ShowMenu();
        int selection = 0;
        if (!(std::cin >> selection)) {
            if (std::cin.eof()) {
                return;
            }
            RecoverInput();
            std::cout << "Ошибка: требуется целый номер метода.\n";
        } else if (!FinishLine()) {
            std::cout << "Ошибка: лишние символы в номере метода.\n";
        } else {
            switch (static_cast<Method>(selection)) {
                case Method::Exit:
                    return;
                case Method::Iteration:
                case Method::Bisection:
                case Method::Newton:
                    RunMethod(static_cast<Method>(selection));
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
}  // namespace Roots
