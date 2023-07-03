#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <memory>
#include <mutex>
#include "http_client/HttpRequestBuilder.hpp"
#include "nlohmann/json.hpp"
#include <fstream>

class Authenticator
{

private:
    std::string m_jwtToken;
    static std::shared_ptr<Authenticator> m_instance;
    static std::mutex m_mutex;
    std::string m_token{""};

    // Private constructor to prevent direct instantiation

private:
    // Authenticator();

    std::string exec(std::string command) 
    {
        char buffer[128];
        std::string result = "";

        // Open pipe to file
        FILE* pipe = popen(command.c_str(), "r");
        if (!pipe) 
        {
            return "popen failed!";
        }

        pclose(pipe);
        return result;

    }

    void decryptChallenge(const std::string privateKeyPath, const std::string challengePath)
    {
        exec(std::string("openssl rsautl -decrypt -keyform DER -inkey ") + privateKeyPath +  " -in " + challengePath + " -out decrypted_data.bin -pkcs -raw");
    }

    void requestChallenge()
    {
        HttpRequestBuilder httpBuilder(HttpRequestBuilder::REQUEST_TYPE::POST_REQUEST, "https://vehicleplus.cloud/authentication/tcu/challenge");
        
        std::unique_ptr<HttpRequest> challengeRequest = httpBuilder
                                                            .MultipartFormDataInit()
                                                            .addFileToFormData("chainedCertificateFile", "../ChainedCertficate.cer")
                                                            .build();

        if(challengeRequest->send() != CURLE_OK)
        {
            std::cout << "failed to resolve authentication challenge!\n";
            return;
        }

        std::cout << challengeRequest->getResponse(); 


        std::ofstream outputFile("./challenge.bin");
        if (outputFile.is_open()) 
        {

            outputFile << challengeRequest->getResponse(); 

        }
        else 
        {
            std::cout << "Unable to open the file." << std::endl;
        }

        outputFile.close();

    }


    std::string sendChallengeSolutionGetNewToken()
    {
        HttpRequestBuilder httpBuilder(HttpRequestBuilder::REQUEST_TYPE::POST_REQUEST, "https://vehicleplus.cloud/authentication/tcu/login");
        
        std::unique_ptr<HttpRequest> challengeRequest = httpBuilder
                                                            .MultipartFormDataInit()
                                                            .addFileToFormData("certificate", "../ChainedCertficate.cer")
                                                            .addFileToFormData("challenge", "./decrypted_data.bin")
                                                            .build();

        if(challengeRequest->send() != CURLE_OK)
        {
            std::cout << "failed to send challenge solution!\n";
        }

        return std::move(challengeRequest->getResponse());
    }


public:

    static std::shared_ptr<Authenticator> getInstance()
    {
        if (m_instance == nullptr)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_instance == nullptr)
            {
                m_instance = std::make_shared<Authenticator>();
            }
        }
        return m_instance;
    }

    bool getNewJWToken()
    {
        requestChallenge();
        decryptChallenge("../private_key.der", "./challenge.bin");
        std::string response = sendChallengeSolutionGetNewToken();
        std::cout << "\n\n\n\n";
        std::cout << response;
        std::cout << "\n\n\n\n";
        using json = nlohmann::json;
        json data = json::parse(response);

        // str.erase(std::remove(str.begin(), str.end(), '"'), str.end());
        m_token =  data["token"];
        std::cout << m_token;

        std::cout << "\n\n\n\n";

        std::cout << data["expiration"];
        return true;
    }

    bool storeToken()
    {
        std::ofstream outputFile("./jwt_token_storage.bin");  
        if (outputFile.is_open()) 
        {
            outputFile << m_jwtToken; 
        }
        else 
        {
            std::cout << "Unable to open the file." << std::endl;
            return false;
        }

        return true;
    }

    bool readToken()
    {
        std::ifstream inputFile("./jwt_token_storage.bin");  // Open the file in input mode
        if (inputFile.is_open()) 
        {
            std::getline(inputFile, m_jwtToken);
        }
        else 
        {
            std::cout << "Unable to open the file." << std::endl;
            return false;
        }
        return true;
    }

    bool checkTokenValidity()
    {
        HttpRequestBuilder HttpBuilder(HttpRequestBuilder::REQUEST_TYPE::POST_REQUEST, "https://abbas.requestcatcher.com/test");
        std::unique_ptr<HttpRequest> httpRequest = HttpBuilder
                                    .addJWTokenToHeader(m_jwtToken)
                                    .build();

        if (httpRequest->send() == CURLE_OK)
        {
            if(httpRequest->getResponse() == "Invalid Token")
            {

            }
        }

        return true;
    }

    const std::string getToken()
    {
        return m_token;
    }
};


std::shared_ptr<Authenticator> Authenticator::m_instance = nullptr;
std::mutex Authenticator::m_mutex;