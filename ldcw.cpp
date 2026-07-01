#include <iostream>
#include <string>
#include <iomanip> // Required for formatting currency (setprecision)
#include <cctype>  // Required for toupper()
#include <cstdlib> // Required for rand() and srand()
#include <ctime>   // Required for time()

using namespace std;

// ==========================================
// STRUCT DEFINITION
// ==========================================
struct BookingRecord {
    string passengerName;
    string driverName;
    string vehicleType;
    string pickupLocation;
    string destination;
    double distance = 0.0;
    double totalFare = 0.0;
    bool hasRecord = false; // Flag to check if a ride was booked
};

// Global struct instance to store the most recent booking
BookingRecord lastBooking;

// ==========================================
// FUNCTION PROTOTYPES
// ==========================================
void displayMenu();
void bookGrabRide();
void estimateFare();
void orderGrabFood();
void viewLastBooking();
void aboutGrab();

// Helper Functions for modularity & validation
double calculateTotalFare(double distance, char rain, char peak, double &baseFare, double &surgeCharge);
char getYesNoInput(string prompt); // New error-handling helper function

// ==========================================
// MAIN FUNCTION
// ==========================================
int main() {
    // Initialize random seed for driver assignment
    srand(time(0)); 
    
    int userChoice = 0;

    do {
        displayMenu();
        cout << "Please select an option (1-6): ";
        cin >> userChoice;

        // Error Handling: Non-numeric input
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\n[!] ERROR: Invalid input. Please enter a valid number." << endl;
            continue;
        }
        
        // Clear trailing characters (e.g. user typed "1abc")
        cin.ignore(1000, '\n');

        switch (userChoice) {
            case 1:
                bookGrabRide();
                break;
            case 2:
                estimateFare();
                break;
            case 3:
                orderGrabFood();
                break;
            case 4:
                viewLastBooking();
                break;
            case 5:
                aboutGrab();
                break;
            case 6:
                cout << "\nThank you for using the Grab SuperApp Simulator. Goodbye!" << endl;
                break;
            default:
                // Error Handling: Invalid menu options
                cout << "\n[!] ERROR: Invalid choice. Please select a number between 1 and 6." << endl;
        }
    } while (userChoice != 6);

    return 0;
}

// ==========================================
// FUNCTION IMPLEMENTATIONS
// ==========================================

void displayMenu() {
    cout << "\n=================================" << endl;
    cout << "         GRAB SUPERAPP           " << endl;
    cout << "=================================" << endl;
    cout << "1. Book GrabRide" << endl;
    cout << "2. Fare Estimator" << endl;
    cout << "3. GrabFood Order" << endl;
    cout << "4. View Last Booking" << endl;
    cout << "5. About Grab Innovation" << endl;
    cout << "6. Exit" << endl;
    cout << "=================================" << endl;
}

// --- HELPER FUNCTION: Get Yes/No Input (Commit 6) ---
char getYesNoInput(string prompt) {
    char input;
    while (true) {
        cout << prompt;
        cin >> input;
        
        if (cin.fail() || (toupper(input) != 'Y' && toupper(input) != 'N')) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "[!] Invalid input. Please enter 'Y' or 'N'.\n";
        } else {
            cin.ignore(1000, '\n'); // Clear buffer
            return toupper(input);
        }
    }
}

// --- HELPER FUNCTION: Calculate Fare & Surge ---
double calculateTotalFare(double distance, char rain, char peak, double &baseFare, double &surgeCharge) {
    // Base formula: RM5.00 + (Distance * RM1.20)
    baseFare = 5.00 + (distance * 1.20);
    double surgeMultiplier = 0.0;

    // Surge Logic (nested if-else) based on Christensen's disruptive pricing model
    if (rain == 'Y' && peak == 'Y') {
        surgeMultiplier = 0.50; // +50% surge
    } else if (rain == 'Y' || peak == 'Y') {
        surgeMultiplier = 0.20; // +20% surge
    }

    surgeCharge = baseFare * surgeMultiplier;
    return baseFare + surgeCharge;
}

// --- MODULE 1: BOOK GRAB RIDE ---
void bookGrabRide() {
    string passengerName, pickup, destination;
    int pax;
    double distance, baseFare, surgeCharge, totalFare;
    char rain, peak;

    cout << "\n--- BOOK GRABRIDE ---" << endl;

    cout << "Enter Passenger Name: ";
    getline(cin, passengerName);
    
    cout << "Enter Pickup Location: ";
    getline(cin, pickup);
    
    cout << "Enter Destination: ";
    getline(cin, destination);

    // Input Validation for Passengers
    while (true) {
        cout << "Number of Passengers (1-6): ";
        cin >> pax;
        if (cin.fail() || pax < 1 || pax > 6) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "[!] Invalid input. Vehicles can only fit 1 to 6 passengers.\n";
        } else {
            cin.ignore(1000, '\n'); // clear buffer
            break;
        }
    }

    // Input Validation for Distance (Reject negative/zero)
    while (true) {
        cout << "Distance (km): ";
        cin >> distance;
        if (cin.fail() || distance <= 0) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "[!] Invalid input. Distance must be a positive number.\n";
        } else {
            cin.ignore(1000, '\n'); // clear buffer
            break;
        }
    }

    // Input Validation for Rain/Peak using new Error Handling helper
    rain = getYesNoInput("Is it raining? (Y/N): ");
    peak = getYesNoInput("Is it peak hour? (Y/N): ");

    // Call helper function for pricing
    totalFare = calculateTotalFare(distance, rain, peak, baseFare, surgeCharge);

    // Vehicle Recommendation Logic
    string vehicleType = (pax <= 4) ? "GrabCar" : "Grab6";

    // Arrays for Driver Assignment
    string driverNames[] = {"Ahmad", "Siti", "Muthu", "Mei Ling", "John", "Ali", "Chong", "Fatima"};
    string grabCarModels[] = {"Perodua Bezza", "Toyota Vios", "Honda City", "Proton Saga", "Nissan Almera"};
    string grab6Models[] = {"Perodua Alza", "Toyota Innova", "Honda BR-V", "Mitsubishi Xpander"};

    // Randomize Driver details
    int numDrivers = sizeof(driverNames) / sizeof(driverNames[0]);
    string assignedDriver = driverNames[rand() % numDrivers];
    
    // Assign proper vehicle model based on capacity
    string assignedVehicleModel;
    if (pax <= 4) {
        int numGrabCar = sizeof(grabCarModels) / sizeof(grabCarModels[0]);
        assignedVehicleModel = grabCarModels[rand() % numGrabCar];
    } else {
        int numGrab6 = sizeof(grab6Models) / sizeof(grab6Models[0]);
        assignedVehicleModel = grab6Models[rand() % numGrab6];
    }
    
    // Generate Random ETA between 2 and 12 minutes
    int eta = (rand() % 11) + 2; 

    // Save to Struct
    lastBooking.passengerName = passengerName;
    lastBooking.driverName = assignedDriver;
    lastBooking.vehicleType = vehicleType + " (" + assignedVehicleModel + ")";
    lastBooking.pickupLocation = pickup;
    lastBooking.destination = destination;
    lastBooking.distance = distance;
    lastBooking.totalFare = totalFare;
    lastBooking.hasRecord = true; // Mark that a booking has been made

    // --- Print Receipt ---
    cout << "\n=================================" << endl;
    cout << "         BOOKING CONFIRMED       " << endl;
    cout << "=================================" << endl;
    cout << "Passenger : " << passengerName << endl;
    cout << "Route     : " << pickup << " -> " << destination << endl;
    cout << "Vehicle   : " << vehicleType << " (Fits " << pax << " pax)" << endl;
    
    // Driver Match Simulation Output
    cout << "---------------------------------" << endl;
    cout << "Driver Found!" << endl;
    cout << "Driver    : " << assignedDriver << endl;
    cout << "Model     : " << assignedVehicleModel << endl;
    cout << "ETA       : " << eta << " minutes" << endl;
    
    cout << "---------------------------------" << endl;
    cout << "--- FARE BREAKDOWN ---" << endl;
    cout << fixed << setprecision(2); // Set output to 2 decimal places
    cout << "Base Fare      : RM " << baseFare << endl;
    if (surgeCharge > 0) {
        cout << "Surge Charge   : RM " << surgeCharge << " (High Demand)" << endl;
    }
    cout << "Total Fare     : RM " << totalFare << endl;
    cout << "---------------------------------" << endl;
    cout << "* Unlike traditional taxis, Grab guarantees this upfront fare. *" << endl;
    cout << "=================================\n" << endl;
}

// --- MODULE 2: FARE ESTIMATOR ---
void estimateFare() {
    double distance, baseFare, surgeCharge, totalFare;
    char rain, peak;

    cout << "\n--- FARE ESTIMATOR ---" << endl;
    
    // Input Validation for Distance (Reject negative/zero)
    while (true) {
        cout << "Enter Distance (km): ";
        cin >> distance;
        if (cin.fail() || distance <= 0) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "[!] Invalid input. Distance must be greater than 0.\n";
        } else {
            cin.ignore(1000, '\n'); // clear buffer
            break;
        }
    }

    // Input Validation for Rain/Peak
    rain = getYesNoInput("Is it raining? (Y/N): ");
    peak = getYesNoInput("Is it peak hour? (Y/N): ");

    // Call helper function
    totalFare = calculateTotalFare(distance, rain, peak, baseFare, surgeCharge);

    cout << "\n--- ESTIMATION RESULT ---" << endl;
    cout << fixed << setprecision(2);
    cout << "Estimated Base Fare: RM " << baseFare << endl;
    if (surgeCharge > 0) cout << "Estimated Surge    : RM " << surgeCharge << endl;
    cout << "TOTAL ESTIMATED    : RM " << totalFare << endl;
    cout << "-------------------------\n" << endl;
}

// --- MODULE 3: GRABFOOD ORDER ---
void orderGrabFood() {
    int restaurantChoice;
    double distance, foodCost, deliveryFee, totalAmount;
    string restaurantName;

    cout << "\n--- GRABFOOD ORDER ---" << endl;
    cout << "Select a Restaurant:" << endl;
    cout << "1. McDonald's (RM 15.00)" << endl;
    cout << "2. KFC        (RM 18.00)" << endl;
    cout << "3. Pizza Hut  (RM 25.00)" << endl;
    
    // Input Validation for Restaurant Selection
    while (true) {
        cout << "Enter your choice (1-3): ";
        cin >> restaurantChoice;
        if (cin.fail() || restaurantChoice < 1 || restaurantChoice > 3) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "[!] Invalid choice. Please select a valid restaurant option (1, 2, or 3).\n";
        } else {
            cin.ignore(1000, '\n'); // clear buffer
            break;
        }
    }

    // Assign price and name based on choice
    switch (restaurantChoice) {
        case 1:
            restaurantName = "McDonald's";
            foodCost = 15.00;
            break;
        case 2:
            restaurantName = "KFC";
            foodCost = 18.00;
            break;
        case 3:
            restaurantName = "Pizza Hut";
            foodCost = 25.00;
            break;
    }

    // Input Validation for Distance
    while (true) {
        cout << "Delivery Distance (km): ";
        cin >> distance;
        if (cin.fail() || distance <= 0) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "[!] Invalid input. Distance must be a positive number.\n";
        } else {
            cin.ignore(1000, '\n'); // clear buffer
            break;
        }
    }

    // Calculate Delivery Fee
    if (distance <= 5.0) {
        deliveryFee = 3.00;
    } else {
        deliveryFee = 6.00;
    }

    // Calculate Total
    totalAmount = foodCost + deliveryFee;

    // Display Output Breakdown
    cout << "\n=================================" << endl;
    cout << "       GRABFOOD RECEIPT          " << endl;
    cout << "=================================" << endl;
    cout << "Restaurant : " << restaurantName << endl;
    cout << "Distance   : " << distance << " km" << endl;
    cout << "---------------------------------" << endl;
    cout << fixed << setprecision(2);
    cout << "Food Cost  : RM " << foodCost << endl;
    cout << "Delivery   : RM " << deliveryFee << endl;
    cout << "---------------------------------" << endl;
    cout << "Total Paid : RM " << totalAmount << endl;
    cout << "=================================\n" << endl;
}

// --- MODULE 4: VIEW LAST BOOKING ---
void viewLastBooking() {
    cout << "\n--- LAST BOOKING RECORD ---" << endl;
    
    if (!lastBooking.hasRecord) {
        cout << "[!] No booking records found. Please book a ride first.\n" << endl;
        return;
    }

    cout << "Passenger : " << lastBooking.passengerName << endl;
    cout << "Driver    : " << lastBooking.driverName << endl;
    cout << "Vehicle   : " << lastBooking.vehicleType << endl;
    cout << "Route     : " << lastBooking.pickupLocation << " -> " << lastBooking.destination << endl;
    cout << "Distance  : " << lastBooking.distance << " km" << endl;
    cout << fixed << setprecision(2);
    cout << "Total Fare: RM " << lastBooking.totalFare << endl;
    cout << "---------------------------\n" << endl;
}

// Placeholder for Module 5
void aboutGrab() {
    cout << "\n---> [Module 5: About Grab Innovation running...]" << endl;
    // TODO in Commit 7: Add information about Christensen's Disruptive Innovation Model.
}