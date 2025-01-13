# STORE-CLI: Your Command-Line E-Commerce Application

STORE-CLI is a command-line e-commerce application built with C++ for managing users and their transactions. It currently uses JSON files for data persistence. Future development will include product and category management and expanded administrator capabilities.

## Features

**For Users:**

- Browse products by category.
- View product details (name, price, description, stock).
- Securely purchase products.
- Top-up account balance.
- View transaction history.
- Edit profile.

**For Administrators:**

- Dashboard with key statistics (total users, active users, products, categories, transactions).
- Product management (add, update, delete, restock).
- Category management (add, update, delete).
- User management (view, toggle access).
- Store management (name, owner name, description, phone number, view)

## Architecture

STORE-CLI follows a multi-tier architecture, separating concerns into different components:

- **Entities:** Represent core data structures like `Product`, `Category`, `User`, and `Transaction`.
- **Repositories:** Handle data persistence using JSON files. A `BaseRepository` provides common functionality.
- **Services:** Implement business logic related to products, categories, users, shopping, and authentication.
- **Menus:** Provide the user interface for both shoppers and administrators.
- **Utils:** Helper functions for input validation and formatting.

## Getting Started

1.  Clone the repository:

```sh
git clone https://github.com/CallMeJaja/STORE-CLI && cd STORE-CLI
```

2.  Create a build directory:

```sh
mkdir build
cd build
```

3.  Generate the build files:

```sh
cmake .. -G "MinGW Makefiles"
```

4. Compile the project:

```sh
cmake --build .
```

5. Run the executable:

```sh
cd .. && .\bin\store-cli.exe
```

or

```sh
cd .. && .\run.bat
```

## Data Storage

Currently uses JSON files for data persistence. This will be replaced with a database in a future version.

## Future Development

- **Implement Session Hadnling:** Add a secure session management system for persistent login and user-specific states.
- **Implement a Database:** Migrate data storage to a relational database (e.g., PostgreSQL, MySQL) or NoSQL database (e.g., MongoDB).
- **Expand Functionality:** Add administrator features, product browsing, shopping cart functionality, and order processing.
- **Error Handling:** Enhance error handling and reporting for greater robustness.
- **Testing:** Implement unit and integration tests.

## Contributing

Contributions are welcome! Please fork the repository and create a pull request with your changes.

## License

This project is licensed under the MIT License.

## Contact

For any questions or suggestions, please open an issue or contact the project maintainer:

rezaaasriano@gmail.com

Or contact me directly on WhatsApp: [https://s.id/CallMeJajaWA]
