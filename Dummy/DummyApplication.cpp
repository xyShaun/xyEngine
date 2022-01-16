#include "DummyApplication.h"
#include <iostream>

using namespace std;

namespace xy
{
	DummyApplication g_App;
	IApplication* g_pApp = &g_App;

	int DummyApplication::Initialize()
	{
		cout << "DummyApp Initialize" << endl;

		return BaseApplication::Initialize();
	}

	void DummyApplication::Finalize()
	{
		cout << "DummyApp Finalize" << endl;

		BaseApplication::Finalize();
	}
}