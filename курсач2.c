#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <locale.h>
#include <string.h>
#include <errno.h>

#define STEP 0.01 // Единый шаг для всего кода

// Функции
double calculate_f(double x);
double calculate_derivative_numerical(double x);
void print_menu(); // печать меню
void table(double s, double e, double st);// печать таблицы
void table_to_file(double s, double e, double st, const char* filename);
double find_min(double w, double q);
double find_max(double w, double q);
double find_x_by_y(double target_y, double a, double b, double eps);
int load_from_file(const char* filename, double* s, double* e, double* st);

// ===================== ГЛАВНАЯ ФУНКЦИЯ ========================
int main() {
    int choice; // выбор пользователя пункта меню

    setlocale(LC_CTYPE, "Rus");

    do {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Ошибка: введите число от 0 до 6!\n");
            while (getchar() != '\n');// Очистка буфера ввода
            continue;
        }
        puts("\n");

        switch (choice) {
            //======================== ПУНКТ 1: поиск Y по X========================
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

            //======================== ПУНКТ 2: запуск таблицы с сохранением в файл ========================
        case 2:
        {
            double s, e, st;
            printf("Введите начало, конец и шаг (через пробел): ");
            if (scanf("%lf %lf %lf", &s, &e, &st) != 3 || st <= 0 || e < s) {
                printf("Ошибка ввода параметров. Требуется: шаг > 0, конец >= начала.\n");
                while (getchar() != '\n');
                break;
            }

            // Выводим таблицу на экран
            table(s, e, st);

            // Предлагаем сохранить в файл
            printf("\nСохранить таблицу в файл? (1 - да, 0 - нет): ");
            int save_choice;
            if (scanf("%d", &save_choice) == 1 && save_choice == 1) {
                char filename[100];
                printf("Введите имя файла (например, table.txt): ");
                scanf("%99s", filename);
                table_to_file(s, e, st, filename);
            }
            break;
        }

        //======================== ПУНКТ 3: запуск поиска экстремумов ========================
        case 3:
            printf("Введите начало интервала: ");
            double w, q;
            if (scanf("%lf", &w) != 1) {
                printf("Ошибка ввода.\n");
                while (getchar() != '\n'); break;
            }
            printf("Введите конец интервала: ");
            if (scanf("%lf", &q) != 1) {
                printf("Ошибка ввода.\n");
                while (getchar() != '\n'); break;
            }
            double min_x = find_min(w, q);
            double max_x = find_max(w, q);
            double min_val = calculate_f(min_x);
            double max_val = calculate_f(max_x);

            printf("\nМинимальное значение на отрезке: %.6f\nПри аргументе: %.2f\n", min_val, min_x);
            printf("Максимальное значение на отрезке: %.6f\nПри аргументе: %.2f\n", max_val, max_x);
            break;

            //======================== ПУНКТ 4: Запуск поиска X по Y ========================
        case 4:
            printf("Введите искомый Y: ");// 1. Ввод искомого Y
            double target_y, a, b, eps;
            if (scanf("%lf", &target_y) != 1) {
                printf("Ошибка ввода.\n");
                while (getchar() != '\n'); // Очистка буфера
                break;
            }
            printf("Начало диапазона поиска: ");// 2. Ввод начала диапазона
            if (scanf("%lf", &a) != 1) {
                printf("Ошибка ввода.\n");
                while (getchar() != '\n');
                break;
            }
            printf("Конец диапазона поиска: ");// 3. Ввод конца диапазона
            if (scanf("%lf", &b) != 1) {
                printf("Ошибка ввода.\n");
                while (getchar() != '\n');
                break;
            }
            if (a >= b) {
                printf("Ошибка ввода: начало должно быть меньше конца.\n");
                break;
            }
            printf("Введите точность (например, 0.001): ");// 5. Ввод точности
            if (scanf("%lf", &eps) != 1 || eps <= 0) {
                printf("Ошибка ввода.\n");
                while (getchar() != '\n');
                eps = 0.001;
            }
            double f_a = calculate_f(a) - target_y;
            double f_b = calculate_f(b) - target_y;
            if (f_a * f_b > 0) {
                printf("\nКорень на заданном диапазоне не найден.\n");
            }
            else {
                double root = find_x_by_y(target_y, a, b, eps); //ВЫЗОВ ФУНКЦИИ
                printf("\nНайдено примерное значение x = %.5f\n", root);
                printf("Проверка: f(%.5f) = %.5f\n", root, calculate_f(root));
            }
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

            //======================== ПУНКТ 6: Загрузить параметры из файла ========================
        case 6:
        {
            double fs = 0, fe = 0, fst = 0;
            char input_filename[100];

            printf("Введите имя файла с параметрами (Enter для input.txt): ");
            while (getchar() != '\n'); // Очистка буфера от предыдущего scanf

            if (fgets(input_filename, sizeof(input_filename), stdin) == NULL) {
                strcpy(input_filename, "input.txt");
            }
            else {
                // Удаляем символ новой строки
                input_filename[strcspn(input_filename, "\n")] = 0;

                // Если Enter нажат сразу, используем input.txt по умолчанию
                if (strlen(input_filename) == 0) {
                    strcpy(input_filename, "input.txt");
                }
            }

            if (load_from_file(input_filename, &fs, &fe, &fst)) {
                printf("Параметры успешно загружены из '%s':\n", input_filename);
                printf("  Начало: %.2f\n", fs);
                printf("  Конец:  %.2f\n", fe);
                printf("  Шаг:    %.2f\n", fst);
                printf("\nТаблица значений:\n");
                table(fs, fe, fst);
            }
            break;
        }

        //======================== ПУНКТ 0 ===============================================
        case 0:
            printf("Выход из программы. До свидания!\n");
            break;

        default:
            printf("Ошибка: введите число от 0 до 6!\n");
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
    printf("| 6. Загрузить из файла       |\n");
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

//Вычисляет производную
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
    printf("\n+------------+------------------------------+\n");
    printf("| %-10s | %-30s |\n", "x", "f(x)");
    printf("+------------+------------------------------+\n");
    for (double x = s; x <= e + st / 2; x += st) {
        double val = calculate_f(x);
        printf("| %-10.2f | %-30.6f |\n", x, val);
    }
    printf("+------------+------------------------------+\n");
}

//Сохранение таблицы в файл (с параметрами в начале для возможности загрузки)
void table_to_file(double s, double e, double st, const char* filename) {
    FILE* fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Ошибка: не удалось создать файл '%s'\n", filename);
        printf("Причина: %s\n", strerror(errno));
        return;
    }

    // САМОЕ ВАЖНОЕ: сохраняем параметры в специальном формате для загрузки
    fprintf(fp, "#PARAMS %.6f %.6f %.6f\n", s, e, st);

    fprintf(fp, "# Таблица значений функции f(x)\n");
    fprintf(fp, "# Интервал: [%.2f, %.2f], шаг: %.2f\n\n", s, e, st);
    fprintf(fp, "+------------+------------------------------+\n");
    fprintf(fp, "| %-10s | %-30s |\n", "x", "f(x)");
    fprintf(fp, "+------------+------------------------------+\n");

    for (double x = s; x <= e + st / 2; x += st) {
        double val = calculate_f(x);
        fprintf(fp, "| %-10.2f | %-30.6f |\n", x, val);
    }

    fprintf(fp, "+------------+------------------------------+\n");
    fclose(fp);
    printf("Таблица успешно сохранена в файл: %s\n", filename);
}

//МИН И МАКС
double find_min(double w, double q) {
    double min_val = calculate_f(w);
    double min_x = w;
    for (double x = w; x <= q + STEP / 2; x += STEP) {
        double current = calculate_f(x);
        if (current < min_val) {
            min_val = current;
            min_x = x;
        }
    }
    return min_x;
}

//МИН И МАКС
double find_max(double w, double q) {
    double max_val = calculate_f(w);
    double max_x = w;
    for (double x = w; x <= q + STEP / 2; x += STEP) {
        double current = calculate_f(x);
        if (current > max_val) {
            max_val = current;
            max_x = x;
        }
    }
    return max_x;
}

//ПОИСК Y ПО X
double find_x_by_y(double target_y, double a, double b, double eps) {
    double left = a, right = b, mid;
    while ((right - left) > eps) {
        mid = (left + right) / 2.0;
        if ((calculate_f(left) - target_y) * (calculate_f(mid) - target_y) <= 0)
            right = mid;
        else
            left = mid;
    }
    return (left + right) / 2.0;
}

// Загрузка параметров из файла (ищет строку #PARAMS)
int load_from_file(const char* filename, double* s, double* e, double* st) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Ошибка: не удалось открыть файл '%s'\n", filename);
        printf("Причина: %s\n", strerror(errno));
        return 0;
    }

    printf("Файл '%s' открыт успешно. Чтение параметров...\n", filename);

    char line[256];

    while (fgets(line, sizeof(line), fp) != NULL) {
        // Ищем специальную строку с параметрами #PARAMS
        if (strncmp(line, "#PARAMS ", 8) == 0) {
            // Нашли строку с параметрами, читаем три числа
            if (sscanf(line + 8, "%lf %lf %lf", s, e, st) == 3) {
                // Проверяем корректность значений
                if (*st <= 0) {
                    printf("Ошибка: шаг должен быть положительным (%.6f)\n", *st);
                    fclose(fp);
                    return 0;
                }

                if (*e < *s) {
                    printf("Ошибка: конец (%.2f) должен быть >= начала (%.2f)\n", *e, *s);
                    fclose(fp);
                    return 0;
                }

                fclose(fp);
                return 1; // Успешно прочитали
            }
            else {
                printf("Ошибка: неверный формат строки #PARAMS\n");
                fclose(fp);
                return 0;
            }
        }
    }

    // Если дошли сюда, значит не нашли #PARAMS
    printf("Ошибка: в файле '%s' не найдена строка с параметрами #PARAMS\n", filename);
    printf("Этот файл не был создан пунктом 2 (или параметры удалены).\n");

    fclose(fp);
    return 0;
}