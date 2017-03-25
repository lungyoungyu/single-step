#include "mydatastore.h"
#include "util.h"

using namespace std;

MyDataStore::MyDataStore() : DataStore() {
	
std::map< std::string, std::set<Product*> > dataMap;

	// Set of all users
	std::set<User*> dataUsers;
}

MyDataStore::~MyDataStore()
{

	// Delete memory in set of total products.
	set<Product*>::iterator it1;
	for(it1 = totalProducts.begin(); it1 != totalProducts.end(); ++it1)
	{
		delete *it1;
	}
	
	// Delete memory in set of total users.
	set<User*>::iterator it2;
	for(it2 = totalUsers.begin(); it2 != totalUsers.end(); ++it2)
	{
		delete *it2;
	}

	// Delete memory in set of total reviews.
	set<Review*>::iterator it3;
	for(it3 = totalReviews.begin(); it3 != totalReviews.end(); ++it3)
	{
		delete *it3;
	}
	
}

/**
 * Adds a product to the data store
 */
void MyDataStore::addProduct(Product* p)
{
	totalProducts.insert(p);

	// Adds products to map.
	set<string> productKeyWords = p->keywords();
	set<string>::iterator it;
	// Iterate through key words of product.
	for(it = productKeyWords.begin(); it != productKeyWords.end(); ++it)
	{
		// If key word already exists in map.
		if(dataMap.find(*it) != dataMap.end())
		{
			// Sets are all unique, so no need to check for repetition.
			dataMap[*it].insert(p);
		}
		// If key word does not exist in map.
		else
		{
			set<Product*> tempSet;
			tempSet.insert(p);
			// Add to map.
			dataMap.insert(make_pair(*it, tempSet));
		}
	}
}

/**
 * Adds a user to the data store
 */
void MyDataStore::addUser(User* u)
{
	totalUsers.insert(u);

	map<User*, queue<Product*> >::iterator it;
	// If user does not exist in map.
	if(carts.find(u) == carts.end())
	{
		// Add user with empty cart.
		queue<Product*> temp;
		carts.insert(make_pair(u, temp));
	}
}

/**
 * Performs a search of products whose keywords match the given "terms"
 *  type 0 = AND search (intersection of results for each term)
 *  type 1 = OR search (union of results for each term)
 */
vector<Product*> MyDataStore::search(vector<string>& terms, int type)
{
	// Check for non-keywords
	vector<Product*> searchProducts;
	map<string, set<Product*> >::iterator it;

	set<Product*> results;

	set<set<Product*> > productSets;

	set<string> keyTerms;
	
	// Check that size of search terms is greater than 0.
	if(terms.size() > 0)
	{
		// Parse terms to keywords
		for(unsigned int j = 0; j < terms.size(); j++)
		{
			// keyTerms = parseStringToWords(terms[j]);
			keyTerms.insert(terms[j]);
		}
		set<string>::iterator i;
		// Iterate through all keywords.
		for(i = keyTerms.begin(); i != keyTerms.end(); ++i)
		{
			it = dataMap.find(*i);
			if(it != dataMap.end())
			{
				productSets.insert(it->second);
			}
			else
			{
				if(type == 0)
				{
					return searchProducts;
				}
			}
		}
	}

	// AND search
	if(type == 0)
	{
		set<set<Product*> >::iterator it2;
		for(it2 = productSets.begin(); it2 != productSets.end(); ++it2)
		{
			results = setIntersection(results, *it2);
		}
	}
	// OR search
	else
	{
		set<set<Product*> >::iterator it2;
		for(it2 = productSets.begin(); it2 != productSets.end(); ++it2)
		{
			results = setUnion(results, *it2);
		}
	}

	set<Product*>::iterator it3;
	for(it3 = results.begin(); it3 != results.end(); ++it3)
	{
		searchProducts.push_back(*it3);
	}
	return searchProducts;
}

/**
 * Reproduce the database file from the current Products and User values
 */
void MyDataStore::dump(ostream& ofile)
{
	// Current Products
	set<Product*> currentProducts;
	map<string, set<Product*> >::iterator it1;	
	for(it1 = dataMap.begin(); it1 != dataMap.end(); ++it1)
	{
		set<Product*> temp = it1->second;
		set<Product*>::iterator it2;
		for(it2 = temp.begin(); it2 != temp.end(); ++it2)
		{
			currentProducts.insert(*it2);
		}
	}

	ofile << "<products>" << endl;
	set<Product*>::iterator i;
	for(i = currentProducts.begin(); i != currentProducts.end(); ++i)
	{
		(*i)->dump(ofile);
	}
	ofile << "</products>" << endl;
	ofile << "<users>" << endl;
	// Current Users
	map<User*, queue<Product*> >::iterator it3;
	for(it3 = carts.begin(); it3 != carts.end(); ++it3)
	{
		(it3->first)->dump(ofile);
	}
	ofile << "</users>" << endl;
	ofile << "<reviews>" << endl;
	// Reviews
	map<Product*, set<Review*> >::iterator it4;
	for(it4 = productReviews.begin(); it4 != productReviews.end(); ++it4)
	{
		set<Review*>::iterator it5;
		for(it5 = it4->second.begin(); it5 != it4->second.end(); ++it5)
		{
			ofile << it4->first->getName() << endl;
			ofile << (*it5)->rating << " " << (*it5)->username << " " << (*it5)->date << " " << (*it5)->reviewText << endl;
		}
	}
	ofile << "</reviews>" << endl;
}

/**
 * Add to carts.
 */
void MyDataStore::addToCarts(string username, Product* p)
{
	// If quantity of product is greater than 0.
	if(p->getQty() > 0)
	{
		// Find user.
		map<User*, queue<Product*> >::iterator it;
		for(it = carts.begin(); it != carts.end(); ++it)
		{
			// If user's name in database matches username.
			if(it->first->getName() == username)
			{
				if(it->second.size() > 0)
				{
					// Add product to cart of user.
					it->second.push(p);
				}
				else
				{
					queue<Product*> temp;
					temp.push(p);
					it->second = temp;
				}
			}
		}
	}
}

/**
 * View cart. Print products in cart with some ascending index number.
 */ 
void MyDataStore::viewCart(string username)
{
	vector<Product*> copy;
	int cartCounter = 0;
	int cartSize = 0;
	queue<Product*> cartProducts;

	// Find user.
	map<User*, queue<Product*> >::iterator it;
	for(it = carts.begin(); it != carts.end(); ++it)
	{
		// If user's name in database matches username.
		if(it->first->getName() == username)
		{
			cartProducts = it->second;
			cartSize = cartProducts.size();
			
			while(cartCounter < cartSize)
			{
				copy.push_back(cartProducts.front());
				cartProducts.pop();
				cartCounter++;
			}
		}
	}

	for(unsigned int i = 0; i < copy.size(); i++)
	{
		cout << "Item " << i + 1 << endl;
		cout << copy[i]->displayString() << endl;
		cout << endl;
		cartProducts.push(copy[i]);
	}
}

/**
 * Buy cart.
 * Iterate through items in username's cart. If the item is in stock
 * and the user has enough money, remove item from the cart, reduce 
 * in stock quantity by 1, and debit product price from user's available
 * credit. If an item is not in stock or the user does not have enough credit,
 * leave item in cart and go on to next product.
 */ 
void MyDataStore::buyCart(string username)
{
	vector<Product*> copy;
	int cartCounter = 0;
	int cartSize = 0;
	queue<Product*> cartProducts;
	User* currentUser;

	// Find user.
	map<User*, queue<Product*> >::iterator it;
	for(it = carts.begin(); it != carts.end(); ++it)
	{
		// If user's name in database matches username.
		if(it->first->getName() == username)
		{
			currentUser = it->first;
			cartProducts = it->second;
			cartSize = cartProducts.size();
			
			while(cartCounter < cartSize)
			{
				copy.push_back(cartProducts.front());
				cartProducts.pop();
				cartCounter++;
			}
		}
	}

	for(unsigned int i = 0; i < copy.size(); i++)
	{
		// If item is in stock and user has enough money
		if(copy[i]->getQty() > 0 && currentUser->getBalance() > copy[i]->getPrice())
		{
			// Reduce stock quantity by 1.
			copy[i]->subtractQty(1);

			if(currentUser != NULL)
			{
				// Debit product price from user's available credit.
				currentUser->deductAmount(copy[i]->getPrice());
			}

			// Remove item from cart by not adding back into cart.
		}
		// If item is not in stock or user does not have enough credit, leave item in cart
		// and go to next product.
		else
		{
			cartProducts.push(copy[i]);
		}
	}

	for(it = carts.begin(); it != carts.end(); ++it)
	{
		// If user's name in database matches username.
		if(it->first->getName() == username)
		{
			currentUser = it->first;
			it->second = cartProducts;
		}
	}

}

/**
 * Adds a review to database.
 */
void MyDataStore::addReview(Review* r)
{	
	string productName = r->prodName;

	// Find product with product name.
	set<Product*>::iterator it2;

	Product* searchProduct;

	for(it2 = totalProducts.begin(); it2 != totalProducts.end(); ++it2)
	{
		if((*it2)->getName() == productName)
		{
			searchProduct = *it2;
		}
	}

	map<Product*, set<Review*> >::iterator it = productReviews.find(searchProduct);

	// If product is found.
	if(it != productReviews.end())
	{
		// Check if any reviews exist for product.
		// If no reviews exist, create new set.
		if(it->second.size() == 0)
		{
			set<Review*> temp;
			temp.insert(r);
			it->second = temp;
		}
		// If reviews already exist, add review to set.
		else
		{
			it->second.insert(r);
		}
	}
	else // If product is not found, create new review paired to product.
	{
		set<Review*> tempSet;
		tempSet.insert(r);
		// Add to map.
		productReviews.insert(make_pair(searchProduct, tempSet));
	}

	totalReviews.insert(r);
}

set<User*> MyDataStore::getUsers()
{
	return totalUsers;
}

map<User*, queue<Product*> > MyDataStore::getCarts()
{
	return carts;
}

map<Product*, set<Review*> > MyDataStore::getProductReviews()
{
	return productReviews;
}

void MyDataStore::removeCart(string username, unsigned int index)
{
	vector<Product*> copy;
    int cartCounter = 0;
    int cartSize = 0;
    queue<Product*> cartProducts;

    // Find user.
    map<User*, queue<Product*> >::iterator it;
    for(it = carts.begin(); it != carts.end(); ++it)
    {
        // If user's name in database matches username.
        if(it->first->getName() == username)
        {
            cartProducts = it->second;
            cartSize = cartProducts.size();
            
            while(cartCounter < cartSize)
            {
                copy.push_back(cartProducts.front());
                cartProducts.pop();
                cartCounter++;
            }
        }
    }

    for(unsigned int i = 0; i < copy.size(); i++)
    {
        if(index != i)
        {
            // Remove item from cart by not adding back into cart.
            cartProducts.push(copy[i]);
        }
    }

    for(it = carts.begin(); it != carts.end(); ++it)
    {
        // If user's name in database matches username.
        if(it->first->getName() == username)
        {
            it->second = cartProducts;
        }
    }
}

set<Review*> MyDataStore::getReviews()
{
	return totalReviews;
}

