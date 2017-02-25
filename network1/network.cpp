/*
Author: Lung-Young Yu
*/

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include "network.h"

using namespace std;

// Constructor
// Default (no-argument constructor)
Network::Network()
{
	totalNumUsers = 0;
}

// Destructor
Network::~Network()
{

}

// Initializes all of the network's information from a file.
// Accepts a char * (string) indicating name of file to 
// read users from. 
// Return 0 on success, -1 on failure.
int Network::read_friends(const char *filename)
{
	ifstream infile(filename);

	if(infile.fail())
	{
		cout << "Unable to open file" << endl;
		return -1;
	}
	int numUsers = 0;
	infile >> numUsers;
	totalNumUsers = numUsers;

	int id = 0;
	string firstName = "";
	string lastName = "";
	string name = "";
	int birthDate = 0;
	int zipCode = 0;
	string friendConnections = "";

	while(! infile.fail())
	{
		for(int i = 0; i < totalNumUsers; i++)
		{
			
			infile >> id;
			infile >> firstName >> lastName;
			name = firstName + " " + lastName;
			infile >> birthDate >> zipCode;

			string emptyLine = "";

			getline(infile, emptyLine);
			getline(infile, friendConnections);
			

			// As you read in file, use information to create
			// a new user and add this user to list of users.
			User addition(name, birthDate, zipCode);

			// Set User's ID.
			addition.setUserID(id);

			
			// Add friend connections to newly created user.
			stringstream friends;
			friends << friendConnections;

			int z = 0;
			while(friends >> z)
			{
				addition.getFriendsList() -> push_back(z);
			}
			usersList.push_back(addition);
		}
		infile.close();
		return 0; // success
	}
	infile.close();
	return -1; // failure
}

// Writes all of the network's information to a file.
// Accept a char * (string) indicating name of file to
// write users to. 
// Return 0 on success, -1 on failure.
int Network::write_friends(const char *filename)
{
	ofstream outfile(filename);
	while(! outfile.fail())
	{
		outfile << usersList.size() << endl; // Number of users
		for(int i = 0; i < usersList.size(); i++)
		{
			outfile << i << endl; // ID Number
			outfile << "\t" << usersList[i].getName() << endl; // Name
			outfile << "\t" << usersList[i].getYear() << endl; // Year
			outfile << "\t" << usersList[i].getZip() << endl; // Zip Code
			outfile << "\t"; // Friends List: list of friends' IDs
			for(int j = 0; j < usersList[i].getFriendsListSize(); j++)
			{
				outfile << usersList[i].getFriendsListID(j) << " ";
			}
			outfile << endl;
		}
		outfile.close();
		return 0; // success
	}
	outfile.close();
	return -1; // failure
}

// Add a User to the Network database. 
// Create a User object with given information.
void Network::add_user(string username, int yr, int zipcode)
{
	User newUser(username, yr, zipcode);
	newUser.setUserID(totalNumUsers);
	usersList.push_back(newUser);
}

// Accepts two strings (first name + last name
// separated by space in between) corresponding to
// names of Users to make friends. 
// Return 0 on success, -1 on failure.
// No self-connection (from user1 to user2).
int Network::add_connection(string name1, string name2)
{
	int id1 = -1;
	int id2 = -1;

	// Get ID of user1
	id1 = get_id(name1);
	// Get ID of user2
	id2 = get_id(name2);

	// Create connection between both users
	// Connect user1 with user2
	usersList[id1].add_friend(id2);
	// Connect user2 with user1
	usersList[id2].add_friend(id1);

	if(id1 != -1 && id2 != -1)
	{
		return 0; // success
	}
	else
	{
		return -1; // failure
	}
}

// Accepts two strings (first name + last name
// separated by space in between) of the names of
// Users to delete friend connections.
// Return 0 on success, -1 if either of users are invalid.
int Network::remove_connection(string name1, string name2)
{
  	int id1 = -1;
	int id2 = -1;

	// Get ID of user1
	id1 = get_id(name1);
	// Get ID of user2
	id2 = get_id(name2);

	// Delete connection between both users.
	// Delete user1 with user2.
	usersList[id1].delete_friend(id2);
	// Delete user2 with user1.
	usersList[id2].delete_friend(id1);

	if(id1 != -1 && id2 != -1)
	{
		return 0; // success
	}
	else
	{
		return -1; // failure
	}
}

// Accepts a user name.
// Returns corresponding ID for that user or -1 otherwise.
int Network::get_id(string username)
{
	int idNumber = -1;
	// Find user with matching username.
	for(int i = 0; i < usersList.size(); i++)
	{
		if(usersList[i].getName() == username)
		{
			// Find corresponding ID of user.
			idNumber = i;
		}
	}
	return idNumber;
}

// Return total number of users.
int Network::getNumUsers()
{
	return totalNumUsers;
}

// Get user with certain ID number.
User Network::getUser(int id)
{
	User search;
	for(int i = 0; i < usersList.size(); i++)
	{
		if(usersList[i].getID() == id)
		{
			search = usersList[i];
		}
	}
	return search;
}