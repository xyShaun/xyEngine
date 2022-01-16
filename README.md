# xyEngine

Windows:
1.在xyEngine目录下新建build文件夹，进入build目录
2.在命令行中执行cmake ..
3.打开xyEngine.sln，编译ALL_BUILD

Linux：
1.在xyEngine目录下运行docker镜像：docker run -it --rm -v $(pwd):/usr/src tim03/clang
2.mkdir build
3.cd build
4.cmake ..
5.make
6.cd Dummy
7.运行可执行文件：./Dummy