#ifndef GET_REQUEST_CLIENT_HPP
#define GET_REQUEST_CLIENT_HPP

#include "HttpRequest.hpp"

class GetRequestClient : public HttpRequest
{

public:
    //! @brief default constructor is deleted
    GetRequestClient() = delete;
    
    //! @brief constructor
    //! @param url the url to which the get request is to be sent
    GetRequestClient(std::string url);

    //! @brief initialize the request params
    //! @return return true if nothing went wrong 
    //! @return return false if something went wrong 
    bool init() override;
};

#endif