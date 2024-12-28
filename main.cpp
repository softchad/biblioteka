#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <chrono>
#include <ctime>

using namespace std;

// Funkcija linijai atspausdinti
void printLine(int length = 50) {
    cout << setfill('-') << setw(length) << "-" << setfill(' ') << endl;
}

// Funkcija pavadinimui su borteliais atspausdinti
void printTitle(const string& title) {
    printLine();
    cout << "| " << left << setw(46) << title << "|" << endl;
    printLine();
}

// Bazine klase LibraryItem - abstrakti klase bibliotekos knygoms
class LibraryItem {
protected:
    static int nextID;      // Kitos knygos unikalus ID
    int id;                 // Knygos unikalus ID
    string title;           // Pavadinimas
    string author;          // Autorius
    int year;               // Leidimo metai
    string category;        // Zanras
    bool isAvailable;       // Prieinamumo busena

public:
    LibraryItem(string title, string author, int year, string category)
            : title(title), author(author), year(year), category(category), isAvailable(true) {
        id = nextID++; // Priskiriame unikalų ID ir didiname jį
    }

    virtual void displayInfo() const = 0; // Abstrakti funkcija informacijos atvaizdavimui

    virtual bool checkAvailability() const {
        return isAvailable;
    }

    virtual void borrowItem() {
        if (!isAvailable) throw runtime_error("Knyga jau rezervuota.");
        isAvailable = false;
    }

    virtual void returnItem() {
        isAvailable = true;
    }

    int getID() const { return id; } // Naujas metodas ID gavimui
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    string getCategory() const { return category; }
    virtual ~LibraryItem() {}
};

// Inicializuojame static ID
int LibraryItem::nextID = 1;

// Knygos klase, paveldi LibraryItem
class Book : public LibraryItem {
public:
    Book(string title, string author, int year, string category)
            : LibraryItem(title, author, year, category) {}

    void displayInfo() const override {
        cout << left << setw(25) << title
             << "| " << setw(20) << author
             << "| " << right << setw(4) << year << " "
             << "| " << left << setw(10) << category
             << "| " << setw(7) << (isAvailable ? "Laisva" : "Uzimta") << " |" << endl;
    }

};

// Vartotojo klase
class User {
private:
    string name;  // Vartotojo vardas
    string password; // Vartotojo slaptažodis

public:
    User(string name, string password) : name(name), password(password) {}

    void displayUserInfo() const {
        cout << "| Vardas: " << setw(15) << name << " |" << endl;
    }

    string getName() const { return name; }
    string getPassword() const { return password; }
};

// Rezervacijos klase
class Reservation {
private:
    User* user;
    LibraryItem* item;
    string reservationDate; // Rezervacijos data
    string returnDate;      // Atsiimimo data

public:
    // Pagrindinis konstruktorius (automatinė data)
    Reservation(User* user, LibraryItem* item)
            : user(user), item(item) {
        auto now = chrono::system_clock::now();
        time_t now_time = chrono::system_clock::to_time_t(now);

        // Rezervacijos data
        char buffer[20];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&now_time));
        reservationDate = string(buffer);

        // Atsiemimo data (pridedame 10 dienų)
        auto return_time = now + chrono::hours(24 * 10);
        time_t return_time_t = chrono::system_clock::to_time_t(return_time);
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&return_time_t));
        returnDate = string(buffer);
    }

    // Konstruktorius su konkrečiomis datomis
    Reservation(User* user, LibraryItem* item, const string& reservationDate, const string& returnDate)
            : user(user), item(item), reservationDate(reservationDate), returnDate(returnDate) {}

    void displayReservationInfo() const {
        printTitle("Rezervacijos informacija");
        cout << "Rezervacijos data: " << reservationDate << endl;
        cout << "Atsiimti iki: " << returnDate << endl;
        user->displayUserInfo();
        printLine();
        item->displayInfo();
        printLine();
    }

    void cancelReservation() {
        item->returnItem();
    }

    User* getUser() const { return user; }
    LibraryItem* getItem() const { return item; }

    string getReservationDate() const {
        return reservationDate;
    }

    string getReturnDate() const {
        return returnDate;
    }
};

// Bibliotekos klase
class Library {
private:
    vector<LibraryItem*> items;
    vector<User*> users;
    vector<Reservation*> reservations;
    User* loggedInUser = nullptr;

public:
    Library() {
        loadItemsFromFile();
    }

    void loadItemsFromFile() {
        ifstream inFile("books.txt");
        if (!inFile.is_open()) {
            cout << "Failas books.txt nerastas. Sukuriamas naujas failas." << endl;
            ofstream outFile("books.txt");
            outFile.close();
            return;
        }

        string title, author, category;
        int year;

        while (getline(inFile, title, '|')) {
            getline(inFile, author, '|');
            inFile >> year;
            inFile.ignore(1); // Skip the '|'
            getline(inFile, category);

            items.push_back(new Book(title, author, year, category));
        }

        inFile.close();
    }

    void saveReservationsToFile() {
        ofstream outFile("reservations.txt");
        for (const auto& res : reservations) {
            outFile << res->getUser()->getName() << "|"
                    << res->getItem()->getTitle() << "|"
                    << res->getItem()->getAuthor() << "|"
                    << res->getItem()->getCategory() << "|"
                    << res->getItem()->checkAvailability() << "|"
                    << res->getReservationDate() << "|"
                    << res->getReturnDate() << endl;
        }
        outFile.close();
    }

    void loadReservationsFromFile() {
        ifstream inFile("reservations.txt");
        if (!inFile.is_open()) {
            cout << "Failas reservations.txt nerastas. Sukuriamas naujas failas." << endl;
            ofstream outFile("reservations.txt");
            outFile.close();
            return;
        }

        string name, title, author, category, reservationDate, returnDate;
        bool isAvailable;

        while (getline(inFile, name, '|')) {
            getline(inFile, title, '|');
            getline(inFile, author, '|');
            getline(inFile, category, '|');
            inFile >> isAvailable;
            inFile.ignore(1); // Skip the '|'
            getline(inFile, reservationDate, '|');
            getline(inFile, returnDate);

            User* user = nullptr;
            for (auto& u : users) {
                if (u->getName() == name) {
                    user = u;
                    break;
                }
            }

            if (!user) {
                user = new User(name, ""); // Jei vartotojas neegzistuoja, sukurti laikiną vartotoją
                users.push_back(user);
            }

            LibraryItem* item = nullptr;
            for (auto& i : items) {
                if (i->getTitle() == title) {
                    item = i;
                    if (!isAvailable) {
                        item->borrowItem(); // Pažymėti knyga kaip rezervuotą
                    }
                    break;
                }
            }

            if (item) {
                reservations.push_back(new Reservation(user, item, reservationDate, returnDate));
            }
        }

        inFile.close();
    }

    void loadUsersFromFile() {
        ifstream inFile("users.txt");
        if (!inFile.is_open()) {
            cout << "Failas users.txt nerastas. Sukuriamas naujas failas." << endl;
            ofstream outFile("users.txt");
            outFile.close();
            return;
        }
        string name, password;
        while (inFile >> name >> password) {
            users.push_back(new User(name, password));
        }
        inFile.close();
    }

    void saveUsersToFile() {
        ofstream outFile("users.txt");
        for (const auto& user : users) {
            outFile << user->getName() << " " << user->getPassword() << endl;
        }
        outFile.close();
    }

    void displayItems() const {
        printTitle("Visos Bibliotekos Knygos");
        cout << "| ID  | " << left << setw(25) << "Pavadinimas"
             << "| " << setw(20) << "Autorius"
             << "| " << setw(4) << "Metai"
             << "| " << setw(9) << "Zanras"
             << "| " << setw(7) << "Statusas" << " |" << endl;
        printLine(80);
        for (const auto& item : items) {
            cout << "| " << setw(4) << item->getID() << "|"; // Pridėtas ID
            item->displayInfo();
        }
        printLine(80);
    }

    void displayAvailableItems() const {
        printTitle("Laisvos Knygos");
        cout << "| ID  | " << left << setw(25) << "Pavadinimas"
             << "| " << setw(20) << "Autorius"
             << "| " << setw(4) << "Metai"
             << "| " << setw(10) << "Zanras"
             << "| Statusas |" << endl;
        printLine(80);

        bool found = false;
        for (const auto& item : items) {
            if (item->checkAvailability()) {
                cout << "| " << setw(4) << item->getID() << "|"; // Spausdiname ID
                item->displayInfo();
                found = true;
            }
        }
        if (!found) {
            cout << "| Visos knygos yra rezervuotos." << endl;
        }
        printLine(80);
    }

    void filterByCategory() const {
        string query;
        cout << "Iveskite zanra: ";
        cin.ignore();
        getline(cin, query);

        transform(query.begin(), query.end(), query.begin(), ::tolower);

        printTitle("Filtruoti Pagal Zanra");
        bool found = false;
        for (const auto& item : items) {
            string itemCategory = item->getCategory();
            transform(itemCategory.begin(), itemCategory.end(), itemCategory.begin(), ::tolower);
            if (itemCategory == query) {
                item->displayInfo();
                found = true;
            }
        }
        if (!found) {
            cout << "| Rezultatu nerasta." << endl;
        }
        printLine();
    }

    void registerUser() {
        string name, password;
        cout << "Iveskite varda: ";
        cin >> name;
        cout << "Iveskite slaptazodi: ";
        cin >> password;
        users.push_back(new User(name, password));
        saveUsersToFile(); // Isaugome vartotojus i faila
        cout << "Vartotojas sekmingai uzregistruotas!" << endl;
    }

    void loginUser() {
        string name, password;
        cout << "Iveskite varda: ";
        cin >> name;
        cout << "Iveskite slaptazodi: ";
        cin >> password;

        for (auto& user : users) {
            if (user->getName() == name && user->getPassword() == password) {
                loggedInUser = user;
                cout << "Sveiki sugrize, " << name << "!" << endl;
                int action;
                do {
                    printTitle("Prisijungusio Vartotojo Meniu");
                    cout << "1. Perziureti visas knygas\n";
                    cout << "2. Perziureti laisvas knygas\n";
                    cout << "3. Filtruoti pagal zanra\n";
                    cout << "4. Rezervuoti knyga\n";
                    cout << "5. Perziureti rezervacijas\n";
                    cout << "6. Redaguoti rezervacija\n";
                    cout << "7. Atsijungti\n";
                    printLine();
                    cout << "Pasirinkimas: ";
                    cin >> action;

                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Klaida: pasirinkimas turi buti skaicius nuo 1 iki 7." << endl;
                        continue;
                    }

                    switch (action) {
                        case 1: displayItems(); break;
                        case 2: displayAvailableItems(); break;
                        case 3: filterByCategory(); break;
                        case 4: makeReservation(); break;
                        case 5: displayReservations(); break;
                        case 6: editReservation(); break;
                        case 7:
                            cout << "Atsijungiate nuo paskyros." << endl;
                            loggedInUser = nullptr; // Išvalome prisijungimo duomenis
                            return; // Grįžtame į pagrindinį meniu
                        default:
                            cout << "Klaida: pasirinkimas turi buti nuo 1 iki 7." << endl;
                    }
                } while (action != 7);

                return;
            }
        }

        cout << "Prisijungimo klaida: neteisingas vardas arba slaptazodis." << endl;
    }

    void makeReservation() {
        if (!loggedInUser) {
            printTitle("Rezervacija Negalima");
            cout << "| Norint rezervuoti knyga, turite buti prisijunge.            |" << endl;
            cout << "| Jeigu paskyros neturite, rekomenduojame sia susikurti.          |" << endl;
            printLine(60);
            return;
        }

        displayAvailableItems();

        int itemID;
        cout << "Pasirinkite knygos ID: ";
        cin >> itemID;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Klaida: neteisingas pasirinkimas." << endl;
            return;
        }

        LibraryItem* selectedItem = nullptr;

        // Ieškome elemento pagal ID
        for (auto& item : items) {
            if (item->getID() == itemID) {
                selectedItem = item;
                break;
            }
        }

        if (!selectedItem) {
            cout << "Klaida: pasirinkta knyga nerasta." << endl;
            return;
        }

        if (!selectedItem->checkAvailability()) {
            cout << "Klaida: pasirinkta knyga jau rezervuota. Pasirinkite kita." << endl;
            return;
        }

        // Sukuriama nauja rezervacija
        Reservation* newReservation = new Reservation(loggedInUser, selectedItem);

        selectedItem->borrowItem();
        reservations.push_back(newReservation);
        saveReservationsToFile(); // Išsaugo rezervacijas faile
        cout << "Rezervacija sekminga!" << endl;
    }

    void editReservation() {
        if (!loggedInUser) {
            printTitle("Rezervaciju Redagavimas");
            cout << "| Norint redaguoti rezervacijas, turite buti prisijunge.          |" << endl;
            printLine(60);
            return;
        }

        // Atvaizduojame visas vartotojo rezervacijas
        printTitle("Jusu Rezervacijos");
        vector<Reservation*> userReservations;
        for (size_t i = 0; i < reservations.size(); ++i) {
            if (reservations[i]->getUser() == loggedInUser) {
                userReservations.push_back(reservations[i]);
                cout << "| " << userReservations.size() << ". " << setw(40) << left << reservations[i]->getItem()->getTitle() << "|" << endl;
            }
        }
        printLine(60);

        if (userReservations.empty()) {
            cout << "| Neturite aktyviu rezervaciju.                                    |" << endl;
            printLine(60);
            return;
        }

        int choice;
        cout << "| Pasirinkite rezervacijos numeri arba 0, kad griztumete atgal:   |" << endl;
        printLine(60);
        cout << "Pasirinkimas: ";
        cin >> choice;

        if (choice == 0) return;

        if (choice < 1 || choice > userReservations.size()) {
            cout << "| Netinkamas pasirinkimas.                                         |" << endl;
            printLine(60);
            return;
        }

        Reservation* selectedReservation = userReservations[choice - 1];

        // Rezervacijos redagavimo meniu
        printTitle("Rezervacijos Redagavimas");
        cout << "| 1. Atsaukti rezervacija                                          |" << endl;
        cout << "| 2. Grizti atgal                                                  |" << endl;
        printLine(60);
        cout << "Pasirinkimas: ";
        int action;
        cin >> action;

        switch (action) {
            case 1:
                selectedReservation->cancelReservation();
                reservations.erase(remove(reservations.begin(), reservations.end(), selectedReservation), reservations.end());
                delete selectedReservation;
                saveReservationsToFile(); // Išsaugome atnaujintas rezervacijas faile
                cout << "| Rezervacija sekmingai atsaukta.                                 |" << endl;
                break;
            case 2:
                cout << "| Griztama atgal.                                                  |" << endl;
                break;
            default:
                cout << "| Netinkamas pasirinkimas.                                         |" << endl;
        }
        printLine(60);
    }

    void displayReservations() const {
        if (!loggedInUser) {
            cout << "Norint perziureti rezervacijas, turite buti prisijunge." << endl;
            return;
        }

        printTitle("Jusu Rezervacijos");
        for (const auto& res : reservations) {
            if (res->getUser() == loggedInUser) {
                res->displayReservationInfo();
            }
        }
    }
};

int main() {
    Library library;
    library.loadUsersFromFile();
    library.loadReservationsFromFile();
    int choice;

    do {
        printTitle("Prisijungimo Sistema");
        cout << "1. Prisijungti\n";
        cout << "2. Registruotis\n";
        cout << "3. Naudotis be paskyros\n";
        cout << "4. Iseiti\n";
        printLine();
        cout << "Pasirinkimas: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Klaida: pasirinkimas turi buti skaicius nuo 1 iki 4." << endl;
            continue;
        }

        switch (choice) {
            case 1: library.loginUser(); break;
            case 2: library.registerUser(); break;
            case 3: {
                int action;
                do {
                    printTitle("Bibliotekos Sistema");
                    cout << "1. Perziureti visas knygas\n";
                    cout << "2. Perziureti laisvas knygas\n";
                    cout << "3. Filtruoti pagal zanra\n";
                    cout << "4. Rezervuoti knyga\n";
                    cout << "5. Grizti i pagrindini meniu\n";
                    printLine();
                    cout << "Pasirinkimas: ";
                    cin >> action;

                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Klaida: pasirinkimas turi buti skaicius nuo 1 iki 5." << endl;
                        continue;
                    }

                    switch (action) {
                        case 1: library.displayItems(); break;
                        case 2: library.displayAvailableItems(); break;
                        case 3: library.filterByCategory(); break;
                        case 4: library.makeReservation(); break;
                        case 5: cout << "Griztate i pagrindini meniu." << endl; break;
                        default: cout << "Klaida: pasirinkimas turi buti nuo 1 iki 5." << endl;
                    }
                } while (action != 5);
                break;
            }
            case 4: cout << "Aciu, kad naudojotes musu sistema!" << endl; break;
            default: cout << "Klaida: pasirinkimas turi buti nuo 1 iki 4." << endl;
        }
    } while (choice != 4);

    library.saveUsersToFile();
    library.saveReservationsToFile();
    return 0;
}
