/*
 * Project Title: Grab SuperApp Simulator (C++)
 * Description: A console-based C++ application simulating Grab's disruptive 
 *              innovation in ride-hailing and ecosystem expansion (GrabFood).
 *              Demonstrates modular programming, structs, arrays, and error handling.
 */

#include <iostream>
#include <string>
#include <iomanip> // Required for formatting currency to 2 decimal places
#include <cctype>  // Required for toupper()
#include <cstdlib> // Required for rand() and srand()
#include <ctime>   // Required for time() seed

using namespace std;

// ==========================================
// NAMED CONSTANTS (Rubric: Code Quality)
// ==========================================
// Replacing magic numbers improves readability and makes
// future fare adjustments straightforward without hunting
// through the codebase for hard-coded values.
const int    BUFFER_SIZE       = 1000;   // cin.ignore() discard limit for invalid input
const double BASE_FARE          = 5.00;   // Flat boarding fee for all GrabRide bookings (RM)
const double RATE_PER_KM        = 1.20;   // Per-kilometre charge applied on top of base fare (RM)
const double SURGE_RAIN_PEAK    = 0.50;   // +50% surge multiplier when BOTH rain AND peak hour
const double SURGE_SINGLE       = 0.20;   // +20% surge multiplier when EITHER rain OR peak hour
const double DELIVERY_FEE_NEAR  = 3.00;   // Flat delivery fee for distances up to 5 km (RM)
const double DELIVERY_FEE_FAR   = 6.00;   // Flat delivery fee for distances beyond 5 km (RM)
const int    MAX_PASSENGERS     = 6;      // Grab's largest vehicle (Grab6) seats 6 passengers
const int    MIN_PASSENGERS     = 1;      // At least one passenger required to book
const int    GRABCAR_PAX_LIMIT  = 4;      // GrabCar seats up to 4; 5+ pax requires Grab6
const int    ETA_MIN            = 2;      // Minimum estimated arrival time in minutes
const int    ETA_MAX            = 12;     // Maximum estimated arrival time in minutes

// ==========================================
// STRUCT DEFINITION
// ==========================================
// Groups all booking details into a single record so they
// can be passed, stored, and recalled as one logical unit.
struct BookingRecord {
    string passengerName;
    string driverName;
    string vehicleType;
    string pickupLocation;
    string destination;
    double distance = 0.0;
    double totalFare = 0.0;
    bool hasRecord = false; // Flag to check if a ride has been booked yet
};

// Global instance to hold the latest booking session data.
// Stored globally so all modules (BookRide, ViewLastBooking)
// can read and write to the same record without needing
// to pass the struct through every function call chain.
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

// Helper Functions for modularity, calculation, and input validation
double calculateTotalFare(double distance, char rain, char peak, double &baseFare, double &surgeCharge);
char getYesNoInput(string prompt); 

// ==========================================
// MAIN FUNCTION
// ==========================================
int main() {
    // Initialize random seed based on current time for random driver/vehicle assignment
    srand(time(0)); 
    
    int userChoice = 0;

    // Main program loop - runs until user inputs '6' to exit
    do {
        displayMenu();
        cout << "Please select an option (1-6): ";
        cin >> userChoice;

        // Mandatory Error Handling: Handle non-numeric string/char inputs
        if (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(BUFFER_SIZE, '\n'); // Discard invalid input in the buffer
            cout << "\n[!] ERROR: Invalid input. Please enter a valid number." << endl;
            continue; // Skip the rest of the loop and show the menu again
        }
        
        // Clear trailing characters to prevent accidental buffer overflow into next prompts
        cin.ignore(BUFFER_SIZE, '\n');

        // Navigation based on user choice
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
                // Error Handling: Reject out-of-range menu options
                cout << "\n[!] ERROR: Invalid choice. Please select a number between 1 and 6." << endl;
        }
    } while (userChoice != 6);

    return 0;
}

// ==========================================
// FUNCTION IMPLEMENTATIONS
// ==========================================

// Displays the main menu of the Grab ecosystem.
// Each option maps to one module, mirroring Grab's real
// superapp home screen where users select the service they need.
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

// --- HELPER FUNCTION: Get Validated Yes/No Input ---
// Repeatedly prompts user until they enter exactly 'Y' or 'N'
// (case-insensitive). Extracted as a helper because 3+ modules
// need Y/N input (rain, peak, booking confirmation, etc.),
// and duplicating this loop would violate DRY principles.
char getYesNoInput(string prompt) {
    char input;
    while (true) {
        cout << prompt;
        cin >> input;
        
        if (cin.fail() || (toupper(input) != 'Y' && toupper(input) != 'N')) {
            cin.clear();
            cin.ignore(BUFFER_SIZE, '\n');
            cout << "[!] Invalid input. Please enter 'Y' or 'N'.\n";
        } else {
            cin.ignore(BUFFER_SIZE, '\n'); // Clear buffer for next inputs
            return toupper(input);
        }
    }
}

// --- HELPER FUNCTION: Calculate Fare & Surge Pricing ---
// Demonstrates dynamic pricing, a key component of Grab's disruptive technology
double calculateTotalFare(double distance, char rain, char peak, double &baseFare, double &surgeCharge) {
    // Base formula: flat boarding fee + (Distance × per-km rate)
    // This transparent pricing directly addresses the taxi overcharging
    // pain point identified in the poster — users know the fare before boarding.
    baseFare = BASE_FARE + (distance * RATE_PER_KM);
    double surgeMultiplier = 0.0;

    // Nested if-else logic for surge multiplier
    // Surge pricing is Grab's technological differentiator: it balances
    // supply and demand algorithmically rather than through ad-hoc
    // taxi-driver negotiation (Christensen's "technology enabler").
    if (rain == 'Y' && peak == 'Y') {
        surgeMultiplier = SURGE_RAIN_PEAK;  // Extreme demand: rain + rush hour
    } else if (rain == 'Y' || peak == 'Y') {
        surgeMultiplier = SURGE_SINGLE;     // Moderate demand: one factor active
    }

    surgeCharge = baseFare * surgeMultiplier;
    return baseFare + surgeCharge;
}

// --- MODULE 1: BOOK GRAB RIDE ---
// Simulates booking a ride, passenger input, and driver matching
void bookGrabRide() {
    string passengerName, pickup, destination;
    int pax;
    double distance, baseFare, surgeCharge, totalFare;
    char rain, peak;

    cout << "\n--- BOOK GRABRIDE ---" << endl;

    // Get passenger details
    cout << "Enter Passenger Name: ";
    getline(cin, passengerName);
    
    cout << "Enter Pickup Location: ";
    getline(cin, pickup);
    
    cout << "Enter Destination: ";
    getline(cin, destination);

    // Input Validation for Passengers
    // Grab's fleet ranges from GrabCar (4-seat) to Grab6 (6-seat).
    // Validating passenger counts prevents impossible bookings and
    // mirrors the real app's group-size awareness.
    while (true) {
        cout << "Number of Passengers (" << MIN_PASSENGERS << "-" << MAX_PASSENGERS << "): ";
        cin >> pax;
        if (cin.fail() || pax < MIN_PASSENGERS || pax > MAX_PASSENGERS) {
            cin.clear();
            cin.ignore(BUFFER_SIZE, '\n');
            cout << "[!] Invalid input. Vehicles can only fit "
                 << MIN_PASSENGERS << " to " << MAX_PASSENGERS << " passengers.\n";
        } else {
            cin.ignore(BUFFER_SIZE, '\n');
            break;
        }
    }

    // Input Validation for Distance (Must be strictly greater than 0)
    while (true) {
        cout << "Distance (km): ";
        cin >> distance;
        if (cin.fail() || distance <= 0) {
            cin.clear();
            cin.ignore(BUFFER_SIZE, '\n');
            cout << "[!] Invalid input. Distance must be a positive number.\n";
        } else {
            cin.ignore(BUFFER_SIZE, '\n'); 
            break;
        }
    }

    // Get environment variables for dynamic pricing
    rain = getYesNoInput("Is it raining? (Y/N): ");
    peak = getYesNoInput("Is it peak hour? (Y/N): ");

    // Calculate final fares via modular helper function
    totalFare = calculateTotalFare(distance, rain, peak, baseFare, surgeCharge);

    // Vehicle Recommendation Logic
    // Grab's initial fleet strategy: GrabCar for standard groups (≤4),
    // Grab6 for larger parties — targeting segments traditional taxis
    // could not efficiently serve with standard sedans.
    string vehicleType = (pax <= GRABCAR_PAX_LIMIT) ? "GrabCar" : "Grab6";

    // Data Arrays for Driver Simulation
    string driverNames[] = {"Ahmad", "Siti", "Muthu", "Mei Ling", "John", "Ali", "Chong", "Fatima"};
    string grabCarModels[] = {"Perodua Bezza", "Toyota Vios", "Honda City", "Proton Saga", "Nissan Almera"};
    string grab6Models[] = {"Perodua Alza", "Toyota Innova", "Honda BR-V", "Mitsubishi Xpander"};

    // Randomly assign a driver from the array
    int numDrivers = sizeof(driverNames) / sizeof(driverNames[0]);
    string assignedDriver = driverNames[rand() % numDrivers];
    
    // Assign proper vehicle model based on capacity recommended
    string assignedVehicleModel;
    if (pax <= GRABCAR_PAX_LIMIT) {
        int numGrabCar = sizeof(grabCarModels) / sizeof(grabCarModels[0]);
        assignedVehicleModel = grabCarModels[rand() % numGrabCar];
    } else {
        int numGrab6 = sizeof(grab6Models) / sizeof(grab6Models[0]);
        assignedVehicleModel = grab6Models[rand() % numGrab6];
    }
    
    // Generate Random ETA (Estimated Time of Arrival)
    // Simulates the GPS-based ETA that traditional taxis could not provide —
    // a core element of Grab's technological advantage over incumbents.
    int eta = (rand() % (ETA_MAX - ETA_MIN + 1)) + ETA_MIN;

    // Generate Random Driver Rating (between 4.0 and 5.0)
    // All Grab drivers must maintain a minimum rating to stay on the platform —
    // this quality-control mechanism directly addresses the taxi safety crisis.
    double driverRating = 4.0 + (rand() % 101) / 100.0;

    // Display Booking Summary before confirmation.
    // Users review all details before committing — this mirrors
    // Grab's real booking flow where the app shows a fare preview
    // with a confirm button, giving users transparency and control.
    cout << "\n--- BOOKING SUMMARY ---" << endl;
    cout << "Passenger : " << passengerName << endl;
    cout << "Route     : " << pickup << " -> " << destination << endl;
    cout << "Vehicle   : " << vehicleType << " (Fits " << pax << " pax)" << endl;
    cout << "Driver    : " << assignedDriver << endl;
    cout << "Model     : " << assignedVehicleModel << endl;
    cout << "ETA       : " << eta << " minutes" << endl;
    cout << fixed << setprecision(2);
    cout << "Total Fare: RM " << totalFare;
    if (surgeCharge > 0) cout << " (incl. RM " << surgeCharge << " surge)";
    cout << endl;
    cout << "---------------------------" << endl;

    // Booking Confirmation — user must explicitly approve before committing.
    // If cancelled, nothing is saved to the struct, so Module 4 will
    // continue to show the previous valid booking (or no record).
    char confirm = getYesNoInput("Confirm Booking? (Y/N): ");
    if (confirm == 'N') {
        cout << "\n[!] Booking cancelled. Returning to main menu.\n" << endl;
        return; // Exit without saving — struct remains unchanged
    }

    // Store confirmed booking into the global struct for Module 4 recall
    lastBooking.passengerName = passengerName;
    lastBooking.driverName = assignedDriver;
    lastBooking.vehicleType = vehicleType + " (" + assignedVehicleModel + ")";
    lastBooking.pickupLocation = pickup;
    lastBooking.destination = destination;
    lastBooking.distance = distance;
    lastBooking.totalFare = totalFare;
    lastBooking.hasRecord = true;

    // Print Electronic Receipt
    cout << "\n=================================" << endl;
    cout << "         BOOKING CONFIRMED       " << endl;
    cout << "=================================" << endl;
    cout << "Passenger : " << passengerName << endl;
    cout << "Route     : " << pickup << " -> " << destination << endl;
    cout << "Vehicle   : " << vehicleType << " (Fits " << pax << " pax)" << endl;

    // Driver Match Output
    cout << "---------------------------------" << endl;
    cout << "Driver Found!" << endl;
    cout << "Driver    : " << assignedDriver << endl;
    cout << "Model     : " << assignedVehicleModel << endl;
    cout << "ETA       : " << eta << " minutes" << endl;

    // Driver Quality Information
    // Displays the quality-control features that differentiated Grab
    // from traditional taxis: verified licensing and customer ratings.
    // These directly address the safety concerns identified on the poster.
    cout << "Rating    : " << fixed << setprecision(1) << driverRating << " / 5.0" << endl;
    cout << "Verification : PSV Licensed" << endl;

    // Financial Breakdown
    cout << "---------------------------------" << endl;
    cout << "--- FARE BREAKDOWN ---" << endl;
    cout << fixed << setprecision(2);
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
// Allows users to check prices without confirming a booking
void estimateFare() {
    double distance, baseFare, surgeCharge, totalFare;
    char rain, peak;

    cout << "\n--- FARE ESTIMATOR ---" << endl;
    
    // Input Validation for Distance
    while (true) {
        cout << "Enter Distance (km): ";
        cin >> distance;
        if (cin.fail() || distance <= 0) {
            cin.clear();
            cin.ignore(BUFFER_SIZE, '\n');
            cout << "[!] Invalid input. Distance must be greater than 0.\n";
        } else {
            cin.ignore(BUFFER_SIZE, '\n'); 
            break;
        }
    }

    // Input Validation for Rain/Peak
    rain = getYesNoInput("Is it raining? (Y/N): ");
    peak = getYesNoInput("Is it peak hour? (Y/N): ");

    // Call helper function
    totalFare = calculateTotalFare(distance, rain, peak, baseFare, surgeCharge);

    // Display Estimation Output
    cout << "\n--- ESTIMATION RESULT ---" << endl;
    cout << fixed << setprecision(2);
    cout << "Estimated Base Fare: RM " << baseFare << endl;
    if (surgeCharge > 0) cout << "Estimated Surge    : RM " << surgeCharge << endl;
    cout << "TOTAL ESTIMATED    : RM " << totalFare << endl;
    cout << "-------------------------\n" << endl;
}

// --- MODULE 3: GRABFOOD ORDER ---
// Simulates ecosystem expansion from transportation to food delivery.
// GrabFood demonstrates Christensen's "entrant improves along trajectory"
// stage — Grab extended its platform beyond ride-hailing into adjacent
// services, increasing its value proposition against single-service taxis.
void orderGrabFood() {
    int restaurantChoice, foodChoice, quantity;
    double distance, foodCost, deliveryFee, totalAmount;
    string restaurantName, foodName;

    cout << "\n--- GRABFOOD ORDER ---" << endl;
    cout << "Select a Restaurant:" << endl;
    cout << "1. McDonald's" << endl;
    cout << "2. KFC" << endl;
    cout << "3. Pizza Hut" << endl;

    // Input Validation for Restaurant Selection
    while (true) {
        cout << "Enter your choice (1-3): ";
        cin >> restaurantChoice;
        if (cin.fail() || restaurantChoice < 1 || restaurantChoice > 3) {
            cin.clear();
            cin.ignore(BUFFER_SIZE, '\n');
            cout << "[!] Invalid choice. Please select a valid restaurant option (1, 2, or 3).\n";
        } else {
            cin.ignore(BUFFER_SIZE, '\n');
            break;
        }
    }

    // Display menu items based on restaurant choice.
    // Each restaurant offers 3 items at different price points,
    // mirroring GrabFood's real-world restaurant partner model
    // where users browse menus before ordering.
    cout << "\n--- MENU ---" << endl;
    switch (restaurantChoice) {
        case 1:
            restaurantName = "McDonald's";
            cout << "1. Big Mac                 RM 10.50" << endl;
            cout << "2. McChicken Meal          RM 12.90" << endl;
            cout << "3. Double Cheeseburger     RM  8.50" << endl;
            break;
        case 2:
            restaurantName = "KFC";
            cout << "1. Zinger Burger Combo     RM 14.90" << endl;
            cout << "2. 2-Piece Chicken         RM 12.50" << endl;
            cout << "3. Snack Plate             RM 16.90" << endl;
            break;
        case 3:
            restaurantName = "Pizza Hut";
            cout << "1. Personal Pan Pizza      RM 16.90" << endl;
            cout << "2. Regular Pizza           RM 27.90" << endl;
            cout << "3. Large Pizza             RM 35.90" << endl;
            break;
    }

    // Input Validation for Food Item Selection
    while (true) {
        cout << "\nSelect a food item (1-3): ";
        cin >> foodChoice;
        if (cin.fail() || foodChoice < 1 || foodChoice > 3) {
            cin.clear();
            cin.ignore(BUFFER_SIZE, '\n');
            cout << "[!] Invalid choice. Please select a valid menu item (1, 2, or 3).\n";
        } else {
            cin.ignore(BUFFER_SIZE, '\n');
            break;
        }
    }

    // Input Validation for Quantity
    while (true) {
        cout << "Enter quantity: ";
        cin >> quantity;
        if (cin.fail() || quantity < 1) {
            cin.clear();
            cin.ignore(BUFFER_SIZE, '\n');
            cout << "[!] Invalid input. Quantity must be at least 1.\n";
        } else {
            cin.ignore(BUFFER_SIZE, '\n');
            break;
        }
    }

    // Assign food name and unit price based on combined restaurant + item choice
    switch (restaurantChoice) {
        case 1: // McDonald's
            if (foodChoice == 1)      { foodName = "Big Mac";             foodCost = 10.50; }
            else if (foodChoice == 2) { foodName = "McChicken Meal";      foodCost = 12.90; }
            else                       { foodName = "Double Cheeseburger"; foodCost = 8.50;  }
            break;
        case 2: // KFC
            if (foodChoice == 1)      { foodName = "Zinger Burger Combo"; foodCost = 14.90; }
            else if (foodChoice == 2) { foodName = "2-Piece Chicken";     foodCost = 12.50; }
            else                       { foodName = "Snack Plate";         foodCost = 16.90; }
            break;
        case 3: // Pizza Hut
            if (foodChoice == 1)      { foodName = "Personal Pan Pizza";  foodCost = 16.90; }
            else if (foodChoice == 2) { foodName = "Regular Pizza";       foodCost = 27.90; }
            else                       { foodName = "Large Pizza";         foodCost = 35.90; }
            break;
    }

    foodCost *= quantity; // Multiply unit price by quantity ordered

    // Input Validation for Delivery Distance
    while (true) {
        cout << "Delivery Distance (km): ";
        cin >> distance;
        if (cin.fail() || distance <= 0) {
            cin.clear();
            cin.ignore(BUFFER_SIZE, '\n');
            cout << "[!] Invalid input. Distance must be a positive number.\n";
        } else {
            cin.ignore(BUFFER_SIZE, '\n');
            break;
        }
    }

    // Delivery fee logic based on distance.
    // Simple two-tier system: nearby deliveries cost less,
    // incentivising users to order from nearby restaurants
    // and keeping delivery economically viable for Grab.
    if (distance <= 5.0) {
        deliveryFee = DELIVERY_FEE_NEAR;
    } else {
        deliveryFee = DELIVERY_FEE_FAR;
    }

    totalAmount = foodCost + deliveryFee;

    // Print Receipt
    cout << "\n=================================" << endl;
    cout << "       GRABFOOD RECEIPT          " << endl;
    cout << "=================================" << endl;
    cout << "Restaurant : " << restaurantName << endl;
    cout << "Item       : " << quantity << "x " << foodName << endl;
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
// Recalls struct data from memory (if a ride was successfully booked)
void viewLastBooking() {
    cout << "\n--- LAST BOOKING RECORD ---" << endl;
    
    // Check struct flag
    if (!lastBooking.hasRecord) {
        cout << "[!] No booking records found. Please book a ride first.\n" << endl;
        return;
    }

    // Display struct data
    cout << "Passenger : " << lastBooking.passengerName << endl;
    cout << "Driver    : " << lastBooking.driverName << endl;
    cout << "Vehicle   : " << lastBooking.vehicleType << endl;
    cout << "Route     : " << lastBooking.pickupLocation << " -> " << lastBooking.destination << endl;
    cout << "Distance  : " << lastBooking.distance << " km" << endl;
    cout << fixed << setprecision(2);
    cout << "Total Fare: RM " << lastBooking.totalFare << endl;
    cout << "---------------------------\n" << endl;
}

// --- MODULE 5: ABOUT GRAB INNOVATION ---
// Bridges Part 1 (poster) and Part 2 (this program) by explaining
// the academic context: how Grab exemplifies Christensen's
// Disruptive Innovation Model and how this console simulator
// represents its key technological features.
void aboutGrab() {
    cout << "\n=================================" << endl;
    cout << "     ABOUT GRAB INNOVATION       " << endl;
    cout << "=================================" << endl;

    // Introduction — what Grab is and its market position
    cout << "[ INTRODUCTION ]" << endl;
    cout << "Grab (formerly MyTeksi) is Southeast Asia's leading" << endl;
    cout << "superapp, offering ride-hailing, food delivery," << endl;
    cout << "digital payments, and financial services across" << endl;
    cout << "8 countries serving over 35 million users." << endl;
    cout << endl;

    // Problem — why Grab was created
    cout << "[ THE PROBLEM ]" << endl;
    cout << "KL taxis ranked worst out of 23 countries due to" << endl;
    cout << "overcharging, meter non-compliance, and safety" << endl;
    cout << "threats. By 2014, SPAD received 10,000 taxi-related" << endl;
    cout << "complaints annually (Razali & Hands, 2016)." << endl;
    cout << endl;

    // Christensen's model — how the theory applies
    cout << "[ DISRUPTIVE INNOVATION MODEL ]" << endl;
    cout << "Christensen (1997) describes how entrants disrupt" << endl;
    cout << "incumbents by targeting overlooked segments with" << endl;
    cout << "simpler, cheaper, or more accessible alternatives." << endl;
    cout << "Grab applied this model to Southeast Asian transport." << endl;
    cout << endl;

    // Disruption mechanism
    cout << "[ HOW GRAB DISRUPTED THE TAXI INDUSTRY ]" << endl;
    cout << "- Upfront fares eliminated meter uncertainty" << endl;
    cout << "- GPS tracking provided safety and accountability" << endl;
    cout << "- Driver ratings incentivised service quality" << endl;
    cout << "- Cashless payments (GrabPay) enabled financial inclusion" << endl;
    cout << endl;

    // Dynamic pricing
    cout << "[ DYNAMIC PRICING ]" << endl;
    cout << "Unlike fixed taxi meters, Grab adjusts fares based" << endl;
    cout << "on real-time demand. Rain and peak-hour surges" << endl;
    cout << "incentivise more drivers onto the road, reducing" << endl;
    cout << "wait times for passengers. This simulator applies" << endl;
    cout << "a +20% or +50% multiplier depending on conditions." << endl;
    cout << endl;

    // Ecosystem expansion
    cout << "[ ECOSYSTEM EXPANSION — GRABFOOD ]" << endl;
    cout << "After capturing ride-hailing, Grab expanded into" << endl;
    cout << "food delivery (2018), leveraging its existing driver" << endl;
    cout << "network. This 'superapp' strategy locks in users" << endl;
    cout << "and raises switching costs — a hallmark of" << endl;
    cout << "successful platform disruption." << endl;
    cout << endl;

    // Conclusion
    cout << "[ CONCLUSION ]" << endl;
    cout << "This simulator recreates Grab's core features to" << endl;
    cout << "demonstrate how technology can transform an entire" << endl;
    cout << "industry. Each module (booking, fare estimation," << endl;
    cout << "food delivery) represents one dimension of the" << endl;
    cout << "disruptive innovation discussed in the poster." << endl;
    cout << "=================================\n" << endl;
}
