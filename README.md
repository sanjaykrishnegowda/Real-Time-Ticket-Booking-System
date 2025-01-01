# Real Time Ticket Booking System

A command-line-based **Real-Time Ticket Booking System** that enables users to search, book, and manage tickets for various modes of transport like buses, trains, and flights. The system also supports user authentication, ticket cancellation, and viewing booking history.

## Features

- **User Authentication:** Register and log in with a username and password.
- **Search Tickets:** Find available tickets based on transport type, origin, destination, and date.
- **Book Tickets:** Book tickets and generate a ticket file with detailed booking information.
- **Cancel Tickets:** Cancel booked tickets and update the cancellation history.
- **View History:** View booked and canceled tickets for a specific user.
- **Payment Integration:** Supports various payment methods for ticket booking.

---

## Requirements

- **C++ Compiler** (e.g., GCC, Visual Studio)
- Basic file handling for user data and ticket data storage

---

## How to Run

1. Clone the repository:

   ```bash
   git clone https://github.com/yourusername/ticket-booking-system.git
   cd ticket-booking-system
   ```

2. Compile the code:

   ```bash
   g++ -o ticket_booking main.cpp
   ```

3. Run the executable:

   ```bash
   ./ticket_booking
   ```

---

## Menu Options and Outputs

### **1. Search Tickets**

- **Description:** Find available tickets based on user input.
- **Input Fields:** Transport type, origin, destination, and date.
- **Sample Output:**

   ![Search Tickets Output](![image](https://github.com/user-attachments/assets/8298183a-5fce-41eb-a954-fa9906bef593)
)

---

### **2. Book Ticket**

- **Description:** Book a ticket from the available options.
- **Input Fields:** Transport type, origin, destination, date, and confirmation.
- **Sample Output:**

   ![Book Ticket Output](images/book_ticket.png)

   After booking, a ticket file (`ticket_<TicketID>.txt`) is generated with the details.

---

### **3. Cancel Booking**

- **Description:** Cancel a booked ticket by providing the ticket ID.
- **Input Fields:** Ticket ID and confirmation.
- **Sample Output:**

   ![Cancel Booking Output](images/cancel_booking.png)

---

### **4. View Booked Tickets**

- **Description:** View the list of tickets booked by the current user.
- **Sample Output:**

   ![View Booked Tickets Output](images/view_booked_tickets.png)

---

### **5. View Canceled Tickets**

- **Description:** View the list of tickets canceled by the current user.
- **Sample Output:**

   ![View Canceled Tickets Output](images/view_canceled_tickets.png)

---

### **6. Exit**

- **Description:** Exit the application.

---

## File Structure

- **main.cpp:** Main code file for the Ticket Booking System.
- **users.txt:** Stores user credentials for authentication.
- **routes.txt:** Stores ticket details (ID, origin, destination, date, transport type, price, and availability).
- **ticket_<TicketID>.txt:** Generated ticket file for each booked ticket.
- **<username>_booked.txt:** File storing booking history of a user.
- **<username>_cancelled.txt:** File storing cancellation history of a user.

---

## Future Enhancements

- Add real-time seat availability.
- Improve error handling.
- Implement a graphical user interface (GUI).

---

## Contributing

Feel free to contribute by submitting issues or pull requests. Ensure your changes are well-documented and tested.

---

## License

This project is licensed under the [MIT License](LICENSE).

