# bagman

A small bit of code to allow Ouster OS1 data to be exported from ROS to an external C++ program using rosbridge.
It should be modifiable to fit any ROS topic you want to make available to some external (potentially remote) process, though I don't claim that this is the most efficient way of doing so.*


These instructions are for Ubuntu 18.04, or 20.04 as noted. Melodic is the release name for 18.04, and Noetic is the release for 20.04. Anywhere `melodic` is used in a command substitute `noetic` or whatever the appropriate name is which corresponds to your distribution.

This repo includes modified code from [easywsclient](https://github.com/dhbaird/easywsclient), which was published under the MIT license.
A copy of the copyright notice and license is available in the `easyws` directory.

## Install ROS

These instructions are directly from the [ROS wiki](http://wiki.ros.org/melodic/Installation/Ubuntu)

```
sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
```

```
sudo apt-key adv --keyserver 'hkp://keyserver.ubuntu.com:80' --recv-key C1CF6E31E6BADE8868B172B4F42ED6FBAB17C654
```

```
sudo apt update
```

```
sudo apt install ros-melodic-desktop-full
```
You shouldn't need the full install but I didn't test it with the bare bones package

Also install rosbridge
```
sudo apt install ros-melodic-rosbridge-suite
```

```
echo "source /opt/ros/melodic/setup.bash" >> ~/.bashrc
source ~/.bashrc
```

For Ubuntu 18.04:
```
sudo apt install python-rosdep python-rosinstall python-rosinstall-generator python-wstool build-essential
```

For Ubuntu 20.04:
```
sudo apt install python3-rosdep python3-rosinstall python3-rosinstall-generator python3-wstool build-essential
```

```
sudo rosdep init
rosdep update
```

## Building the Ouster PacketMsg

You need to build the Ouster PacketMsg and make it available to ROS.*
One way to accomplish this is building [Ouster's sample node](https://github.com/ouster-lidar/ouster_example).

```
source /opt/ros/melodic/setup.bash
git clone https://github.com/ouster-lidar/ouster_example.git
export CMAKE_PREFIX_PATH=path/to/ouster_example
```

Note that `path/to/ouster_example` is where you cloned the repo

```
mkdir -p myworkspace/src && cd myworkspace && ln -s /absolute/path/to/ouster_example ./src/ && catkin_make -DCMAKE_BUILD_TYPE=Release
```

```
echo "source path/to/myworkspace/devel/setup.bash" >> ~/.bashrc
source devel/setup.bash
```

## Building this repo

This project uses Make
```
make
```

## Running the server and demo program

There are several steps to launching the rosbridge server and demo program, which require multiple terminals.

In the first terminal launch `roscore`
```
roscore
```

In a second terminal, begin the playback of the rosbag
```
rosbag play path/to/some/rosbag.bag --pause -r 0.1
```
Including `--pause` prevents the bag from playing immediately.
Press space to unpause and begin playing.

The `-r 0.1` limits the bag playback to 1/10th of real-time. This may be necessary to prevent packets from being dropped.

In a third terminal you can check that the topics are available
```
rostopic list
```
You should see the topics from your bag
```
user@VirtualBox:~$ rostopic list
/clock
/os1_node/imu_packets
/os1_node/lidar_packets
/rosout
/rosout_agg
```

Now you can launch the rosbridge server
```
roslaunch rosbridge_server rosbridge_websocket.launch
```
The default configuration opens the websocket on port 9090, which is precofigured in the client source code.

Finally, you can launch the client app
```
./bagman.elf
```

## Troubleshooting

If something fails it's probably due to the setup files not being sourced; try re-opening the shell, or source them manually:

```
source /opt/ros/melodic/setup.bash
source path/to/myworkspace/devel/setup.bash
```

*I didn't try to understand how ROS works, I just needed to do this thing, so some steps may be unnecessary or inefficient
