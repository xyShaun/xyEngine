编译命令：
clang++ -luser32.lib -lgdi32.lib -o WinMain_GDI.exe WinMain_GDI.cpp
cl user32.lib gdi32.lib WinMain_GDI.cpp (左下角搜索Command Prompt for VS 2019)

clang++ -l user32.lib -l ole32.lib -l d2d1.lib -o WinMain_D2D.exe WinMain_D2D.cpp

调试方法：
1.clang-cl -c -Z7 -o WinMain_D2D.obj WinMain_D2D.cpp
2.使用VS的链接器，生成PDB文件：link -debug user32.lib ole32.lib d2d1.lib WinMain_D2D.obj
3.启动VS的调试窗口：devenv /debug WinMain_D2D.exe
4.把源文件拖入调试窗口中，打断点调试