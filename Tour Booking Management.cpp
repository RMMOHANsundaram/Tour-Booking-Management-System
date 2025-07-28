
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <stdexcept>
using namespace std;

template <typename T>
T getInput(const string &prompt) {
    T val;
    cout << prompt;
    cin >> val;
    if (cin.fail()) throw runtime_error(" Invalid input format.");
    return val;
}

bool signUp() {
    string username, password;
    cout << "\n                                ----- SIGN UP -----\n";
    cout << "                                Enter new username: ";
    cin >> username;
    cout << "                                Enter new password: ";
    cin >> password;

    ofstream out("users.txt", ios::app);
    if (!out) {
        cerr << "                                Could not open users.txt for writing.\n";
        return false;
    }

    out << username << " " << password << endl;
    out.close();
    cout << "                                Registration successful! You can now sign in.\n";
    return true;
}

bool signIn() {
    string username, password, fileUser, filePass;
    cout << "\n                                ----- SIGN IN -----\n";
    cout << "                                Enter username: ";
    cin >> username;
    cout << "                                Enter password: ";
    cin >> password;

    ifstream in("users.txt");
    if (!in) {
        cerr << "                                Could not open users.txt for reading.\n";
        return false;
    }

    while (in >> fileUser >> filePass) {
        if (fileUser == username && filePass == password) {
            cout << "                                Login successful!\n";
            return true;
        }
    }

    cout << "                                Invalid username or password.\n";
    return false;
}

class Print {
public:
    virtual void getDetails() = 0;
    virtual void printDetails(ofstream &out) const = 0;
    virtual ~Print() {}
};

class Customer : public Print {
    string name, gender, phone;
    int age, id;
public:
    Customer() : id(rand() % 901 + 99) {}
    void getDetails() override {
        cout << "                                Enter customer name: "; cin.ignore(); getline(cin, name);
        age = getInput<int>("                                Enter age: ");
        cin.ignore(); cout << "                                Enter gender: "; getline(cin, gender);
        cout << "                                Enter phone number: "; getline(cin, phone); cout << endl;
    }
    void printDetails(ofstream &out) const override {
        out << "                                Customer ID: " << id << endl
            << "                                Name: " << name << endl
            << "                                Age: " << age << endl
            << "                                Gender: " << gender << endl
            << "                                Phone Number: " << phone << endl;
    }
};

class Tour {
protected:
    int tourID, seats;
    string details;
public:
    bool getTour() {
        tourID = getInput<int>("                                Enter Tour ID (101, 102, 103...): ");
        ifstream in("tour" + to_string(tourID) + ".txt");
        if (!in) throw runtime_error(" Tour file not found.");
        details.clear(); string line;
        while (getline(in, line)) details += line + "\n";
        size_t pos = details.find("Available Seats:");
        if (pos != string::npos) seats = stoi(details.substr(pos + 16));
        cout << "\n                                ----- Tour " << tourID << " Details -----\n" << details;
        return true;
    }
    void printDetails(ofstream &out) const {
        out << "\n                                Tour Details:\n" << details;
    }
    int getSeats() const { return seats; }
};

class Package : public Tour {
    int type;
public:
    void choosePackage() {
        type = getInput<int>("                                Enter 1 for International or 2 for National Package: ");
        string file = (type == 1) ? "international.txt" : (type == 2) ? "national.txt" : "";
        if (file.empty()) throw invalid_argument(" Invalid package type.");
        ifstream in(file);
        if (!in) throw runtime_error(" Package file not found.");
        cout << "\n                                ----- Package Details -----\n";
        for (string line; getline(in, line);) cout << line << endl;
    }
    void printPackageDetails(ofstream &out) const {
        out << "\n                                Package Type: " << (type == 1 ? "International" : "National") << endl;
        printDetails(out);
    }
};

class Booking {
    int booked;
    float price, total;
    string bookingDateTime;
public:
    bool getBookingDetails() {
        int avail = getInput<int>("                                Can you please check no of available seats by entering it: ");
        booked = getInput<int>("                                Enter number of seats to book: ");
        if (booked > avail) throw out_of_range(" Not enough seats available.");
        price = getInput<float>("                                Can you please confirm the price per head by entering it: $");
        total = price * booked;
        cout << "                                Total price(incl GST) : $" << total << endl;
        time_t now = time(0); char* dt = ctime(&now); bookingDateTime = dt;
        return true;
    }
    void printBooking(ofstream &out) const {
        out << "\n                                Booking Date and Time: " << bookingDateTime
            << "                                Seats Booked: " << booked << endl
            << "                                Total Price(incl GST) : $" << total << endl;
    }
    int getSeatsBooked() const { return booked; }
    float getTotalPrice() const { return total; }
};

class Payment {
    float total, paid;
    int mode;
    string details;
public:
    void setTotal(float t) { total = t; }

    bool makePayment() {
        cout << "\n                                Choose Payment Method:\n";
        cout << "                                1. Card\n";
        cout << "                                2. UPI\n";
        cout << "                                3. Net Banking\n";
        mode = getInput<int>("                                Enter option (1-3): ");

        switch (mode) {
            case 1: {
                string cardNo, expiry, cvv;
                cout << "                                Enter Card Number: "; cin >> cardNo;
                cout << "                                Enter Expiry Date (MM/YY): "; cin >> expiry;
                cout << "                                Enter CVV: "; cin >> cvv;
                details = "Card ending with " + cardNo.substr(cardNo.length() - 4);
                break;
            }
            case 2: {
                string upi;
                cout << "                                Enter UPI ID: "; cin >> upi;
                details = "Paid via UPI ID: " + upi;
                break;
            }
            case 3: {
                string bank;
                cout << "                                Enter Bank Name: "; cin.ignore(); getline(cin, bank);
                details = "Paid via Net Banking (" + bank + ")";
                break;
            }
            default:
                throw invalid_argument(" Invalid payment method.");
        }

        paid = getInput<float>("                                Enter amount to pay: $");
        if (paid < total) throw runtime_error(" Insufficient funds.");
        cout << "                                Payment successful! Change: $" << fixed << setprecision(2) << paid - total << endl;
        return true;
    }

    void printPaymentDetails(ofstream &out) const {
        out << "\n                                Payment Method: ";
        if (mode == 1) out << "Card\n";
        else if (mode == 2) out << "UPI\n";
        else out << "Net Banking\n";

        out << "                                " << details << endl;
        out << "                                Amount Paid: $" << paid << endl;
        out << "                                Change Returned: $" << fixed << setprecision(2) << paid - total << endl;
    }
};

class TourOperator {
public:
    void manageBooking() {
        try {
            char c;
            cout << "                                Do you want to book a tour? (Y/N): ";
            cin >> c; if (toupper(c) != 'Y') return;

            Package *p = new Package;
            Booking b; Payment pay;

            p->choosePackage();
            p->getTour();
            b.getBookingDetails();

            int seats = b.getSeatsBooked();
            vector<Customer*> customers(seats);
            for (int i = 0; i < seats; ++i) {
                cout << "\n                                Enter details for passenger " << i + 1 << ":\n";
                customers[i] = new Customer;
                customers[i]->getDetails();
            }

            cout << "                                Tickets once purchased cannot be non-refundable/non-cancellable...\n";
            cout<<endl;
            cout << "                                By paying and booking you are agreeing to our terms and services...\n";
            cout<<endl;
            cout<<"                                                    (for terms & conditions visit www.tourmgntt&s.com)"<<endl;
            cout<<endl;
            cout << "                                The above mentioned prices are inclusive of foods and accommodation...\n\n";

            pay.setTotal(b.getTotalPrice());
            pay.makePayment();

            ofstream out("yourtour.txt");
            if (!out) throw runtime_error(" Error opening yourtour.txt file.");

            out << "                                              ----- Tour Booking Summary -----\n\n";
            out << "                                Booking ID: " << rand() % 901 + 99 << endl;

            for (int i = 0; i < seats; ++i) {
                out << "\n                                Passenger " << i + 1 << " Details:\n";
                customers[i]->printDetails(out);
                delete customers[i];
            }

            b.printBooking(out);
            p->printPackageDetails(out);
            pay.printPaymentDetails(out);

            out << "\n                                Tickets once purchased cannot be non-refundable/non-cancellable...\n";
            out << "                                By paying and booking you are agreeing to our terms and services...\n";
            out << "                                                    (for terms & conditions visit www.tourmgntt&s.com)\n";
            out << "                                The above mentioned prices are inclusive of foods and accommodation...\n\n";
            out << "                                                    ----- FOR ANY QUERIES CONTACT +91 6374668011 -----\n";
            out << "                                                    ----- THANK YOU FOR BOOKING WITH US. BON VOYAGE -----\n";

            delete p;
            cout << "\n                                Booking successful. You'll receive your invoice at 'yourtour.txt'.\n";
        } catch (const exception &e) {
            cerr << "\n                                Error:" << e.what() << endl;
        }
    }
};

int main() {
    cout << "                                ----- TOUR BOOKING MANAGEMENT SYSTEM -----\n\n";
    srand(static_cast<unsigned>(time(0)));

    int choice;
    do {
        cout << "\n                                1. Sign In\n";
        cout << "                                2. Sign Up\n";
        cout << "                                3. Exit\n";
        cout << "                                Enter choice: ";
        cin >> choice;

        if (choice == 1 && signIn()) {
            TourOperator().manageBooking();
        } else if (choice == 2) {
            signUp();
        } else if (choice == 3) {
            cout << "                                Exiting the program.\n";
            break;
        } else if (choice == 1) {
            cout << "                                Sign-In failed. Try again.\n";
        }
    } while (choice != 3);

    cout << "\n                                ----- THANK YOU FOR BOOKING WITH US. BON VOYAGE -----\n\n";
    return 0;
}

