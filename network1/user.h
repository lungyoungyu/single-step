/*
Author: Lung-Young Yu
*/

#ifndef USER_H
#define USER_H

#include <string>
#include <vector>

class User {
 public:
 	User();
 	User(std::string name, int year, int zip);
 	~User();
 	void add_friend(int id);
 	void delete_friend(int id);
 	int getID();
 	std::string getName();
 	int getYear();
 	int getZip();
 	std::vector <int> * getFriendsList();

 	int getFriendsListSize();
 	int getFriendsListID(int index);
 	void setUserID(int id);

 private:
 	// set to entry/index in users array/vector of
 	// Network class where user is located
 	int user_id;
 	// user name that consists of first and last name
 	// separated by spaces
 	std::string user_name;
 	// indicate birth year of user
 	int user_year;
 	// indicate user's zip code
 	int user_zip;
 	// list of integer entries for friend connections
 	// each entry will be corresponding integer ID of friend
 	// use array: need variable to track number of friends
 	// int connections[100]; 
 	// use vector: no need for variable to track number of friends
 	std::vector <int> friendsList;
};

#endif