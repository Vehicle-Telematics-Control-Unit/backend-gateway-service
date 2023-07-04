#include <iostream>
#include <thread>
#include <memory>
#include "http_client/HttpRequestBuilder.hpp"
#include "someipService/ServiceManagerAdapter.hpp"
#include "servicesId.hpp"
#include "nlohmann/json.hpp"
#include "Authenticator.hpp"
#include "Internetconnectivity.hpp"

#define TESTING_MODE 0


void test()
{
    using json = nlohmann::json;
    json exampleJson = {
        {"code", 1234},
        {"description", "bad lol"},
        {"state", "faulty"},
        {"route", "alerts/receive/"}
    };
    if(exampleJson.count("route") == 0)
    {
        std::cout << "didn't have a specific route!";
        return;
    }
    #if TESTING_MODE
    HttpRequestBuilder HttpBuilder(HttpRequestBuilder::REQUEST_TYPE::POST_REQUEST, std::string("https://abbas.requestcatcher.com/test"));
    #else
    HttpRequestBuilder HttpBuilder(HttpRequestBuilder::REQUEST_TYPE::POST_REQUEST, std::string("https://vehicleplus.cloud/") + std::string(exampleJson["route"]));
    exampleJson.erase("route");
    #endif
    std::unique_ptr<HttpRequest> httpRequest = HttpBuilder
                                .addDataToHeader("Content-Type", "application/json")
                                .addDataToBody(exampleJson.dump())
                                .addJWTokenToHeader(Authenticator::getInstance()->getToken())
                                .build();

    if (httpRequest->send() == CURLE_OK)
    {
        std::cout << "[" << httpRequest->getResponse() << "]\n";
        std::cout << "[" << httpRequest->getResponseHeader() << "]\n";
    }

}

void sendPostRequestToServer(const std::shared_ptr<vsomeip::message> _request, const std::shared_ptr<vsomeip::application> _app)
{
    std::thread{[](const std::shared_ptr<vsomeip::message> request, const std::shared_ptr<vsomeip::application> app)
    {
        std::cout << "NEW REQUEST ON THE WAY!\n";
        if(!InternetConnectivity::isConnectedToInternet())
        {
            std::cout << "couldn't send request not connected to internet !!";
            std::shared_ptr<vsomeip::message> its_response = vsomeip::runtime::get()->create_response(request);
            std::shared_ptr<vsomeip::payload> response_payload = vsomeip::runtime::get()->create_payload();

            std::string vsomeipResponse = R"({
                "response_code": 502,
                "response": ""
            })";
            std::vector<vsomeip::byte_t> response_payload_data = std::vector<uint8_t>(vsomeipResponse.begin(), vsomeipResponse.end());
            response_payload->set_data(response_payload_data);
            its_response->set_payload(response_payload);
            app->send(its_response);
            
            return;
        }
        std::shared_ptr<vsomeip::payload> its_payload = request->get_payload();
        vsomeip::length_t l = its_payload->get_length();

        using json = nlohmann::json;
        json jsonRequest = json::parse(std::string((char*)its_payload->get_data(), its_payload->get_length()));

        #if TESTING_MODE
        HttpRequestBuilder HttpBuilder(HttpRequestBuilder::REQUEST_TYPE::POST_REQUEST, std::string("https://abbas.requestcatcher.com/test"));
        #else
        if(jsonRequest.count("route") == 0)
        {
            std::cout << "didn't have a specific route!";
            return;
        }
        HttpRequestBuilder HttpBuilder(HttpRequestBuilder::REQUEST_TYPE::POST_REQUEST, std::string("https://vehicleplus.cloud/") + std::string(jsonRequest["route"]));
        jsonRequest.erase("route");
        #endif

        std::unique_ptr<HttpRequest> httpRequest = HttpBuilder
                                    .addDataToHeader("Content-Type", "application/json")
                                    .addDataToBody(jsonRequest.dump())
                                    .addJWTokenToHeader(Authenticator::getInstance()->getToken())
                                    .build();

        json vsomeipResponseJson;
        if (httpRequest->send() == CURLE_OK)
        {
            std::cout << "- response [" << httpRequest->getResponse() << "]\n";
            std::cout << "- response code [" << httpRequest->getResponseCode() << "]\n";
            std::cout << "- response header [" << httpRequest->getResponseHeader() << "]\n";
            // std::string responseMsg = httpRequest->getResponse();
            vsomeipResponseJson["response_code"] = httpRequest->getResponseCode();
            vsomeipResponseJson["response"] = httpRequest->getResponse();
        }
        std::string vsomeipResponse = vsomeipResponseJson.dump();
        std::cout << "\n\n\n vsomeipResponseJson: " << vsomeipResponseJson << '\n';

        std::shared_ptr<vsomeip::message> its_response = vsomeip::runtime::get()->create_response(request);
        std::shared_ptr<vsomeip::payload> response_payload = vsomeip::runtime::get()->create_payload();

        std::vector<vsomeip::byte_t> response_payload_data = std::vector<uint8_t>(vsomeipResponse.begin(), vsomeipResponse.end());
        response_payload->set_data(response_payload_data);
        its_response->set_payload(response_payload);

        app->send(its_response);
    }, _request, _app}.detach();  
}

void RequestJWTokenWaitTillAcquired()
{
    while(!InternetConnectivity::isConnectedToInternet()) { std::this_thread::sleep_for(std::chrono::seconds(1)); }

    while(Authenticator::getInstance()->getToken() == "")
    {
        if(!Authenticator::getInstance()->getNewJWToken())
        {
            while(!InternetConnectivity::isConnectedToInternet()) { std::this_thread::sleep_for(std::chrono::seconds(1)); }
        }
        else
        {
            return;
        }
    }
}


int main()
{
    std::thread ie_thread([]{
        while(1)
        {
            std::cout << "internet connection: " << (InternetConnectivity::isConnectedToInternet() ? "true" : "false") << '\n' << std::flush;
            std::this_thread::sleep_for(std::chrono::seconds(3));
        }
    });


    using service_id_call = ServiceManagerAdapter::serviceIdAndCallBack;

    std::shared_ptr<vsomeip::application> app{vsomeip::runtime::get()->create_application("backend_gateway")};

    if (!app->init())
    {
        std::cerr << "Couldn't initialize application" << std::endl;
        return -1;
    }

    std::vector<service_id_call> offeredServices{
        {BACKEND_SERVER_SERVICE_ID, BACKEND_SERVER_INSTANCE_ID, BACKEND_SERVER_METHOD_ID, std::bind(sendPostRequestToServer, std::placeholders::_1, app)}};

    ServiceManagerAdapter vsomeService{app, offeredServices};

    if (!vsomeService.init())
    {
        std::cerr << "Couldn't initialize vsomeip services" << std::endl;
        return -1;
    }
    
    RequestJWTokenWaitTillAcquired();

    vsomeService.start();
}