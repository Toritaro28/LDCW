#include <iostream>
#include <string>
#include <limits>
#include <iomanip> // Required for formatting currency (setprecision)
#include <cctype>  // Required for toupper()

using namespace std;

// ==========================================
// FUNCTION PROTOTYPES
// ==========================================
void displayMenu();
void bookGrabRide();
void estimateFare();
void orderGrabFood();
void viewLastBooking();
void aboutGrab();

// Helper Function for Fare Calculation (Modular Programming)
double calculateTotalFare(double distance, char rain, char peak, double &baseFare, double &surgeCharge);

// ==========================================
// MAIN FUNCTION
// ==========================================
int main() {
    int userChoice = 0;

    do {
        displayMenu();
        cout << "Please select an option (1-6): ";
        cin >> userChoice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n[!] ERROR: Invalid input. Please enter a number." << endl;
            continue;
        }

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

// --- HELPER FUNCTION: Calculate Fare & Surge ---
double calculateTotalFare(double distance, char rain, char peak, double &baseFare, double &surgeCharge) {
    // Base formula: RM5.00 + (Distance * RM1.20)
    baseFare = 5.00 + (distance * 1.20);
    double surgeMultiplier = 0.0;

    rain = toupper(rain);
    peak = toupper(peak);

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
    
    // Clear buffer before reading strings
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

    cout << "Enter Passenger Name: ";
    getline(cin, passengerName);
    
    cout << "Enter Pickup Location: ";
    getline(cin, pickup);
    
    cout << "Enter Destination: ";
    getline(cin, destination);

    // Input Validation for Passengers
    do {
        cout << "Number of Passengers (1-6): ";
        cin >> pax;
        if(cin.fail() || pax < 1 || pax > 6) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "[!] Invalid input. Vehicles can only fit 1 to 6 passengers.\n";
        }
    } while (pax < 1 || pax > 6);

    // Input Validation for Distance
    do {
        cout << "Distance (km): ";
        cin >> distance;
        if(cin.fail() || distance <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "[!] Invalid input. Distance must be a positive number.\n";
        }
    } while (distance <= 0);

    cout << "Is it raining? (Y/N): ";
    cin >> rain;
    cout << "Is it peak hour? (Y/N): ";
    cin >> peak;

    // Vehicle Recommendation Logic
    string vehicleType = (pax <= 4) ? "GrabCar" : "Grab6";

    // Call helper function
    totalFare = calculateTotalFare(distance, rain, peak, baseFare, surgeCharge);

    // Print Receipt
    cout << "\n=================================" << endl;
    cout << "         BOOKING CONFIRMED       " << endl;
    cout << "=================================" << endl;
    cout << "Passenger : " << passengerName << endl;
    cout << "Route     : " << pickup << " -> " << destination << endl;
    cout << "Vehicle   : " << vehicleType << " (Fits " << pax << " pax)" << endl;
    // Driver logic will be added in Commit 3
    cout << "\n--- FARE BREAKDOWN ---" << endl;
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
    
    // Input Validation for Distance
    do {
        cout << "Enter Distance (km): ";
        cin >> distance;
        if(cin.fail() || distance <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "[!] Invalid input. Distance must be greater than 0.\n";
        }
    } while (distance <= 0);

    cout << "Is it raining? (Y/N): ";
    cin >> rain;
    cout << "Is it peak hour? (Y/N): ";
    cin >> peak;

    // Call helper function
    totalFare = calculateTotalFare(distance, rain, peak, baseFare, surgeCharge);

    cout << "\n--- ESTIMATION RESULT ---" << endl;
    cout << fixed << setprecision(2);
    cout << "Estimated Base Fare: RM " << baseFare << endl;
    if (surgeCharge > 0) cout << "Estimated Surge    : RM " << surgeCharge << endl;
    cout << "TOTAL ESTIMATED    : RM " << totalFare << endl;
    cout << "-------------------------\n" << endl;
}

// Placeholder for Module 3
void orderGrabFood() {
    cout << "\n---> [Module 3: GrabFood Order running...]" << endl;
    // TODO in Commit 4: Add restaurant selection and delivery fee logic here.
}

// Placeholder for Module 4
void viewLastBooking() {
    cout << "\n---> [Module 4: View Last Booking running...]" << endl;
    // TODO in Commit 5: Add struct logic to display saved ride.
}

// Placeholder for Module 5
void aboutGrab() {
    cout << "\n---> [Module 5: About Grab Innovation running...]" << endl;
    // TODO in Commit 7: Add information about Christensen's Disruptive Innovation Model.
}