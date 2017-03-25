#include "book.h"
#include <ostream>
#include <sstream>

using namespace std;

Book::Book(const string category, const string name, double price, int qty, string ISBN, string author) : 
	Product(category, name, price, qty) {
		category_ = category;
		ISBN_ = ISBN;
		author_ = author;
}

/*
 * Returns the appropriate keywords that this product should be associated with
 * to index the product.
 * Keywords: title of book, words in author's name, book's ISBN number
 */
set<string> Book::keywords() const
{
	// Convert title of book to lowercase.
	string title = convToLower(name_);

	// Add author's name.
	title += author_;
	
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
		// Check if keywords are 2 or more characters in length (including author's name).
		if(keyWord.size() >= 2)
		{
			stringSet.insert(keyWord);
		}
	}

	// Add book's ISBN number.
	stringSet.insert(ISBN_);
	return stringSet;
}

/*
 * Returns a string to display the product info for hits of the search.
 */
string Book::displayString() const
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
	info = getName() + "\n" + "Author: " + author_ + " ISBN: " + ISBN_ + "\n" + priceString + " " + qtyString + " left.";
	return info;
}

/*
 * Outputs the product info in the database format.
 */
void Book::dump(ostream& os) const
{
	os << "book" << "\n" << name_ << "\n" << price_ << "\n" << qty_ << endl;
	os << ISBN_ << "\n" << author_ << endl;
}