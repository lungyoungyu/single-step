#include "movie.h"
#include <ostream>
#include <sstream>

using namespace std;

Movie::Movie(const string category, const string name, double price, int qty, string genre, string rating) : 
	Product(category, name, price, qty) {

	category_ = category;
	genre_ = genre;
	rating_ = rating;
}

/*
 * Returns the appropriate keywords that this product should be associated with
 * to index the product.
 * Keywords: name of movie, genre of movie
 */
set<string> Movie::keywords() const
{
	// Convert name of movie to lowercase.
	string title = convToLower(name_);
	
	set<string> stringSet;
	stringstream words;

	for(unsigned int i = 0; i < title.size(); i++)
	{
		if(ispunct(title[i]))
		{
			title.erase(i, 1);
			title.insert(i, 1, ' ');
		}
	}

	words << title;
	string keyWord;

	while(words >> keyWord)
	{
		// Check if keywords are 2 or more characters in length.
		if(keyWord.size() >= 2)
		{
			stringSet.insert(keyWord);
		}
	}

	// Insert name of genre.
	stringSet.insert(genre_);

	return stringSet;
}

/*
 * Returns a string to display the product info for hits of the search.
 */
string Movie::displayString() const
{
	// Convert price from double to string
	stringstream strs;
	strs << price_;
	string priceString = strs.str();
	// Convert quantity from int to string
	stringstream ss;
	ss << qty_;
	string qtyString = ss.str();

	string info;
	info = getName() + "\n" + "Genre: " + genre_ + " Rating: " + rating_ + "\n" + priceString + " " + qtyString + " left.";
	return info;
}

/*
 * Outputs the product info in the database format.
 */
void Movie::dump(ostream& os) const
{
	os << "movie" << "\n" << name_ << "\n" << price_ << "\n" << qty_ << endl;
	os << genre_ << "\n" << rating_ << endl;
}