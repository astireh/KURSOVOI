#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <locale.h>


// Функции 
double calculate_f(double x);
double calculate_derivative_numerical(double x);
void print_menu(); // печать меню
void table(double s, double e, double st);// печать таблицы
double find_min(double w, double q, double* min_val);
double find_max(double w, double q, double* max_val);
double find_root_bisection(double (*f)(double), double target_y, double a, double b, double eps);



// ===================== ГЛАВНАЯ ФУНКЦИЯ ========================
int main() {
    int choice; // выбор пользователя пункта меню
    setlocale(LC_CTYPE, "Rus");

    do {
        print_menu();

        if (scanf("%d", &choice) != 1) {
            printf("Ошибка: введите число от 0 до 5!\n");
            while (getchar() != '\n');// Очистка буфера ввода
            continue;
        }
        puts("\n");
        switch (choice) {
            //======================== ПУНКТ 1: поиск У по Х========================
        case 1:
            printf("Введите x: ");
            double x;
            if (scanf("%lf", &x) == 1) {
                double res = calculate_f(x);//ВЫЗОВ ФУНКЦИИ
                printf("F(%lf) = %lf\n", x, res);
            }
            else {
                printf("Ошибка ввода числа.\n");
                while (getchar() != '\n');// Очистка буфера ввода
            }
            break;
            //======================== ПУНКТ 2: запуск таблицы ========================
        case 2:
            printf("Введите начало, конец и шаг (через пробел): ");//Для ввода нужных символов
            double s, e, st;
            if (scanf("%lf %lf %lf", &s, &e, &st) < 3 || st <= 0 || e < s) {
                printf("Ошибка ввода параметров.\n");
                while (getchar() != '\n'); break;
            }
            table(s, e, st);//ВЫЗОВ ФУНКЦИИ
            break;
            //======================== ПУНКТ 3: запуск поиска экстремумов ========================
        case 3:
            printf("Введите начало интервала: ");
            double w, q;
            double min_val, max_val;
            if (scanf("%lf", &w) != 1) {
                printf("Ошибка ввода.\n");
                while (getchar() != '\n'); break;
            }
            printf("Введите конец интервала: ");
            if (scanf("%lf", &q) != 1) {
                printf("Ошибка ввода.\n");
                while (getchar() != '\n'); break;
            }
            double min_x = find_min(w, q, &min_val);  // получаем аргумент и значение минимума
            double max_x = find_max(w, q, &max_val);  // получаем аргумент и значение максимума

            printf("\nМинимальное значение на отрезке: %.6f\nПри аргументе: %.2f\n", min_val, min_x);
            printf("Максимальное значение на отрезке: %.6f\nПри аргументе: %.2f\n", max_val, max_x);
            break;
            //======================== ПУНКТ 4: Запуск поиска X по Y ========================
        case 4:
            printf("Введите искомый Y: ");
            double target_y, a, b, eps;

            if (scanf("%lf", &target_y) != 1) {
                printf("Ошибка ввода.\n");
                while (getchar() != '\n'); break;
            }
            printf("Начало диапазона поиска: ");
            if (scanf("%lf", &a) != 1) {
                printf("Ошибка ввода.\n");
                while (getchar() != '\n'); break;
            }
            printf("Конец диапазона поиска: ");
            if (scanf("%lf", &b) != 1) {
                printf("Ошибка ввода.\n");
                while (getchar() != '\n'); break;
            }
            if (a >= b) {
                printf("Ошибка ввода.\n");
                break;
            }
            printf("Введите точность (например, 0.001): ");
            if (scanf("%lf", &eps) != 1 || eps <= 0) {
                printf("Ошибка ввода\n");
                eps = 0.001;
            }

            double (*current_f)(double) = calculate_f; // или другая выбранная функция
            double root = find_root_bisection(current_f, target_y, a, b, eps);

            if (!isnan(root)) {
                printf("Найдено примерное значение x = %.5f\n", root);
                printf("Проверка: f(%.5f) = %.5f\n", root, current_f(root));
            }
            else {
                printf("Корень на заданном диапазоне не найден.\n");
            }
            break;
            break;
            //======================== ПУНКТ 5: Производная в точке ========================

        case 5:
            printf("Введите x для поиска производной: ");
            double z;
            if (scanf("%lf", &z) == 1) {
                printf("F'(%lf) = %lf\n", z, calculate_derivative_numerical(z));//ВЫЗОВ ФУНКЦИИ
            }
            else {
                printf("Ошибка ввода.\n");
                while (getchar() != '\n');
            }
            break;
            //======================== ПУНКТ 0 ===============================================
        case 0: printf("Выход из программы. До свидания!\n"); break;

        default: printf("Ошибка: введите число от 0 до 5!\n");
        }
    } while (choice != 0);// Функция будет работать пока пользователь не ввёл 0

    return 0;
}


//Шапка меню
void print_menu() {
    printf("\n========= Главное меню ========\n");
    printf("| 1. Значение функции в точке |\n");
    printf("| 2. Таблица значений         |\n");
    printf("| 3. Поиск минимума/максимума |\n");
    printf("| 4. Поиск X по Y             |\n");
    printf("| 5. Поиск производной в точке|\n");
    printf("| 0. Выход                    |\n");
    printf("===============================\n");
    printf("Выберите пункт: ");
}

//Вычисляет значение функции f(x).Возвращает значение функции
double calculate_f(double x) {
    if (x < -2) {
        return atan(x * x);
    }
    else if (x < 0) {
        return (exp(x) - 1) / x;
    }
    else {
        double sum = 0;
        for (int n = 0; n <= 12; n++) {
            double t = pow(-1, n) * pow(x, n + 1) / (n + 1);
            sum += t;
        }
        return sum;
    }
}

//Вычисляет  производную
double calculate_derivative_numerical(double x) {
    if (x < -2) {
        return 2 * x / (1 + pow(x, 4));
    }
    else if (x < 0) {
        return (exp(x) * (x - 1) + 1) / (x * x);
    }
    else {
        return (1 + pow(x, 13)) / (1 + x);
    }
}

//Таблица
void table(double s, double e, double st) {
    printf("\n+------------+--------------------------------+\n");
    printf("| %-10s | %-30s |\n", "x", "f(x)");
    printf("+------------+--------------------------------+\n");

    for (double x = s; x <= e; x += st) {
        double val = calculate_f(x); // Твоя функция расчета
        printf("| %-10.2f | %-30.6f |\n", x, val);
    }
    printf("+------------+--------------------------------+\n");
}

//МИН И МАКС
double find_min(double w, double q, double* min_val) {
    double min_x = w;  // аргумент минимума
    *min_val = calculate_f(w); // значение функции в начале интервала

    for (double x = w; x <= q + 0.0001; x += 0.01) {
        double current = calculate_f(x);
        if (current < *min_val) {
            *min_val = current;
            min_x = x;
        }
    }
    return min_x; // возвращаем аргумент, при котором достигается минимум
}

double find_max(double w, double q, double* max_val) {
    double max_x = q;  // аргумент максимума
    *max_val = calculate_f(q); // значение функции в конце интервала

    for (double x = w; x <= q + 0.0001; x += 0.01) {
        double current = calculate_f(x);
        if (current > *max_val) {
            *max_val = current;
            max_x = x;
        }
    }
    return max_x; // возвращаем аргумент, при котором достигается максимум
}


//ПОИСК У ПО Х
double find_root_bisection(double (*f)(double), double target_y, double a, double b, double eps) {
    double left = a;
    double right = b;
    double mid;

    while ((right - left) > eps) {
        mid = (left + right) / 2.0;
        double y_mid = f(mid) - target_y;
        double y_left = f(left) - target_y;

        if (y_left * y_mid <= 0) {
            right = mid;
        }
        else {
            left = mid;
        }
    }

    return (left + right) / 2.0;
}
