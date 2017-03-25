#ifndef MYDATASTORE_H
#define MYDATASTORE_H

#include "datastore.h"
#include <map>
#include <queue>

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QListWidget>

#include <sstream>

class MyDataStore : public DataStore {
public:
	MyDataStore();
    ~MyDataStore();
    /**
     * Adds a product to the data store
     */
    void addProduct(Product* p);

    /**
     * Adds a user to the data store
     */
    void addUser(User* u);

    /**
     * Performs a search of products whose keywords match the given "terms"
     *  type 0 = AND search (intersection of results for each term) while
     *  type 1 = OR search (union of results for each term)
     */
    std::vector<Product*> search(std::vector<std::string>& terms, int type);

    /**
     * Reproduce the database file from the current Products and User values
     */
    void dump(std::ostream& ofile);

    /**
 	 * Add to carts.
 	 */
	void addToCarts(std::string username, Product* p);

	/**
 	 * View cart. Print products in cart with some ascending index number.
 	 */ 
	void viewCart(std::string username);

    /**
     * Buy cart.
     * Iterate through items in username's cart. If the item is in stock
     * and the user has enough money, remove item from the cart, reduce 
     * in stock quantity by 1, and debit product price from user's available
     * credit. If an item is not in stock or the user does not have enough credit,
     * leave item in cart and go on to next product.
     */ 
    void buyCart(std::string username);

    /**
    * Adds a review
    */
    void addReview(Review* r);

    std::set<User*> getUsers();

    std::map<User*, std::queue<Product*> > getCarts();

    std::map<Product*, std::set<Review*> > getProductReviews();

    void removeCart(std::string username, unsigned int index);

    std::set<Review*> getReviews();

private:
	// Map of string of keywords to sets of products.
	std::map< std::string, std::set<Product*> > dataMap;

	// Map of users to carts (queue of products).
	std::map<User*, std::queue<Product*> > carts;

    // Set of total products.
    std::set<Product*> totalProducts;
    
    // Set of total users.
    std::set<User*> totalUsers;

    // Map of products to sets of reviews.
    std::map<Product*, std::set<Review*> > productReviews;

    // Set of total reviews.
    std::set<Review*> totalReviews;
};

#endif