#ifndef POSITIONMSG_HPP
#define POSITIONMSG_HPP

#include "base64.hpp"

#include <string>
#include <vector>

class navsatfixmsg
{
public:
  int8_t status;
  uint8_t service;

  double latitude;
  double longitude;
  double altitude;

  double covariance[9];

  uint8_t position_covariance_type;

  navsatfixmsg() { }

  navsatfixmsg(const std::string& message)
  {
    size_t start, size;

    start = message.find("{\"status\": ") + 11;
    size = message.substr(start).find(",");
    status = std::stoi(message.substr(start, size).c_str());

    start = message.find("\"service\": ") + 11;
    size = message.substr(start).find(",");
    service = std::stoi(message.substr(start, size).c_str());

    start = message.find("\"latitude\": ") + 12;
    size = message.substr(start).find(",");
    latitude = std::atof(message.substr(start, size).c_str());

    start = message.find("\"longitude\": ") + 13;
    size = message.substr(start).find(",");
    longitude = std::atof(message.substr(start, size).c_str());

    start = message.find("\"altitude\": ") + 12;
    size = message.substr(start).find(",");
    altitude = std::atof(message.substr(start, size).c_str());

    start = message.find("\"position_covariance\": [") + 24;
    size = message.substr(start).find(",");
    covariance[0] = std::atof(message.substr(start, size).c_str());

    start += size + 1;
    size = message.substr(start).find(",");
    covariance[1] = std::atof(message.substr(start, size).c_str());

    start += size + 1;
    size = message.substr(start).find(",");
    covariance[2] = std::atof(message.substr(start, size).c_str());

    start += size + 1;
    size = message.substr(start).find(",");
    covariance[3] = std::atof(message.substr(start, size).c_str());

    start += size + 1;
    size = message.substr(start).find(",");
    covariance[4] = std::atof(message.substr(start, size).c_str());

    start += size + 1;
    size = message.substr(start).find(",");
    covariance[5] = std::atof(message.substr(start, size).c_str());

    start += size + 1;
    size = message.substr(start).find(",");
    covariance[6] = std::atof(message.substr(start, size).c_str());

    start += size + 1;
    size = message.substr(start).find(",");
    covariance[7] = std::atof(message.substr(start, size).c_str());

    start += size + 1;
    size = message.substr(start).find("]");
    covariance[8] = std::atof(message.substr(start, size).c_str());

    start = message.find("\"position_covariance_type\": ") + 28;
    size = 2;
    position_covariance_type = std::stoi(message.substr(start, size).c_str());
  }
};

#endif // POSITIONMSG_HPP
