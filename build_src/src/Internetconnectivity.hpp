#ifndef INTERNET_CONNECTIVITY_HPP_
#define INTERNET_CONNECTIVITY_HPP_

#include <cstdio>

class InternetConnectivity
{
public:
    static bool isConnectedToInternet()
    {
        // ping Google DNS once
        int returnCode = system("ping -c 1 -w 500 8.8.8.8 > /dev/null 2>&1");

        // Check the return code to determine if the command was successful
        if (returnCode == 0)
            return true;
        else 
            return false;
    }
};

#endif