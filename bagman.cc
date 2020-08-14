#include "packetmsg.hpp"
#include "navsatfixmsg.hpp"

#include <easyws/easywsclient.hpp>

#include <iostream>
#include <iomanip>
#include <string>

int main(int argc, char* argv[])
{
  easywsclient::websocket::pointer
    imu_socket(easywsclient::websocket::from_url("ws://localhost:9090/"));
  easywsclient::websocket::pointer
    lidar_socket(easywsclient::websocket::from_url("ws://localhost:9090/"));
  easywsclient::websocket::pointer
    gnss_socket(easywsclient::websocket::from_url("ws://localhost:9090/"));

  if (!imu_socket || !lidar_socket || !gnss_socket)
  {
    std::cerr << "Failed to open websocket connection" << std::endl;
    exit(1);
  }

  imu_socket->send("{\"op\":\"subscribe\",\"topic\": \
      \"/os1_node/imu_packets\",\"type\":\"ouster_ros/PacketMsg\"}");
  lidar_socket->send("{\"op\":\"subscribe\",\"topic\": \
      \"/os1_node/lidar_packets\",\"type\":\"ouster_ros/PacketMsg\"}");
  gnss_socket->send("{\"op\":\"subscribe\",\"topic\": \
      \"/mavros/global_position/global\",\"type\":\"sensor_msgs/NavSatFix\"}");

  size_t i, j, lidar_count = 0, imu_count = 0;
  imu_packetmsg imu_data;
  lidar_packetmsg lidar_data;
  navsatfixmsg gnss_data;

  while (imu_socket->getReadyState() != easywsclient::websocket::CLOSED
      && lidar_socket->getReadyState() != easywsclient::websocket::CLOSED
      && gnss_socket->getReadyState() != easywsclient::websocket::CLOSED)
  {
    bool imu_updated = false;
    bool lidar_updated = false;
    bool gnss_updated = false;

    imu_socket->poll();
    imu_socket->dispatch([imu_socket, &imu_data, &imu_updated]
        (const std::string& message)
        {
          imu_updated = true;
          imu_data = imu_packetmsg(message);
        });

    lidar_socket->poll();
    lidar_socket->dispatch([lidar_socket, &lidar_data, &lidar_updated]
        (const std::string& message)
        {
          lidar_updated = true;
          lidar_data = lidar_packetmsg(message);
        });

    gnss_socket->poll();
    gnss_socket->dispatch([gnss_socket, &gnss_data, &gnss_updated]
        (const std::string& message)
        {
          gnss_updated = true;
          gnss_data = navsatfixmsg(message);
        });

    if (imu_updated)
      std::cout << "IMU packet " << (imu_count++) << ":"
        << "\n  diag time: " << imu_data.imu_diag_time_ns
        << "\n  acc read time: " << imu_data.acc_read_time_ns
        << "\n  gyro read time: " << imu_data.gyro_read_time_ns
        << "\n  acc x: " << imu_data.acc_x
        << "\n  acc y: " << imu_data.acc_y
        << "\n  acc z: " << imu_data.acc_z
        << "\n  ang_vel x: " << imu_data.ang_vel_x
        << "\n  ang_vel y: " << imu_data.ang_vel_y
        << "\n  ang_vel z: " << imu_data.ang_vel_z
        << std::endl;

    if (lidar_updated)
    {
      for (i = 0; i < 16; ++i)
      {
        std::cout << " LiDAR packet " << lidar_count
          << " (azimuth block " << i << "):"
          << "\n  timestamp: " << lidar_data.timestamp[i]
          << "\n  measurement ID: " << lidar_data.measurement_id[i]
          << "\n  frame ID: " << lidar_data.frame_id[i]
          << "\n  encoder count: " << lidar_data.encoder_count[i]
          << "\n  azimuth status: "
          << (lidar_data.azimuth_data_block_status[i] ? "Valid" : "Invalid")
          << std::endl;

        for (j = 0; j < 64; ++j)
        {
          std::cout << "    data block: " << j
            << ", rng: " << lidar_data.range_mm[i][j]
            << ", S/N: " << lidar_data.signal_photons[i][j]
            << " / " << lidar_data.noise_photons[i][j]
            << std::endl;
        }
      }
      ++lidar_count;
    }

    if (gnss_updated)
      std::cout << "status: " << static_cast<int>(gnss_data.status)
        << "\nservice: " << static_cast<int>(gnss_data.service)
        << "\nlatitude: " << gnss_data.latitude
        << "\nlongitude: " << gnss_data.longitude
        << "\naltitude: " << gnss_data.altitude
        << "\nENU covariance:\n"
        << "\t"   << std::setw(8) << gnss_data.covariance[0]
        << ", \t" << std::setw(8) << gnss_data.covariance[1]
        << ", \t" << std::setw(8) << gnss_data.covariance[2]
        << "\n\t" << std::setw(8) << gnss_data.covariance[3]
        << ", \t" << std::setw(8) << gnss_data.covariance[4]
        << ", \t" << std::setw(8) << gnss_data.covariance[5]
        << "\n\t" << std::setw(8) << gnss_data.covariance[6]
        << ", \t" << std::setw(8) << gnss_data.covariance[7]
        << ", \t" << std::setw(8) << gnss_data.covariance[8]
        << "\nPosition Covariance Type: "
        << static_cast<int>(gnss_data.position_covariance_type)
        << std::endl;
  }
}
