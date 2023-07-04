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
    struct curl_slist* m_headers{NULL};
    //! @brief response inserted inside the responseString
    std::string m_responseString{};
    //! @brief response header inside the responseString
    std::string m_responseHeaderString{};
    std::shared_ptr<std::string> requestBodyContent{nullptr};
    curl_mime *m_mime {nullptr};
    curl_mimepart *m_mime_part{nullptr};
    bool m_sendMultipartData {false};
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
    void addDataToBody(std::shared_ptr<std::string> data);

    //! @brief Add JWT token to Request header
    //! @param jwtToken token to be added to request header
    void addJWTokenToHeader(const std::string& jwtToken);

    //! @brief get response string
    //! @return return response string
    std::string getResponse();

    //! @brief sends response code
    //! @return return response code int64
    int64_t getResponseCode();

    //! @brief sends GetRequestClient
    //! @return return response header string
    std::string getResponseHeader();

    //! @brief sends GetRequestClient
    //! @return true if succeed
    //! @return false if fails
    CURLcode send();

    //! @brief  used to send Multipart form-data
    void MultipartFormDataInit();
    
    //! @brief add data to multiform data NOTE: you have to call MultipartFormDataInit first
    void addTextToFormData(const std::string& name, const std::string& text);

    //! @brief add file to multiform data NOTE: you have to call MultipartFormDataInit first
    void addFileToFormData(const std::string& name, const std::string& fileDir);

    //! @brief every request inherit from it to initialize its request params
    virtual bool init() = 0;


    //! @brief destructor
    virtual ~HttpRequest();
};

#endif