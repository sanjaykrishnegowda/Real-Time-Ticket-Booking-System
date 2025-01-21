#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <iomanip>
#include <limits>
using namespace std;

// Class to manage tickets
class Ticket {
public:
//data members of Ticket class
    int ticketID;
    string origin;
    string destination;
    string date;
    bool isAvailable;
    string transportType;
    double price; 

// constructor to initialize ticket details 
    Ticket(int id, string org, string dest, string dt, string transport, double p, bool available = true)
        : ticketID(id), origin(org), destination(dest), date(dt), transportType(transport), price(p), isAvailable(available) {}
};

// Class to manage booking system
class BookingSystem {
private:
    vector<Ticket> tickets;              // List of tickets
    unordered_map<int, string> bookings; // TicketID -> Customer Name

// Load tickets from file
    void loadTickets() {
        ifstream file("routes.txt");
        if (!file.is_open()) {
            cerr << "Error: Could not open tickets file.\n";
            return;
        }
    // Read ticket details from file
        int id, available;
        string org, dest, date, transport;
        double price;
        while (file >> id >> org >> dest >> date >> transport >> price >> available) {
            tickets.emplace_back(id, org, dest, date, transport, price, available == 1);
        }
        file.close();
    }

    // Save tickets to file
    void saveTickets() {
        ofstream file("routes.txt");
        if (!file.is_open()) {
            cerr << "Error: Could not save tickets file.\n";
            return;
        }
// Write ticket details to file
        for (const auto& ticket : tickets) {
            file << ticket.ticketID << " " << ticket.origin << " " << ticket.destination << " "
                 << ticket.date << " " << ticket.transportType << " " << ticket.price << " "
                 << (ticket.isAvailable ? 1 : 0) << "\n";
        }
        file.close();
    }

    // Function to generate a ticket after booking
    void generateTicket(const Ticket& ticket, const string& customerName) {
        // Create a file named ticket_<ticketID>.txt
        ofstream ticketFile("ticket_" + to_string(ticket.ticketID) + ".txt");
        if (!ticketFile.is_open()) {
            cerr << "Error generating ticket file.\n";
            return;
        }

        // Write ticket details to the file
        ticketFile << "==== TICKET ====\n";
        ticketFile << "Ticket ID: " << ticket.ticketID << "\n";
        ticketFile << "Customer: " << customerName << "\n";
        ticketFile << "Origin: " << ticket.origin << "\n";
        ticketFile << "Destination: " << ticket.destination << "\n";
        ticketFile << "Date: " << ticket.date << "\n";
        ticketFile << "Transport: " << ticket.transportType << "\n";
        ticketFile << "Price: INR " << fixed << setprecision(2) << ticket.price << "\n";
        ticketFile << "Status: Booked\n";
        ticketFile << "****************\n";

        ticketFile.close();
        cout << "Ticket generated successfully! Check 'ticket_" << ticket.ticketID << ".txt' for your ticket details.\n";
    }

public:
// constructor to load tickets
    BookingSystem() {
        loadTickets();
    }
// destructor to save tickets
    ~BookingSystem() {
        saveTickets();
    }
// function to search tickets
    void searchTickets(const string& transport, const string& origin, const string& destination, const string& date) {
        bool found = false;
        cout << "\nAvailable " << transport << " Tickets:\n";
        cout << setw(10) << "TicketID" << setw(15) << "Origin" << setw(15) << "Destination"
             << setw(15) << "Date" << setw(15) << "Transport" << setw(10) << "Price" << setw(10) << "Status" << "\n";
// Display available tickets based on the search criteria
        for (const auto& ticket : tickets) {
            if (ticket.transportType == transport && ticket.origin == origin && ticket.destination == destination
                && ticket.date == date && ticket.isAvailable) {
                cout << setw(10) << ticket.ticketID << setw(15) << ticket.origin
                     << setw(15) << ticket.destination << setw(15) << ticket.date
                     << setw(15) << ticket.transportType << setw(10) << ticket.price
                     << setw(10) << "Available" << "\n";
                found = true;
            }
        }

        if (!found) {
            cout << "No tickets available for the given criteria.\n";
        }
    }
// function to book tickets
    void bookTicket(const string& transport, const string& origin, const string& destination, const string& date, const string& customerName) {
    bool found = false;
    for (auto& ticket : tickets) {
        if (ticket.transportType == transport && ticket.origin == origin && ticket.destination == destination
            && ticket.date == date && ticket.isAvailable) {
            cout << "\nTicket found: " << ticket.ticketID << " " << ticket.origin << " -> " << ticket.destination
                 << " on " << ticket.date << " (" << ticket.transportType << ")\n";
            cout << "Price: INR " << ticket.price << "\n";
            cout << "Do you want to book this ticket? (y/n): ";
            char choice;
            cin >> choice;

            // Handle invalid input for booking confirmation
            while (choice != 'y' && choice != 'Y' && choice != 'n' && choice != 'N') {
                cout << "Invalid choice. Please enter 'y' for yes or 'n' for no: ";
                cin >> choice;
            }

            if (choice == 'y' || choice == 'Y') {
                // Ask for payment after booking
                if (payForTicket(ticket.price)) {
                    ticket.isAvailable = false;
                    bookings[ticket.ticketID] = customerName;
                    cout << "Ticket booked successfully for " << customerName << "!\n";

                    // Save to user's booked ticket history
                    ofstream bookedFile(customerName + "_booked.txt", ios::app);
                    if (bookedFile.is_open()) {
                        bookedFile << ticket.ticketID << " " << ticket.origin << " -> " << ticket.destination
                                   << " on " << ticket.date << " (" << ticket.transportType << ")\n";
                        bookedFile.close();
                    } else {
                        cerr << "Error opening booked file for " << customerName << endl;
                    }

                    // Generate ticket after booking
                    generateTicket(ticket, customerName);

                    // Save the updated tickets to routes.txt
                    saveTickets();

                    found = true;
                    break;
                } else {
                    cout << "Payment failed. Booking was not completed.\n";
                }
            } else {
                cout << "Booking canceled.\n";
            }
        }
    }

    if (!found) {
        cout << "No available tickets found for the given criteria.\n";
    }
}

// function to pay for tickets
  bool payForTicket(double ticketPrice) {
    // Automatically set the payment to the ticket price
    double payment = ticketPrice;

    // Display the payment options
    cout << "\nSelect a payment method:\n";
    cout << "1. Debit Card\n";
    cout << "2. UPI Payment\n";
    cout << "3. Gpay/Phonepay\n";
    
    int choice;
    string paymentMethod;
    
    while (true) { // Loop until a valid option is selected
        cout << "Enter your choice (1/2/3): ";
        cin >> choice;
         // Check for invalid input (non-numeric input)
        if (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Invalid choice, please enter a valid option (1-3).\n";
        }

        if (choice == 1) {
            paymentMethod = "Debit Card";
            break;
        } else if (choice == 2) {
            paymentMethod = "UPI Payment";
            break;
        } else if (choice == 3) {
            paymentMethod = "Gpay/Phonepay";
            break;
        } else {
            cout << "Invalid choice. Please select a valid option (1/2/3).\n";
        }
    }

    // Confirm the payment
    cout << "\nYou have selected " << paymentMethod << " as your payment method.\n";
    cout << "The total payment amount is: INR " << fixed << setprecision(2) << payment << "\n";
    cout << "Do you confirm the payment of INR " << fixed << setprecision(2) << payment << "? (y/n): ";
    
    char confirm;
    cin >> confirm;
    // Handle invalid input for booking confirmation
            while (confirm != 'y' && confirm != 'Y' && confirm != 'n' && confirm != 'N') {
                cout << "Invalid choice. Please enter 'y' for yes or 'n' for no: ";
                cin >> confirm;
            }


    if (confirm == 'y' || confirm == 'Y') {
        cout << "\nPayment of INR " << fixed << setprecision(2) << payment << " via " << paymentMethod << " successful.\n";
        return true;  // Payment is successful
    } else {
        cout << "Payment canceled. Booking not completed.\n";
        return false;  // Payment is canceled
    }
}

// function to cancel booking
    void cancelBooking(int ticketID, const string& customerName) {
        bool found = false;
        for (auto& ticket : tickets) {
            if (ticket.ticketID == ticketID && !ticket.isAvailable) {
                cout << "\nDo you want to cancel this booking? (y/n): ";
                char choice;
                cin >> choice;
                  // Handle invalid input for booking confirmation
            while (choice != 'y' && choice != 'Y' && choice != 'n' && choice != 'N') {
                cout << "Invalid choice. Please enter 'y' for yes or 'n' for no: ";
                cin >> choice;
            }
                if (choice == 'y' || choice == 'Y') {
                    ticket.isAvailable = true;
                    bookings.erase(ticket.ticketID);
              // Update user's booking history
                std::ofstream updateHistory(customerName + "_booked.txt", std::ios::app);
                if (updateHistory.is_open()) {
                    updateHistory << "Past Booking Ticket Cancelled\n";
                    updateHistory.close();
                } else {
                    std::cerr << "Error opening Booked file for " << customerName << std::endl;
                }

                    // Save to user's canceled ticket history
                    ofstream cancelledFile(customerName + "_cancelled.txt", ios::app);
                    if (cancelledFile.is_open()) {
                        cancelledFile << ticket.ticketID << " " << ticket.origin << " -> " << ticket.destination
                                      << " on " << ticket.date << " (" << ticket.transportType << ")\n";
                        cancelledFile.close();
                    } else {
                        cerr << "Error opening cancelled file for " << customerName << endl;
                    }

                    cout << "Booking canceled.\n";

                    // Save the updated tickets to routes.txt
                    saveTickets();

                    found = true;
                }
                break;
            }
        }

        if (!found) {
            cout << "Ticket not found.\n";
        }
    }
// function to view booked tickets
    void viewBookedTickets(const string& customerName) {
        bool found = false;
        cout << "\nBooked Tickets for " << customerName << ":\n";
        cout << setw(10) << "TicketID" << setw(15) << "Origin" << setw(15) << "Destination"
             << setw(15) << "Date" << setw(15) << "Transport" << setw(10) << "Price" << "\n";

        // Display user's booked tickets from the file
        ifstream bookedFile(customerName + "_booked.txt");
        if (bookedFile.is_open()) {
            string line;
            while (getline(bookedFile, line)) {
                cout << line << "\n";
                found = true;
            }
            bookedFile.close();
        } 
        if (!found) {
            cout << "  No booked tickets found for " << customerName << ".\n";
        }
    }
// function to view cancelled tickets
    void viewCancelledTickets(const string& customerName) {
        bool found = false;
        cout << "\nCancelled Tickets for " << customerName << ":\n";
        cout << setw(10) << "TicketID" << setw(15) << "Origin" << setw(15) << "Destination"
             << setw(15) << "Date" << setw(15) << "Transport" << setw(10) << "Price" << "\n";

        // Display user's cancelled tickets from the file
        ifstream cancelledFile(customerName + "_cancelled.txt");
        if (cancelledFile.is_open()) {
            string line;
            while (getline(cancelledFile, line)) {
                cout << line << "\n";
                found = true;
            }
            cancelledFile.close();
        } 
        if (!found) {
            cout << "  No canceled tickets found for " << customerName << ".\n";
        }
    }
};

// User authentication system
class Authentication {
public:
// function to authenticate user
    bool authenticate(string& currentUser) {
        unordered_map<string, string> users;
        ifstream infile("users.txt");
        string username, password;

        // Load users
        while (infile >> username >> password) {
            users[username] = password;
        }
        infile.close();

        char choice;
        while (true) {
            cout << "Do you have an account? (y/n): ";
            cin >> choice;

            // Check for valid input
            if (choice == 'y' || choice == 'Y') {
                // Login process
                while (true) {
                    cout << "Enter username: ";
                    cin >> username;
                    cout << "Enter password: ";
                    cin >> password;

                    if (users.find(username) != users.end() && users[username] == password) {
                        currentUser = username;
                        cout << "Login successful!\n";
                        return true;
                    } else {
                        cout << "Invalid credentials. Please enter valid credentials.\n";
                    }
                }
            } else if (choice == 'n' || choice == 'N') {
                // Registration process
                cout << "Register a new account.\n";
                cout << "Enter username: ";
                cin >> username;

                // Check if username already exists
                if (users.find(username) != users.end()) {
                    cout << "Username already exists. Please login or choose a different username.\n";
                    continue;
                }

                cout << "Enter password: ";
                cin >> password;

                ofstream outfile("users.txt", ios::app);
                outfile << username << " " << password << "\n";
                outfile.close();

                currentUser = username;
                cout << "Account created successfully!\n";
                return true;
            } else {
                cout << "Invalid input. Please enter 'y' or 'n'.\n";
            }
        }
    }
};
// main function
int main() {
    string currentUser;
    // object of Authentication class
    Authentication auth;
    if (!auth.authenticate(currentUser)) {
        cerr << "Authentication failed.\n";
        return 1;
    }
// object of BookingSystem class
    BookingSystem bookingSystem;

    while (true) {
        int choice;
        cout << "\n==== Ticket Booking System ====\n";
        cout << "1. Search Tickets\n2. Book Ticket\n3. Cancel Booking\n4. View Booked Tickets\n5. View Canceled Tickets\n6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        // Check for invalid input (non-numeric input)
        if (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Invalid choice, please enter a valid option (1-6).\n";
        } else if (choice == 1) {
            string transport, origin, destination, date;
            cout << "Enter transport type (Bus/Train/Flight): ";
            cin >> transport;
            cout << "Enter origin: ";
            cin >> origin;
            cout << "Enter destination: ";
            cin >> destination;
            cout << "Enter date (YYYY-MM-DD): ";
            cin >> date;

            bookingSystem.searchTickets(transport, origin, destination, date);
        } else if (choice == 2) {
            string transport, origin, destination, date, customerName;
            cout << "Enter transport type (Bus/Train/Flight) : ";
            cin >> transport;
            cout << "Enter origin: ";
            cin >> origin;
            cout << "Enter destination: ";
            cin >> destination;
            cout << "Enter date (YYYY-MM-DD): ";
            cin >> date;
            bookingSystem.bookTicket(transport, origin, destination, date, currentUser);
        } else if (choice == 3) {
            int ticketID;
            cout << "Enter ticket ID to cancel: ";
            cin >> ticketID;

            bookingSystem.cancelBooking(ticketID, currentUser);
        } else if (choice == 4) {
            bookingSystem.viewBookedTickets(currentUser);
        } else if (choice == 5) {
            bookingSystem.viewCancelledTickets(currentUser);
        } else if (choice == 6) {
            cout << "Exiting system.\n";
            break;
        } else {
            cout << "Invalid choice, please enter a valid option (1-6).\n";
        }
    }

    return 0;
}
