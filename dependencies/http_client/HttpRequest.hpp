#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <curl/curl.h>
#include <iostream>
#include <atomic>
#include <memory>

//! @brief  HttpRequest is an abstract class that
//!         every Http request type inherit from 
//!         and derive its functionalities
class HttpRequest
{
private:
    //! @brief  indicates if curl_global_init is initalized or not
    //!         it's an atomic operation to avoid multiple initialization
    //!         if concurrent access occurred
    static std::atomic<bool> s_isGloballyInitialized;

protected:
    //! @brief CURL pointer used to initialize request params
    CURL* m_curlPtr{nullptr};
    //! @brief Http Request headers
    struct curl_slist* m_headers{nullptr};
    //! @brief response inserted inside the responseString
    std::string m_responseString{};
    //! @brief response header inside the responseString
    std::string m_responseHeaderString{};
    std::shared_ptr<std::string> requestBodyContent{nullptr};
public:

    static void initHttpRequestsGlobal();
    static void cleanHttpRequestsGlobal();
    //! @brief default constructor is deleted
    HttpRequest() = delete;
    
    //! @brief parameterized constructor
    HttpRequest(const std::string& url);

    //! @brief Add data to Request header
    //! @param data data to be added to request header
    void addDataToHeader(const std::string& data);

    //! @brief  Add data to Request body
    //! @param  data a smart pointer carries the data to be added to request body
    //!         take care that data is not copied therefore shared pointer is needed
    void addDataToBody(const std::shared_ptr<std::string>& data);

    //! @brief Add JWT token to Request header
    //! @param jwtToken token to be added to request header
    void addJWTtokenToHeader(const std::string& jwtToken);

    //! @brief sends GetRequestClient
    //! @return return response string
    const std::string getResponse();

    //! @brief sends GetRequestClient
    //! @return return response header string
    const std::string getResponseHeader();

    //! @brief sends GetRequestClient
    //! @return true if succeed
    //! @return false if fails
    CURLcode send();

    //! @brief every request inherit from it to initialize its request params
    virtual bool init() = 0;

    //! @brief destructor
    virtual ~HttpRequest();
};

#endif