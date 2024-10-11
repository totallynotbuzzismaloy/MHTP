#include <iostream>
#include <vector>
#include <cmath>

const double K_a{1.1}; //константа равновесия

int main(){
    int n{5};//Кол-во тарелок
    int g_kub{20 * 1000};//Начальная загрузка куба колонны смесью, моль    
    double g{0.25 * 1000};//Количество молей жидкости на i-ой тарелке
    double x_w{0.05};//начальное приближение
    int M{2};
    
    std::vector<std::vector<double>>x(n, std::vector<double>(n));// Коэффициент активности ацетона в жидкости
    std::vector<std::vector<double>>y(n, std::vector<double>(n));// Коэффициент активности ацетона в паре 
    double L{g}, V{g}, nu{0.4}, eps{0.01};

    for (int k = 0; k < n; ++k) {
        x[k].assign(n, x_w);
        y[k].assign(n, 0.0);
    }

    for (int i = 0; i < M; ++i) {
        y[i][0] = K_a * x[i][0];
        x[i][1] = (g_kub * x[i][0] + g * y[i][0]) / g;
        y[i][1] = y[i][0];

    for (int j = 2; j < n; ++j) {
        y[i][j] = y[i][0];
        x[i][j] = (g * x[i][j - 1] + g * y[i][j - 1] - g * y[i][j - 2]) / g;
    } 

    double s = g_kub * x[i][0] + 4 * g * x[i][1];
    double xk = (g_kub * x[0][0]) / (s + (g_kub - (5 * g)));

    std::cout << "Итерация: " << i + 1 << std::endl << "__________________________" << std::endl;

    for (int j = 0; j < n; ++j) {
    std::cout << "x" << j + 1 << " = " << std::round(x[i][j] * 1000.0) / 1000.0 << "\t|\ty" << j + 1 << " = " << std::round(y[i][j] * 1000.0) / 1000.0 << std::endl;
    }

    std::cout << "xw(k+1) = " << std::round(xk * 1000.0) / 1000.0 << std::endl;

    if (std::abs(xk - x[i][0]) < eps) {
    std::cout << "Сошлось!" << std::endl;
    break;
    }

    x[i + 1][0] = xk;
    } 

    return 0;
}