#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <iostream>
#include <set>


/** Complete the setIntersection and setUnion functions below
 *  in this header file (since they are templates  
 */

/**
 * Intersection of two sets is made up of only elements that are present in both sets.
 */
template <typename T>
 std::set<T> setIntersection( const std::set<T>& s1, const std::set<T>& s2)
{
	if(s1.size() == 0)
	{
		return s2;
	}
	if(s2.size() == 0)
	{
		return s1;
	}
	
	typename std::set<T> intersectionSet;

	// Insert elements that are in both set1 and set2.
	for(typename std::set<T>::iterator it1 = s1.begin(); it1 != s1.end(); ++it1)
	{
		for(typename std::set<T>::iterator it2 = s2.begin(); it2 != s2.end(); ++it2)
		{
			if(*it1 == *it2)
			{
				intersectionSet.insert(*it1);
			}
		}
	}
	return intersectionSet;
}

/**
 * Union of two sets is made up of elements that are present in either one of the sets, or in both.
 */
template <typename T>
 std::set<T> setUnion( const std::set<T>& s1, const std::set<T>& s2)
{
	if(s1.size() == 0)
	{
		return s2;
	}
	if(s2.size() == 0)
	{
		return s1;
	}

	typename std::set<T> unionSet;

	// Check if both sets are equal

	// Insert all elements of set1.
	for(typename std::set<T>::iterator it1 = s1.begin(); it1 != s1.end(); ++it1)
	{
		unionSet.insert(*it1);
	}
	// Insert all elements of set2 that are not in set1.
	for(typename std::set<T>::iterator it2 = s2.begin(); it2 != s2.end(); ++it2)
	{
		// Check if elements of set2 are not already in unionSet
		typename std::set<T>:: iterator iter;
		for(iter = unionSet.begin(); iter != unionSet.end(); ++iter)
		{
			if(*it2 != *iter)
			{
				unionSet.insert(*it2);
			}
		}
	}

	return unionSet;
}

/***********************************************/
/* Prototypes of functions defined in util.cpp */
/***********************************************/

std::string convToLower(std::string src);

std::set<std::string> parseStringToWords(std::string line);

// Used from http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// trim from start
std::string &ltrim(std::string &s) ;

// trim from end
std::string &rtrim(std::string &s) ;

// trim from both ends
std::string &trim(std::string &s) ;
#endif
