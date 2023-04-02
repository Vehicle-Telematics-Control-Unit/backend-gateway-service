#ifndef HTTP_REQUEST_BUILDER_HPP
#define HTTP_REQUEST_BUILDER_HPP

#include <string>
#include "HttpRequest.hpp"
#include <memory>

class HttpRequestBuilder{
private:
    std::unique_ptr<HttpRequest> m_httpRequest{nullptr};
public:
    enum REQUEST_TYPE{
        POST_REQUEST,
        GET_REQUEST,
        PUT_REQUEST
    };

    HttpRequestBuilder(REQUEST_TYPE requestType, const std::string& url);
    //! @brief  insert data to header
    //! @param  key string containing header key
    //! @param  value string header key value
    HttpRequestBuilder& addDataToHeader(const std::string& key, const std::string& value);
    //! @brief  insert jwt token to header
    //! @param  jwtToken string containing body data
    HttpRequestBuilder& addJWTtokenToHeader(const std::string& jwtToken);
    //! @brief  insert body data take care data is copied better use std::move(data)
    //! @param  data string containing body data
    HttpRequestBuilder& addDataToBody(const std::string& data);
    HttpRequestBuilder& addDataToBody(const uint8_t* data, uint32_t size);
    //! @brief  insert body data take care data is copied better use std::move(data)
    //! @param  data string containing body data
    HttpRequestBuilder& addDataToBody(std::string&& data);
    //! @brief  build the HttpRequest
    //! @return unique pointer to the build http request
    std::unique_ptr<HttpRequest> build();

};

#endif