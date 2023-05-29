#include <iostream>
#include <thread>
#include <memory>
#include "http_client/HttpRequestBuilder.hpp"
#include "someipService/ServiceManagerAdapter.hpp"
#include "servicesId.hpp"
#include "nlohmann/json.hpp"
#include "Authenticator.hpp"


void test()
{
    using json = nlohmann::json;
    json exampleJson = {
        {"happy", true},
        {"pi", 3.141},
    };
    HttpRequestBuilder HttpBuilder(HttpRequestBuilder::REQUEST_TYPE::POST_REQUEST, "https://abbas.requestcatcher.com/test");
    std::unique_ptr<HttpRequest> httpRequest = HttpBuilder
                                .addJWTokenToHeader("token")
                                .addDataToHeader("Content-Type", "application/json")
                                .addDataToBody(exampleJson.dump(4))
                                .build();

    if (httpRequest->send() == CURLE_OK)
    {
        std::cout << "[" << httpRequest->getResponse() << "]\n";
        std::cout << "[" << httpRequest->getResponseHeader() << "]\n";
    }

}

void sendPostRequestToServer(const std::shared_ptr<vsomeip::message> &_request, const std::shared_ptr<vsomeip::application> &app)
{
    test();

    HttpRequestBuilder HttpBuilder(HttpRequestBuilder::REQUEST_TYPE::POST_REQUEST, "https://abbas.requestcatcher.com/test");
    std::string token = "TOKEN";
    std::unique_ptr<HttpRequest> httpRequest = HttpBuilder
                                .addDataToHeader("Content-Type", "application/json")
                                .addDataToBody(_request->get_payload()->get_data(), _request->get_payload()->get_length())
                                .addJWTokenToHeader(Authenticator::getInstance()->getToken())
                                .build();

    if (httpRequest->send() == CURLE_OK)
    {
        std::cout << "[" << httpRequest->getResponse() << "]\n";
        std::cout << "[" << httpRequest->getResponseHeader() << "]\n";
    }

    std::shared_ptr<vsomeip::message> its_response = vsomeip::runtime::get()->create_response(_request);
    std::shared_ptr<vsomeip::payload> its_payload = vsomeip::runtime::get()->create_payload();

    // std::vector<vsomeip::byte_t> its_payload_data = {'O', 'K', '\n', 0};
    // its_payload->set_data(its_payload_data);
    // its_response->set_payload(its_payload);

    // app->send(its_response);
}


int main()
{
    test();

    Authenticator::getInstance()->getNewJWToken();

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