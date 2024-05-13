#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <bits/stdc++.h>
#include "nlohmann/json.hpp"
using json = nlohmann::json;
using namespace std;

// Define a map to track room availability
map<int, bool> roomAvailability;

struct Booking {
  string name;
  int roomNumber;
};

vector<Booking> bookings;

const string ADMIN_PASSWORD = "admin123"; // Password for admin

// Function to display all bookings
void displayBookings() {
  cout << "Current bookings:" << endl;
  for (const Booking& booking : bookings) {
    cout << "Name: " << booking.name << ", Room Number: " << booking.roomNumber << endl;
  }
}

// Function to display available rooms
void displayAvailableRooms() {
  cout << "Available rooms:" << endl;
  for (const auto& room : roomAvailability) {
    if (room.second) {
      cout << "Room Number: " << room.first << endl;
    }
  }
}

// Function to add a booking
void addBooking(const string& userName) {
  Booking newBooking;
  newBooking.name = userName;
  
  displayAvailableRooms();
  
  cout << "Enter room number: ";
  cin >> newBooking.roomNumber;
  cin.ignore(); // Clear the input buffer
  
  auto it = roomAvailability.find(newBooking.roomNumber);
  if (it == roomAvailability.end()) {
    cout << "Invalid room number." << endl;
    return;
  }
  
  if (!it->second) {
    cout << "Room " << newBooking.roomNumber << " is already booked. Please choose another room." << endl;
    return;
  }
  
  it->second = false; // Mark room as booked
  
  bookings.push_back(newBooking);
  cout << "Booking added successfully." << endl;
}

// Function to cancel a booking
void cancelBooking() {
  int roomNumber;
  cout << "Enter room number to cancel booking: ";
  cin >> roomNumber;
  cin.ignore(); // Clear the input buffer
  
  auto it = roomAvailability.find(roomNumber);
  if (it != roomAvailability.end()) {
    it->second = true; // Mark room as available
    
    for (auto bookIt = bookings.begin(); bookIt != bookings.end(); ++bookIt) {
      if (bookIt->roomNumber == roomNumber) {
        bookings.erase(bookIt);
        cout << "Booking cancelled successfully." << endl;
        return;
      }
    }
  }
  
  cout << "Booking not found." << endl;
}

// Function to cancel a booking for guests
void cancelBookingGuest(const string& userName) {
  cout << "Your bookings:" << endl;
  bool foundBooking = false;
  for (auto it = bookings.begin(); it != bookings.end(); ++it) {
    if (it->name == userName) {
      cout << "Room Number: " << it->roomNumber << endl;
      foundBooking = true;
    }
  }
  
  if (!foundBooking) {
    cout << "You have no bookings to cancel." << endl;
    return;
  }
  
  int roomNumber;
  cout << "Enter room number to cancel booking: ";
  cin >> roomNumber;
  cin.ignore(); // Clear the input buffer
  
  auto it = roomAvailability.find(roomNumber);
  if (it != roomAvailability.end()) {
    it->second = true; // Mark room as available
    
    for (auto bookIt = bookings.begin(); bookIt != bookings.end(); ++bookIt) {
      if (bookIt->roomNumber == roomNumber && bookIt->name == userName) {
        bookings.erase(bookIt);
        cout << "Booking cancelled successfully." << endl;
        return;
      }
    }
  }
  
  cout << "Booking not found or you do not have permission to cancel this booking." << endl;
}

// Function for guest menu
void guestMenu(const string& guestName) {
  cout << "Welcome, " << guestName << "!" << endl;
    
  bool exit = false;
  int choice;
  while (!exit) {
    cout << "\n1. Display available rooms" << endl;
    cout << "2. Book a room" << endl;
    cout << "3. Cancel booking" << endl; // Added cancel booking option
    cout << "4. Exit" << endl;
    cout << "Enter your choice: ";
    cin >> choice;
    cin.ignore(); // Clear the input buffer
    switch (choice) {
      case 1:
        displayAvailableRooms();
        break;
      case 2:
        addBooking(guestName);
        break;
      case 3:
        cancelBookingGuest(guestName); // Call cancelBookingGuest for guests
        break;
      case 4:
        cout << "Exiting..." << endl;
        exit = true;
        break;
      default:
        cout << "Invalid choice. Please enter a number between 1 and 4." << endl;
    }
  }
}

// Function to save bookings to file
void saveBookingsToFile() {
  json jsonData;
  for (const Booking& booking : bookings) {
    jsonData.push_back({{"name", booking.name}, {"roomNumber", booking.roomNumber}});
  }

  ofstream fout("bookings.txt");
  if (fout.is_open()) {
    fout << jsonData.dump(2); // Write formatted JSON with indentation
    fout.close();
  } else {
    cerr << "Unable to open file to save bookings." << endl;
  }
}

// Function to load bookings from file and update roomAvailability
void loadBookingsFromFile() {
  ifstream fin("bookings.txt");
  if (fin.is_open()) {
    json jsonData;
    fin >> jsonData;
    fin.close();

    bookings.clear(); // Clear existing bookings
    roomAvailability.clear(); // Clear existing room availability
    
    // Initialize room availability with all rooms initially available
    for (int roomNumber = 101; roomNumber <= 107; ++roomNumber) {
      roomAvailability[roomNumber] = true;
    }

    for (const auto& bookingJson : jsonData) {
      Booking booking;
      booking.name = bookingJson["name"].get<string>();
      booking.roomNumber = bookingJson["roomNumber"].get<int>();
      bookings.push_back(booking);
      
      // Mark booked room as unavailable
      auto it = roomAvailability.find(booking.roomNumber);
      if (it != roomAvailability.end()) {
        it->second = false;
      }
    }
  }
}

int main() {
  loadBookingsFromFile(); // Load existing bookings from file
  
  cout << "Welcome to the hotel booking system!" << endl;
  cout << "Are you a guest or an administrator?" << endl;
  cout << "Enter 'guest' or 'admin': ";
  string role;
  cin >> role;
  cin.ignore(); // Clear the input buffer

  string guestName;
  string password;
  
  if (role == "guest") {
    cout << "Enter your name: ";
    getline(cin, guestName);
    guestMenu(guestName); // Call guestMenu function
  } else if (role == "admin") {
    cout << "Enter admin password: ";
    string password;
    getline(cin, password);
    
    if (password == ADMIN_PASSWORD) {
      cout << "Welcome, admin!" << endl;
      
      bool exit = false;
      int choice;
      while (!exit) {
        cout << "\n1. Display bookings" << endl;
        cout << "2. Display available rooms" << endl;
        cout << "3. Book a room" << endl;
        cout << "4. Cancel booking" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // Clear the input buffer
        switch (choice) {
          case 1:
            displayBookings();
            break;
          case 2:
            displayAvailableRooms();
            break;
          case 3:
            cout << "Enter guest name: ";
            getline(cin, guestName);
            addBooking(guestName);
            break;
          case 4:
            cancelBooking();
            break;
          case 5:
            cout << "Exiting..." << endl;
            exit = true;
            break;
          default:
            cout << "Invalid choice. Please enter a number between 1 and 5." << endl;
        }
      }
    } else {
      cout << "Incorrect password. Access denied." << endl;
    }
  } else {
    cout << "Invalid role. Exiting..." << endl;
  }

  saveBookingsToFile(); // Save bookings to file before exiting
  return 0;
}
