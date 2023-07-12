#ifndef INTERNET_CONNECTIVITY_HPP_
#define INTERNET_CONNECTIVITY_HPP_

#include <cstdio>
#include <atomic>
class InternetConnectivity
{
public:
    static std::atomic<bool> internetConnectionState;
    
    static bool checkInternetConnection()
    {
        // ping Google DNS once
        InternetConnectivity::internetConnectionState = system("ping -c 1 -i 0.1 8.8.8.8 > /dev/null 2>&1") ? false : true;
        return internetConnectionState;
    }
};

std::atomic<bool> InternetConnectivity::internetConnectionState{false};
#endif