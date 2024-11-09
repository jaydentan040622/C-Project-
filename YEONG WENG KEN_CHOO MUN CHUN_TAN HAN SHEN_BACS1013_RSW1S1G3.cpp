#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <algorithm> // Include the algorithm header for sorting
#include <vector> // Include the vector header for using vectors
#include <cctype> // Include the cctype header for toupper()
#include <limits>
#include <map>
using namespace std;

// Function prototypes
void logo();
void First_page();
void login();
void registration();
void forgot();
void customer_registration();
void home_page();
string generateMembershipNumber();
void menu();
void topUpMembership();
void updateCustomerRecord(const string& membershipNumber, double topUpAmount);
void updateCustomerRecord(const string& membershipNumber, double balance_payment, int points);
bool checkCustomerExists(const string& membershipNumber); // Function to check if a customer exists
void Food_Menu();
void process_Order();
void order_Item();
void modify_Order();
void increase_Quantity(int orderNumber);
void decrease_Quantity(int orderNumber);
void remove_Order(int orderNumber);
int calculate_MembershipPoints(double orderTotal);
void payment(double orderTotal);
void payment_receipt(string membership_card_num, double orderTotal, double balance_payment, double redeemAmount, bool pointsRedeemed);
string get_membership_card();
bool validate_membership_card(string membership_card_num);
double check_balance(string membership_card_num, double orderTotal);
void cancel_payment();
void checkBalance_Points();
bool redeemPoints(string membership_card_num, double& balance_payment, int& points, double orderTotal, double& redeemAmount);
void exit();

//global variable
const string RecordsFileName = "customer_records.txt";

// Constants representing the number of menu items and their respective prices.
const int NUM_ITEMS = 5;
const double PRICES_A[NUM_ITEMS] = { 6.60, 5.50, 8.50, 9.50, 7.50 };
const double PRICES_B[NUM_ITEMS] = { 15.60, 16.60, 20.60, 19.60, 15.60 };
const string ITEM_NAMES[NUM_ITEMS] = { "Spicy Chicken Taco", "Chicken Taco", "Crunchwrap Supreme", "Taco Supreme", "Grilled Stuft Burrito" };

// Constants for dine-in service charge rate and takeaway packaging fee.
const double DINE_IN_SERVICE_CHARGE_RATE = 0.10;
const double TAKEAWAY_PACKAGING_FEE = 2.00;


// ANSI escape codes for color
const string blueColor = "\033[1;34m";
const string resetColor = "\033[0m";
const string ANSI_BOLD_CYAN = "\033[1;36m";
const string ANSI_RESET = "\033[0m";
#define BOLD "\033[1m"
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"


struct Customer {
    string name;
    string contactNumber;
    string membershipCardNumber;
    double topUpAmount;
    double balance;
    int membershipPoints;
};

const int MAX_CUSTOMERS = 100; // Maximum number of customers, adjust as needed
Customer customers[MAX_CUSTOMERS]; // Array to store customer data
int numCustomers = 0; // Current number of customers

// Structure for holding individual order details.
struct OrderItem {
    int itemIndex;// Index of the item in the menu
    char choice; // Either 'A' or 'B' indicating price choices
    int quantity; // Number of items ordered
};

vector<OrderItem> currentOrder; // A list to store all items in the current order.

// Function to read customer records from the file
vector<Customer> readCustomerRecords(const string& fileName) {
    vector<Customer> customers;
    ifstream inputFile(fileName);

    if (!inputFile.is_open()) {
        cout << "Error opening customer records file!" << endl;
        return customers; // Return an empty vector in case of an error
    }

    Customer customer;
    string line;

    while (getline(inputFile, line)) {
        if (line.find("Name: ") != string::npos) {
            customer.name = line.substr(line.find(":") + 2); // Extract the name

            getline(inputFile, line); // Read the next line (Contact Number)
            customer.contactNumber = line.substr(line.find(":") + 2); // Extract the contact number

            getline(inputFile, line); // Read the next line (Membership Card Number)
            customer.membershipCardNumber = line.substr(line.find(":") + 2); // Extract the membership card number

            getline(inputFile, line); // Read the next line (Balance)
            customer.topUpAmount = stod(line.substr(line.find(":") + 2)); // Extract and convert the balance to double

            getline(inputFile, line); // Read the next line (Membership Points)
            customer.membershipPoints = stoi(line.substr(line.find(":") + 2)); // Extract and convert the points to int

            customers.push_back(customer); // Add the customer to the vector
        }
    }

    inputFile.close();
    return customers;
}

// Function to write customer records back to the file (after struct)
void writeCustomerRecords(const string& fileName, const vector<Customer>& customers) {
    ofstream outputFile(fileName);

    if (!outputFile.is_open()) {
        cout << "Error opening customer records file for writing!" << endl;
        return;
    }

    for (const Customer& customer : customers) {
        outputFile << "Name: " << customer.name << endl;
        outputFile << "Contact Number: " << customer.contactNumber << endl;
        outputFile << "Membership Card Number: " << customer.membershipCardNumber << endl;
        outputFile << "Balance: " << customer.topUpAmount << endl;
        outputFile << "Membership Points: " << customer.membershipPoints << endl;
        outputFile << endl;
    }

    outputFile.close();
}

// Custom comparison function for sorting customers by name
bool compareCustomersByName(const Customer& a, const Customer& b) {
    return a.name < b.name;
}


int main() {
    // Initialize customer data and load records
    vector<Customer> customers = readCustomerRecords(RecordsFileName);

    First_page();
    home_page();

    return 0;
}


void logo() {

    cout << blueColor << setfill('*') << setw(155) << " " << endl << endl;
    cout << blueColor << "\t\t ___ ___  _______   ___ ___   ___  _______  _______  ______     _______   ___  _______  _______ " << endl;
    cout << blueColor << "\t\t|   Y   ||   _   | (   Y   ) |   ||   _   ||   _   ||   _  \\   |   _   \\ |   ||       ||   _   |" << endl;
    cout << blueColor << "\t\t|.      ||.  1___|  \\  1  /  |.  ||.  1___||.  1   ||.  |   |  |.  1   / |.  ||.|   | ||.  1___|" << endl;
    cout << blueColor << "\t\t|. \\_/  ||.  __)_   /  _  \\  |.  ||.  |___ |.  _   ||.  |   |  |.  _   \\ |.   |-|.  |-'.  __)_ " << endl;
    cout << blueColor << "\t\t|:  |   ||:  1   | /:  |   \\ |:  ||:  1   ||:  |   ||:  |   |  |:  1    \\|:  |  |:  |  |:  1   |" << endl;
    cout << blueColor << "\t\t|::.|:. ||::.. . |(::. |:.  )|::.||::.. . ||::.|:. ||::.|   |  |::.. .  /|::.|  |::.|  |::.. . |" << endl;
    cout << blueColor << "\t\t`--- ---'`-------' `--- ---' `---'`-------'`--- ---'`--- ---'  `-------' `---'  `---'  `-------'" << endl << endl;
    cout << blueColor << setfill('*') << setw(155) << " " << resetColor << endl << endl;

}

void First_page() {
    system("cls");
    cout << blueColor << "\t\t\t\t\t\tFOOD ORDERING SYSTEM (TACO)\n" << " " << resetColor << endl;
    logo();

    cout << " ";
    cout << BOLD << CYAN << "        __    __    ___  _         __   ___   ___ ___    ___      ______   ___        ___   __ __  ____       ____    ____   ____    ___ " << RESET << endl;
    cout << BOLD << RED << "\t|  |__|  |  /  _]| |       /  ] /   \\ |   |   |  /  _]    |      | /   \\      /   \\ |  |  ||    \\     |    \\  /    | /    |  /  _]" << RESET << endl;
    cout << BOLD << YELLOW << "\t|  |  |  | /  [_ | |      /  / |     || _   _ | /  [_     |      ||     |    |     ||  |  ||  D  )    |  o  )|  o  ||   __| /  [_ " << RESET << endl;
    cout << BOLD << YELLOW << "\t|  |  |  ||    _]| |___  /  /  |  O  ||  \\_/  ||    _]    |_|  |_||  O  |    |  O  ||  |  ||    /     |   _/ |     ||  |  ||    _]" << RESET << endl;
    cout << BOLD << GREEN << "\t|  `  '  ||   [_ |     |/   \\_ |     ||   |   ||   [_       |  |  |     |    |     ||  :  ||    \\     |  |   |  _  ||  |_ ||   [_ " << RESET << endl;
    cout << BOLD << GREEN << "\t \\      / |     ||     |\\     ||     ||   |   ||     |      |  |  |     |    |     ||     ||  .  \\    |  |   |  |  ||     ||     |" << RESET << endl;
    cout << BOLD << RED << "\t  \\_/\\_/  |_____||_____|\\____/  \\___/ |___|___||_____|      |__|   \\___/      \\___/  \\__,_||__|\\_|    |__|   |__|__||___,_||_____|" << RESET << endl;

    cout << endl;



    cout << "\n\nPress ENTER to continue...";
    cin.get();
    system("cls");
}

void home_page() {
    system("cls");
    string option;
    logo();

    // Display the menu
    cout << "\033[1m\t\t+=====+=================+==+=======+\033[0m" << endl; // Bold
    cout << "\033[1m\t\t| \033[94mHOME PAGE\033[0m                    |  |\033[0m" << endl; // Blue text
    cout << "\033[1m\t\t+=====+=================+==+=======+\033[0m" << endl;
    cout << "\033[1m\t\t| \033[92m1.\033[0m  | \033[96mLOGIN\033[0m                  |  |\033[0m" << endl; // Green text
    cout << "\033[1m\t\t| \033[92m2.\033[0m  | \033[96mREGISTER\033[0m               |  |\033[0m" << endl; // Green text
    cout << "\033[1m\t\t| \033[92m3.\033[0m  | \033[96mFORGOT PASSWORD\033[0m        |  |\033[0m" << endl; // Green text
    cout << "\033[1m\t\t| \033[91m4.\033[0m  | \033[95mEXIT\033[0m                   |  |\033[0m" << endl; // Red text
    cout << "\033[1m\t\t+=====+=================+==+=======+\033[0m" << endl;

    do {
        cout << "\n\t\t\t Please enter your choice : ";
        cin >> option;
        cout << endl;

        // Check if input is a valid integer
        if (option == "1" || option == "2" || option == "3" || option == "4") {
            switch (stoi(option)) {
            case 1:
                login();
                break;
            case 2:
                registration();
                break;
            case 3:
                forgot();
                break;
            case 4:
                exit();
                break;
            }
        }
        else {
            cout << "\t\t\t Invalid Input! " << endl;
            cout << "\t\t\t Please select from the options given above \n" << endl;
        }

    } while (option != "4");
}

void login() {
    while (true) {
        int count = 0;
        string userID, spass, id, pass;
        system("cls");
        logo();
        cout << " _     ____  _____ _  _        ____  ____  _____ _____" << endl;
        cout << "/ \\   /  _ \\/  __// \\/ \\  /|  /  __\\/  _ \\/  __//  __/" << endl;
        cout << "| |   | / \\|| |  _| || |\\ ||  |  \\/|| / \\|| |  _|  \\" << endl;
        cout << "| |_\\/| \\_/|| |_//| || | \\||  |  __/| |-||| |_//|  /_" << endl;
        cout << "\\____/\\____/\\____\\_\\/_/  \\|  \\_/   \\_/ \\|\\____\\_____/" << endl;
        cout << setfill('-') << setw(60) << " " << endl << endl;
        cout << ANSI_BOLD_CYAN << "\t\t USERNAME : --> " << ANSI_RESET;
        cin >> userID;
        cout << ANSI_BOLD_CYAN << "\t\t PASSWORD : --> " << ANSI_RESET;
        cin >> pass;

        ifstream f1("records.txt");
        while (f1 >> id >> spass) {
            if (id == userID && spass == pass) {
                count = 1;
                system("cls");
            }
        }
        f1.close();
        if (count == 1) {
            cout << "Welcome, " << GREEN << BOLD << userID << RESET << CYAN << "!\nYour LOGIN was successful!\n" << RESET << endl;
            menu();
            break;
        }
        else {
            cout << "\nLOGIN ERROR: Please check your username and password.\n\n";
            int choice;

            while (true) {
                cout << "Press 1 to try again or 2 to go back to the main menu: ";
                cin >> choice;

                if (choice == 2) {
                    home_page();
                    break;
                }
                else if (choice == 1) {
                    // The loop will continue and prompt for login again
                    break;
                }
                else {
                    cout << "Invalid choice! Please try again." << endl;
                }
            }
        }

    }
}

void registration() {
    string ruserID, rpassword;
    system("cls");
    logo();

    // Username and password must be between 4-8 characters
    while (ruserID.length() < 4 || ruserID.length() > 8 || rpassword.length() < 4 || rpassword.length() > 8) {
        cout << " ____  _____ _____ _  ____  _____  _____ ____    ____  ____  _____ _____" << endl;
        cout << "/  __\\/  __//  __// \\/ ___\\/ __\\//  __//  __\\  /  __\\/  _ \\/  __//  __/" << endl;
        cout << "|  \\/||  \\  | |  _| ||    \\  / \\  |  \\\\|  \\/|  |  \\/|| / \\|| |  _|  \\" << endl;
        cout << "|    /|  /_ | |_//| |\\___ |  | |  |  /_ |    /  |  __/| |-||| |_//|  /_ " << endl;
        cout << "\\_/\\_\\\\____\\\\____\\\\_/\\____/  \\_/  \\____\\\\_/\\_\\  \\_/   \\_/ \\|\\____\\\\____\\" << endl;
        cout << setfill('-') << setw(60) << " " << endl << endl;
        cout << "\n\t\t Username and password must be between 4-8 characters \n";
        cout << "\t\tEnter the username : --> ";
        cin >> ruserID;
        cout << "\t\tEnter the password : --> ";
        cin >> rpassword;

        // Else, invalid input
        if (ruserID.length() < 4 || ruserID.length() > 8 || rpassword.length() < 4 || rpassword.length() > 8) {
            cout << "\n\t\t\t Invalid input! Please try again \n";
        }
    }

    ofstream f1("records.txt", ios::app);
    // Write username and password on the same line, separated by a space
    f1 << ruserID << " " << rpassword << endl;
    f1.close();

    system("cls");
    cout << "\n\t\t\t Registration is successful!\n";
    home_page();
}

void forgot() {
    int option;
    system("cls");
    logo();
    cout << "\t You forgot the password? No worries \n";
    cout << setfill('-') << setw(60) << " " << endl << endl;
    cout << "\tPress 1 to search your id by username " << endl;
    cout << "\tPress 2 to go back to the main menu" << endl;
    cout << "\t Enter your choice : --> ";
    cin >> option;

    cin.ignore();


    switch (option) {
    case 1: {
        int count = 0;
        int option;
        string suserID, sID, spass;
        cout << "\n\t\t Enter the username which you remembered : ";
        cin >> suserID;
        ifstream f2("records.txt");
        while (f2 >> sID >> spass) {
            if (sID == suserID) {
                count = 1;
                cout << "\n\n Your account is found! \n";
                cout << "\n Your password is : --> " << spass << endl;
                cout << "Press 1 to go back to the main menu: ";
                cin >> option;
                switch (option) {
                case 1:
                    home_page();
                    break;
                default:
                    cout << "Invalid choice! Please try again." << endl;
                }
            }
        }
        f2.close();
        if (count == 0) {
            cout << "\n\t Sorry! Your account is not found! \n";
        }
        break;
    }
    case 2: {
        home_page();
    }
    default: {
        cout << "\t\t\t Wrong choice! Please try again " << endl;
        forgot();
    }
    }
}

void menu() {
    string option;
    double orderTotal;

    logo();
    cout << setfill('*') << setw(30) << "\t Menu \t" << setfill('*') << setw(25) << " " << endl << endl;
    cout << "\t| Press 1 to Food Ordering Service    |" << endl;
    cout << "\t| Press 2 to Customers Registration   |" << endl;
    cout << "\t| Press 3 to Membership top up        |" << endl;
    cout << "\t| Press 4 to Check balance and point  |" << endl;
    cout << "\t| Press 5 to Log Out                  |" << endl;

    while (true) {
        cout << "\n\t\t\t Please enter your choice : ";
        cin >> option;
        cout << endl;

        if (option == "1") {
            process_Order();
            break; // Break the loop after executing the selected option
        }
        else if (option == "2") {
            customer_registration();
            break;
        }
        else if (option == "3") {
            topUpMembership();
            break;
        }
        else if (option == "4") {
            checkBalance_Points();
            break;
        }
        else if (option == "5") {
            home_page();
            break;
        }
        else {
            cout << "\t\t\t Invalid Input! " << endl;
            cout << "\t\t\t Please select from the options given above \n" << endl;
        }
    }
}

void customer_registration() {
    double balance = 0.0;
    Customer newCustomer;
    string membershipCardNumber;
    string contact;
    int count = 0;
    newCustomer.membershipCardNumber = generateMembershipNumber();

    // Initialize top-up value and membership points to zero
    newCustomer.topUpAmount = 0.0;
    newCustomer.balance = 0.0; // Initialize balance here
    newCustomer.membershipPoints = 0;

    // Input customer details
    system("cls");
    logo();
    while (true) {
        cout << "Please Enter Customer Details:" << endl << "---------------------------------" << endl;
        cout << "\t Name: --> ";
        cin.ignore();
        getline(cin, newCustomer.name);

        // Name cannot contain digits
        while (newCustomer.name.find_first_of("0123456789") != string::npos) {
            cout << "\033[91m" << "\t Invalid input! Name cannot contain digits. Please try again " << "\033[0m" << endl;
            cout << "\t Name: --> ";
            getline(cin, newCustomer.name);
        }

        // Name cannot contain special characters except / for Keshor a/l Suresh
        while (newCustomer.name.find_first_of("!@#$%^&*()_+{}|:<>?/.,';][=-`~") != string::npos) {
            cout << "\033[91m" << "\t Invalid input! The name includes special characters. Please try again! " << "\033[0m" << endl;
            cout << "\t Name: --> ";
            getline(cin, newCustomer.name);
        }

        cout << "\t Contact Number: --> ";
        cin >> contact;

        // Check if the contact number first character is '0'
        while (contact[0] != '0') {
            cout << "\033[91m" << "\t Invalid input! Contact number must start with '0'. Please try again " << "\033[0m" << endl;
            cout << "\t Contact Number: --> ";
            cin >> contact; // Use 'contact' for input
        }

        // Phone number must be 10-11 digits and contain only digits
        while (contact.length() < 10 || contact.length() > 11 || contact.find_first_not_of("0123456789") != string::npos) {
            cout << "\033[91m" << " \t Invalid input! Contact number must be 10-11 digits and contain only digits. Please try again " "\033[0m" << endl;
            cout << "\t Contact Number: --> ";
            cin >> contact; // Use 'contact' for input
        }

        ifstream f1("customer_records.txt");
        count = 0;
        while (f1 >> newCustomer.contactNumber) {
            if (contact == newCustomer.contactNumber) {
                count = 1;
                break;
            }
        }
        f1.close();
        if (count == 1) {
            cout << "\033[91m" << "\n \tThis contact number is already registered! Please try again" << "\033[0m";
            cout << "\n\t Contact Number: --> ";
            cin >> contact;
        }
        else {
            break; // Exit the loop when a unique contact number is provided
        }
    }

    cout << "\033[94m"; // Sky blue color (success)
    cout << "\n\t Registration is successful!\n\n";
    cout << "Generated Membership Card Number:" << newCustomer.membershipCardNumber << endl;
    cout << "\033[0m";  // Reset text color to default

    // Display the customer details
    cout << "\nPlease Confirm Customer Details:" << endl
        << "---------------------------------" << endl;
    cout << "Customer Details:" << endl
        << "---------------------------------" << endl;
    cout << "\t Name: " << newCustomer.name << endl;
    // Format the contact number before displaying it
    contact.insert(3, "-");
    cout << "\t Contact Number: " << contact << endl;
    // Remove the '-' from the contact number
    contact.erase(remove(contact.begin(), contact.end(), '-'), contact.end());
    newCustomer.contactNumber = contact; // Assign 'contact' to newCustomer.contactNumber

    cout << "\t Membership Card Number: " << newCustomer.membershipCardNumber << endl;
    cout << "\t Balance: RM " << fixed << setprecision(2) << newCustomer.balance << endl;
    cout << "\t Membership Points: " << newCustomer.membershipPoints << endl;

    // Instead of opening the file here, add the new customer to the vector
    vector<Customer> customers = readCustomerRecords(RecordsFileName);
    customers.push_back(newCustomer);

    // Sort customers by name
    sort(customers.begin(), customers.end(), compareCustomersByName);

    // Write sorted customer records back to the file
    writeCustomerRecords(RecordsFileName, customers);

    // Add the new customer to the array
    if (numCustomers < MAX_CUSTOMERS) {
        customers[numCustomers++] = newCustomer;
    }
    else {
        cout << "Maximum number of customers reached!" << endl;
        return;
    }

    cout << "\033[94m" << "\nRegistration successful!" << "\033[0m" << endl;

    char topUpOption;

    do {
        cout << "Do you want to top up your membership balance? (y/n): ";
        cin >> topUpOption;
        topUpOption = toupper(topUpOption); // Convert the input to uppercase

        if (topUpOption == 'Y') {
            topUpMembership();  // Pass the balance variable
        }
        else if (topUpOption == 'N') {
            system("cls");
            cout << "\033[94m" << "Thank you for registering!" << "\033[0m" << endl;
            system("cls");
            menu();
        }
        else {
            cout << "\033[91m" << "Invalid option! Please try again." << "\033[0m" << endl;
        }
    } while (topUpOption != 'Y' && topUpOption != 'N');
}

string generateMembershipNumber() {
    srand(static_cast<unsigned>(time(nullptr)));
    int randomPart = rand() % 900000 + 100000;
    return "ABC" + to_string(randomPart);
}

void topUpMembership() {
    string membershipNumber, choice;
    double topUpAmount;
    int option;

    system("cls");
    logo();
    while (true) {
        cout << "Membership Card Top-Up" << endl;
        cout << "Enter Customer's Membership Card Number: ";
        cin >> membershipNumber;

        // Check if the customer exists in the records
        bool customerExists = checkCustomerExists(membershipNumber);

        if (!customerExists) {
            cout << "Customer not found! Please try again." << endl;
            cout << "Press 1 to try again or 2 to go back to the main menu: ";
            cin >> choice;
            if (choice == "1") {
                continue; // Continue the loop to prompt for the membership number again
            }
            else if (choice == "2") {
                system("cls");
                menu();
                return; // Exit the function
            }
            else {
                cout << "Invalid choice! Please try again." << endl;
            }
        }
        else {
            break; // Exit the loop when a valid customer is found
        }
    }

    // Use an array to store top-up options and their corresponding prices
    const int numOptions = 4;
    string topUpOptions[numOptions] = { "RM 5.00", "RM 10.00", "RM 30.00", "RM 50.00" };
    double topUpPrices[numOptions] = { 5.00, 10.00, 30.00, 50.00 };

    // Display top-up options
    cout << "Select Top-Up Amount:" << endl;
    for (int i = 0; i < numOptions; i++) {
        cout << i + 1 << ". " << topUpOptions[i] << endl;
    }

    cout << "Enter option: ";
    cin >> option;

    // Check if the option is valid
    if (option >= 1 && option <= numOptions) {
        topUpAmount = topUpPrices[option - 1]; // Get the corresponding top-up amount
    }
    else {
        cout << "Invalid option! Please select an option from the menu." << endl;
        return;
    }

    // Deduct the service charge
    topUpAmount -= 0.50;

    // Update the customer's balance and record
    updateCustomerRecord(membershipNumber, topUpAmount);
    cout << "\nTop-Up successful!" << endl;

    // Retrieve the balance from the file
    ifstream f1("customer_records.txt");
    string line;
    while (getline(f1, line)) {
        if (line.find("Membership Card Number: " + membershipNumber) != string::npos) {
            // Update the balance line
            getline(f1, line);
            double balance = stod(line.substr(line.find(":") + 1));
            cout << "Your current balance is: RM " << fixed << setprecision(2) << balance << endl;

            // Ask the user to top up again or go back to the main menu
            while (true) {
                cout << "Press Y to top up again or N to go back to the main menu: ";
                cin >> choice;

                // Convert the user's input to uppercase
                choice[0] = toupper(choice[0]);

                if (choice == "Y") {
                    break; // Exit the loop and allow the user to top up again
                }
                else if (choice == "N") {
                    system("cls");
                    menu();
                    return; // Exit the function
                }
                else {
                    cout << "Invalid choice! Please try again." << endl;
                    cout << "Press Y to top up again or N to go back to the main menu: ";
                    cin >> choice;
                }
            }
        }
    }
}


bool checkCustomerExists(const string& membershipCardNumber) {
    ifstream inputFile("customer_records.txt");
    if (!inputFile.is_open()) {
        cout << "Error opening customer records file!" << endl;
        return false;
    }

    string line;
    bool found = false;

    while (getline(inputFile, line)) {
        if (line.find("Membership Card Number: " + membershipCardNumber) != string::npos) {
            found = true;
            break;
        }
    }

    inputFile.close();
    return found;
}

void updateCustomerRecord(const string& membershipNumber, double topUpAmount) {
    ifstream inputFile("customer_records.txt");
    if (!inputFile.is_open()) {
        cout << "Error opening customer records file!" << endl;
        return;
    }

    ofstream tempFile("temp_records.txt");
    if (!tempFile.is_open()) {
        cout << "Error opening temporary file for writing!" << endl;
        inputFile.close();
        return;
    }

    string line;
    bool found = false;

    while (getline(inputFile, line)) {
        if (line.find("Membership Card Number: " + membershipNumber) != string::npos) {
            // Update the balance line
            getline(inputFile, line);
            double balance = stod(line.substr(line.find(":") + 1));
            balance += topUpAmount;
            tempFile << "Membership Card Number: " + membershipNumber << endl;
            tempFile << fixed << setprecision(2) << "Balance: " << balance << endl;
            found = true;
        }
        else {
            tempFile << line << endl;
        }
    }

    inputFile.close();
    tempFile.close();

    if (!found) {
        cout << "Customer not found!" << endl;
        remove("temp_records.txt"); // Remove the temporary file
        return;
    }

    // Replace the original file with the temporary file
    if (remove("customer_records.txt") != 0) {
        cout << "Error removing original file!" << endl;
        return;
    }

    if (rename("temp_records.txt", "customer_records.txt") != 0) {
        cout << "Error renaming temporary file!" << endl;
    }
}

// Displays the Taco menu.
void Food_Menu()
{
    system("cls");
    logo();
    cout << CYAN << "+==========================+============================+" << endl;
    cout << CYAN << "|                      Taco  Menu                       |" << endl;
    cout << CYAN << "+==========================+============================+" << endl;
    cout << CYAN << "| 1 . Spicy Chicken Taco   |      4.Taco supreme        |" << endl;
    cout << CYAN << "|                          |                            |" << endl;
    cout << CYAN << "| A[RM 6.60]   B[RM 15.60] | A[RM 9.50]   B[RM19.60]    |" << endl;
    cout << CYAN << "+--------------------------+----------------------------+" << endl;
    cout << CYAN << "|   2. Chicken Taco        |   5. Grilled Stuft Burrito |" << endl;
    cout << CYAN << "|                          |                            |" << endl;
    cout << CYAN << "| A [5.50]     B [16.60]   | A[RM 7.50]    B[RM15.60]   |" << endl;
    cout << CYAN << "+--------------------------+----------------------------+" << endl;
    cout << CYAN << "| 3. Crunchwrap Supreme    |   6. None                  |" << endl;
    cout << CYAN << "|                          |                            |" << endl;
    cout << CYAN << "| A[RM 8.50]   B[RM 20.60] |                            |" << endl;
    cout << CYAN << "+--------------------------+----------------------------+" << RESET << endl;
    cout << RED << "  A - ALa-cart      B - Set ";
    cout << blueColor << " " << endl;
}

// Manages the entire ordering process.
void process_Order()
{
    // Variables to store the total price, dining option, service charge, and packaging fee.
    double orderTotal = 0.0;
    string diningOption;
    double serviceCharge = 0.0;
    double packagingFee = 0.0;

    bool validDiningOption = false;
    while (!validDiningOption)
    {
        system("cls");
        logo(); // Display the logo of the establishment.

        // Ask the user to choose their dining option.
        cout << "Please choose your dining option:" << endl;
        cout << "  1. Dine in \t \t \t 2. Takeaway " << endl;
        cout << " " << endl;
        cout << "Enter your choice (1 or 2): ";
        cin >> diningOption;

        // Check if the entire string is composed of digits and then check for valid choices.
        if (all_of(diningOption.begin(), diningOption.end(), ::isdigit))
        {
            if (diningOption == "1")
            {
                serviceCharge = DINE_IN_SERVICE_CHARGE_RATE; // Calculate service charge as 10% of total amount
                validDiningOption = true;
            }
            else if (diningOption == "2")
            {
                packagingFee = TAKEAWAY_PACKAGING_FEE;
                validDiningOption = true;
            }
            else
            {
                cout << "Invalid choice! Please choose either 1 or 2." << endl;
                cin.clear();  // Clear the error flag if any
                cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Clear the buffer
            }
        }
        else
        {
            cout << "Invalid input! Please enter a numeric choice of either 1 or 2." << endl;
            cin.clear(); // Clear the error flag if any
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Clear the buffer
        }
    }
    system("cls");

    // Variables to handle user's decision on continuing or modifying the order.
    char continueOrdering = 'Y';

    char reorder = 'Y';
    // Another loop to handle reordering within the main order.
    while (toupper(continueOrdering) == 'Y')
    {
        char reorder = 'Y';

        while (toupper(reorder) == 'Y')
        {
            order_Item();

            // Ask the user if they want to order another item.
            while (true) {  // Loop until valid response
                cout << "Do you want to order another item? (Y/N): ";
                cin >> reorder;
                reorder = toupper(reorder);

                if (reorder == 'Y' || reorder == 'N') {
                    break; // Exit the inner loop if valid input
                }
                else {
                    cout << "Invalid input! Please enter 'Y' or 'N'." << endl;
                    cin.clear(); // Clear the error flag if any
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the buffer
                }
            }
        }

        char modify;
        // Ask the user if they wish to modify the order.
        // Keep asking until a valid response is received
        while (true) {
            cout << "Do you want to modify any order? (Y/N): ";
            cin >> modify;

            // Convert to uppercase to allow for both 'y' and 'Y' (similarly 'n' and 'N')
            modify = toupper(modify);

            // Check if the input is valid
            if (modify == 'Y' || modify == 'N') {
                break; // Exit the loop if valid input
            }
            else {
                cout << "Invalid input! Please enter 'Y' or 'N'." << endl;
                cout << " " << endl;
                cin.clear(); // Clear the error flag if any
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the buffer
            }
        }

        if (modify == 'Y') {
            modify_Order(); // If the user chooses to modify, call the modifyOrder function.
        }

        // Calculate the subtotal of the order.
        double orderSubtotal = 0.0;

        for (const OrderItem& item : currentOrder)
        {
            double price = (item.choice == 'A') ? PRICES_A[item.itemIndex] : PRICES_B[item.itemIndex];
            orderSubtotal += price * item.quantity;
        }

        // Calculate the total price including any additional fees.
        orderTotal = orderSubtotal;
        orderTotal += (orderTotal * serviceCharge); // Add service charge as a percentage of the total amount
        orderTotal += packagingFee;

        // Display the subtotal and total amounts
        system("cls");
        cout << "Subtotal: RM" << fixed << setprecision(2) << orderSubtotal << endl;
        cout << "Total price for this order: RM" << fixed << setprecision(2) << orderTotal << endl;

        while (true) {
            cout << "Do you want to continue ordering? (Y/N): ";
            cin >> continueOrdering;

            // Convert to uppercase to allow for both 'y' and 'Y' (similarly 'n' and 'N')
            continueOrdering = toupper(continueOrdering);

            // Check if the input is valid
            if (continueOrdering == 'Y' || continueOrdering == 'N') {
                if (continueOrdering == 'N') {
                    // To main menu
                    payment(orderTotal);
                }
                break; // Exit the loop if valid input
            }
            else {
                cout << "Invalid input! Please enter 'Y' or 'N'." << endl;
                cin.clear(); // Clear the error flag if any
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the buffer
            }
        }
    }
}

// Handles ordering a single item.
void order_Item()
{
    // Variables to store user input for item index, choice of price (A or B), and the quantity.
    string indexStr;
    string choiceStr;
    string quantityStr, option;

    // Display the menu to the user.
    Food_Menu();

    // Loop until a valid item number is provided by the user.
    while (true) {
        cout << "Enter the item number (1-" << NUM_ITEMS << "): ";
        cin >> indexStr;

        // First, validate that the entered string is composed only of digits.
        if (all_of(indexStr.begin(), indexStr.end(), ::isdigit))
        {
            int index = stoi(indexStr); // Convert the string to an integer.

            // Check if the entered item number is within the valid range.
            if (index >= 1 && index <= NUM_ITEMS) {
                break;  // Valid input, exit the loop
            }
            else if (index == 6) {  // Check if user entered 6
                return;  // If 6 is entered, return to process_Order immediately
            }
            else {
                cout << "Invalid item number!" << endl;
            }
        }
        else {
            cout << "Invalid input! Please enter a numeric item number." << endl;
        }

        cout << " " << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Loop until a valid choice (A or B) is given
    while (true) {
        cout << "Choose option A(ALA-cart) or B(Set) for " << ITEM_NAMES[stoi(indexStr) - 1] << ": ";
        cin >> choiceStr;

        // Convert the entire string to uppercase
        for_each(choiceStr.begin(), choiceStr.end(), [](char& c) {
            c = ::toupper(c);
            });

        // Validate if the entire string is composed only of alphabets.
        if (all_of(choiceStr.begin(), choiceStr.end(), ::isalpha))
        {
            // Now check if the string is either "A" or "B".
            if (choiceStr == "A" || choiceStr == "B") {
                break;  // Valid input, exit the loop
            }
            else {
                cout << "Invalid choice! Please choose 'A' or 'B'." << endl;
            }
        }
        else {
            cout << "Invalid input! Please enter a valid alphabet choice." << endl;
        }

        cout << " " << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Loop until a valid numeric quantity is provided by the user.
    while (true)
    {
        cout << "Enter quantity: ";
        cin >> quantityStr;

        bool isValid = true;
        // Check if the entered string is entirely composed of digits.
        for (char c : quantityStr) {
            if (!isdigit(c)) {
                isValid = false;
                break;
            }
        }

        // Check if the quantity is a positive number.
        if (!isValid || stoi(quantityStr) <= 0) {
            cout << "Invalid quantity!" << endl;
        }
        else {
            break;  // Valid numeric input, exit the loop
        }
    }
    // Add the ordered item details to the currentOrder list.
    currentOrder.push_back({ stoi(indexStr) - 1, choiceStr[0], stoi(quantityStr) });
}

string generateOrderKey(int itemIndex, char choice) {
    return to_string(itemIndex) + "-" + choice;
}

// Handles modifications to the current order.
void modify_Order()
{
    system("cls");
    // Variables to hold the user's choices.
    string choiceStr;
    int choiceInt;
    string option;
    const string ADDITIONAL_ITEMS[] = { "coke", "fries" };
    map<string, pair<int, double>> aggregatedOrders;
    for (const OrderItem& order : currentOrder) {
        string key = generateOrderKey(order.itemIndex, order.choice);
        double price = (order.choice == 'A') ? PRICES_A[order.itemIndex] : PRICES_B[order.itemIndex];
        aggregatedOrders[key].first += order.quantity;
        aggregatedOrders[key].second += order.quantity * price;
    }

    // Display the aggregated orders:
    cout << "Current Orders:" << endl;
    int count = 1;
    for (const auto& pair : aggregatedOrders) {
        string key = pair.first;
        int quantity = pair.second.first;
        double total_price = pair.second.second;

        int itemIndex = stoi(key.substr(0, key.find("-")));
        char choice = key.back();

        // Display the order details.
        cout << count++ << ". " << ITEM_NAMES[itemIndex] << " (" << choice << ") x " << quantity << " [RM " << fixed << setprecision(2) << total_price << "]" << endl;

        // If the item is a 'B' item, display the additional items.
        if (choice == 'B') {
            cout << "   Includes: ";
            for (const string& addItem : ADDITIONAL_ITEMS) {
                cout << addItem << " ";
            }
            cout << endl;
        }

    }


    // Loop until a valid order number is entered.
    while (true)
    {
        cout << "Enter order number to modify (1-" << currentOrder.size() << ") or 0 to finish: ";
        cin >> choiceStr;

        // Validate if the entire string is composed only of digits.
        if (all_of(choiceStr.begin(), choiceStr.end(), ::isdigit))
        {
            choiceInt = stoi(choiceStr); // Convert the string to integer.

            // Check if the converted integer choice is within valid range.
            if (choiceInt >= 0 && choiceInt <= currentOrder.size()) {
                break;
            }
            else {
                cout << "Invalid order number!" << endl;
                cout << " " << endl;
            }
        }
        else
        {
            cout << "Invalid input! Please enter a numeric order number." << endl;
        }

        cin.clear(); // Clear the error state of cin.
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore any additional characters.
    }

    // If choice is 0, exit the function.
    if (choiceInt == 0) {
        return;
    }

    // Loop until a valid option (Increase, Decrease, or Remove) is selected.
    while (true)
    {
        cout << "1. Increase Quantity" << endl;
        cout << "2. Decrease Quantity" << endl;
        cout << "3. Remove Order" << endl;
        cout << "Enter your option: ";
        cin >> option;

        // Validate if the entire string is composed only of digits.
        if (all_of(option.begin(), option.end(), ::isdigit))
        {
            // Check if the entered numeric option string is one of the valid choices.
            if (option == "1" || option == "2" || option == "3")
            {
                break;
            }
            else
            {
                cout << "Invalid option!" << endl;
            }
        }
        else
        {
            cout << "Invalid input! Please enter a numeric option." << endl;
        }

        cout << " " << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Perform the action based on the chosen option.
    if (option == "1")
    {
        increase_Quantity(choiceInt - 1); // Call function to increase quantity.
    }
    else if (option == "2")
    {
        decrease_Quantity(choiceInt - 1); // Call function to decrease quantity.
    }
    else if (option == "3")
    {
        remove_Order(choiceInt - 1); // Call function to remove the order.
    }
    else
    {
        // This shouldn't happen given the above checks, but it's safe to keep.
        cout << "Invalid option!" << endl;
        cout << " " << endl;
    }
}

// Increases the quantity of an already ordered item.
void increase_Quantity(int orderNumber)
{
    cout << "Enter quantity to increase: ";
    int quantity;
    cin >> quantity;

    if (quantity <= 0)
    {
        cout << "Invalid quantity!" << endl;
        cout << " " << endl;
        return;
    }

    currentOrder[orderNumber].quantity += quantity;
}


void decrease_Quantity(int orderNumber)
{
    int quantityToDecrease;
    string key = generateOrderKey(currentOrder[orderNumber].itemIndex, currentOrder[orderNumber].choice);

    cout << "Enter quantity to decrease: ";
    cin >> quantityToDecrease;

    if (quantityToDecrease <= 0)
    {
        cout << "Invalid quantity!" << endl;
        cout << " " << endl;
        return;
    }

    // Find all orders with the same type and choice, and reduce the quantity
    for (OrderItem& order : currentOrder) {
        if (generateOrderKey(order.itemIndex, order.choice) == key) {
            if (order.quantity <= quantityToDecrease) {
                quantityToDecrease -= order.quantity;
                order.quantity = 0;
            }
            else {
                order.quantity -= quantityToDecrease;
                quantityToDecrease = 0;
            }
            if (quantityToDecrease == 0) break;
        }
    }

    // Clean up orders with quantity of 0
    currentOrder.erase(remove_if(currentOrder.begin(), currentOrder.end(), [](const OrderItem& order) {
        return order.quantity == 0;
        }), currentOrder.end());
}

// Removes an item from the order list.
void remove_Order(int orderNumber)
{
    string key = generateOrderKey(currentOrder[orderNumber].itemIndex, currentOrder[orderNumber].choice);

    // Remove all orders with the same type and choice
    currentOrder.erase(remove_if(currentOrder.begin(), currentOrder.end(), [&](const OrderItem& order) {
        return generateOrderKey(order.itemIndex, order.choice) == key;
        }), currentOrder.end());
}

// Calculates and displays the membership points earned.
int calculate_MembershipPoints(double orderTotal)
{
    int points = static_cast<int>(orderTotal * 10);  // RM 1 = 10 points
    return points;
}

void payment(double orderTotal) {
    int points;
    double balance;
    string membership_card_num = get_membership_card();

    // Redeem points first and update balance_payment and redeemAmount.
    double redeemAmount;
    bool pointsRedeemed = redeemPoints(membership_card_num, balance, points, orderTotal, redeemAmount);

    // Check the balance after redemption.
    balance = check_balance(membership_card_num, orderTotal);

    // Display payment receipt, passing redeemAmount and pointsRedeemed.
    payment_receipt(membership_card_num, orderTotal, balance, redeemAmount, pointsRedeemed);
}

bool redeemPoints(string membership_card_num, double& balance_payment, int& points, double orderTotal, double& redeemAmount) {
    // Calculate the value of points to redeem (1 point = 0.01 RM).
    double pointsValue = points * 0.01;
    string option;

    ifstream f1("customer_records.txt");
    string line;
    bool customerFound = false;

    while (getline(f1, line)) {
        if (line.find("Membership Card Number: " + membership_card_num) != string::npos) {
            // Update the balance line
            getline(f1, line);
            double balance = stod(line.substr(line.find(":") + 1));
            getline(f1, line);
            int customerPoints = stoi(line.substr(line.find(":") + 1)); // Use a different variable name for customer points
            cout << "Your points is " << customerPoints << endl;
            double pointsValue = customerPoints * 0.01;
            string redeemOption;
            cout << "Do you want to redeem points to cover the order fee? (Y/N): ";
            cin >> redeemOption;
            customerFound = true;

            if (redeemOption == "Y" || redeemOption == "y") {
                if (pointsValue < orderTotal) {
                    cout << "Error: Insufficient points to redeem. Your current points value: RM" << pointsValue << endl;
                    return false; // Points were not redeemed due to insufficient balance
                }
                else {
                    // Redeem points to cover the order total.
                    redeemAmount = orderTotal; // Set the redeemAmount
                    balance_payment -= redeemAmount;
                    customerPoints -= static_cast<int>(redeemAmount * 100); // Deduct points equivalent to redeemedAmount
                    cout << "Points redeemed: RM" << redeemAmount << endl;

                    // Update the customer's points in the file
                    // You can implement the code to update the points in the data file here.
                    // This might involve opening the file in write mode and updating the points field for the customer.

                    return true; // Points were successfully redeemed
                }
            }
            else if (redeemOption == "N" || redeemOption == "n") {
                cout << "Points not redeemed." << endl;
                return false; // Points were not redeemed
            }

            do {
                cout << "Your balance in your card is only RM " << balance << ". Do you want to top up?" << endl << "[1] to top up or [2] to cancel payment: ";
                cin >> option;
                if (option == "1") {
                    topUpMembership(); // Implement your top-up logic here.
                    // You may need to update the balance here after topping up.
                }
                else if (option == "2") {
                    cancel_payment();
                    menu();
                    return false; // Payment canceled
                }
            } while (option != "1" && option != "2");

            return false; // Points were not redeemed
        }
    }

    if (!customerFound) {
        cout << "Customer not found." << endl;
    }
}


void payment_receipt(string membership_card_num, double orderTotal, double balance_payment, double redeemAmount, bool pointsRedeemed) {
    double service_charge, point;
    string option;
    int points;
    double subtotal_receipt = 0;
    const string ADDITIONAL_ITEMS[] = { "Coke", "Fries" };
    cout << "----------------------RECEIPT----------------------" << endl;
    cout << setfill(' ') << setw(10) << left << "Quantity" << setw(25) << left << "TACO Name" << setw(5) << left << "Set" << setw(15) << left << "Price(RM)" << endl;

    // Sort the currentOrder based on quantity in descending order.
    std::sort(currentOrder.begin(), currentOrder.end(), [](const OrderItem& a, const OrderItem& b) {
        return a.quantity > b.quantity;
        });

    map<string, pair<int, double>> aggregatedOrders;
    for (const OrderItem& order : currentOrder) {
        string key = generateOrderKey(order.itemIndex, order.choice);
        double price = (order.choice == 'A') ? PRICES_A[order.itemIndex] : PRICES_B[order.itemIndex];
        aggregatedOrders[key].first += order.quantity;
        aggregatedOrders[key].second += order.quantity * price;
    }

    // Display the aggregated orders:
    int count = 1;
    for (const auto& pair : aggregatedOrders) {
        string key = pair.first;
        int quantity = pair.second.first;
        double total_price = pair.second.second;

        int itemIndex = stoi(key.substr(0, key.find("-")));
        char choice = key.back();

        // Display the order details.
        count++;
        cout << setw(10) << left << quantity << setw(25) << left << ITEM_NAMES[itemIndex] << setw(4) << left << choice << setw(9) << right << fixed << setprecision(2) << total_price << endl;
        subtotal_receipt += total_price;

        // If the item is a 'B' item, display the additional items.
        if (choice == 'B') {
            for (const string& addItem : ADDITIONAL_ITEMS) {
                cout << "\t    ADDON: " << addItem << endl;
            }
        }
    }

    cout << "---------------------------------------------------" << endl;
    service_charge = orderTotal - subtotal_receipt;
    cout << "Service Charge" << setw(34) << right << fixed << setprecision(2) << service_charge << endl;
    cout << "---------------------------------------------------" << endl;
    cout << "Subtotal" << setw(40) << right << fixed << setprecision(2) << subtotal_receipt << endl;
    cout << "Total" << setw(43) << right << fixed << setprecision(2) << orderTotal << endl;
    cout << "---------------------------------------------------" << endl;

    // Retrieve the balance from the file
    ifstream f1("customer_records.txt");
    string line;
    while (getline(f1, line)) {
        if (line.find("Membership Card Number: " + membership_card_num) != string::npos) {
            // Update the balance line
            getline(f1, line);
            double balance = stod(line.substr(line.find(":") + 1));
            cout << "Initial Balance" << setw(33) << right << fixed << setprecision(2) << balance << endl;
            if (pointsRedeemed) {
                // Display the final balance including the redeemed points
                //ASSIGN BALANCE_PAYMENT AS BALANCE
                balance_payment = balance;
                cout << "Final Balance (Points Redeemed)" << setw(17) << right << fixed << setprecision(2) << balance << "\n";
            }
            else {
                // Display the final balance without redeeming points
                cout << "Final Balance" << setw(35) << right << fixed << setprecision(2) << balance_payment << "\n";
            }
        }
    }
    f1.close();

    if (pointsRedeemed) {
        // Display the redeemAmount if points were redeemed
        cout << "Redeemed Amount" << setw(33) << right << fixed << setprecision(2) << redeemAmount << "\n";
    }

    // Calculate points based on the provided order total
    int pointsEarned = calculate_MembershipPoints(orderTotal);

    if (pointsRedeemed) {
        ifstream f1("customer_records.txt");
        ofstream tempFile("temp_customer_records.txt"); // Temporary file

        string line;
        bool customerFound = false;

        while (getline(f1, line)) {
            if (line.find("Membership Card Number: " + membership_card_num) != string::npos) {
                // Update the balance line
                getline(f1, line);
                double balance = stod(line.substr(line.find(":") + 1));
                getline(f1, line);
                int points = stoi(line.substr(line.find(":") + 1));

                // Update the points based on the redeemed amount
                points -= static_cast<int>(redeemAmount * 10);
                pointsEarned = 0;
                // Write the updated line to the temporary file
                tempFile << "Membership Card Number: " << membership_card_num << endl;
                tempFile << "Balance: " << balance << endl;
                tempFile << "Points: " << points << endl;

                customerFound = true;
            }
            else {
                // Copy other lines as they are to the temporary file
                tempFile << line << endl;
            }
        }

        f1.close();
        tempFile.close();

        if (!customerFound) {
            cout << "Customer not found." << endl;
        }
        else {
            // Remove the original data file
            remove("customer_records.txt");
            // Rename the temporary file to the original data file
            rename("temp_customer_records.txt", "customer_records.txt");
        }


        if (!customerFound) {
            cout << "Customer not found." << endl;
        }
    }

    // Display and update points
    cout << "Points Earned" << setw(35) << right << fixed << setprecision(2) << pointsEarned << "\n\n\n";


    // Update membership points
    updateCustomerRecord(membership_card_num, balance_payment, pointsEarned);

    do {
        cout << "Would you like to go back to the menu page or log out?" << endl << "[1] Back to Menu Page or [2] to Log Out: ";
        cin >> option;
        if (option == "1") {
            menu();
            break;
        }
        else if (option == "2") {
            home_page();
            break;
        }
    } while (option != "1" || option != "2");
}

string get_membership_card() {
    string membership_card_num;
    char choice;
    do {
        cout << "Enter your membership card number: ";
        cin >> membership_card_num;
        if (validate_membership_card(membership_card_num)) {
            cout << "Valid Membership Card Number\n\n";
            return membership_card_num;
        }
        else {
            cout << "Invalid Membership Card Number. [Y] to Try Again or [N] to Customer Register\n";
            cin >> choice;
            choice = toupper(choice);
            if (choice == 'Y' || choice == 'N') {
                if (choice == 'N') {
                    customer_registration();
                    break;
                }
                else {
                    get_membership_card();
                }
            }
            else {
                cout << "Invalid input. [Y] to Try Again or [N] to Customer Register\n";
            }
        }
    } while (choice != 'Y');
}

bool validate_membership_card(string membership_card_num) {
    if (membership_card_num.length() != 9) {
        return false;
    }

    for (int i = 0; i < 3; ++i) {
        if (!isalpha(membership_card_num[i])) {
            return false;
        }
    }

    for (int i = 3; i < 9; ++i) {
        if (!isdigit(membership_card_num[i])) {
            return false;
        }
    }

    bool customerExists = checkCustomerExists(membership_card_num);

    if (!customerExists) {
        return false;
    }

    return true;
}

double check_balance(string membership_card_num, double orderTotal) {
    //retrieve the balance from the file
    ifstream f1("customer_records.txt");
    string line;
    string option;
    while (getline(f1, line)) {
        if (line.find("Membership Card Number: " + membership_card_num) != string::npos) {
            // Update the balance line
            getline(f1, line);
            double balance = stod(line.substr(line.find(":") + 1));
            if (balance >= orderTotal) {
                balance -= orderTotal;
                return balance;
            }
            else if (balance < orderTotal) {
                do {
                    cout << "Your balance in your card is only RM " << balance << ". Do you want to top up?" << endl << "[1] to top up or [2] to cancel payment: ";
                    cin >> option;
                    if (option == "1") {
                        topUpMembership();
                    }
                    else if (option == "2") {
                        cancel_payment();
                        menu();
                        break;
                    }
                } while (option != "1" || option != "2");
            }
        }
    }
    f1.close();
}


void updateCustomerRecord(const string& membershipNumber, double balance_payment, int points) {
    ifstream inputFile("customer_records.txt");
    if (!inputFile.is_open()) {
        cout << "Error opening customer records file!" << endl;
        return;
    }

    ofstream tempFile("temp_records.txt");
    if (!tempFile.is_open()) {
        cout << "Error opening temporary file for writing!" << endl;
        inputFile.close();
        return;
    }

    string line;
    bool found = false;

    while (getline(inputFile, line)) {
        if (line.find("Membership Card Number: " + membershipNumber) != string::npos) {
            // Update the balance and membership line
            getline(inputFile, line);  // Read the balance line
            tempFile << "Membership Card Number: " + membershipNumber << endl;
            tempFile << fixed << setprecision(2) << "Balance: " << balance_payment << endl;  // Update balance
            // Update or add the membership points line
            getline(inputFile, line);
            double points_earned = stod(line.substr(line.find(":") + 1));
            points += points_earned;
            tempFile << "Membership Points: " << points << endl;
            found = true;
        }
        else {
            tempFile << line << endl;  // Copy other lines as is
        }
    }

    inputFile.close();
    tempFile.close();

    if (!found) {
        cout << "Customer not found!" << endl;
        remove("temp_records.txt"); // Remove the temporary file
        return;
    }

    // Replace the original file with the temporary file
    if (remove("customer_records.txt") != 0) {
        cout << "Error removing original file!" << endl;
        // Handle the error, e.g., log it or display an error message.
    }
    else {
        // Rename the temporary file to the original file
        if (rename("temp_records.txt", "customer_records.txt") != 0) {
            cout << "Error renaming temporary file!" << endl;
            // Handle the renaming error here.
        }
    }
}

// Removes all item from the order list.
void cancel_payment() {
    currentOrder.clear();
}

void checkBalance_Points() {
    system("cls");
    logo();
    string membership_card_num = get_membership_card();
    ifstream f1("customer_records.txt");
    string line;
    while (getline(f1, line)) {
        if (line.find("Membership Card Number: " + membership_card_num) != string::npos) {
            // Update the balance line
            getline(f1, line);
            double balance = stod(line.substr(line.find(":") + 1));
            cout << "Your balance is RM " << balance << endl;
            getline(f1, line);
            int points = stoi(line.substr(line.find(":") + 1));
            cout << "Your points is " << points << endl;
        }
    }
    f1.close();
    string option;
    do {
        cout << "Would you like to back to the menu page or log out?" << endl << "[1] Back to Menu Page or [2] to Log Out: ";
        cin >> option;
        if (option == "1") {
            system("cls");
            menu();
            break;
        }
        else if (option == "2") {
            system("cls");
            home_page();
            break;
        }
        else {
            cout << "Invalid input. [1] Back to Menu Page or [2] to Log Out: ";
        }
    } while (option != "1" || option != "2");
}

void exit() {
    system("cls");
    cout << BOLD << RED << " _______  ___ ___  _______  ______   ___ ___     ___ ___  _______  ___ ___ " << RESET << endl;
    cout << BOLD << RED << "|       ||   Y   ||   _   ||   _  \\ |   Y   )   |   Y   ||   _   ||   Y   |" << RESET << endl;
    cout << BOLD << YELLOW << "|.|   | ||.  1   ||.  1   ||.  |   ||.  1  /    |   1   ||.  |   ||.  |   |" << RESET << endl;
    cout << BOLD << YELLOW << "`-|.  |-'|.  _   ||.  _   ||.  |   ||.  _  \\     \\_   _/ |.  |   ||.  |   |" << RESET << endl;
    cout << BOLD << GREEN << "  |:  |  |:  |   ||:  |   ||:  |   ||:  |   \\     |:  |  |:  1   ||:  1   |" << RESET << endl;
    cout << BOLD << GREEN << "  |::.|  |::.|:. ||::.|:. ||::.|   ||::.| .  )    |::.|  |::.. . ||::.. . |" << RESET << endl;
    cout << BOLD << CYAN << "  `---'  `--- ---'`--- ---'`--- ---'`--- ---'     `---'  `-------'`-------'" << RESET << endl;
    cout << endl;

    cout << "Thank you for choosing our services! We're here to make your experience extraordinary. If you have any questions or need assistance, feel free to reach out. We look forward to serving you again soon!" << endl;
    cout << endl;

}