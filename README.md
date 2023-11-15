# CommanderChris - Build and Run Guide 🚀

Welcome to CommanderChris! This guide will walk you through the steps to build and run the application. 🛠️

## Prerequisites 📋

Before you begin, make sure you have the following prerequisites installed on your system:

- CMake 🔧
- Make 🛠️

## Getting Started 🌟

1. Clone this repository to your local machine:

   ```bash
   git clone https://github.com/markorenic/CommanderChris.git
   ```

2. Navigate to the project's root directory:

   ```bash
   cd CommanderChris
   ```

## Build and Run 🏗️

1. Create a build directory and move into it:

   ```bash
   mkdir build
   cd build
   ```

2. Configure the project using CMake:

   ```bash
   cmake ..
   ```

3. Build the project with multiple CPU cores (adjust the number as needed):

   ```bash
   make
   ```

4. Copy the scrapers into the same directory as the executable:
   
   ```bash
   cp -r ../src/scrapers scrapers
   ```

5. Run CommanderChris:

   ```bash
   ./CommanderChris
   ```

## Contributing 💡

If you would like to contribute to CommanderChris, please open an issue or submit a pull request on [GitHub](https://github.com/markorenic/CommanderChris).

## License 📄

This project is licensed under the [MIT License](LICENSE).
