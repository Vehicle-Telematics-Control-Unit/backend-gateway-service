#ifndef POST_REQUEST_CLIENT_HPP
#define POST_REQUEST_CLIENT_HPP

#include "HttpRequest.hpp"

class PostRequestClient : public HttpRequest
{

public:
    //! @brief default constructor is deleted
    PostRequestClient() = delete;
    
    //! @brief constructor
    //! @param url the url to which the post request is to be sent
    PostRequestClient(const std::string& url);

    //! @brief initialize the request params
    //! @return return true if nothing went wrong 
    //! @return return false if something went wrong 
    bool init() override;
};

#endif