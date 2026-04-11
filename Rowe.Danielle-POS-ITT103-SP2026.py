import datetime

#List of products
products = {
    "Rice/lb": {"price": 85.0, "stock": 20},
    "Milk": {"price": 250.0, "stock": 15},
    "Bread": {"price": 450.0, "stock": 25},
    "Egg": {"price": 60.0, "stock": 30},
    "Chicken/lb": {"price": 245.0, "stock": 10},
    "Oxtail/lb": {"price": 600.0, "stock": 40},
    "Toothpaste": {"price": 380.0, "stock": 35},
    "Soap": {"price": 500.0, "stock": 20},
    "Sugar/lb": {"price": 105.0, "stock": 25},
    "Flour/lb": {"price": 95.0, "stock": 20},
    "Cooking Oil": {"price": 650.0, "stock": 15},
    "Butter": {"price": 750.0, "stock": 10},
    "Cheese": {"price": 800.0, "stock": 12},
    "Soda": {"price": 120.0, "stock": 30},
    "Juice": {"price": 100.0, "stock": 25},
    "Water": {"price": 100.0, "stock": 50},
    "Syrup": {"price": 210.0, "stock": 40},
    "Cereal": {"price": 1100.0, "stock": 15},
    "Tea Bag (Box of 12)": {"price": 400.0, "stock": 20},
    "Packet Instant Coffee": {"price": 60.0, "stock": 100}
}

cart = {}

#Validation of stock availabilty
def get_valid_number(prompt):
    while True:
        try:
            value = float(input(prompt))
            if value < 0:
                print("[ERROR] Value cannot be negative.")
            else:
                return value
        except ValueError:
            print("[ERROR] Invalid input. Enter a number.")

def get_valid_int(prompt):
    while True:
        try:
            value = int(input(prompt))
            if value <= 0:
                print("[ERROR] Must be greater than 0.")
            else:
                return value
        except ValueError:
            print("[ERROR] Invalid input. Enter a whole number.")

#Menu
def show_menu():
    print("\n---------POS MENU---------")
    print("1. Add Item")
    print("2. Remove Item")
    print("3. View Cart")
    print("4. Checkout")
    print("5. New Transaction")
    print("6. Exit")

#Add item
def add_item():
    print("\nAvailable Products:")
    print("-"*40)
    for item, info in products.items():
        print(f"{item:<15} ${info['price']:<8.2f} Stock: {info['stock']}")
    print("-"*40)

    #Loops until a valid product is selected
    while True:
        user_input = input("Enter product name: ").lower()
        matches = [item for item in products if user_input in item.lower()]
        if len(matches) == 1:
            name = matches[0]
            break
        elif len(matches) > 1:
            print("Multiple matches found:", matches)
        else:
            print("Product not found. Try again.")

    qty = get_valid_int("Enter quantity: ")
    if qty > products[name]["stock"]:
        print("Not enough stock available.")
        return

    if name in cart:
        cart[name]["qty"] += qty
    else:
        cart[name] = {"price": products[name]["price"], "qty": qty}

    products[name]["stock"] -= qty
    print("Item added to cart.")

#To remove an item
def remove_item():
    if not cart:
        print("Cart is empty.")
        return

    name = input("Enter item to remove: ").title()
    if name not in cart:
        print("Item not in cart.")
        return

    qty = get_valid_int("Enter quantity to remove: ")
    if qty > cart[name]["qty"]:
        print("You cannot remove more than in cart.")
        return

    if qty == cart[name]["qty"]:
        products[name]["stock"] += cart[name]["qty"]
        del cart[name]
    else:
        cart[name]["qty"] -= qty
        products[name]["stock"] += qty

    print("Item removed.")

#To view indide the cart
def view_cart():
    if not cart:
        print("Cart is empty.")
        return

    print("\n------- CART -------")
    subtotal = 0
    for item, details in cart.items():
        total = details["price"] * details["qty"]
        subtotal += total
        print(f"{item:<15}{details['qty']:>5}{details['price']:>10.2f}{total:>10.2f}")
    print("--------------------")
    print(f"{'Subtotal:':<25}${subtotal:.2f}")

#Calculations
def calculate_totals():
    subtotal = sum(details["price"] * details["qty"] for details in cart.values())
    tax = subtotal * 0.10
    discount = subtotal * 0.05 if subtotal > 5000 else 0
    total = subtotal + tax - discount
    return subtotal, tax, discount, total

#Payment
def process_payment(total):
    while True:
        payment = get_valid_number("Enter amount paid: $")
        if payment < total:
            print("Payment too low.")
        else:
            return payment, payment - total

#Reciept
def print_receipt(subtotal, tax, discount, total, paid, change):
    now = datetime.datetime.now()

    print("\n" + "="*40)
    print("        Best Buy Retail Store        ")
    print(" 13 Princess Street, Downtown Kingston")
    print("        Tel: (876) 562-8242")
    print("="*40)
    print(f"Date: {now.strftime('%Y-%m-%d')}  Time: {now.strftime('%H:%M:%S')}")
    print("-"*40)
    print(f"{'Item':<15}{'Qty':>5}{'Price':>10}{'Total':>10}")
    print("-"*40)

    for item, details in cart.items():
        total_item = details["price"] * details["qty"]
        print(f"{item:<15}{details['qty']:>5}{details['price']:>10.2f}{total_item:>10.2f}")

    print("-"*40)
    print(f"{'Subtotal:':<25}${subtotal:.2f}")
    print(f"{'Tax (10%):':<25}${tax:.2f}")
    if discount > 0:
        print(f"{'Discount:':<25}-${discount:.2f}")
    print("-"*40)
    print(f"{'TOTAL:':<25}${total:.2f}")
    print("-"*40)
    print(f"{'Cash Paid:':<25}${paid:.2f}")
    print(f"{'Change:':<25}${change:.2f}")
    print("="*40)
    print("     THANK YOU FOR SHOPPING WITH US!")
    print("         PLEASE COME AGAIN!")
    print("="*40)

#To signalize items being low in stock
def check_low_stock():
    low_items = [item for item, info in products.items() if info["stock"] < 5]
    if low_items:
        print("\n⚠ LOW STOCK ALERT:")
        for item in low_items:
            print(f"{item} ({products[item]['stock']} left)")

#Main
def main():
    global cart
    while True:
        show_menu()
        choice = input("Choose an option: ")

        if choice == "1":
            add_item()
        elif choice == "2":
            remove_item()
        elif choice == "3":
            view_cart()
        elif choice == "4":
            if not cart:
                print("Cart is empty.")
                continue
            confirm = input("Proceed to checkout? (y/n): ").lower()
            if confirm != "y":
                continue
            subtotal, tax, discount, total = calculate_totals()
            paid, change = process_payment(total)
            print_receipt(subtotal, tax, discount, total, paid, change)
            check_low_stock()
            cart.clear()  # automatically reset cart after checkout
        elif choice == "5":
            cart.clear()
            print("New transaction started.")
        elif choice == "6":
            print(" Exiting system.")
            break
        else:
            print("Invalid option.")

#Run code
main()
