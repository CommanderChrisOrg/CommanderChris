# CommanderChris - Build and Run Guide 🚀

Welcome to CommanderChris! This guide will walk you through the steps to build and run the application. 🛠️

![demo](https://github.com/CommanderChrisOrg/CommanderChris/assets/43506607/d49843ea-6049-4b08-951f-be44e4a1d57d)



## Prerequisites 📋

Before you begin, make sure you have the following prerequisites installed on your system:

- [CMake](https://cmake.org/) 🔧
- [Make](https://www.gnu.org/software/make/) 🛠️
- [Boost](https://www.boost.org/) ⚡

### macOS 🍏
- Open Terminal and run:
  ```
  brew install make cmake boost
  ```

### Windows 💻
- For CMake, download and install from [CMake's official website](https://cmake.org/download/). Make sure to add it to the system PATH.
- For Make and Boost:
  - First, install Chocolatey from [Chocolatey's website](https://chocolatey.org/).
  - Then, open Command Prompt and run:
    ```
    choco install make
    ```
  - Download Boost from [Boost's official website](https://www.boost.org/users/download/), extract the files, and place them in a directory of your choice.

### Linux 🐧
- Open Terminal and run:
  ```
  sudo apt-get install make cmake libboost-all-dev
  ```
  

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
   make -j4
   ```

4. Run CommanderChris:

   ```bash
   ./chris
   ```

## Contributing 💡

If you would like to contribute to CommanderChris, please open an issue or submit a pull request on [GitHub](https://github.com/markorenic/CommanderChris).

## License 📄

[TBD]
