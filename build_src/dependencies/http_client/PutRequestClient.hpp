#ifndef PUT_REQUEST_CLIENT_HPP
#define PUT_REQUEST_CLIENT_HPP

#include "HttpRequest.hpp"

class PutRequestClient : public HttpRequest
{

public:
    //! @brief default constructor is deleted
    PutRequestClient() = delete;
    
    //! @brief constructor
    //! @param url the url to which the put request is to be sent
    PutRequestClient(const std::string& url);

    //! @brief initialize the request params
    //! @return return true if nothing went wrong 
    //! @return return false if something went wrong 
    bool init() override;
};

#endif