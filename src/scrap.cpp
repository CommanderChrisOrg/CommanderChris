#include "scrap.hpp"

#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
#include <filesystem>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

const fs::path scrapersDir = "scrapers";
const fs::path contextDir = "context";

std::string get_context() {
    if (!fs::exists(contextDir) || !fs::is_directory(contextDir)) {
        std::cerr << "Failed to load context. Invalid directory.";
        return "";
    }

    std::ostringstream context;
    try {        
        // Iterate over the directory contents
        for (const auto& entry : fs::directory_iterator(contextDir)) {
            // Check if it's a file, not a directory
            if (entry.is_regular_file()) {
                std::ifstream file(entry.path());
                if (file) context << file.rdbuf();
                else std::cerr << "Unable to open file: " << entry.path() << std::endl;
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return "";
    }
    return context.str();
}

void run_scraper() {
    const std::vector<fs::path> executables = {
        fs::path("get_hardware_macos.sh"),
        fs::path("get_network_macos.sh"),
        fs::path("get_processes_macos.sh"),
        // fs::path("get_software_linux.sh"),
        fs::path("get_software_macos.sh")
    };

    std::vector<pid_t> processes;

    // Create context diretory if non-existent
    if (!fs::exists(contextDir) && !fs::create_directory(contextDir)) {
        std::cerr << "Failed to create directory." << std::endl;
    }

    // Runs the scrapers
    for (const fs::path exec : executables) {
        const fs::path command = scrapersDir / exec;

        pid_t pid = fork();
        processes.push_back(pid);

        if (pid < 0) {
            std::cerr << "Failed to fork a new process." << std::endl;
        } else if (pid == 0) {
            // Child process
            char *args[] = {
                const_cast<char*>(command.c_str()),
                const_cast<char*>(contextDir.c_str()),
                NULL
            };
            // char *args[] = {"context", NULL};
            execvp(args[0], args);
            std::cerr << "Error executing " << command << std::endl;
            exit(1);
        }
    }

    // Wait for the scrapers to finish executing
    for (const pid_t pid : processes) {
        int status;
        waitpid(pid, &status, 0); // Wait for the child process to finish
        if (status != 0) {
            // TODO: logging
        }
    }
}


