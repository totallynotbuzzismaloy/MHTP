#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>

using namespace std;

double const P = 760;

// Функция для вычисления давления пара по уравнению Антуана
double antoineEquation(double A, double B, double C, double T) {
    return exp(A - (B / (C + T)));
}

// Функция для вычисления величин ?ij
double calculateLambda(double theta_i, double theta_j, double deltaLambda_ij, double R, double T) {
    return (theta_j / theta_i) * exp(-deltaLambda_ij / (R * T));
}

// Функция для вычисления коэффициентов активности компонентов
void calculateActivityCoefficients(double x1, double x2, double lambda12, double lambda21, double& gamma1, double& gamma2) {
    gamma1 = exp(-log(x1 + lambda12 * x2) + x2 * (lambda12 / (x1 + lambda12 * x2) - lambda21 / (x2 + lambda21 * x1)));
    gamma2 = exp(-log(x2 + lambda21 * x1) - x1 * (lambda12 / (x1 + lambda12 * x2) - lambda21 / (x2 + lambda21 * x1)));
}

// Функция для вычисления концентраций компонентов в паровой фазе
void calculateVaporPhaseConcentrations(double gamma1, double gamma2, double P0_1, double P0_2, double x1, double x2, double& y1, double& y2) {
    y1 = gamma1 * P0_1 / P;
    y2 = gamma2 * P0_2 / P;
}

int main() {
    // Открываем файл для записи
    //ofstream file("C:\\Users\\Gogle\\Desktop\\output.txt", ios::trunc);
    ofstream file("output.txt", ios::trunc);
    // Проверяем, успешно ли открылся файл
    if (!file.is_open()) {
        cout << "Ошибка открытия файла." << endl;
        return 1; // Возвращаем ненулевой код ошибки
    }

    // Данные исходной задачи
    double A1 = 16.65, B1 = 2940.46, C1 = -35.93; // Эмпирические коэффициенты для компонента 1
    double A2 = 18.30, B2 = 3816.44, C2 = -46.13; // Эмпирические коэффициенты для компонента 2
    double theta1 = 104.0, theta2 = 18.7; // Мольные объемы компонентов
    double deltaLambda12 = 180.31, deltaLambda21 = 1722.1; // ??ij
    double R = 1.987; // Газовая постоянная
    double T_initial = 333.15; // Начальная температура в Кельвинах
    double epsilon = 0.01; // Точность расчета
    double x1 = 0.5, x2 = 1 - x1; // доля содержания ацетона, и x2 - вода

    // Шаг 2: Задание начальных значений
    double T_current = T_initial;
    
    file.fill(' ');
    file.precision(3);
    file << "*________*________*__________*__________*________*________*______*______*___________________*" << endl;
    file << "|  P0_1  |  P0_2  | lambda12 | lambda21 | gamma1 | gamma2 |  y1  |  y2  | fabs(y1 + y2 - 1) |" << endl;
    file << "*--------*--------*----------*----------*--------*--------*------*------*-------------------*" << endl;
    // Цикл итераций для коррекции температуры
    do {
        //шаг 3
        double P0_1 = antoineEquation(A1, B1, C1, T_current);
        double P0_2 = antoineEquation(A2, B2, C2, T_current);
        // Шаг 4: Вычисление величин ?ij
        double lambda12 = calculateLambda(theta1, theta2, deltaLambda12, R, T_current);
        double lambda21 = calculateLambda(theta2, theta1, deltaLambda21, R, T_current);

        // Шаг 5: Вычисление коэффициентов активности
        double gamma1, gamma2;
        calculateActivityCoefficients(x1, x2, lambda12, lambda21, gamma1, gamma2);

        // Шаг 6: Вычисление концентраций компонентов в паровой фазе
        double y1, y2;
        calculateVaporPhaseConcentrations(gamma1, gamma2, P0_1, P0_2, x1, x2, y1, y2);
        
        // Вывод данных
        file << "|" << setw(8) << P0_1 << fixed
        << "|" << setw(8) << P0_2 << fixed
        << "|" << setw(10) << lambda12 << fixed 
        << "|" << setw(10) << lambda21 << fixed 
        << "|" << setw(8) << gamma1 << fixed
        << "|" << setw(8) << gamma2 << fixed 
        << "|" << setw(6) << y1 << fixed
        << "|" << setw(6) << y2 << fixed 
        << "|";
        // Проверка выполнения материального баланса
        if (fabs(y1 + y2 - 1) <= epsilon) {
            file << setw(19) << fabs(y1 + y2 - 1) << "|" <<'\n';
            file << "*--------*--------*----------*----------*--------*--------*------*------*-------------------*" << endl;
    // Если баланс выполнен, вывод результатов и завершение цикла
            file << "Boiling point: " << T_current << " K" << endl;
            file << "Conc in first phase:" << endl;
            file << "y1 = " << y1 << endl;
            file << "y2 = " << y2 << endl;
            break;
        }
        else {
            file << setw(19) << fabs(y1 + y2 - 1) << "|" <<'\n';
            // Если баланс не выполнен, коррекция температуры и новая итерация
            T_current -= 0.01; // Пример коррекции температуры, можно использовать другой метод коррекции
        }
        file << "*--------*--------*----------*----------*--------*--------*------*------*-------------------*" << endl;
    
    } while (true);
    file.close();

    return 0;
}