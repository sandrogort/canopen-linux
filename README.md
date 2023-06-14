# canopen-linux

## Repository Structure

```text
+- cmake        : submodule referencing the repository 'cmake-scripts'
+- debug        : debugger configurations
+- dependencies : external managed components
+- src          : canopen example project
|  +- app       : application source code
|  +- config    : configuration and main file
|  +- driver    : canopen target specific drivers
```

## CANopen Demo for Linux

This project demonstrates the usage of the free [CANopen Stack Project](https://canopen-stack.org). The included application is the Quickstart - "*CANopen clock application*", described in detail at the [project website](https://canopen-stack.org/latest/start/quickstart/).

### Usage

#### Development Tools

Download and install these free tools for your system:

- Install the build tools [Cmake](https://cmake.org/)
- Install the build system [Ninja](https://ninja-build.org/)
- Install gcc toolchain

#### Submodule: cmake-scripts

The basic collection of re-usable CMake scripts are placed as a Git submodule in the directory `/cmake`. The Git submodule is a reference to a specific commit hash of the Git repository [cmake-scripts](https://github.com/embedded-office/cmake-scripts).

*Small reminder: when cloning this repository you need to initialize and update the submodules:*

```bash
# clone Git repository and initialize submodules:
git clone --recurse-submodules <repository>

# or, in case you have already cloned the Git repository:
git clone <repository>
cd <repository-directory>
git submodule update --init
```

#### Project dependencies

Linux package can-utils is required for socketcan library and provides useful tools like candump. In Debian based systems it can be installed as following.

```bash
sudo apt-get install can-utils
```

### Build instructions

Just type in the project root directory:

```bash
# configure the project for debugging
cmake --preset debug

# build the application for your target
cmake --build ./build/debug
```

### Running application

Virtual CAN bus must be setup prior to running to application as following. A script to do so can be found under script/start_vcan.sh.

```bash
sudo modprobe vcan
sudo ip link add dev can0 type vcan
sudo ip link set up can0
```

Open a new console window and start candump to observer CAN traffic.

```bash
candump -td can0
```

Start application and pass the CAN interface name.

```bash
./canopen-linux can0
```
