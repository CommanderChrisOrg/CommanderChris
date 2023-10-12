#include <iostream>
#include <string>
#include <curl/curl.h>
#include <regex>

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string queryGPT(const std::string& data) {
    CURL* curl = curl_easy_init();
    CURLcode res;
    std::string readBuffer;

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/chat/completions");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "Authorization: Bearer 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");  // Replace with your key
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

    return readBuffer;
}

std::string extractContentFromResponse(const std::string& response) {
    std::string key = "\"content\": ";
    size_t startIdx = response.find(key);
    if (startIdx == std::string::npos) return ""; // Key not found

    startIdx += key.length(); // Move index to start of content value

    // Find the closing quote of the content value
    size_t endIdx = response.find("\"", startIdx + 1);
    if (endIdx == std::string::npos) return ""; // Closing quote not found

    return response.substr(startIdx + 1, endIdx - startIdx - 1); // +1 to skip the opening quote
}

int main() {
    // 1. Get user input
    std::cout << "Enter your command line-related question: ";
    std::string userQuestion;
    std::getline(std::cin, userQuestion);

    // 2. Set context & 3. Ask GPT to categorize user's query
    std::string context = "You are assisting with categorizing command line-related queries into one of the following categories: 1) File & Directory Navigation, 2) File & Directory Manipulation, 3) File Content Inspection, 4) File System Status & Disk Usage, 5) File Permissions & Ownership, 6) Archiving & Compression, 7) Searching & Finding.";
    std::string question = "Given the question '" + userQuestion + "', which single category number does it belong to? Just the number, please.";
    std::string model_name = "gpt-3.5-turbo";  // Adjust this to your desired model

    std::string contextAndQuestion = "[{\"role\":\"system\",\"content\":\"" + context + "\"},{\"role\":\"user\",\"content\":\"" + question + "\"}]";

    std::string categorizationPrompt = "{\n"
    "  \"model\": \"" + model_name + "\",\n"
    "  \"messages\": " + contextAndQuestion + ",\n"
    "  \"max_tokens\": 150\n"
    "}";


    
    std::string categoryResponse = queryGPT(categorizationPrompt);
    std::cout<<categoryResponse<<std::endl;

    // Extract category (this is a basic extraction, improvements might be needed based on actual GPT responses)
    int categoryNum = -1;
    std::regex r("\"content\": \"(\\d+)\"");
    std::smatch match;

    if (std::regex_search(categoryResponse, match, r) && match.size() > 1) {
        categoryNum = std::stoi(match.str(1));
    }

    std::cout << categoryNum << std::endl;

    // 4. Depending on the category, craft the final prompt
    std::string finalPrompt;
    switch(categoryNum) {
        case 1:  // File & Directory Navigation
            finalPrompt = "Provide a concise terminal command related to navigating files or directories for: '" + userQuestion + "'. Avoid explanations, just the command. Just the command, please.Do not include any additional text or explanations.";
            break;
        case 2:  // File & Directory Manipulation
            finalPrompt = "I need a direct terminal command to manipulate files or directories for the task: '" + userQuestion + "'. No extra details, just the command. Just the command, please.Do not include any additional text or explanations.";
            break;
        case 3:  // File Content Inspection
            finalPrompt = "What's the succinct terminal command to inspect the contents of a file in relation to: '" + userQuestion + "'? Only the command, no elaboration. Just the command, please.Do not include any additional text or explanations.";
            break;
        case 4:  // File System Status & Disk Usage
            finalPrompt = "For checking file system status or disk usage, what would be the exact terminal command for: '" + userQuestion + "'? Just the command, please.Do not include any additional text or explanations.";
            break;
        case 5:  // File Permissions & Ownership
            finalPrompt = "In terms of adjusting file permissions or ownership via the terminal, how would one execute the task: '" + userQuestion + "'? Just the command, please.Do not include any additional text or explanations.";
            break;
        case 6:  // Archiving & Compression
            finalPrompt = "If someone wanted to achieve the goal: '" + userQuestion + "' in the context of archiving or compression, what's the straight-to-the-point terminal command?  Just the command please.Do not include any additional text or explanations.";
            break;
        case 7:  // Searching & Finding
            finalPrompt = "When searching or finding files from the terminal for the purpose: '" + userQuestion + "', what's the direct command without any additional context? Just the command please. Do not include any additional text or explanations.";
            break;
        default:
            std::cerr << "Invalid or unrecognized category." << std::endl;
            return -1;
    }

    // std::string commandData = "{\"prompt\":\"" + finalPrompt + "\", \"max_tokens\":150}";
    // std::string commandData = "{\"model\":\"" + model_name + "\",\"messages\":" + finalPrompt + ",\"max_tokens\":150}";

    finalPrompt = "[{\"role\":\"user\",\"content\":\"" + finalPrompt + "\"}]";


    std::string commandData = "{\n"
    "  \"model\": \"" + model_name + "\",\n"
    "  \"messages\": " + finalPrompt + ",\n"
    "  \"max_tokens\": 150\n"
    "}";

    std::cout << commandData << std::endl;

    std::string commandResponse = queryGPT(commandData);

    std::cout << commandResponse << std::endl;

    // 5. Extract & display the command to the user (basic extraction, assumes response contains command within backticks)
    std::string command;

    // Try extracting command within backticks
    size_t startIdx = commandResponse.find('`');
    size_t endIdx = commandResponse.rfind('`');

    if (startIdx != std::string::npos && endIdx != std::string::npos && startIdx < endIdx) {
        command = commandResponse.substr(startIdx+1, endIdx-startIdx-1);
    } 
    // If no backticks are found, assume the entire response is the command
    else {
        command = extractContentFromResponse(commandResponse);
        // command = commandResponse;
    }

    // Display the extracted command
    if (!command.empty()) {
        std::cout << "Command: " << command << std::endl;
    } else {
        std::cerr << "Failed to extract command from the response." << std::endl;
    }

    return 0;
}


