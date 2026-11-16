
#include <iostream>
#include <windows.h>
#include <string>
#include <stdexcept>
using namespace std;

class Starship {
    string name;
    int maxLoad, load = 0, fuel;

public:
    Starship(const string& n, int m, int f)
        : name(n), maxLoad(m), fuel(f) {
        if (maxLoad <= 0)
            throw invalid_argument("Грузоподъёмность должна быть положительной");
    }

    string getName() const { return name; }

    void loadCargo(const string& item, int weight) {
        if (weight <= 0)
            throw invalid_argument("Вес должен быть положительным");

        if (load + weight > maxLoad)
            throw runtime_error("Перегрузка: " + item + " не помещается");

        load += weight;
        cout << "Загружено: " << item << " (" << weight
            << " т), всего " << load << "/" << maxLoad << " т\n";
    }

    void launch() {
        if (fuel < 50)
            throw runtime_error("Недостаточно топлива");

        fuel -= 50;
        cout << name << ": Старт! На борту " << load
            << " т, остаток топлива " << fuel << "%\n";
    }
};

class Airlock {
    string shipName;

public:
    explicit Airlock(const string& n) : shipName(n) {
        cout << "[Шлюз " << shipName << " открыт]\n";
    }

    ~Airlock() {
        cout << "[Шлюз " << shipName << " закрыт]\n";
    }
};

void loadManifest(Starship& ship, const string items[],
    const int weights[], int count) {
    Airlock airlock(ship.getName());

    for (int i = 0; i < count; i++)
        ship.loadCargo(items[i], weights[i]);
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    try {
        Starship ghost("Призрак", 0, 100);
    }
    catch (const invalid_argument& e) {
        cout << "Корабль не создан: " << e.what() << '\n';
    }

    Starship dream("Мечта", 100, 60);

    try {
        dream.loadCargo("Продукты", 30);
        dream.loadCargo("Вода", 40);
        dream.loadCargo("Руда", 50);
        dream.launch();
    }
    catch (const invalid_argument& e) {
        cout << "Некорректные данные: " << e.what() << '\n';
    }
    catch (const runtime_error& e) {
        cout << "Ошибка полёта: " << e.what() << '\n';
    }

    cout << "Рейс продолжается\n";

    try {
        dream.launch();
        dream.launch();
    }
    catch (const exception& e) {
        cout << "Ошибка: " << e.what() << '\n';
    }

    Starship ship("Грузовик-7", 120, 100);

    string items1[] = { "Почта", "Запчасти" };
    int weights1[] = { 10, 20 };

    loadManifest(ship, items1, weights1, 2);

    string items2[] = { "Генератор", "Броня", "Спутник" };
    int weights2[] = { 50, 40, 30 };

    try {
        loadManifest(ship, items2, weights2, 3);
    }
    catch (const exception& e) {
        cout << "Центр управления: " << e.what() << '\n';
    }

    return 0;
}

/*

1. Исключение — способ сообщить об ошибке.
   false можно проигнорировать, а исключение нужно обработать.
   Конструктор не возвращает значение, поэтому использует throw.

2. try выполняет код, throw выбрасывает исключение,
   catch его обрабатывает. Продукты и вода загружены,
   руда — нет. Старт пропущен, поскольку после throw
   программа выходит из блока try.

3. const exception& позволяет избежать копирования
   и сохранить реальный тип исключения.
   invalid_argument и runtime_error наследуются от exception.

4. Раскрутка стека — автоматическое уничтожение локальных
   объектов при распространении исключения.
   Шлюз закрылся благодаря вызову деструктора Airlock.
*/
