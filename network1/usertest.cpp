#include <iostream>
#include "user.h"
#include "user.cpp"

using namespace std;

int main()
{
  User bob;
  bob.add_friend(90018);
  bob.add_friend(90271);
  bob.delete_friend(90018);
  cout << bob.getFriendsList() << endl;
  return 0;
}
