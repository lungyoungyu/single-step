#include "clothing.h"
#include <ostream>
#include <sstream>

using namespace std;

Clothing::Clothing(const string category, const string name, double price, int qty, string size, string brand) : 
	Product(category, name, price, qty) {

		category_ = category;
		size_ = size;
		brand_ = brand;
}

/*
 * Returns the appropriate keywords that this product should be associated with
 * to index the product.
 * Keywords: name of clothing, 
 */
set<string> Clothing::keywords() const
{
	// Convert name of clothing to lowercase.
	string title = convToLower(name_);

	// Add name of brand.
	title += brand_;
	
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
		// Check if keywords are 2 or more characters in length (including name of brand).
		if(keyWord.size() >= 2)
		{
			stringSet.insert(keyWord);
		}
	}

	return stringSet;
}

/*
 * Returns a string to display the product info for hits of the search.
 */
string Clothing::displayString() const
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
	info = getName() + "\n" + "Size: " + size_ + " Brand: " + brand_ + "\n" + priceString + " " + qtyString + " left.";
	return info;
}

/*
 * Outputs the product info in the database format.
 */
void Clothing::dump(ostream& os) const
{
	os << "clothing" << "\n" << name_ << "\n" << price_ << "\n" << qty_ << endl;
	os << size_ << "\n" << brand_ << endl;
}