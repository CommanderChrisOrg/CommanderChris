#include <iostream>
#include <string>
#include <curl/curl.h>

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
        std::string authorizationHeader = "Authorization: Bearer ";
        authorizationHeader += std::getenv("OPENAI_API_KEY");
        headers = curl_slist_append(headers, authorizationHeader.c_str());  // Replace with your key
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
        
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

    return readBuffer;
}

std::size_t findAndReplace(std::string &str, std::string toReplace, std::string replaceWith, size_t cur_pos = 0){
    std::size_t pos = str.find(toReplace, cur_pos);
    if (pos == std::string::npos) return std::string::npos;
    str.replace(pos, toReplace.length(), replaceWith);
    return pos + replaceWith.length();
}

void findAndReplaceAll(std::string &str, std::string toReplace, std::string replaceWith){
    std::size_t pos = 0;
    while((pos = findAndReplace(str, toReplace, replaceWith, pos)) != std::string::npos);
}

std::string processPromptString(std::string str){
    findAndReplaceAll(str, "\"", "\\\"");
    findAndReplaceAll(str, "\n", "\\n");
    return str;
}

std::string processCommandString(std::string str){
    findAndReplaceAll(str, "\\\"", "\"");
    findAndReplaceAll(str, "bash", "");
    findAndReplaceAll(str, "shell", "");
    findAndReplaceAll(str, "\\n", "\n");
    return str;
}

std::string getCommandFromPrompt(std::string context, std::string prompt){
    std::string model_name = "gpt-4-1106-preview";  // Adjust this to your desired model

    std::string request_messages = "[{\"role\":\"system\",\"content\":\"CONTEXT BEGIN\\n\\n"+processPromptString(context)+"\\n\\nCONTEXT END\\n\\nPlease provide the command that answers the user's question. The format for your response is\\n\\nAttempts:[potential commands with explanations]\\nFinal Answer:```[command]```\\n\\nYou can use the information between \\\"CONTEXT BEGIN\\\" and \\\"CONTEXT END\\\" to aid in constructing your response\"},{\"role\":\"user\",\"content\":\"" + processPromptString(prompt) + "\"}]";

    std::string commandData = "{\n"
    "  \"model\": \"" + model_name + "\",\n"
    "  \"messages\": " + request_messages + ""
    "}";

    std::string commandResponse = queryGPT(commandData);

    std::cout << commandResponse << std::endl;

    // Try extracting command within triple backticks
    size_t answerIdx = commandResponse.find("Final Answer");
    if(answerIdx == std::string::npos) return "";

    size_t startIdx = commandResponse.find("```", answerIdx) + 3;
    if(startIdx == std::string::npos) return "";

    size_t endIdx = commandResponse.find("```", startIdx + 1);
    if(endIdx == std::string::npos) return "";

    return processCommandString(commandResponse.substr(startIdx, endIdx - startIdx));
}
