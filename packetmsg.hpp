#ifndef PACKETMSG_HPP
#define PACKETMSG_HPP

#include "base64.hpp"

#include <string>
#include <vector>

class imu_packetmsg
{
public:
  uint64_t imu_diag_time_ns;
  uint64_t acc_read_time_ns;
  uint64_t gyro_read_time_ns;

  float acc_x;
  float acc_y;
  float acc_z;

  float ang_vel_x;
  float ang_vel_y;
  float ang_vel_z;

  imu_packetmsg() { }

  imu_packetmsg(const std::string& message)
  {
    std::string decoded = base64_decode(message.substr(
          message.find("\"buf\": \"") + 8, 68));

    imu_diag_time_ns = (*(reinterpret_cast<const uint64_t*> (&decoded[0])));
    acc_read_time_ns = (*(reinterpret_cast<const uint64_t*> (&decoded[8])));
    gyro_read_time_ns = (*(reinterpret_cast<const uint64_t*>
          (&decoded[16])));

    acc_x = (*(reinterpret_cast<const float*> (&decoded[24])));
    acc_y = (*(reinterpret_cast<const float*> (&decoded[28])));
    acc_z = (*(reinterpret_cast<const float*> (&decoded[32])));

    ang_vel_x = (*(reinterpret_cast<const float*> (&decoded[36])));
    ang_vel_y = (*(reinterpret_cast<const float*> (&decoded[40])));
    ang_vel_z = (*(reinterpret_cast<const float*> (&decoded[44])));
  }


};

class lidar_packetmsg
{
public:
  std::vector<uint64_t> timestamp;
  std::vector<uint16_t> measurement_id;
  std::vector<uint16_t> frame_id;
  std::vector<uint32_t> encoder_count;

  std::vector<std::vector<uint32_t>> range_mm;
  std::vector<std::vector<uint16_t>> signal_photons;
  std::vector<std::vector<uint16_t>> reflectivity;
  std::vector<std::vector<uint16_t>> noise_photons;

  std::vector<bool> azimuth_data_block_status;

  lidar_packetmsg() { }

  lidar_packetmsg(const std::string& message)
  {
    std::string decoded = base64_decode(message.substr(
          message.find("\"buf\": \"") + 8, 16812));

    timestamp.clear();
    measurement_id.clear();
    frame_id.clear();
    encoder_count.clear();
    range_mm.clear();
    signal_photons.clear();
    reflectivity.clear();
    noise_photons.clear();
    azimuth_data_block_status.clear();

    for (size_t azimuth_block = 0; azimuth_block < 16; ++azimuth_block)
    {
      timestamp.push_back(*(reinterpret_cast<const uint64_t*>
            (&decoded[(azimuth_block * 788) + 0])));
      measurement_id.push_back(*(reinterpret_cast<const uint16_t*>
            (&decoded[(azimuth_block * 788) + 8])));
      frame_id.push_back(*(reinterpret_cast<const uint16_t*>
            (&decoded[(azimuth_block * 788) + 10])));
      encoder_count.push_back(*(reinterpret_cast<const uint32_t*>
            (&decoded[(azimuth_block * 788) + 12])));

      std::vector<uint32_t> block_range_mm;
      std::vector<uint16_t> block_signal_photons;
      std::vector<uint16_t> block_reflectivity;
      std::vector<uint16_t> block_noise_photons;

      for (size_t pixel = 0; pixel < 64; ++pixel)
      {
        block_range_mm.push_back(*(reinterpret_cast<const uint32_t*>
              (&decoded[(azimuth_block * 788) + 16 + (pixel * 12)])));
        block_signal_photons.push_back(*(reinterpret_cast<const uint16_t*>
              (&decoded[(azimuth_block * 788) + 16 + (pixel * 12) + 4])));
        block_reflectivity.push_back(*(reinterpret_cast<const uint16_t*>
              (&decoded[(azimuth_block * 788) + 16 + (pixel * 12) + 6])));
        block_noise_photons.push_back(*(reinterpret_cast<const uint16_t*>
              (&decoded[(azimuth_block * 788) + 16 + (pixel * 12) + 10])));
      }

      range_mm.push_back(block_range_mm);
      signal_photons.push_back(block_signal_photons);
      reflectivity.push_back(block_reflectivity);
      noise_photons.push_back(block_noise_photons);

      azimuth_data_block_status.push_back(
          !!decoded[(azimuth_block * 788) + 784]);
    }
  }

};

#endif // PACKETMSG_HPP
