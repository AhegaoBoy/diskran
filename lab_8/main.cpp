#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Структура для хранения информации о добавке
struct Additive {
    vector<int> components;  // Количество каждого вещества в добавке
    int price;               // Цена добавки
    int index;               // Индекс добавки
};

// Основная функция решения задачи
int main() {
    int M, N;

    cin >> M >> N;

    vector<Additive> additives(M);
    for (int i = 0; i < M; i++) {
        additives[i].components.resize(N);

        for (int j = 0; j < N; j++) {
            cin >> additives[i].components[j];
        }
        cin >> additives[i].price;
        additives[i].index = i + 1;
    }

    vector<bool> covered(N, false);  // Отслеживание покрытых веществ
    vector<int> selected;            // Выбранные добавки

    while (true) {
        int best_idx = -1;
        double best_ratio = 0.0;

        for (int i = 0; i < M; i++) {
            int new_covers = 0;

            for (int j = 0; j < N; j++) {
                if (additives[i].components[j] > 0 && !covered[j]) {
                    new_covers++;
                }
            }

            if (new_covers > 0) {
                double ratio = static_cast<double>(new_covers) / additives[i].price;
                if (ratio > best_ratio) {
                    best_ratio = ratio;
                    best_idx = i;
                }
            }
        }

        if (best_idx == -1) break; // Если подходящая добавка не найдена, прерываем

        selected.push_back(additives[best_idx].index);

        // Обновляем массив covered
        for (int j = 0; j < N; j++) {
            if (additives[best_idx].components[j] > 0) {
                covered[j] = true;
            }
        }

        // Проверяем, покрыты ли все вещества
        bool all_covered = true;
        for (bool c : covered) {
            if (!c) {
                all_covered = false;
                break;
            }
        }

        if (all_covered) break; // Если всё покрыто, выходим
    }

    // Проверяем, покрыты ли все вещества (перепроверка)
    bool all_covered = true;
    for (bool c : covered) {
        if (!c) {
            all_covered = false;
            break;
        }
    }

    if (all_covered) {
        cout << "Выбранные добавки: ";
        for (int idx : selected) {
            cout << idx << " ";
        }
        cout << endl;
    } else {
        cout << "-1" << endl;
    }

    return 0;
}