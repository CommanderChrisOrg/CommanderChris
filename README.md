# CommanderChris - Build and Run Guide 🚀

Welcome to CommanderChris! This guide will walk you through the steps to build and run the application. 🛠️

![demo](https://github.com/CommanderChrisOrg/CommanderChris/assets/43506607/8f1b4fcc-35f0-4da3-9767-f036f27d16e8)

## Description 📖

CommanderChris is a cutting-edge CLI tool for Linux and MacOS, utilizing GPT-4 for intelligent, personalized command suggestions. Tailored to your system's specifics, it simplifies command-line operations, making it an invaluable tool for both beginners and experts. 🌐🤖

### Key Features 🔑

- **Intuitive Interface**: Ask any system-related question and receive an easy-to-understand answer. 🗣️👂
- **Personalized Suggestions**: Chris has a deep understanding of your system. 💻🔧
- **Adaptive Learning**: Improves over time based on your usage patterns. 📈🧠
- **Cross-Platform Support**: Works seamlessly on Linux and MacOS. 🍏🐧
- **Privacy-Focused**: Your data stays on your machine, ensuring security and privacy. 🔒🛡️

## Prerequisites 📋

Before you begin, ensure the following prerequisites are installed on your system:

- [CMake](https://cmake.org/) 🔧
- [Make](https://www.gnu.org/software/make/) 🛠️
- [Boost](https://www.boost.org/) ⚡

### macOS 🍏
- Open Terminal and run:
  ```
  brew install make cmake boost
  ```

### Windows 💻
- For CMake, download and install from [CMake's official website](https://cmake.org/download/). Add it to the system PATH.
- For Make and Boost:
  - Install Chocolatey from [Chocolatey's website](https://chocolatey.org/).
  - Open Command Prompt and run:
    ```
    choco install make
    ```
  - Download Boost from [Boost's official website](https://www.boost.org/users/download/), and extract the files to your desired directory.

### Linux 🐧
- Open Terminal and run:
  ```
  sudo add-apt-repository ppa:mhier/libboost-latest
  sudo apt update
  sudo apt-get install make cmake boost1.83
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

Interested in contributing to CommanderChris? Open an issue or submit a pull request on [GitHub](https://github.com/markorenic/CommanderChris).

## License 📄

[TBD]

Join the CommanderChris community today and elevate your command-line experience! 💫🚀
