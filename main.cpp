#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>

using namespace std;

const int MAX_USERS = 100; // Максимальное количество пользователей

// Структура для хранения учетных данных одного пользователя
struct UserData {
    string username;
    string password;
    bool isAdmin;

    UserData() {}
    UserData(const string& username, const string& password, bool isAdmin)
        : username(username), password(password), isAdmin(isAdmin) {}
};

// Функция для загрузки учетных данных из файла
int loadUserDataFromFile(UserData userData[], const string& filename) {
    ifstream file(filename);
    int numUsers = 0;
    if (file.is_open()) {
        string username, password;
        bool isAdmin;
        while (file >> username >> password >> isAdmin) {
            userData[numUsers++] = UserData(username, password, isAdmin);
            if (numUsers >= MAX_USERS) {
                cerr << "Превышено максимальное количество пользователей." << endl;
                break;
            }
        }
        file.close();
    } else {
        cerr << "Ошибка открытия файла: " << filename << endl;
    }
    return numUsers;
}

// Функция для сохранения учетных данных в файл
void saveUserDataToFile(const UserData userData[], int numUsers, const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        for (int i = 0; i < numUsers; ++i) {
            file << userData[i].username << " " << userData[i].password << " " << userData[i].isAdmin << endl;
        }
        file.close();
    } else {
        cerr << "Ошибка открытия файла для записи: " << filename << endl;
    }
}

// Функция для регистрации нового пользователя
void registerUser(UserData userData[], int& numUsers, bool& isAdmin, const string& username, const string& password) {
    if (numUsers >= MAX_USERS) {
        cout << "Достигнуто максимальное количество пользователей." << endl;
        return;
    }
    for (int i = 0; i < numUsers; ++i) {
        if (userData[i].username == username) {
            cout << "Пользователь с таким именем уже существует." << endl;
            return;
        }
    }
    userData[numUsers++] = UserData(username, password, isAdmin);
    saveUserDataToFile(userData, numUsers, "users.txt");
    cout << "Пользователь успешно зарегистрирован!" << endl;
}

// Функция для проверки правильности введенного логина и пароля
bool authenticateUser(const UserData userData[], int numUsers, const string& username, const string& password, bool& isAdmin) {
    for (int i = 0; i < numUsers; ++i) {
        if (userData[i].username == username && userData[i].password == password) {
            isAdmin = userData[i].isAdmin;
            return true;
        }
    }
    return false;
}

int main() {
    srand(time(0)); // Инициализация генератора случайных чисел

    cout << "Добро пожаловать в клавиатурный тренажер!" << endl;

    UserData userData[MAX_USERS];
    int numUsers = loadUserDataFromFile(userData, "users.txt");

    int choice;
    cout << "Выберите действие:" << endl;
    cout << "1. Регистрация" << endl;
    cout << "2. Авторизация" << endl;
    cin >> choice;

    if (choice == 1) {
        string username, password;
        cout << "Введите имя пользователя: ";
        cin >> username;
        cout << "Введите пароль: ";
        cin >> password;

        cout << "Выберите тип пользователя:" << endl;
        cout << "1. Пользователь" << endl;
        cout << "2. Администратор" << endl;
        cin >> choice;

        bool isAdmin = (choice == 2);
        registerUser(userData, numUsers, isAdmin, username, password);
    } else if (choice == 2) {
        string username, password;
        cout << "Введите имя пользователя: ";
        cin >> username;
        cout << "Введите пароль: ";
        cin >> password;

        bool isAdmin = false;
        if (!authenticateUser(userData, numUsers, username, password, isAdmin)) {
            cout << "Ошибка аутентификации. Проверьте правильность введенных данных." << endl;
            return 1;
        }

        cout << "Вы успешно вошли в систему." << endl;
        if (isAdmin) {
            cout << "Вы вошли как администратор." << endl;
        } else {
            cout << "Вы вошли как обычный пользователь." << endl;
        }
    } else {
        cout << "Неверный выбор." << endl;
        return 1;
    }

    // Остальной код программы для тренажера остается без изменений

    cout << "Постарайтесь вводить слова как можно быстрее и без ошибок." << endl;
    cout << "Для выхода введите 'exit'." << endl;

    string words[] = {"привет", "мир", "клавиатура", "программирование", "спасибо", "отлично"};
    const int numWords = sizeof(words) / sizeof(words[0]);

    while (true) {
        // Выбираем случайное слово из массива
        int randIndex = rand() % numWords;
        string targetWord = words[randIndex];

        cout << "Введите слово: " << targetWord << endl;

        clock_t startTime = clock(); // Засекаем время начала ввода

        string userInput;
        cin >> userInput;

        if (userInput == "exit") {
            cout << "До свидания!" << endl;
            break;
        }

        clock_t endTime = clock(); // Засекаем время окончания ввода
        double duration = double(endTime - startTime) / CLOCKS_PER_SEC * 1000; // Преобразуем в миллисекунды

        int correctChars = 0;
        int minLen = min(userInput.length(), targetWord.length());
        for (int i = 0; i < minLen; ++i) {
            if (userInput[i] == targetWord[i]) {
                correctChars++;
            }
        }

        double accuracy = (correctChars * 100.0) / targetWord.length();

        if (userInput == targetWord) {
            cout << "Правильно! Затраченное время: " << duration << " миллисекунд. Точность: " << accuracy << "%" << endl;
        } else {
            cout << "Неправильно. Попробуйте еще раз. Точность: " << accuracy << "%" << endl;
        }
    }

    return 0;
}
