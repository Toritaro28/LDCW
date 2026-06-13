#include <iostream>
#include <string>
#include <limits> // Required for clearing input buffer (cin.ignore)

using namespace std;

// ==========================================
// FUNCTION PROTOTYPES (Declarations)
// ==========================================
void displayMenu();
void bookGrabRide();
void estimateFare();
void orderGrabFood();
void viewLastBooking();
void aboutGrab();

// ==========================================
// MAIN FUNCTION
// ==========================================
int main() {
    int userChoice = 0;

    // The do-while loop keeps the application running until user chooses to exit (6)
    do {
        displayMenu();
        cout << "Please select an option (1-6): ";
        cin >> userChoice;

        // ERROR HANDLING: Check if user entered a letter or symbol instead of a number
        if (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "\n[!] ERROR: Invalid input. Please enter a number." << endl;
            continue; // Skip the rest of the loop and show the menu again
        }

        // Navigate to the selected module
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

// Placeholder for Module 1
void bookGrabRide() {
    cout << "\n---> [Module 1: Book GrabRide running...]" << endl;
    // TODO in Commit 2: Add passenger input and surge pricing logic here.
}

// Placeholder for Module 2
void estimateFare() {
    cout << "\n---> [Module 2: Fare Estimator running...]" << endl;
    // TODO in Commit 2: Add fare estimation logic here.
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