#include "GetRequestClient.hpp"
#include <string>

GetRequestClient::GetRequestClient(std::string url) : HttpRequest(url)
{
    if (!m_curlPtr)
    {
        std::cout << "curl_easy_init failed !! \n";
        return;
    }
}

bool GetRequestClient::init()
{
    if (!m_curlPtr)
    {
        std::cout << "curl_easy_init failed !! \n";
        return false;
    }
    curl_easy_setopt(m_curlPtr, CURLOPT_CUSTOMREQUEST, "GET");    
    return true;
}
