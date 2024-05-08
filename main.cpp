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
void addBooking() {
  Booking newBooking;
  cout << "Enter name: ";
  getline(cin, newBooking.name);
  
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
  
  bool exit = false;
  int choice;
  while (!exit) {
    cout << "\n1. Display bookings" << endl;
    cout << "2. Add booking" << endl;
    cout << "3. Cancel booking" << endl;
    cout << "4. Exit" << endl;
    cout << "Enter your choice: ";
    cin >> choice;
    cin.ignore(); // Clear the input buffer
    switch (choice) {
      case 1:
        displayBookings();
        break;
      case 2:
        addBooking();
        break;
      case 3:
        cancelBooking();
        break;
      case 4:
        cout << "Exiting..." << endl;
        exit = true;
        break;
      default:
        cout << "Invalid choice. Please enter a number between 1 and 4." << endl;
    }
  }

  saveBookingsToFile(); // Save bookings to file before exiting
  return 0;
}
