#include <iostream>
#include "IApplication.h"

using namespace std;
using namespace xy;

namespace xy
{
	extern IApplication* g_pApp;
}

int main(int argc, char** argv)
{
	cout << "Hello, xyEngine!" << endl;

	int ret = g_pApp->Initialize();
	if (ret != 0)
	{
		cout << "App Initialization failed, the program is about to exit" << endl;
		return ret;
	}

	while (!g_pApp->IsExit())
	{
		g_pApp->Tick();
	}

	g_pApp->Finalize();

	return ret;
}