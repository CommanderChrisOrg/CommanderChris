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

    startIdx += key.length() + 1; // Move index to start of content value

    // Find the closing quote of the content value
    size_t endIdx = response.find("\n", startIdx) - 1;
    if (endIdx == std::string::npos) return ""; // Closing quote not found

    return response.substr(startIdx, endIdx - startIdx); // +1 to skip the opening quote
}

bool findAndReplace(std::string &str, std::string toReplace, std::string replaceWith){
    std::size_t pos = str.find(toReplace);
    if (pos == std::string::npos) return false;
    str.replace(pos, toReplace.length(), replaceWith);
    return true;
}

std::string processCommandString(std::string str){
    while(findAndReplace(str, "bash", ""));
    while(findAndReplace(str, "shell", ""));
    while(findAndReplace(str, "\\n", ""));
    while(findAndReplace(str, "\\\"", "\""));
    while(findAndReplace(str, "$ ", ""));
    return str;
}

std::string getCommandFromPrompt(std::string prompt){
    std::string model_name = "gpt-3.5-turbo";  // Adjust this to your desired model

    std::string request_messages = "[{\"role\":\"user\",\"content\":\"" + prompt + "\\nGive me a unix command\"}]";

    std::string commandData = "{\n"
    "  \"model\": \"" + model_name + "\",\n"
    "  \"messages\": " + request_messages + ",\n"
    "  \"max_tokens\": 200\n"
    "}";

    std::string commandResponse = queryGPT(commandData);

    std::string refined_request_messages = "[{\"role\":\"user\",\"content\":\"" + prompt + "\\nGive me a unix command\"},{\"role\":\"assistant\",\"content\":\"" + extractContentFromResponse(commandResponse) + "\"},{\"role\":\"user\",\"content\":\"Now act as a unix professional, who is an expert in the field. How would you criticize your reply to the question above and correct its mistakes?\"}]";

    std::string refinedCommandData = "{\n"
    "  \"model\": \"" + model_name + "\",\n"
    "  \"messages\": " + refined_request_messages + "\n"
    "}";

    std::string refinedCommandResponse = queryGPT(refinedCommandData);

    // Try extracting command within backticks
    size_t startIdx = refinedCommandResponse.find("```") + 3;
    if(startIdx == std::string::npos) return "";

    size_t endIdx = refinedCommandResponse.find("```", startIdx + 1);
    if(endIdx == std::string::npos) return "";

    return processCommandString(refinedCommandResponse.substr(startIdx, endIdx - startIdx));
}
