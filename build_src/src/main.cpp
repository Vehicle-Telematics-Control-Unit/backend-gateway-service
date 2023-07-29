#include <iostream>
#include <thread>
#include <atomic>
#include <memory>
#include <boost/asio.hpp>
#include "http_client/HttpRequestBuilder.hpp"
#include "someipService/ServiceManagerAdapter.hpp"
#include "servicesId.hpp"
#include "nlohmann/json.hpp"
#include "Authenticator.hpp"
#include "Internetconnectivity.hpp"

#define TESTING_MODE 0

#define OTA_TIME_OUT 20000





#define OTA_SERVER_IP "127.0.0.1"
#define OTA_SERVER_PORT 12349


void skipUpdate()
{
    std::cout << ">>>>>>>>> update will be skipped\n";
    std::cout << ">>>>>>>>> sending skip to OTA client\n";
    using boost::asio::ip::udp;
    using boost::asio::ip::address;
    std::string message = "no";
    boost::asio::io_service io_service;
    udp::socket socket(io_service);
    udp::endpoint remote_endpoint = udp::endpoint(address::from_string(OTA_SERVER_IP), OTA_SERVER_PORT);
    socket.open(udp::v4());

    boost::system::error_code err;
    auto sent = socket.send_to(boost::asio::buffer(message), remote_endpoint, 0, err);
    socket.close();
}

void startUpdate()
{
    std::cout << ">>>>>>>>> starting update \n";
    std::cout << ">>>>>>>>> sending start to OTA client \n";
    using boost::asio::ip::udp;
    using boost::asio::ip::address;
    std::string message = "yes";
    boost::asio::io_service io_service;
    udp::socket socket(io_service);
    udp::endpoint remote_endpoint = udp::endpoint(address::from_string(OTA_SERVER_IP), OTA_SERVER_PORT);
    socket.open(udp::v4());

    boost::system::error_code err;
    auto sent = socket.send_to(boost::asio::buffer(message), remote_endpoint, 0, err);
    socket.close();

}

void startOTAProcedure()
{

    std::cout << "start OTA\n";
    HttpRequestBuilder HttpBuilderRequestOTA(HttpRequestBuilder::REQUEST_TYPE::POST_REQUEST, std::string("https://vehicleplus.cloud/OTA/TCU/features/checkout"));

    std::unique_ptr<HttpRequest> httpRequestRequestOTA = HttpBuilderRequestOTA
                                                .addDataToHeader("Content-Type", "application/json")
                                                .addDataToBody(std::string("{}"))
                                                .addJWTokenToHeader(Authenticator::getInstance()->getToken())
                                                .build();


    if (httpRequestRequestOTA->send() != CURLE_OK)
    {
        std::cout << "failed to send request !\n";
        return;
    }
    std::string response = httpRequestRequestOTA->getResponse();
    std::cout << response << "__\n";
    std::cout << httpRequestRequestOTA->getResponseHeader();
    std::cout << response << "__\n";

    using json = nlohmann::json;
    json jsonResponse = json::parse(response);

    if(!jsonResponse.count("update"))
    {
        std::cout << "no update attribute\n";
        return;
    }
    if(jsonResponse["update"] == false)
    {
        skipUpdate();
        return;
    }

    HttpRequestBuilder HttpBuilderOTA(HttpRequestBuilder::REQUEST_TYPE::POST_REQUEST, std::string("https://vehicleplus.cloud/OTA/TCU/features/update"));

    std::unique_ptr<HttpRequest> httpRequestOTA = HttpBuilderOTA
                                                   .MultipartFormDataInit()
                                                   .addJWTokenToHeader(Authenticator::getInstance()->getToken())
                                                   .build();

    if (httpRequestOTA->send() != CURLE_OK)
    {
        std::cout << "failed to send request !\n";
        return;
    }

    std::cout << httpRequestOTA->getResponse();

    std::ofstream outputFile("/composefiles/docker-compose.yaml");
    if (outputFile.is_open())
    {

        outputFile << httpRequestOTA->getResponse();
    }
    else
    {
        std::cout << "Unable to open the file." << std::endl;
    }

    outputFile.close();

    startUpdate();
}

void test()
{
    using json = nlohmann::json;
    json exampleJson = {
        {"code", 1234},
        {"description", "bad lol"},
        {"state", "faulty"},
        {"route", "alerts/receive/"}};
    if (exampleJson.count("route") == 0)
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

void downloadFileService(std::shared_ptr<vsomeip::message> _request, const std::shared_ptr<vsomeip::application> _app)
{
    std::thread{[](const std::shared_ptr<vsomeip::message> request, const std::shared_ptr<vsomeip::application> app)
                {
                    std::shared_ptr<vsomeip::payload> its_payload = request->get_payload();
                    vsomeip::length_t l = its_payload->get_length();

                    using json = nlohmann::json;
                    json jsonRequest = json::parse(std::string((char *)its_payload->get_data(), its_payload->get_length()));

#if TESTING_MODE
                    HttpRequestBuilder HttpBuilder(HttpRequestBuilder::REQUEST_TYPE::POST_REQUEST, std::string("https://abbas.requestcatcher.com/test"));
#else
                    if (jsonRequest.count("route") == 0)
                    {
                        std::cout << "didn't have a specific route!";
                        return;
                    }

                    if (jsonRequest.count("filepath") == 0)
                    {
                        std::cout << "didn't have a specific filepath!";
                        return;
                    }
                    HttpRequestBuilder HttpBuilder(HttpRequestBuilder::REQUEST_TYPE::POST_REQUEST, std::string("https://vehicleplus.cloud/") + std::string(jsonRequest["route"]));
                    jsonRequest.erase("route");
#endif

                    std::unique_ptr<HttpRequest> httpRequest = HttpBuilder
                                                                   .MultipartFormDataInit()
                                                                   .addJWTokenToHeader(Authenticator::getInstance()->getToken())
                                                                   .build();

                    if (httpRequest->send() != CURLE_OK)
                    {
                        std::cout << "failed to send request !\n";
                        return;
                    }

                    std::cout << httpRequest->getResponse();

                    std::ofstream outputFile(jsonRequest["filepath"]);
                    if (outputFile.is_open())
                    {

                        outputFile << httpRequest->getResponse();
                    }
                    else
                    {
                        std::cout << "Unable to open the file." << std::endl;
                    }

                    outputFile.close();
                },
                _request, _app}
        .detach();
}

void sendPostRequestToServer(std::shared_ptr<vsomeip::message> _request, const std::shared_ptr<vsomeip::application> _app)
{
    std::thread{[](const std::shared_ptr<vsomeip::message> request, const std::shared_ptr<vsomeip::application> app)
                {
                    std::cout << "NEW REQUEST ON THE WAY!\n";
                    if (!InternetConnectivity::internetConnectionState.load())
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
                    json jsonRequest = json::parse(std::string((char *)its_payload->get_data(), its_payload->get_length()));

#if TESTING_MODE
                    HttpRequestBuilder HttpBuilder(HttpRequestBuilder::REQUEST_TYPE::POST_REQUEST, std::string("https://abbas.requestcatcher.com/test"));
#else
                    if (jsonRequest.count("route") == 0)
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
                },
                _request, _app}
        .detach();
}

void RequestJWTokenWaitTillAcquired()
{
    while (!InternetConnectivity::internetConnectionState)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    while (Authenticator::getInstance()->getToken() == "")
    {
        if (!Authenticator::getInstance()->getNewJWToken())
        {
            while (!InternetConnectivity::checkInternetConnection())
            {
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }
        else
        {
            return;
        }
    }
}

int main()
{
    std::thread ie_thread([]
                          {
        while(1)
        {
            std::cout << "internet connection: " << (InternetConnectivity::checkInternetConnection() ? "true" : "false") << '\n' << std::flush;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        } });

    using service_id_call = ServiceManagerAdapter::serviceIdAndCallBack;

    std::shared_ptr<vsomeip::application> app{vsomeip::runtime::get()->create_application("backend_gateway")};

    if (!app->init())
    {
        std::cerr << "Couldn't initialize application" << std::endl;
        return -1;
    }

    std::vector<service_id_call> offeredServices{
        {BACKEND_SERVER_SERVICE_ID, BACKEND_SERVER_INSTANCE_ID, BACKEND_SERVER_METHOD_ID, std::bind(sendPostRequestToServer, std::placeholders::_1, app)},
        {BACKEND_SERVER_DOWNLOAD_FILE_SERVICE_ID, BACKEND_SERVER_DOWNLOAD_FILE_INSTANCE_ID, BACKEND_SERVER_DOWNLOAD_FILE_METHOD_ID, std::bind(downloadFileService, std::placeholders::_1, app)}};

    ServiceManagerAdapter vsomeService{app, offeredServices};

    if (!vsomeService.init())
    {
        std::cerr << "Couldn't initialize vsomeip services" << std::endl;
        return -1;
    }

    std::mutex otaMtx;
    bool OTAstarted{false};
    bool OTAtimeOut{false};

    std::thread OTAtimer ([&]{
        std::this_thread::sleep_for(std::chrono::seconds(20));
        {
            std::unique_lock ul(otaMtx);
            OTAtimeOut = true;
            if(!OTAstarted)
                skipUpdate();
        }
    });

    RequestJWTokenWaitTillAcquired();

    {
        std::unique_lock ul(otaMtx);
        if(!OTAtimeOut)
        {
            OTAstarted = true;
            startOTAProcedure();
        }
    }
    vsomeService.start();
}