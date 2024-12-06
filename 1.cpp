#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

// Forward declarations
class Customer;
class Product;
class Seller;
class PaymentStrategy;

// Base class for the marketplace
class Marketplace {
private:
    vector<Seller> sellers;
    vector<Customer> customers;
    vector<Product> products;

public:
    void addSeller(const Seller& seller);
    void addCustomer(const Customer& customer);
    void addProduct(const Product& product);
    const vector<Product>& listProducts() const;
    Product* findProduct(const string& productName);
};

// Class representing a Product
class Product {
private:
    string name;
    double price;
    int quantity;
    int sellerId;

public:
    Product(const string& name, double price, int quantity, int sellerId)
        : name(name), price(price), quantity(quantity), sellerId(sellerId) {}

    const string& getName() const { return name; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }
    int getSellerId() const { return sellerId; }

    bool purchase(int quantityToBuy);
};

bool Product::purchase(int quantityToBuy) {
    if (quantityToBuy <= quantity) {
        quantity -= quantityToBuy;
        return true;
    }
    return false;
}

// Class representing a Seller
class Seller {
private:
    string name;
    int id;

public:
    Seller(const string& name, int id) : name(name), id(id) {}

    void addProduct(Marketplace& marketplace, const string& productName, double price, int quantity);
    int getId() const { return id; }
    const string& getName() const { return name; }
};

void Seller::addProduct(Marketplace& marketplace, const string& productName, double price, int quantity) {
    marketplace.addProduct(Product(productName, price, quantity, id));
}

// Abstract PaymentStrategy class
class PaymentStrategy {
public:
    virtual ~PaymentStrategy() = default;
    virtual bool pay(double amount, double& balance) const = 0;
};

// Derived Payment strategies
class CashPayment : public PaymentStrategy {
public:
    bool pay(double amount, double& balance) const override {
        if (balance >= amount) {
            balance -= amount;
            return true;
        }
        return false;
    }
};

class CardPayment : public PaymentStrategy {
public:
    bool pay(double amount, double& balance) const override {
        if (balance >= amount) {
            balance -= amount;
            return true;
        }
        return false;
    }
};

class CryptoPayment : public PaymentStrategy {
public:
    bool pay(double amount, double& balance) const override {
        if (balance >= amount) {
            balance -= amount;
            return true;
        }
        return false;
    }
};

// Class representing a Customer
class Customer {
private:
    string name;
    double balance;
    shared_ptr<PaymentStrategy> paymentMethod;

public:
    Customer(const string& name, double balance)
        : name(name), balance(balance) {}

    void setPaymentMethod(const shared_ptr<PaymentStrategy>& method) {
        paymentMethod = method;
    }

    bool buyProduct(Product& product, int quantity);
};

bool Customer::buyProduct(Product& product, int quantity) {
    double totalCost = product.getPrice() * quantity;
    if (product.getQuantity() >= quantity && paymentMethod && paymentMethod->pay(totalCost, balance)) {
        product.purchase(quantity);
        cout << "Purchase successful!" << endl;
        cout << "Receipt: \n";
        cout << "Product: " << product.getName() << "\n";
        cout << "Quantity: " << quantity << "\n";
        cout << "Total Cost: " << totalCost << "\n";
        cout << "Remaining Balance: " << balance << "\n";
        return true;
    }
    cout << "Purchase failed." << endl;
    return false;
}

// Marketplace implementation
void Marketplace::addSeller(const Seller& seller) {
    sellers.push_back(seller);
}

void Marketplace::addCustomer(const Customer& customer) {
    customers.push_back(customer);
}

void Marketplace::addProduct(const Product& product) {
    products.push_back(product);
}

const std::vector<Product>& Marketplace::listProducts() const {
    return products;
}

Product* Marketplace::findProduct(const std::string& productName) {
    for (auto& product : products) {
        if (product.getName() == productName) {
            return &product;
        }
    }
    return nullptr;
}

int main() {
    Marketplace marketplace;

    // Create sellers
    Seller seller1("Alice", 1);
    Seller seller2("Bob", 2);

    marketplace.addSeller(seller1);
    marketplace.addSeller(seller2);

    // Add products
    seller1.addProduct(marketplace, "Laptop", 1000.0, 5);
    seller2.addProduct(marketplace, "Phone", 500.0, 10);

    // Create customers
    Customer customer1("John", 2000.0);
    customer1.setPaymentMethod(std::make_shared<CashPayment>());

    marketplace.addCustomer(customer1);

    // List products
    cout << "Available products: \n";
    for (const auto& product : marketplace.listProducts()) {
        cout << "- " << product.getName() << " ($" << product.getPrice() << ", Quantity: " << product.getQuantity() << ")\n";
    }

    // Customer makes a purchase
    Product* productToBuy = marketplace.findProduct("Laptop");
    if (productToBuy) {
        customer1.buyProduct(*productToBuy, 2);
    }

    return 0;
}
