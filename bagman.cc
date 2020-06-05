#include "packetmsg.hpp"

#include <easyws/easywsclient.hpp>

#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
  easywsclient::websocket::pointer
    imu_socket(easywsclient::websocket::from_url("ws://localhost:9090/"));
  easywsclient::websocket::pointer
    lidar_socket(easywsclient::websocket::from_url("ws://localhost:9090/"));

  if (!imu_socket || !lidar_socket)
  {
    std::cerr << "Failed to open websocket connection" << std::endl;
    exit(1);
  }

  imu_socket->send("{\"op\":\"subscribe\",\"topic\": \
      \"/os1_node/imu_packets\",\"type\":\"ouster_ros/PacketMsg\"}");
  lidar_socket->send("{\"op\":\"subscribe\",\"topic\": \
      \"/os1_node/lidar_packets\",\"type\":\"ouster_ros/PacketMsg\"}");

  imu_packetmsg imu_data;
  lidar_packetmsg lidar_data;

  while (imu_socket->getReadyState() != easywsclient::websocket::CLOSED
      && lidar_socket->getReadyState() != easywsclient::websocket::CLOSED)
  {
    bool imu_updated = false;
    bool lidar_updated = false;

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


    if (imu_updated)
      std::cout << "IMU data:"
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
      std::cout << "LiDAR data (block 0):"
        << "\n  timestamp: " << lidar_data.timestamp[0]
        << "\n  measurement ID: " << lidar_data.measurement_id[0]
        << "\n  frame ID: " << lidar_data.frame_id[0]
        << "\n  encoder count: " << lidar_data.encoder_count[0]
        << "\n  azimuth status: "
        << (lidar_data.azimuth_data_block_status[0] ? "Valid" : "Invalid")
        << std::endl;
  }
}
