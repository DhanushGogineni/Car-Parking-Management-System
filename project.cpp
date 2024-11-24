#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
using namespace std;

class User {
public:
    string username;
    string password;

    void registerUser() {
        cout << "\nEnter Username: ";
        cin >> username;
        cout << "Enter Password: ";
        cin >> password;
    }

    bool login(const string& user, const string& pass) {
        return (username == user && password == pass);
    }
};

class Admin {
public:
    void viewUsers();
    void viewCars();
    void searchCar();
    void sortCars();
};

void Admin::viewUsers() {
    ifstream userFile("users.txt");
    if (!userFile.is_open()) {
        cout << "Error opening user file!" << endl;
        return;
    }

    string line;
    cout << "\nRegistered Users:\n";
    cout << "------------------------\n";
    while (getline(userFile, line)) {
        cout << line << endl;
    }
    userFile.close();
}

void Admin::viewCars() {
    ifstream in("carparkdb.txt");
    if (!in.is_open()) {
        cout << "Error opening file!" << endl;
        return;
    }

    string line;
    cout << "\nRegistered Cars:\n";
    cout << "------------------------\n";
    while (getline(in, line)) {
        cout << line << endl;
    }
    in.close();
}

void Admin::searchCar() {
    string searchCarID;
    cout << "Enter Car ID to search: ";
    cin >> searchCarID;

    ifstream in("carparkdb.txt");
    if (!in.is_open()) {
        cout << "Error opening file!" << endl;
        return;
    }

    string line;
    bool found = false;
    while (getline(in, line)) {
        if (line.find(searchCarID) != string::npos) {
            cout << "Car found: " << line << endl;
            found = true;
            break;
        }
    }
    in.close();

    if (!found) {
        cout << "Car ID not found!" << endl;
    }
}

struct Car {
    string drivername;
    string carname;
    string carid;
    int timestay;
    double fare;

    // Method to parse a line and initialize a Car object
    static Car fromLine(const string& line) {
        Car car;
        size_t pos = 0;
        size_t nextPos;

        // Parse drivername
        pos = line.find("Driver Name: ") + 13;
        nextPos = line.find(", Car Name:");
        car.drivername = line.substr(pos, nextPos - pos);

        // Parse carname
        pos = nextPos + 12;
        nextPos = line.find(", Car ID:");
        car.carname = line.substr(pos, nextPos - pos);

        // Parse carid
        pos = nextPos + 10;
        nextPos = line.find(", Time Stay:");
        car.carid = line.substr(pos, nextPos - pos);

        // Parse timestay
        pos = nextPos + 12;
        nextPos = line.find(" hours", pos);
        car.timestay = stoi(line.substr(pos, nextPos - pos));

        return car;
    }
};

// Merge Sort Functions
void merge(vector<Car>& cars, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<Car> L(n1);
    vector<Car> R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = cars[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = cars[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i].timestay <= R[j].timestay) {
            cars[k] = L[i];
            i++;
        } else {
            cars[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        cars[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        cars[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(vector<Car>& cars, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(cars, left, mid);
        mergeSort(cars, mid + 1, right);
        merge(cars, left, mid, right);
    }
}

void Admin::sortCars() {
    ifstream in("carparkdb.txt");
    if (!in.is_open()) {
        cout << "Error opening file!" << endl;
        return;
    }

    vector<Car> cars;
    string line;

    while (getline(in, line)) {
        cars.push_back(Car::fromLine(line));
    }
    in.close();

    mergeSort(cars, 0, cars.size() - 1);

    cout << "\nSorted Cars by Time Stayed:\n";
    cout << "------------------------\n";
    for (const auto& car : cars) {
        cout << "Driver Name: " << car.drivername
             << "\n"
             << ", Car Name: " << car.carname
             << "\n"
             << ", Car ID: " << car.carid
             << "\n"
             << ", Time Stay: " << car.timestay << " hours"
             << "\n";

    }
}

class ParkingManagement {
public:
    void park();
    void removeCar();
    double calculateFare(int hours);
};

double ParkingManagement::calculateFare(int hours) {
    const double ratePerHour = 10.0;
    return hours * ratePerHour;
}

void ParkingManagement::park() {
    cout << "\n-------------> Car Parking System <-------------" << endl;

    ofstream out("carparkdb.txt", ios::app);
    Car car;

    cout << "Enter Your Name: ";
    cin.ignore();
    getline(cin, car.drivername);

    cout << "Enter Your Car Name: ";
    getline(cin, car.carname);

    cout << "Enter Your Car Number Plate: ";
    cin >> car.carid;

    cout << "Enter Your Time Stay In Hours: ";
    cin >> car.timestay;

    car.fare = calculateFare(car.timestay);

    out << "Driver Name: " << car.drivername
        << ", Car Name: " << car.carname
        << ", Car ID: " << car.carid
        << ", Time Stay: " << car.timestay << " hours"
        << ", Fare: $" << car.fare << endl;

    out.close();
    cout << "\nYour Car Is Parked Now!" << endl;
    cout << "Total Fare: $" << car.fare << endl;
    system("PAUSE");
}

void ParkingManagement::removeCar() {
    system("CLS");
    string rcarid;
    cout << "Enter Your Car ID: ";
    cin >> rcarid;

    ifstream in("carparkdb.txt");
    ofstream out("temp.txt");
    string line;
    bool found = false;

    while (getline(in, line)) {
        if (line.find(rcarid) != string::npos) {
            found = true;
        } else {
            out << line << endl;
        }
    }
    in.close();
    out.close();

    remove("carparkdb.txt");
    rename("temp.txt", "carparkdb.txt");

    if (found) {
        cout << "\nCar removed successfully!" << endl;
    } else {
        cout << "\nCar ID not found!" << endl;
    }
    system("PAUSE");
}

class System {
private:
    User user;
    Admin admin;
    ParkingManagement parkObj;

public:
    void run();
    void userRegistration();
    void userLogin();
};

void System::userRegistration() {
    user.registerUser();
    ofstream userFile("users.txt", ios::app);
    userFile << user.username << "," << user.password << endl;
    userFile.close();
}

void System::userLogin() {
    string userName, password;
    cout << "Enter Username: ";
    cin >> userName;
    cout << "Enter Password: ";
    cin >> password;

    ifstream userFile("users.txt");
    string line;
    bool loggedIn = false;

    while (getline(userFile, line)) {
        if (line == userName + "," + password) {
            loggedIn = true;
            break;
        }
    }
    userFile.close();

    if (loggedIn) {
        cout << "\nLogin successful!" << endl;

        int choice;
        while (true) {
            cout << "\n1. Park Car\n";
            cout << "2. Remove Car\n";
            cout << "3. Logout\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    parkObj.park();
                    break;
                case 2:
                    parkObj.removeCar();
                    break;
                case 3:
                    return;
                default:
                    cout << "\nInvalid choice!" << endl;
            }
        }
    } else {
        cout << "\nInvalid username or password!" << endl;
        system("PAUSE");
    }
}

void System::run() {
    while (true) {
        system("CLS");
        int choice;
        cout << "\nWelcome to Car Parking Management System\n";
        cout << "==============================\n";
        cout << "1. Create Account\n";
        cout << "2. Login\n";
        cout << "3. Admin Panel\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                userRegistration();
                break;
            case 2:
                userLogin();
                break;
            case 3:
                {
                    string adminPassword;
                    cout << "Enter Admin Password: ";
                    cin >> adminPassword;
                    if (adminPassword == "cat") {
                        int adminChoice;
                        while (true) {
                            cout << "\nAdmin Panel:\n";
                            cout << "1. View Users\n";
                            cout << "2. View Cars\n";
                            cout << "3. Search Car\n";
                            cout << "4. Sort Cars by Time Stayed\n";
                            cout << "5. Logout\n";
                            cout << "Enter your choice: ";
                            cin >> adminChoice;

                            switch (adminChoice) {
                                case 1:
                                    admin.viewUsers();
                                    break;
                                case 2:
                                    admin.viewCars();
                                    break;
                                case 3:
                                    admin.searchCar();
                                    break;
                                case 4:
                                    admin.sortCars();
                                    break;
                                case 5:
                                    goto exitAdminPanel;
                                default:
                                    cout << "Invalid choice!" << endl;
                            }
                        }
                    exitAdminPanel:
                        continue;
                    } else {
                        cout << "Incorrect Password!" << endl;
                    }
                    system("PAUSE");
                }
                break;
            case 4:
                cout << "Thank you for using the Car Parking Management System!" << endl;
                exit(0);
            default:
                cout << "Invalid choice!" << endl;
        }
    }
}

int main() {
    System parkingSystem;
    parkingSystem.run();
    return 0;
}