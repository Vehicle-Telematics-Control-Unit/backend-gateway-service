#include "HttpRequest.hpp"
#include <string>
#include <memory>


std::atomic<bool> HttpRequest::s_isGloballyInitialized{false};

static size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string *data)
{
    data->append((char *)ptr, size * nmemb);
    return size * nmemb;
}

static size_t headerCallback(char* buffer, size_t size,
    size_t nitems, void* userdata)
{
    std::string *headers = (std::string*) userdata;
    headers->append(buffer, nitems * size);
    return nitems * size;
}

HttpRequest::HttpRequest(const std::string& url) : m_curlPtr(curl_easy_init())
{
    if (!m_curlPtr)
    {
        std::cout << "curl_easy_init failed !! \n";
        return;
    }
    if(!s_isGloballyInitialized)
    {
        initHttpRequestsGlobal();
    }

    //! send to url
    curl_easy_setopt(m_curlPtr, CURLOPT_URL, url.c_str());
    //! HttpRequest::writeFunction get's called with response data
    curl_easy_setopt(m_curlPtr, CURLOPT_WRITEFUNCTION, writeFunction);
    // Allow redirect
    curl_easy_setopt(m_curlPtr, CURLOPT_FOLLOWLOCATION, 1L);
    // User Https protocol
    curl_easy_setopt(m_curlPtr, CURLOPT_DEFAULT_PROTOCOL, "https");
    //! data inserted inside m_responseString
    curl_easy_setopt(m_curlPtr, CURLOPT_WRITEDATA, &m_responseString);
    //! call back for header in response
    curl_easy_setopt(m_curlPtr, CURLOPT_HEADERFUNCTION, headerCallback);
    //! store response's header in m_responseHeaderString
    curl_easy_setopt(m_curlPtr, CURLOPT_HEADERDATA, &m_responseHeaderString);

    //! curl version
    // curl_easy_setopt(m_curlPtr, CURLOPT_USERAGENT, std::string(std::string("curl/") + curl_version_info(CURLVERSION_NOW)->version).c_str());
    //! enforce the verification of peer using CA
    curl_easy_setopt(m_curlPtr, CURLOPT_SSL_VERIFYPEER, 1L);
    //! enforce verification of peer that it's actually who it claims to be
    curl_easy_setopt(m_curlPtr,  CURLOPT_SSL_VERIFYHOST, 1L);
    
    // TODO:
    // m_headers = curl_slist_append(m_headers, "Accept: application/json");
    // m_headers = curl_slist_append(m_headers, "Content-Type: application/json");
    // m_headers = curl_slist_append(m_headers, "charset: utf-8");

    std::cout << "new request client\n";
}

void HttpRequest::initHttpRequestsGlobal()
{
    if (!s_isGloballyInitialized)
    {
        std::cout << "Global init !!\n";
        curl_global_init(CURL_GLOBAL_ALL);
        s_isGloballyInitialized = true;
    }
}

void HttpRequest::cleanHttpRequestsGlobal()
{
    if(s_isGloballyInitialized)
        curl_global_cleanup();
    s_isGloballyInitialized = false;
}

void HttpRequest::addDataToHeader(const std::string& data)
{
    //! this returns a linked list
    //! assign the net ll to headers list
    m_headers = curl_slist_append(m_headers, data.c_str());
}

void HttpRequest::addDataToBody(std::shared_ptr<std::string> bodyData)
{
    requestBodyContent = bodyData;
    curl_easy_setopt(m_curlPtr, CURLOPT_POSTFIELDS, requestBodyContent.get()->c_str());
}

void HttpRequest::addJWTokenToHeader(const std::string& jwtToken)
{
    addDataToHeader("Authorization: Bearer " + jwtToken);
}

CURLcode HttpRequest::send()
{
    // applyHeaders
    curl_easy_setopt(m_curlPtr, CURLOPT_HTTPHEADER, m_headers);
 
    // apply multipart data if used
    if(m_sendMultipartData)
        curl_easy_setopt(m_curlPtr, CURLOPT_MIMEPOST, m_mime);

    //! Perform the request, res will get the return code
    CURLcode res = curl_easy_perform(m_curlPtr);
 
    //! Check for errors
    if (res != CURLE_OK)
    {
        std::cout <<  "curl_easy_perform() failed:"  <<  curl_easy_strerror(res);
    }
    return res;
}

std::string HttpRequest::getResponse()
{
    return m_responseString;
}

int64_t HttpRequest::getResponseCode()
{
    int64_t response_code;
    curl_easy_getinfo(m_curlPtr, CURLINFO_RESPONSE_CODE, &response_code);
    printf("HTTP Status Code: %ld\n", response_code);
    return response_code;
}

std::string HttpRequest::getResponseHeader()
{
    return m_responseHeaderString;
}

HttpRequest::~HttpRequest()
{  
    if(m_curlPtr)
        curl_easy_cleanup(m_curlPtr);
    if(m_headers)
        curl_slist_free_all(m_headers);
    if(m_mime)
        curl_mime_free(m_mime);
}

void HttpRequest::MultipartFormDataInit()
{
    std::cout << "oui";
    m_mime = curl_mime_init(m_curlPtr);
    m_sendMultipartData = true;
}

void HttpRequest::addTextToFormData(const std::string& name, const std::string& text)
{
    if(!m_sendMultipartData)
    {
        std::cout << "init the use of MultipartFormDataInit first";
        return;
    }
    m_mime_part = curl_mime_addpart(m_mime);
    curl_mime_name(m_mime_part, name.c_str());
    curl_mime_data(m_mime_part, text.c_str(), CURL_ZERO_TERMINATED);
}


void HttpRequest::addFileToFormData(const std::string& name, const std::string& fileDir)
{
    if(!m_sendMultipartData)
    {
        std::cout << "init the use of MultipartFormDataInit first";
        return;
    }

    m_mime_part = curl_mime_addpart(m_mime);
    std::cout << curl_mime_name(m_mime_part, name.c_str());
    std::cout << curl_mime_filedata(m_mime_part, fileDir.c_str());
}