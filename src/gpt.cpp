#include <iostream>
#include <string>
#include <curl/curl.h>
#include <boost/json.hpp>
#include "context.hpp"

namespace json=boost::json;

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string query_gpt(const std::string& data) {
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

std::string clean_command_string(std::string str){
    str.erase(0, str.find_first_not_of("\n"));
    str.erase(str.find_last_not_of("\n") + 1);
    return str;
}

std::string get_command_from_prompt(std::string prompt){
    std::string model_name = "gpt-4-1106-preview";  // Adjust this to your desired model

    json::array request_messages {
        json::object{
            {"role", "system"},
            {"content", "CONTEXT BEGIN\n\n" + get_context() + "\nCONTEXT END\n\nPlease provide the command that answers the user's question. The format for your response is\n\nAttempts:[potential commands with explanations]\nFinal Answer:```[command]```\n\nYou can use the information between \"CONTEXT BEGIN\" and \"CONTEXT END\" to aid in constructing your response"}
        },
        json::object{
            {"role", "user"},
            {"content", prompt}
        }
    }; 

    json::object command_data {
        {"model", model_name},
        {"messages", request_messages}
    };

    json::object response_data = json::parse(query_gpt(json::serialize(command_data))).as_object();

    std::string response = boost::json::value_to<std::string>(response_data.at("choices").as_array()[0].as_object().at("message").as_object().at("content"));

    // Try extracting command within triple backticks
    size_t answer_idx = response.find("Final Answer");
    if(answer_idx == std::string::npos) return "";

    size_t start_idx = response.find("```", answer_idx) + 3;
    if(start_idx == std::string::npos) return "";

    size_t end_idx = response.find("```", start_idx + 1);
    if(end_idx == std::string::npos) return "";

    return clean_command_string(response.substr(start_idx, end_idx - start_idx));
}
