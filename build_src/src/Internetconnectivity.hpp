#ifndef INTERNET_CONNECTIVITY_HPP_
#define INTERNET_CONNECTIVITY_HPP_

#include <cstdio>

class InternetConnectivity
{
public:
    static bool isConnectedToInternet()
    {
        FILE *output;

        if (!(output = popen("/sbin/route -n | grep -c '^0\\.0\\.0\\.0'", "r")))
        {
            return 1;
        }
        unsigned int i;
        fscanf(output, "%u", &i);
        pclose(output);
        if (i == 1)
            return true;
        return false;
    }
};

#endif