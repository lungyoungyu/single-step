/*
Author: Lung-Young Yu
Email: lungyouy@usc.edu
*/

#include <iostream>
#include "user.h"

using namespace std;

// Constructor
User::User()
{
	user_id = -1;
	user_name = "";
	user_year = -1;
	user_zip = -1;

	depth = -1;
	predecessor = -2;
	score = 0;
	check = false;
}

User::User(string name, int year, int zip)
{
	user_name = name;
	user_year = year;
	user_zip = zip;
	depth = -1;
	predecessor = -2;
	score = 0;
	check = false;
}

// Destructor
User::~User()
{

}

// Accept ID of a user to add as a friend.
// If indicated user is already friend of
// this user, do nothing (don't add second time).
void User::add_friend(int id)
{
	bool repeat = false;
	for(int i = 0; i < friendsList.size(); i++)
	{
		// if id is already a friend of user, do nothing.
		if(friendsList[i] == id)
		{
			repeat = true;
		}
	}
	// if id is not a friend of user, add id of user
	// to add as friend.
	if(repeat == false)
	{
		friendsList.push_back(id);
	}
}

// Accept ID of a user to delete as a friend.
// ID should be 'removed' and all following
// friend ID's moved up one slot. If friend ID is
// NOT in the list of friends, do nothing.
void User::delete_friend(int id)
{
	bool inList = false;
	int index = -1;
	for(int i = 0; i < friendsList.size(); i++)
	{
		// if id is in list of friends, do nothing.
		if(friendsList[i] == id)
		{
			inList = true;
			index = i;
		}
	}
	// if id is in list of friends, remove id.
	if(inList == true)
	{
		friendsList.erase(friendsList.begin() + index);
	}
}

// Return user's ID.
int User::getID()
{
	return user_id;
}

// Return user's name.
string User::getName()
{
	return user_name;
}

// Return user's birth year.
int User::getYear()
{
	return user_year;
}

// Return user's zip code.
int User::getZip()
{
	return user_zip;
}

// Return pointer to friends list.
vector <int> * User::getFriendsList()
{
	return &friendsList;
}

// Return size of friends list.
int User::getFriendsListSize()
{
	return friendsList.size();
}

// Return ID of index of friends list.
int User::getFriendsListID(int index)
{
	return friendsList[index];
}

// Set Users ID to a value.
void User::setUserID(int id)
{
	user_id = id;
}

// Returns depth.
int& User::getDepth()
{
	return depth;
}

// Set Users depth to a value.
void User::setDepth(int depthValue)
{
	depth = depthValue;
}

// Returns predecessor.
int& User::getPredecessor()
{
	return predecessor;
}

// Set Users predecessor to a value.
void User::setPredecessor(int predecessorValue)
{
	predecessor = predecessorValue;
}

// Returns score.
int User::getScore()
{
	return score;
}

// Sets score of user to a value.
void User::setScore(int setScore)
{
	score = setScore;
}

// Returns true or false depending on whether user has been checked
bool User::getCheck()
{
	return check;
}

// Sets check of user to false or true
void User::setCheck(bool checkValue)
{
	check = checkValue;
}