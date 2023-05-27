#include "PutRequestClient.hpp"
#include <string>

PutRequestClient::PutRequestClient(const std::string& url) : HttpRequest(url)
{
    if (!m_curlPtr)
    {
        std::cout << "curl_easy_init failed !! [put client] \n";
        return;
    }
}

bool PutRequestClient::init()
{
    if (!m_curlPtr)
    {
        std::cout << "curl_easy_init failed !! [put client init] \n";
        return false;
    }
    curl_easy_setopt(m_curlPtr, CURLOPT_CUSTOMREQUEST, "PUT");    
    return true;
}
