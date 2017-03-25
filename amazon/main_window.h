#include <mydatastore.h>

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QListWidget>
#include <QComboBox>
#include <QFormLayout>
#include <QRadioButton>
#include <QButtonGroup>

#include <string>
#include <vector>
#include <set>

class MainWindow : public QWidget
{
	Q_OBJECT
public:
	MainWindow(MyDataStore* data);
	~MainWindow();

private slots:
	void displaySearch();
	void addProductCart();
	void viewCart();
	void buyCart();
	void removeCart();
	void saveToFile();
	void getProductReviews(int index);
	void displayReviewWindow();
	void addReview();
	void quit();
	void quitReview();

private:	
	void andOrSearch();
	std::set<User*> getUsers();

	// Layouts
	QGridLayout* overallLayout;

	// Widgets
	QVBoxLayout* formLayout;
	QLabel* titleLabel;
	QHBoxLayout* searchLayout;
	QLabel* searchLabel;
	QLineEdit* searchTermsInput;
	QButtonGroup* searchButtonGroup;
	QHBoxLayout* searchButtonLayout;
	QPushButton* searchButton;
	QRadioButton* andSearchButton;
	QRadioButton* orSearchButton;
	QVBoxLayout* searchResultsLayout;
	QLabel* searchResultsLabel;
	QListWidget* searchResultsList;
	QHBoxLayout* sortSearchLayout;
	QRadioButton* alphabeticalSortButton;
	QRadioButton* ratingSortButton;
	QButtonGroup* sortSearchGroup;
	QHBoxLayout* currentUserLayout;
	QLabel* selectUsersLabel;
	QComboBox* currentUsers;
	QPushButton* createReviewButton;
	QPushButton* addtocartButton;
	QPushButton* viewCartButton;
	QHBoxLayout* saveFileLayout;
	QLabel* saveLabel;
	QLineEdit* saveFileInput;
	QPushButton* savetofileButton;
	QVBoxLayout* productReviewsLayout;
	QLabel* productReviewsLabel;
	QListWidget* productReviewsList;

	// Cart Window
	QWidget* cartWindow;
	QListWidget* cartList;
	QVBoxLayout* overallCartLayout;
	QHBoxLayout* cartButtonLayout;
	QPushButton* buyCartButton;
	QPushButton* removeCartButton;

	// Add Review Window
	QWidget* reviewWindow;
	QVBoxLayout* overallReviewLayout;
	QLabel* reviewUser;
	QHBoxLayout* reviewDate;
	QLabel* monthLabel;
	QComboBox* reviewMonth;
	QLabel* dayLabel;
	QComboBox* reviewDay;
	QLabel* yearLabel;
	QComboBox* reviewYear;
	QHBoxLayout* reviewLayout;
	QLabel* reviewLabel;
	QLineEdit* reviewInput;
	QHBoxLayout* ratingLayout;
	QLabel* ratingLabel;
	QComboBox* ratingInput;
	QPushButton* addReviewButton;
	QPushButton* refreshReviewsButton;
	QPushButton* quitReviewButton;

	QPushButton* quitButton;

	// // Data
	MyDataStore* datastore;

	// List of search results
    std::vector<Product*> searchedProducts;

};