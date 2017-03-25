#ifndef MSORT_H
#define MSORT_H

#include <vector>
#include <string>
#include <iostream>

using namespace std;

// Comparator for product names alphabetically
struct ProductNameComp {
	bool operator()(const Product* lhs, const Product* rhs)
	{
		return (lhs->getName() < rhs->getName());
	}
};

// Comparator for ratings from highest to lowest
struct DoubleComp {
	bool operator()(const double& lhs, const double& rhs)
	{
	  return lhs > rhs;
	}
};

// Comparator for dates of reviews from most recent to least recent
struct AlphaStrComp {
    bool operator()(const string& lhs, const string& rhs) 
    { // Uses string's built in operator< 
      // to do lexicographic (alphabetical) comparison
      return lhs > rhs; 
    }
};

template<class T, class Comparator>
void mergeSort (std::vector<T>& myArray, Comparator comp);

template<class T, class Comparator>
vector<T> sort (std::vector<T>& myArray, Comparator comp);

template<class T, class Comparator>
vector<T> mergeSortHelper (std::vector<T>& listOne, std::vector<T>& listTwo, Comparator comp);

/*
 * Returns merged array.
 */
template<class T, class Comparator>
void mergeSort (std::vector<T>& myArray, Comparator comp)
{
	if(myArray.size() == 0)
	{
		return;
	}

	myArray = sort(myArray, comp);
}

/*
 * Merge sort.
 */
template <class T, class Comparator>
vector<T> sort (std::vector<T>& myArray, Comparator comp)
{
	if(myArray.size() == 1)
	{
		return myArray;
	}
	else
	{
		std::vector<T> listOne(myArray.size()/2);
		std::vector<T> listTwo(myArray.size() - myArray.size()/2);

		for(unsigned int i = 0; i < myArray.size() / 2; i++)
		{
			listOne[i] = myArray[i];
		}
	
		for(unsigned int j = 0; j < listTwo.size(); j++)
		{
			listTwo[j] = myArray[myArray.size()/2 + j];
		}

		mergeSort(listOne, comp);
		mergeSort(listTwo, comp);
		return mergeSortHelper(listOne, listTwo, comp);
	}
} 

template <class T, class Comparator>
vector<T> mergeSortHelper (std::vector<T>& listOne, std::vector<T>& listTwo, Comparator comp)
{
	vector<T> output(listOne.size() + listTwo.size());
	unsigned int outputCounter = 0;
	unsigned int onePos = 0;
	unsigned int twoPos = 0;

	while(onePos < listOne.size() && twoPos < listTwo.size())
	{
		if(comp(listOne[onePos], listTwo[twoPos]))
		{
			output[outputCounter] = listOne[onePos];
			onePos++;
			outputCounter++;
		}
		else
		{
			output[outputCounter] = listTwo[twoPos];
			twoPos++;
			outputCounter++;
		}
	}

	while(onePos < listOne.size())
	{
		output[outputCounter] = listOne[onePos];
		onePos++;
		outputCounter++;
	}

	while(twoPos < listTwo.size())
	{
		output[outputCounter] = listTwo[twoPos];
		twoPos++;
		outputCounter++;
	}

	return output;
}

#endif
