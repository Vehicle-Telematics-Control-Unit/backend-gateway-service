#include <iostream>
#include <thread>
#include <memory>
#include "http_client/HttpRequestBuilder.hpp"
#include "someipService/ServiceManagerAdapter.hpp"
#include "servicesId.hpp"
#include "nlohmann/json.hpp"
#include "Authenticator.hpp"

#define TESTING_MODE 1


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
    HttpRequestBuilder HttpBuilder(HttpRequestBuilder::REQUEST_TYPE::POST_REQUEST, std::string("https://vehicleplus.cloud/") + std::string(exampleJson["route"]));
    exampleJson.erase("route");
    std::unique_ptr<HttpRequest> httpRequest = HttpBuilder
                                .addDataToHeader("Content-Type", "application/json")
                                .addDataToBody(exampleJson.dump(4))
                                .addJWTokenToHeader(Authenticator::getInstance()->getToken())
                                .build();

    if (httpRequest->send() == CURLE_OK)
    {
        std::cout << "[" << httpRequest->getResponse() << "]\n";
        std::cout << "[" << httpRequest->getResponseHeader() << "]\n";
    }

}

void sendPostRequestToServer(const std::shared_ptr<vsomeip::message> &_request, const std::shared_ptr<vsomeip::application> &app)
{
    std::shared_ptr<vsomeip::payload> its_payload = _request->get_payload();
    vsomeip::length_t l = its_payload->get_length();

    using json = nlohmann::json;
    json jsonRequest = json::parse(std::string((char*)its_payload->get_data(), its_payload->get_length()));

    
    #if TESTING_MODE
    std::cout << "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH";
    HttpRequestBuilder HttpBuilder(HttpRequestBuilder::REQUEST_TYPE::POST_REQUEST, std::string("https://abbas.requestcatcher.com/test"));
    #else
    if(jsonRequest.count("route") == 0)
    {
        std::cout << "didn't have a specific route!";
        return;
    }
    HttpRequestBuilder HttpBuilder(HttpRequestBuilder::REQUEST_TYPE::POST_REQUEST, std::string("https://vehicleplus.cloud/" + jsonRequest["route"]));
    jsonRequest.erase("route");
    #endif

    std::unique_ptr<HttpRequest> httpRequest = HttpBuilder
                                .addDataToHeader("Content-Type", "application/json")
                                .addDataToBody(jsonRequest.dump(4))
                                .addJWTokenToHeader(Authenticator::getInstance()->getToken())
                                .build();

    if (httpRequest->send() == CURLE_OK)
    {
        std::cout << "[" << httpRequest->getResponse() << "]\n";
        std::cout << "[" << httpRequest->getResponseHeader() << "]\n";
    }

    std::shared_ptr<vsomeip::message> its_response = vsomeip::runtime::get()->create_response(_request);
    std::shared_ptr<vsomeip::payload> response_payload = vsomeip::runtime::get()->create_payload();

    // std::vector<vsomeip::byte_t> response_payload_data = std::vector<uint8_t>(httpRequest->getResponse().begin(), httpRequest->getResponse().end());
    std::string responseMsg = httpRequest->getResponse();
    std::vector<vsomeip::byte_t> response_payload_data = std::vector<uint8_t>(responseMsg.begin(), responseMsg.end());
    response_payload->set_data(response_payload_data);
    its_response->set_payload(response_payload);

    app->send(its_response);
}


int main()
{
    Authenticator::getInstance()->getNewJWToken();

    // test();

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
    vsomeService.start();
}