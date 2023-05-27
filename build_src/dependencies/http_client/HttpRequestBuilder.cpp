#include <string>
#include <memory>
#include <vector>
#include "HttpRequestBuilder.hpp"
#include "GetRequestClient.hpp"
#include "PostRequestClient.hpp"
#include "PutRequestClient.hpp"

HttpRequestBuilder::HttpRequestBuilder(REQUEST_TYPE requestType, const std::string& url)
{
    switch (requestType)
    {
    case GET_REQUEST:
        m_httpRequest = std::make_unique<GetRequestClient>(url);
        break;
    case POST_REQUEST:
        m_httpRequest = std::make_unique<PostRequestClient>(url);
        break;
    case PUT_REQUEST:
        m_httpRequest = std::make_unique<PutRequestClient>(url);
        break;
    default:
        break;
    }

    m_httpRequest->init();
}

HttpRequestBuilder& HttpRequestBuilder::addDataToHeader(const std::string& key, const std::string& value)
{
    m_httpRequest->addDataToHeader(key + ": " + value);
    return *this;
}

HttpRequestBuilder& HttpRequestBuilder::addJWTokenToHeader(const std::string& jwtToken)
{
    m_httpRequest->addJWTokenToHeader(jwtToken);
    return *this;
}


HttpRequestBuilder& HttpRequestBuilder::addDataToBody(const std::string& data)
{
    m_httpRequest->addDataToBody(std::make_shared<std::string>(data));
    return *this;
}

HttpRequestBuilder& HttpRequestBuilder::addDataToBody(const uint8_t* data, uint32_t size)
{
    m_httpRequest->addDataToBody(std::make_shared<std::string>((char*)data, size));
    return *this;
}

HttpRequestBuilder& HttpRequestBuilder::addDataToBody(std::string&& data)
{
    m_httpRequest->addDataToBody(std::make_shared<std::string>(std::move(data)));
    return *this;
}


std::unique_ptr<HttpRequest> HttpRequestBuilder::build()
{
    return std::move(m_httpRequest);
}

HttpRequestBuilder& HttpRequestBuilder::MultipartFormDataInit()
{
    m_httpRequest->MultipartFormDataInit();
    return *this;
}

HttpRequestBuilder& HttpRequestBuilder::addTextToFormData(const std::string& name, const std::string& text)
{
    m_httpRequest->addTextToFormData(name, text);
    return *this;
}

HttpRequestBuilder& HttpRequestBuilder::addFileToFormData(const std::string& name, const std::string& fileDir)
{
    m_httpRequest->addFileToFormData(name, fileDir);
    return *this;
}