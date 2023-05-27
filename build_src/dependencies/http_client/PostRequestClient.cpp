#include "PostRequestClient.hpp"
#include <string>

PostRequestClient::PostRequestClient(const std::string& url) : HttpRequest(url)
{
    if (!m_curlPtr)
    {
        std::cout << "curl_easy_init failed !! [post client] \n";
        return;
    }
}

bool PostRequestClient::init()
{
    if (!m_curlPtr)
    {
        std::cout << "curl_easy_init failed !! [post client init] \n";
        return false;
    }
    curl_easy_setopt(m_curlPtr, CURLOPT_CUSTOMREQUEST, "POST");    
    return true;
}
