#pragma once

#include <cstdint>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <unordered_map>

// checkout system
struct customer_t {
    uint64_t    id{0};    // unique
    std::string name;
    std::string phone;
    std::string address;
};

struct category_t {
    uint64_t    id{0};    // unique, clothes, shoes, food, drink, laptop, etc.
    double      price{0};
    std::string name;
    std::string brand;
};

struct product_t {
    uint64_t product_id{0};
    uint64_t category_id{0};
    // in stock date, ...
};

struct order_record_t {
    uint64_t               customer_id{0};    // necessary??
    uint64_t               creation_datetime{0};
    std::vector<product_t> items;
};

struct payment_method_t {
    uint8_t  type{0};    // credit card, debit card, cash, apple pay, etc.
    uint64_t account_id{0};
    // whatever others ...
};

struct IInventory {
    virtual std::vector<category_t> GetAllCategories() const                                  = 0;
    virtual bool                    GetProduct(uint64_t product_id, product_t& product) const = 0;
    virtual uint32_t                GetTotalProductInStock(uint64_t category_id) const        = 0;
    virtual bool                    AddCategory(const category_t& cat)                        = 0;
    virtual bool                    AddProduct(const product_t& product)                      = 0;
    virtual bool                    ReduceProduct(const product_t& product, uint32_t count)   = 0;
};

struct ICustomerDataAccess {
    virtual bool     GetCustomer(uint64_t customer_id, customer_t& customer) const                              = 0;
    virtual bool     GetCustomer(const std::string& customer_phone, customer_t& customer) const                 = 0;
    virtual uint64_t AddCustomer(const std::string& name, const std::string& phone, const std::string& address) = 0;
    virtual bool     UpdateCustomer(uint64_t customer_id, const std::string& phone, const std::string& address) = 0;
};

struct IShoppingCart {
    virtual bool           AddProduct(uint64_t product_id)                   = 0;
    virtual bool           AddProducts(uint64_t category_id, uint32_t count) = 0;
    virtual bool           RemoveProduct(uint64_t product_id)                = 0;
    virtual order_record_t Populate()                                        = 0;
};

struct ICheckout {
    virtual bool Checkout(const order_record_t& order, const payment_method_t& method) = 0;
};

struct Inventory : public IInventory {
    std::vector<category_t> GetAllCategories() const override
    {
        // TODO: some cache rather than return std::vector<category_t> ...
        return {};
    }
    bool GetProduct(uint64_t product_id, product_t& product) const override
    {
        if (auto iter = products_.find(product_id); iter != products_.end()) {
            product = iter->second;
            return true;
        }
        return false;
    }
    uint32_t GetTotalProductInStock(uint64_t category_id) const override
    {
        if (auto iter = countProductsInStock_.find(category_id); iter != countProductsInStock_.end()) {
            return iter->second;
        }
        return 0;
    }
    bool AddCategory(const category_t& cat) override
    {
        // TODO: ...
        return true;
    }
    bool AddProduct(const product_t& product) override
    {
        // TODO: ...
        return true;
    }
    bool ReduceProduct(const product_t& product, uint32_t count) override
    {
        // TODO: ...
        return true;
    }

    // <category id, category_t>, cover the whole inventory
    std::unordered_map<uint64_t, category_t> categories_;

    // <category_id, # products in stock>
    std::unordered_map<uint64_t, uint32_t> countProductsInStock_;

    // <product id, product_t>, cover the whole inventory
    std::unordered_map<uint64_t, product_t> products_;
};

struct CustomerDB : public ICustomerDataAccess {

    bool     GetCustomer(uint64_t customer_id, customer_t& customer) const override { return true; }
    bool     GetCustomer(const std::string& customer_phone, customer_t& customer) const override { return true; }
    uint64_t AddCustomer(const std::string& name, const std::string& phone, const std::string& address) override
    {
        return 0;
    }
    bool UpdateCustomer(uint64_t customer_id, const std::string& phone, const std::string& address) override
    {
        return 0;
    }
};

struct ShoppingCart : public IShoppingCart {
    ShoppingCart(customer_t customer) : customer_{customer} {}

    bool AddProduct(uint64_t product_id) override
    {
        // TODO: ...
        return true;
    }
    bool AddProducts(uint64_t category_id, uint32_t count) override
    {
        // TODO: ...
        return true;
    }
    bool RemoveProduct(uint64_t product_id) override
    {
        // TODO: ...
        return true;
    }
    order_record_t Populate() override { return order_; }

    order_record_t order_;
    customer_t     customer_;
};

struct MoneyPrinter : public ICheckout {
    MoneyPrinter(IInventory* inventory) : inventory_{inventory} {}
    bool Checkout(const order_record_t& order, const payment_method_t& method) override
    {
        // TODO: ...

        for (const auto& item : order.items) {
            inventory_->ReduceProduct(item, 1);
        }
        return true;
    }
    IInventory* inventory_{nullptr};
};

void run_checkout_system()
{
    Inventory shared_inventory;
    shared_inventory.AddCategory({});
    shared_inventory.AddProduct({});

    CustomerDB shared_customer_db;

    std::string phone("123");
    customer_t  my_customer;
    shared_customer_db.GetCustomer(phone, my_customer);

    ShoppingCart my_cart{my_customer};
    my_cart.AddProduct(10);
    my_cart.AddProduct(20);
    my_cart.AddProducts(30, 2);
    my_cart.RemoveProduct(20);

    auto my_order = my_cart.Populate();

    payment_method_t my_payment;

    MoneyPrinter shared_printer{&shared_inventory};
    shared_printer.Checkout(my_order, my_payment);
};

