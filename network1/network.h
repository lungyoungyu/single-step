/*
Author: Lung-Young Yu
*/

#ifndef NETWORK_H
#define NETWORK_H
#include <string>
#include "user.h"

class Network {
 public:
  Network();
  ~Network();
  int read_friends(const char *filename);
  int write_friends(const char *filename);
  void add_user(std::string username, int yr, int zipcode);
  int add_connection(std::string name1, std::string name2);
  int remove_connection(std::string name1, std::string name2);
  int get_id(std::string username);

  int getNumUsers();
  User getUser(int id);

 private:
  // vector of User objects	
  std::vector <User> usersList;
  // number of users
  int totalNumUsers;
};

#endif