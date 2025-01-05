# J-STORE: Your One-Stop E-commerce Solution

J-STORE is a command-line e-commerce application built with C++. It provides a platform for users to browse products, manage their shopping carts, and complete purchases. Administrators can manage products, categories, and user accounts, as well as access various reports for business insights.

## Features

**For Users:**

- Browse products by category.
- View product details (name, price, description, stock).
- Manage shopping cart.
- Securely purchase products.
- Top-up account balance.
- View transaction history.
- Edit profile (planned).

**For Administrators:**

- Dashboard with key statistics (total users, active users, products, categories, transactions).
- Product management (add, update, delete, restock).
- Category management (add, update, delete).
- User management (view, toggle access).
- Generate reports (transactions, revenue, user spending).
- Low stock alerts.

## Architecture

J-STORE follows a multi-tier architecture, separating concerns into different components:

- **Entities:** Represent core data structures like `Product`, `Category`, `User`, and `Transaction`.
- **Repositories:** Handle data persistence using JSON files. A `BaseRepository` provides common functionality.
- **Services:** Implement business logic related to products, categories, users, shopping, and authentication.
- **Menus:** Provide the user interface for both shoppers and administrators.
- **Utils:** Helper functions for input validation and formatting.

## Getting Started

1.  Clone the repository: (Instructions on how to clone would go here if this were a real project hosted online).
2.  Compile the code: `g++ src/main.cpp src/menus/*.cpp src/services/*.cpp`.
3.  Run the executable: `.\a.exe`.

## Usage

Upon launching the application, you'll be presented with the main menu. Choose to sign in, sign up, or view services. Follow the on-screen prompts to navigate through the application.

## Contributing

Contributions are welcome! (Instructions for contributing, including coding style guidelines and branching strategies, would be added here).

## Future Development

- Implement a database for more robust data management.
- Enhanced search and filtering capabilities.
- Order management features.
- Improved reporting and analytics.
- Graphical User Interface (GUI).

## Contact

(Contact information or a link to a developer profile would be here).

## License

(License information, such as MIT or GPL, would be stated here).
