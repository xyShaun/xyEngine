编译命令：
clang++ -lxcb -o LinuxMain_XCB LinuxMain_XCB.cpp
g++ -o LinuxMain_XCB LinuxMain_XCB.cpp -lxcb

clang++ -lxcb -lX11 -lX11-xcb -lGL -o LinuxMain_OGL LinuxMain_OGL.cpp

安装依赖库：
sudo apt install libxcb1-dev
sudo apt install libx11-dev
sudo apt install libx11-xcb-dev
sudo apt install libgl-dev
