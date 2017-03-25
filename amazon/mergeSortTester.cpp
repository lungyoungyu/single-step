/*
#include "msort.h"

using namespace std;

struct IntComp {
    bool operator()(const int& lhs, const int& rhs) 
    { // Uses string's built in operator< 
      // to do lexicographic (alphabetical) comparison
      return lhs < rhs; 
    }
  };


  struct AlphaStrComp {
    bool operator()(const string& lhs, const string& rhs) 
    { // Uses string's built in operator< 
      // to do lexicographic (alphabetical) comparison
      return lhs < rhs; 
    }
  };

int main(int argc, char* argv[])
{
	vector<string> list;
	list.push_back("d");
	list.push_back("c");
	list.push_back("a");
	list.push_back("b");
	list.push_back("f");



	// List: 3 5 1 0 44

	// IntComp comp1;
	AlphaStrComp comp2;

	mergeSort(list, comp2);

	for(int i = 0; i < list.size(); i++)
	{
		cout << list[i] << endl;
	}

	return 0;
}
*/
