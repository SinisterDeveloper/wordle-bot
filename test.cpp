#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <curl/curl.h>

// Callback to write fetched data into a std::string
size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t totalSize = size * nmemb;
    std::string *buffer = static_cast<std::string *>(userp);
    buffer->append(static_cast<char *>(contents), totalSize);
    return totalSize;
}

int main()
{
    const std::string url = "https://gist.githubusercontent.com/cfreshman/a03ef2cba789d8cf00c08f767e0fad7b/raw/c46f451920d5cf6326d550fb2d6abb1642717852/wordle-answers-alphabetical.txt";

    CURL *curl;
    CURLcode res;
    std::string response;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Follow redirects if any
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            std::cerr << "curl_easy_perform() failed: "
                      << curl_easy_strerror(res) << std::endl;
        }
        else
        {
            // Log the full response to the console
            std::cout << "--- Full cURL Response ---\n"
                      << response << "\n--------------------------\n";
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    std::vector<std::string> words;
    std::stringstream ss(response);
    std::string line;
    while (std::getline(ss, line))
    {
        if (!line.empty())
        {
            words.push_back(line);
        }
    }

    // Verify by printing first 10 words
    for (size_t i = 0; i < 10 && i < words.size(); i++)
    {
        std::cout << words[i] << "\n";
    }

    std::cout << "Total words: " << words.size() << std::endl;

    return 0;
}