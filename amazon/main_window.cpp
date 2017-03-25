#include "main_window.h"
#include "mydatastore.h"
#include "product.h"
#include "db_parser.h"
#include "product_parser.h"
#include "util.h"
#include "msort.h"

#include <set>
#include <sstream>
#include <fstream>

using namespace std;

MainWindow::MainWindow(MyDataStore* data)
{
	datastore = data;

	// Title
	setWindowTitle("Amazon");

	// Overall Layout
	overallLayout = new QGridLayout();

	// Form to add quotes
	// Layout
	formLayout = new QVBoxLayout();
	overallLayout->addLayout(formLayout, 0, 0, -1, 1);

    // Title
    titleLabel = new QLabel("Welcome to Amazon!");
    formLayout->addWidget(titleLabel);
    QFont font = titleLabel->font();
    font.setPointSize(16);
    font.setBold(true);
    titleLabel->setFont(font);

	// Search layout
	searchLayout = new QHBoxLayout();
	formLayout->addLayout(searchLayout);

	// Search Label
	searchLabel = new QLabel("Search:");
	searchLayout->addWidget(searchLabel);

    // Text entry box input: any number of search terms separate by whitespace
    searchTermsInput = new QLineEdit();
    searchLayout->addWidget(searchTermsInput);

    // AND, OR button layout
    searchButtonGroup = new QButtonGroup();
    searchButtonLayout = new QHBoxLayout();
    formLayout->addLayout(searchButtonLayout);

    // Non-text entry buttons: choose either AND or OR search
    andSearchButton = new QRadioButton("And");
    searchButtonLayout->addWidget(andSearchButton);

    orSearchButton = new QRadioButton("Or");
    searchButtonLayout->addWidget(orSearchButton);

    andSearchButton->setChecked(true);
    orSearchButton->setChecked(false);

    searchButtonGroup->addButton(andSearchButton);
    searchButtonGroup->addButton(orSearchButton);

    // Search button
    searchButton = new QPushButton("Search");
    connect(searchButton, SIGNAL(clicked()), this, SLOT(displaySearch()));
    searchLayout->addWidget(searchButton);

    // List layout
    searchResultsLayout = new QVBoxLayout();
    overallLayout->addLayout(searchResultsLayout, 0, 1, 1, 1);


    // List box of products: match last search showing their product info
    searchResultsLabel = new QLabel("Products:");
    searchResultsLayout->addWidget(searchResultsLabel);
    searchResultsList = new QListWidget;
    searchResultsLayout->addWidget(searchResultsList);
    connect(searchResultsList, SIGNAL(currentRowChanged(int)), this, SLOT(getProductReviews(int)));
    overallLayout->addWidget(searchResultsList, 0, 1, 1, 1);

    // Sort Search Buttons Layout
    sortSearchGroup = new QButtonGroup();
    sortSearchLayout = new QHBoxLayout();
    formLayout->addLayout(sortSearchLayout);

    // Sort search results by: alphabetical order of product name
    alphabeticalSortButton = new QRadioButton("Sort by Alphabetical Order");
    sortSearchLayout->addWidget(alphabeticalSortButton);
    alphabeticalSortButton->setChecked(true);

    // Sort search results by: highest average rating to lowest average rating
    ratingSortButton = new QRadioButton("Sort by Average Rating");
    sortSearchLayout->addWidget(ratingSortButton);
    sortSearchGroup->addButton(alphabeticalSortButton);
    sortSearchGroup->addButton(ratingSortButton);

    // Select Current User Layout
    currentUserLayout = new QHBoxLayout();
    formLayout->addLayout(currentUserLayout);

    // Select current user: typing their name, combobox
    selectUsersLabel = new QLabel("Current User:");
    currentUserLayout->addWidget(selectUsersLabel);
    currentUsers = new QComboBox();
    currentUserLayout->addWidget(currentUsers);

    // Add users to combobox
    std::set<User*> temp = datastore->getUsers();
    set<User*>::iterator it;
    for(it = temp.begin(); it != temp.end(); ++it)
    {
        QString userName = QString::fromStdString((*it)->getName());
        currentUsers->addItem(userName);
    }

    // Create New Review button
    createReviewButton = new QPushButton("Create New Review");
    connect(createReviewButton, SIGNAL(clicked()), this, SLOT(displayReviewWindow()));
    formLayout->addWidget(createReviewButton);

    // Add-to-Cart button
    addtocartButton = new QPushButton("Add to Cart");
    connect(addtocartButton, SIGNAL(clicked()), this, SLOT(addProductCart()));
    formLayout->addWidget(addtocartButton);


    // View-Cart button: display items in cart in second window
    viewCartButton = new QPushButton("View Cart");
    connect(viewCartButton, SIGNAL(clicked()), this, SLOT(viewCart()));
	formLayout->addWidget(viewCartButton);

	// Reviews layout
	productReviewsLayout = new QVBoxLayout();
	overallLayout->addLayout(productReviewsLayout, 1, 1, 1, 1);

    // Separate widget displaying list of reviews corresponding to currently selected product
    productReviewsLabel = new QLabel("Review(s):");
    productReviewsLayout->addWidget(productReviewsLabel);
	productReviewsList = new QListWidget();
	productReviewsLayout->addWidget(productReviewsList);

    // Save File Layout
    saveFileLayout = new QHBoxLayout();
    formLayout->addLayout(saveFileLayout);

    // Save File Label
    saveLabel = new QLabel("File name:");
    saveFileLayout->addWidget(saveLabel);

    // Text entry box input: file to save to
    saveFileInput = new QLineEdit();
    saveFileLayout->addWidget(saveFileInput);

    // Save database to new file: alow filename to be specified by user
    savetofileButton = new QPushButton("Save to File");
    connect(savetofileButton, SIGNAL(clicked()), this, SLOT(saveToFile()));
    saveFileLayout->addWidget(savetofileButton);

    refreshReviewsButton = new QPushButton("Refresh List of Reviews");
    connect(refreshReviewsButton, SIGNAL(clicked()), this, SLOT(displaySearch()));
    formLayout->addWidget(refreshReviewsButton);

    // Quit program
    quitButton = new QPushButton("Quit");
    connect(quitButton, SIGNAL(clicked()), this, SLOT(quit()) );
    formLayout->addWidget(quitButton);

    // Set overall layout
    setLayout(overallLayout);
}

MainWindow::~MainWindow()
{
}

// Functions for Qt
void MainWindow::andOrSearch()
{
    searchResultsList->clear();
    searchedProducts.resize(0);

    if(searchTermsInput->text().isEmpty())
    {
        searchTermsInput->setPlaceholderText("Please input search words.");
    }

    int type = 0;
    if(orSearchButton->isChecked())
    {
        type = 1;
    }

	if(searchTermsInput->text().isEmpty())
	{
		return;
	}

	stringstream ss(searchTermsInput->text().toStdString());

	string term;
	vector<string> terms;
	while(ss >> term)
	{
		term = convToLower(term);
        terms.push_back(term);
    }
    vector<Product*> listProducts = datastore->search(terms, type);

    // Sort alphabetically by product name.
    if(alphabeticalSortButton->isChecked())
    {
        ProductNameComp nameComp;
        mergeSort(listProducts, nameComp);
        searchedProducts = listProducts;
    }

    if(ratingSortButton->isChecked())
    {

        vector<double> totalAverageRatings;

        map<Product*, set<Review*> > mapProductReviews = datastore->getProductReviews();
        map<Product*, set<Review*> >::iterator productReviewsIterator;

        for(productReviewsIterator = mapProductReviews.begin(); productReviewsIterator != mapProductReviews.end(); ++productReviewsIterator)
        {
            for(unsigned int i = 0; i < listProducts.size(); i++)
            {
                int total = 0;
                int count = 0;

                if(listProducts[i] == productReviewsIterator->first)
                {
                    // Iterate through reviews
                    set<Review*> currentProductReviews = productReviewsIterator->second;

                    if(currentProductReviews.size() > 0)
                    {
                        set<Review*>::iterator reviewsIterator;
                        for(reviewsIterator = currentProductReviews.begin(); reviewsIterator != currentProductReviews.end(); ++reviewsIterator)
                        {
                            total += (*reviewsIterator)->rating;
                            count++;
                        }
                        double average = (total * 1.0) / count;
                        listProducts[i]->setAverageRating(average);
                        totalAverageRatings.push_back(average);
                    }
                    else
                    {
                        listProducts[i]->setAverageRating(0.0);
                        totalAverageRatings.push_back(0.0);
                    }
                }
            }
        }

        DoubleComp dblComp;

        mergeSort(totalAverageRatings, dblComp);

        vector<Product*> sortedRatingsProducts;

        for(unsigned int a = 0; a < totalAverageRatings.size(); a++)
        {
            for(unsigned int b = 0; b < listProducts.size(); b++)
            {
                if(totalAverageRatings[a] == listProducts[b]->getAverageRating())
                {                    
                    sortedRatingsProducts.push_back(listProducts[b]);
                }
            }
        }
        searchedProducts = sortedRatingsProducts;
    }
}

void MainWindow::displaySearch()
{
    andOrSearch();
	for(unsigned int i = 0; i < searchedProducts.size(); i++)
	{
		// Name
		QString productName = QString::fromStdString(searchedProducts[i]->getName());
        // Price
        QString productPrice = QString::number(searchedProducts[i]->getPrice());
        // Combined String
        QString combined = productName + "\n" + "$" + productPrice + "\n";
		
        searchResultsList->addItem(combined);

	}
}

void MainWindow::addProductCart()
{
    // Get currently selected user's name
    string username = currentUsers->currentText().toStdString();

    vector<int> indexes;

    // Get currently selected product(s)
    QList<QListWidgetItem*> temp = searchResultsList->selectedItems();
    for(int i = 0; i < searchResultsList->count(); i++)
    {
        if(searchResultsList->item(i)->isSelected())
        {
            indexes.push_back(i);
        }
    }

    for(unsigned int j = 0; j < indexes.size(); j++)
    {
        datastore->addToCarts(username, searchedProducts[indexes[j]]);
    }
}

void MainWindow::viewCart()
{
    this->setDisabled(true);

    // Get currently selected user's name
    QString username = currentUsers->currentText();

    // Open new cart window
    cartWindow = new QWidget();
    cartWindow->setFixedSize(500, 500);

    // Set layout
    cartWindow->setWindowTitle(username + "'s Cart");
    overallCartLayout = new QVBoxLayout(); 

    cartList = new QListWidget(); 
    overallCartLayout->addWidget(cartList);  

    cartButtonLayout = new QHBoxLayout();
    overallCartLayout->addLayout(cartButtonLayout);
    buyCartButton = new QPushButton("Buy Cart");
    connect(buyCartButton, SIGNAL(clicked()), this, SLOT(buyCart()));
    removeCartButton = new QPushButton("Remove Product from Cart");
    connect(removeCartButton, SIGNAL(clicked()), this, SLOT(removeCart()));

    cartButtonLayout->addWidget(buyCartButton);
    cartButtonLayout->addWidget(removeCartButton);

    map<User*, queue<Product*> > temp = datastore->getCarts();

    vector<Product*> copy;
    int cartCounter = 0;
    int cartSize = 0;
    queue<Product*> cartProducts;

    string nameOfUser = username.toStdString();

    // Find user.
    map<User*, queue<Product*> >::iterator it;
    for(it = temp.begin(); it != temp.end(); ++it)
    {
        // If user's name in database matches username.
        if(it->first->getName() == nameOfUser)
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
        QString displayProduct = QString::fromStdString(copy[i]->displayString());
        cartList->addItem(displayProduct);
    }

    cartWindow->setLayout(overallCartLayout);
    cartWindow->show();
}

void MainWindow::buyCart()
{
    // Get currently selected user's name
    string username = currentUsers->currentText().toStdString();
    datastore->buyCart(username);
}

void MainWindow::removeCart()
{
    string username = currentUsers->currentText().toStdString();

    vector<unsigned int> indexes;

    // Get currently selected product(s)
    QList<QListWidgetItem*> temp = cartList->selectedItems();
    for(int i = 0; i < cartList->count(); i++)
    {
        if(cartList->item(i)->isSelected())
        {
            indexes.push_back(i);
        }
    }

    if(indexes.size() == 0)
    {
    	return;
    }

    for(unsigned int x = 0; x < indexes.size(); x++)
    {
    	datastore->removeCart(username, indexes[x]);
    }

    map<User*, queue<Product*> > temp1 = datastore->getCarts();

    vector<Product*> copy;
    int cartCounter = 0;
    int cartSize = 0;
    queue<Product*> cartProducts;

    map<User*, queue<Product*> >::iterator it;
    for(it = temp1.begin(); it != temp1.end(); ++it)
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

    cartList->clear();

    for(unsigned int j = 0; j < copy.size(); j++)
    {
        QString displayProduct = QString::fromStdString(copy[j]->displayString());
        cartList->addItem(displayProduct);
    }

    cartWindow->show();


}

void MainWindow::saveToFile()
{
    string filename = saveFileInput->text().toStdString();
    if(saveFileInput->text().isEmpty())
    {
        saveFileInput->setPlaceholderText("Please input a filename.");
    }
    else
    {
        std::ofstream ofile(filename.c_str());
        datastore->dump(ofile);
        ofile.close();
        saveFileInput->clear();
    }
}

void MainWindow::getProductReviews(int index)
{
    productReviewsList->clear();

    string temp;
    if(index >= 0)
    {
        temp = searchedProducts[index]->getName();

    }

    vector<Review*> correspondingReviews;

    // Get reviews.
    set<Review*> totalReviews = datastore->getReviews();

    set<Review*>::iterator it;

    vector<Review*> productReviews;

    for(it = totalReviews.begin(); it != totalReviews.end(); ++it)
    {
        if((*it)->prodName == temp)
        {
            productReviews.push_back(*it);
        }
    }

    vector<string> sortedDates;

    for(unsigned int z = 0; z < productReviews.size(); z++)
    {
        // Add to vector
        sortedDates.push_back(productReviews[z]->date);
    }

    AlphaStrComp comp1;
    mergeSort(sortedDates, comp1);

    vector<Review*> sortedReviews;

    set<Review*> listTotalReviews = datastore->getReviews();

    for(unsigned int i = 0; i < sortedDates.size(); i++)
    {
        set<Review*>::iterator it3;
        for(it3 = listTotalReviews.begin(); it3 != listTotalReviews.end(); ++it3)
        {
            if(sortedDates[i] == (*it3)->date)
            {
                sortedReviews.push_back(*it3);
            }
        }
    }

    // Eliminate duplicates.
    for(unsigned int l = 0; l < sortedReviews.size(); l++)
    {
        Review* tempReview = sortedReviews[l];
        int count = 0;
        for(unsigned int h = 0; h < sortedReviews.size(); h++)
        {
            if(tempReview == sortedReviews[h])
            {
                if(count == 1)
                {
                    sortedReviews.erase(sortedReviews.begin() + h);
                }
                count++;
            }
        }
    }

    for(unsigned int j = 0; j < sortedReviews.size(); j++)
    {
        QString reviewRating = QString::number(sortedReviews[j]->rating);
        QString reviewUsername = QString::fromStdString("Username: " + sortedReviews[j]->username);
        QString reviewDate = QString::fromStdString("Date: " + sortedReviews[j]->date);
        QString reviewText = QString::fromStdString(sortedReviews[j]->reviewText);
        QString combined = reviewDate + "\n" + reviewUsername + "\n" + "Rating: " + reviewRating + "/5" + "\n" + reviewText + "\n";
        productReviewsList->addItem(combined);
    }

}

void MainWindow::displayReviewWindow()
{
    this->setDisabled(true);
    // Get currently selected user's name
    QString name = currentUsers->currentText();

    // Open new reviews window
    reviewWindow = new QWidget();
    reviewWindow->setFixedSize(400, 150);

    reviewWindow->resize(reviewWindow->sizeHint());
    // Set layout
    reviewWindow->setWindowTitle("Add Review");
    overallReviewLayout = new QVBoxLayout(); 

    reviewUser = new QLabel("Username: " + name);
    overallReviewLayout->addWidget(reviewUser);

    reviewDate = new QHBoxLayout();
    overallReviewLayout->addLayout(reviewDate);

    monthLabel = new QLabel("Month:");
    reviewDate->addWidget(monthLabel);
    reviewMonth = new QComboBox();
    reviewDate->addWidget(reviewMonth);
    dayLabel = new QLabel("Day:");
    reviewDate->addWidget(dayLabel);
    reviewDay = new QComboBox();
    reviewDate->addWidget(reviewDay);
    yearLabel = new QLabel("Year:");
    reviewDate->addWidget(yearLabel);
    reviewYear = new QComboBox();
    reviewDate->addWidget(reviewYear);

    reviewLayout = new QHBoxLayout();
    overallReviewLayout->addLayout(reviewLayout);
    reviewLabel = new QLabel("Review:");
    reviewLayout->addWidget(reviewLabel);
    reviewInput = new QLineEdit();
    reviewLayout->addWidget(reviewInput);

    ratingLayout = new QHBoxLayout();
    overallReviewLayout->addLayout(ratingLayout);
    ratingLabel = new QLabel("Rating:");
    ratingLayout->addWidget(ratingLabel);
    ratingInput = new QComboBox();
    ratingLayout->addWidget(ratingInput);
    addReviewButton = new QPushButton("Add Review");
    connect(addReviewButton, SIGNAL(clicked()), this, SLOT(addReview()));
    ratingLayout->addWidget(addReviewButton);

    quitReviewButton = new QPushButton("Quit");
    overallReviewLayout->addWidget(quitReviewButton);
    connect(quitReviewButton, SIGNAL(clicked()), this, SLOT(quitReview()));

    // Add values to month combobox
    for(int i = 1; i <= 12; i++)
    {
        QString monthValues = QString::number(i);
        reviewMonth->addItem(monthValues);
    }

    // Add values to day combobox
    for(int j = 1; j <= 31; j++)
    {
        QString dayValues = QString::number(j);
        reviewDay->addItem(dayValues);
    }

    // Add values to year combobox
    for(int k = 2016; k <= 2030; k++)
    {
        QString yearValues = QString::number(k);
        reviewYear->addItem(yearValues);
    }

    // Add values to rating
    for(int m = 1; m <= 5; m++)
    {
        QString ratingValues = QString::number(m);
        ratingInput->addItem(ratingValues);
    }

    reviewWindow->setLayout(overallReviewLayout);
    reviewWindow->show();
}

void MainWindow::addReview()
{   

    if(reviewInput->text().isEmpty())
    {
        reviewInput->setPlaceholderText("Please input a review.");
    }
    else
    {
        // Get currently selected user's name
        QString name = currentUsers->currentText();

        // Get currently selected item
        int index = searchResultsList->currentRow();

        if(index < 0)
        {
            return;
        }

        Product* tempProduct = searchedProducts[index];

        Review* newReview = new Review();

        newReview->prodName = tempProduct->getName();
        newReview->rating = ratingInput->currentText().toInt();
        newReview->username = name.toStdString();
        string fullDate = reviewYear->currentText().toStdString() + "-" + reviewMonth->currentText().toStdString() + "-" + reviewDay->currentText().toStdString();
        newReview->date = fullDate;
        newReview->reviewText = reviewInput->text().toStdString();

        // Add Review
        datastore->addReview(newReview);
    }
}

void MainWindow::quit()
{
	QApplication::exit();
}

void MainWindow::quitReview()
{
    reviewWindow->close();
    this->setDisabled(false);
}
