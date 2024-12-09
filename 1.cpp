#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

// Предварительное объявление классов 
class Customer;
class Product;
class Seller;
class PaymentStrategy;

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

class PaymentStrategy {
public:
    virtual ~PaymentStrategy() = default;
    virtual bool pay(double amount, double& balance) const = 0;
    virtual string getPaymentMethodName() const = 0; // Новый метод для возврата названия метода оплаты
};

class CashPayment : public PaymentStrategy {
public:
    bool pay(double amount, double& balance) const override {
        if (balance >= amount) {
            balance -= amount;
            return true;
        }
        return false;
    }
    string getPaymentMethodName() const override {
        return "Cash";
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
    string getPaymentMethodName() const override {
        return "Card";
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
    string getPaymentMethodName() const override {
        return "Crypto";
    }
};

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
        cout << "Payment Method: " << paymentMethod->getPaymentMethodName() << "\n"; // Добавлен вывод метода оплаты
        cout << "Remaining Balance: " << balance << "\n";
        return true;
    }
    cout << "Purchase failed." << endl;
    return false;
}

void Marketplace::addSeller(const Seller& seller) {
    sellers.push_back(seller);
}

void Marketplace::addCustomer(const Customer& customer) {
    customers.push_back(customer);
}

void Marketplace::addProduct(const Product& product) {
    products.push_back(product);
}

const vector<Product>& Marketplace::listProducts() const {
    return products;
}

Product* Marketplace::findProduct(const string& productName) {
    for (auto& product : products) {
        if (product.getName() == productName) {
            return &product;
        }
    }
    return nullptr;
}

int main() {
    Marketplace marketplace;

    Seller seller1("Alice", 1);
    Seller seller2("Bob", 2);

    marketplace.addSeller(seller1);
    marketplace.addSeller(seller2);

    seller1.addProduct(marketplace, "Laptop", 1000.0, 5);
    seller2.addProduct(marketplace, "Phone", 500.0, 10);

    Customer customer1("John", 2100.0);
    customer1.setPaymentMethod(make_shared<CryptoPayment>());

    marketplace.addCustomer(customer1);

    cout << "Available products: \n";
    for (const auto& product : marketplace.listProducts()) {
        cout << "- " << product.getName() << " ($" << product.getPrice() << ", Quantity: " << product.getQuantity() << ")\n";
    }

    Product* productToBuy = marketplace.findProduct("Laptop");
    if (productToBuy) {
        customer1.buyProduct(*productToBuy, 2);
    }

    return 0;
}
