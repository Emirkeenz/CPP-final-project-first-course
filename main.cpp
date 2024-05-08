#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include "nlohmann/json.hpp"
using json = nlohmann::json;
using namespace std;

struct Booking {
  string name;
  int roomNumber;

  // Serialize booking to JSON
  void toJson(json& jsonData) const {
    jsonData = json{{"name", name}, {"roomNumber", roomNumber}};
  }

  // Deserialize booking from JSON
  void fromJson(const json& jsonData) {
    jsonData.at("name").get_to(name);
    jsonData.at("roomNumber").get_to(roomNumber);
  }
};

vector<Booking> bookings;

// Function to display all bookings
void displayBookings() {
  cout << "Current bookings:" << endl;
  for (const Booking& booking : bookings) {
    cout << "Name: " << booking.name << ", Room Number: " << booking.roomNumber << endl;
  }
}

// Function to add a booking
void addBooking() {
  Booking newBooking;
  cout << "Enter name: ";
  getline(cin, newBooking.name);
  cout << "Enter room number: ";
  cin >> newBooking.roomNumber;
  cin.ignore(); // Clear the input buffer
  bookings.push_back(newBooking);
  cout << "Booking added successfully." << endl;
}

// Function to cancel a booking
void cancelBooking() {
  int roomNumber;
  cout << "Enter room number to cancel booking: ";
  cin >> roomNumber;
  cin.ignore(); // Clear the input buffer
  for (auto it = bookings.begin(); it != bookings.end(); ++it) {
    if (it->roomNumber == roomNumber) {
      bookings.erase(it);
      cout << "Booking cancelled successfully." << endl;
      return;
    }
  }
  cout << "Booking not found." << endl;
}

// Function to save bookings to file
void saveBookingsToFile() {
  json j;
  for (const Booking& booking : bookings) {
    json bookingJson;
    booking.toJson(bookingJson);
    j.push_back(bookingJson);
  }

  ofstream outFile("bookings.txt");
  if (outFile.is_open()) {
    outFile << j.dump(4); // Write formatted JSON with indentation
    outFile.close();
  } else {
    cerr << "Unable to open file to save bookings." << endl;
  }
}

// Function to load bookings from file
void loadBookingsFromFile() {
  ifstream inFile("bookings.txt");
  if (inFile.is_open()) {
    json j;
    inFile >> j;
    inFile.close();

    bookings.clear(); // Clear existing bookings

    for (const auto& bookingJson : j) {
      Booking booking;
      booking.name = bookingJson["name"].get<string>();
      booking.roomNumber = bookingJson["roomNumber"].get<int>();
      bookings.push_back(booking);
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