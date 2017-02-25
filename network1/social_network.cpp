/*
Author: Lung-Young Yu
*/

#include <cstring>
#include <string>
#include <iomanip>
#include <iostream>
#include "network.h"
#include "user.h"

using namespace std;

// Create a Network object.
// Read Users data from a text file whose
// name is specified as a command line argument.
// Loop through progression of displaying a menu
// of options, accepting user input, and processing the option.
int main(int argc, char *argv[])
{
  // Display options.
  cout << "Option 1. Add a user: provide name (first and last), birth year, and zip code input on same line." << endl;
  cout << "Option 2. Add friend connection: provide two usernames to make friends." << endl;
  cout << "Option 3. Remove friend connection: provide two usernames to remove as friends." << endl;
  cout << "Option 4. Print users." << endl;
  cout << "Option 5. List friends: provide name of user to view friends." << endl;
  cout << "Option 6. Write to file: provide filename to write user database to." << endl;
  cout << "Exit the program on ANY invalid command number." << endl;

  int optionNumber;
  cin >> optionNumber;
  Network network;
  network.read_friends(argv[1]);
  while(optionNumber >= 1 && optionNumber <= 6)
  {
  	// Option 1. Add a user.
  	if(optionNumber == 1)
  	{
  		string firstName = "";
  		string lastName = "";
  		string username = "";
  		int year = -1;
  		int zipcode = -1;

  		cin >> firstName;
  		cin >> lastName;
  		username = firstName + " " + lastName;
  		cin >> year;
  		cin >> zipcode;
  		network.add_user(username, year, zipcode);
  	}
  	// Option 2. Add friend connection.
  	else if(optionNumber == 2)
  	{
  		string firstname1 = "";
  		string lastname1 = "";
  		string username1 = "";

  		string firstname2 = "";
  		string lastname2 = "";
  		string username2 = "";

  		cin >> firstname1;
  		cin >> lastname1;
  		cin >> firstname2;
  		cin >> lastname2;

  		username1 = firstname1 + " " + lastname1;
  		username2 = firstname2 + " " + lastname2;

  		// Check if both users exist
  		if(network.get_id(username1) != -1 && network.get_id(username2) != -1)
  		{
  			// Add friend connection
  			network.add_connection(username1, username2);
  		}
  		else 
  		{
  			cout << "Error: One of your users does not exist!" << endl;
  		}
  	}
  	// Remove friend connection.
  	else if(optionNumber == 3)
  	{
  		string firstname1 = "";
  		string lastname1 = "";
  		string username1 = "";

  		string firstname2 = "";
  		string lastname2 = "";
  		string username2 = "";

  		cin >> firstname1;
  		cin >> lastname1;
  		cin >> firstname2;
  		cin >> lastname2;

  		username1 = firstname1 + " " + lastname1;
  		username2 = firstname2 + " " + lastname2;

  		// Check if both users exist
  		if(network.get_id(username1) != -1 && network.get_id(username2) != -1)
  		{
  			// Add friend connection
  			network.remove_connection(username1, username2);
  		}
  		else 
  		{
  			cout << "Error: One of your users does not exist!" << endl;
  		}
  	}
  	// Print users.
  	else if(optionNumber == 4)
  	{
  		cout << endl;
  		cout << "ID";
  		cout << "      " << "Name"; // space 6 in between
  		cout << "            " << "Year"; // space 12 in between
  		cout << "    " << "Zip" << endl; // space 4 in between
  		cout << "===========================================================" << endl;
  		for(int i = 0; i < network.getNumUsers(); i++)
  		{
  			int idNumber = i;
  			string name = "";
  			int year = -1;
  			int zip;

  			User current = network.getUser(idNumber);
  			name = current.getName();
  			year = current.getYear();
  			zip = current.getZip();

  			cout << idNumber << ".      ";
  			cout << left << setw(16) << name;
  			cout << left << setw(8) << year;
  		    cout << left << setw(7) << zip << endl;
  		}
  	}
  	// List friends.
  	else if(optionNumber == 5)
  	{
  		string firstname = "";
  		string lastname = "";
  		string username = "";

  		cin >> firstname;
  		cin >> lastname;
  		username = firstname + " " + lastname;

  		cout << endl;
  		cout << "ID";
  		cout << "      " << "Name"; // space 6 in between
  		cout << "            " << "Year"; // space 12 in between
  		cout << "    " << "Zip" << endl; // space 4 in between
  		cout << "===========================================================" << endl;

  		int id = 0;
  		id = network.get_id(username);
  		User current = network.getUser(id);

  		for(int i = 0; i < current.getFriendsListSize(); i++)
  		{
  			int tempID = current.getFriendsListID(i);
  			User temp = network.getUser(tempID);
  			cout << tempID << ".      ";
  			cout << left << setw(16) << temp.getName();
  			cout << left << setw(8) << temp.getYear();
  			cout << left << setw(7) << temp.getZip() << endl;
  		}
  	}
  	// Write to file.
  	else if(optionNumber == 6)
  	{
  		char* fileName = new char[50];
  		cin >> fileName;
  		network.write_friends(fileName);
      delete [] fileName;
  	}
  	cout << endl;
    cout << "Option 1. Add a user: provide name (first and last), birth year, and zip code input on same line." << endl;
    cout << "Option 2. Add friend connection: provide two usernames to make friends." << endl;
    cout << "Option 3. Remove friend connection: provide two usernames to remove as friends." << endl;
    cout << "Option 4. Print users." << endl;
    cout << "Option 5. List friends: provide name of user to view friends." << endl;
    cout << "Option 6. Write to file: provide filename to write user database to." << endl;
    cout << "Exit the program on ANY invalid command number." << endl;

    cin >> optionNumber;
  }
  // exit program on ANY invalid command number
  return 0;
}