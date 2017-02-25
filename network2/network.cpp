/*
Author: Lung-Young Yu
Email: lungyouy@usc.edu
*/

#include <algorithm>
#include <deque>
#include <set>
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
	/*User search;
	for(int i = 0; i < usersList.size(); i++)
	{
		if(usersList[i].getID() == id)
		{
			search = usersList[i];
		}
	}
	return search;*/
	return usersList[id];
}

User* Network::getUserPointer(int id)
{
	return &usersList[id];
}


// perform BFS Algorithm
// returns shortest path if found, 0 if not found
void Network::bfs(int from, int to)
{
	// initialize all vertices as 'not found' by setting depth = -1.
	for(int i = 0; i < usersList.size(); i++)
	{
		getUserPointer(i)->setDepth(-1);
		getUserPointer(i)->setPredecessor(-1);
	}
	// Create a list, Q.
	deque<int> Q;
	// Add start vertex, u to Q.
	Q.push_back(from);
	// Mark u as 'found' and depth = 0
	User *first = getUserPointer(from);
	first->setDepth(0);
	first->setPredecessor(-1);
	int x = 0;
	
	while(! Q.empty())
	{
		// remove front item
		x = Q.front();
		Q.pop_front();
		User* temp = getUserPointer(x);
		// for each neighbor of temp
		for(int j = 0; j < temp->getFriendsListSize(); j++)
		{
			User* neighbor = getUserPointer(temp->getFriendsListID(j));
			
			if(neighbor->getDepth() == -1)
			{
				// add neighbor to back of the list, Q
				Q.push_back(neighbor->getID());
				// mark neighbor as found by setting depth of y = depth of x + 1
				neighbor->setDepth(temp->getDepth() + 1);
				// set predecessor of neighbor
				neighbor->setPredecessor(temp->getID());
			}
			/*
			if(neighbor->getID() == to)
			{
				// return neighbor->getDepth();
				exit;
			}
			*/
		}
	}
	// Q is empty and last vertex "to" never found
	// No path present between two specified users
}

// a shortest path starting at user "from" and ending
// at user "to". If no path exists, returns empty vector
vector<int> Network::shortest_path(int from, int to)
{
	vector<int> path;
	bfs(from, to);
	int curr = 0;
	path.push_back(to);
	curr = getUser(to).getPredecessor();
	// while not "from"
	while(curr != -1)
	{
		path.push_back(curr);
		User temp = getUser(curr);
		curr = temp.getPredecessor();
	}
	reverse(path.begin(), path.end());
	return path;
}

 // a list of the components of the network
 vector<vector<int> > Network::groups()
 {
 	// predecessors for groups are always -1, breadth first search 0 -> 5, predecessor for 5 is -1, NO PATH
 	// loop through all nodes, cross out ones not -1, pick next 2

 	vector<vector<int> > groups;

 	int vectorIndex = 0;

 	// Iterate through all users
 	for(int i = 0; i < totalNumUsers; i++)
 	{
 		User temp = getUser(i);
 		// If user has depth of -1
 		// (user is part of a disjoint group)
 		if(temp.getDepth() == -1)
 		{
 			// Create new vector index and add to groups
 			vector<int> tempVector;
 			// Increase vector index counter by 1
 			vectorIndex++;
 			// Breadth-first search from each user to last user
 			// if user is not part of a group
 			bfs(i, totalNumUsers - 1);

 			for(int x = 0; x < totalNumUsers; x++)
 			{
 				User temp2 = getUser(x);
 				int temp2ID = 0;
 				temp2ID = temp2.getID();
 				if(temp2.getDepth() != -1)
 				{
 					// Add user to group vector
 					tempVector.push_back(temp2ID);
 				}
 			}
 			groups.push_back(tempVector);
 		}
 	}

 	return groups;
}

// suggested friends with highest score for this user.
// fills in "score" at the given pointer.
// if no suggestions exist, give empty vector and
//    set score to be -1
vector<int> Network::suggest_friends(int who, int& score)
{
	// Run BFS algorithm between this user and all other users
	for(int i = 0; i < totalNumUsers; i++)
	{
		if(i != who)
		{
			bfs(who, i);
		}
		User* checker = getUserPointer(i);
		checker->setCheck(false);
	}

	vector<int> suggestions;
	int scoreValue = 0;

	User* whoUser = getUserPointer(who);

	// Loop through each user, not including who
	for(int j = 0; j < totalNumUsers; j++)
	{
		if(j != who)
		{
			User* temp = getUserPointer(j);
			// For each user, if depth is 2
			if(temp->getDepth() == 2)
			{
				// score for each user
				int tempScore = 0;

				/* Alternative method
				// Loop through connections of this user (who)
				for(int x = 0; x < whoUser->getFriendsListSize(); x++)
				{
					int whoFriendID = whoUser->getFriendsListID(x);
					User* whoFriend = getUserPointer(whoFriendID);
					// Loop through connections of other user
					for(int y = 0; y < temp->getFriendsListSize(); y++)
					{
						int tempFriendID = temp->getFriendsListID(y);
						User* tempFriend = getUserPointer(tempFriendID);
						// Loop through connections of who's connections
						for(int j = 0; j < whoFriend->getFriendsListSize(); j++)
						{
							int whoFriendID = whoFriend->getFriendsListID(j);
							// Loop through connections of other user's connections
							for(int h = 0; h < tempFriend->getFriendsListSize(); h++)
							{
								int tempFriendID = tempFriend->getFriendsListID(h);
								// if friends are the same (same ID)
								// increase score by 1
								if(whoFriendID == tempFriendID)
								{
									//whoFriend->setCheck(true);
									tempScore++;
									cout << whoFriendID << " " << tempFriendID << endl;
								}
							}
						}
					}
				}
				*/

				// vector of friends that are in common
				vector<int> commonFriendFriends;
				// vector of who's friend's friends
				vector<int> whoFriendFriendStorage;
				// vector of other friend's friends
				vector<int> otherFriendFriendStorage;
				
				// Loop through who's friend's friends and store 
				// their IDs in whoFriendFriendStorage
				for(int x = 0; x < whoUser->getFriendsListSize(); x++)
				{
					int whoFriendID = whoUser->getFriendsListID(x);
					User* whoFriend = getUserPointer(whoFriendID);

					for(int y = 0; y < whoFriend->getFriendsListSize(); y++)
					{
						int whoFriendFriendID = whoFriend->getFriendsListID(y);
						whoFriendFriendStorage.push_back(whoFriendFriendID);
					}
				}
				
				// Loop through other user's friend's friends and
				// store their IDs in otherFriendFriendStorage
				for(int j = 0; j < temp->getFriendsListSize(); j++)
				{
					int tempFriendID = temp->getFriendsListID(j);
					User* tempFriend = getUserPointer(tempFriendID);
					for(int h = 0; h < tempFriend->getFriendsListSize(); h++)
					{
						int tempFriendFriendID = tempFriend->getFriendsListID(h);
						otherFriendFriendStorage.push_back(tempFriendFriendID);
					}
				}

				// If IDs of friends match, store the matching IDs to 
				// commonFriendFriends
				for(int l = 0; l < whoFriendFriendStorage.size(); l++)
				{
					int count = 0;
					for(int k = 0; k < otherFriendFriendStorage.size(); k++)
					{
						if(count == 0)
						{
							if(whoFriendFriendStorage[l] == otherFriendFriendStorage[k])
							{
								tempScore++;
								count++;

							}
						}
					}
				}
				// Set score for each user
				temp->setScore(tempScore);
			}
		}
	}

	// Add users with score that matches largest
	// score attained to vector of suggested friends
	for(int z = 0; z < totalNumUsers; z++)
	{
		User* check = getUserPointer(z);
		if(check->getScore() > scoreValue)
		{
			scoreValue = check->getScore();
		}
	}

	for(int e = 0; e < totalNumUsers; e++)
	{
		User* check = getUserPointer(e);
		if(check->getScore() == scoreValue)
		{
			suggestions.push_back(e);
		}
	}
	// If no suggestions exist: set score to be -1
	if(suggestions.size() == 0)
	{
		score = -1;
	}
	else
	{
		score = scoreValue/2;
	}

	return suggestions;
}