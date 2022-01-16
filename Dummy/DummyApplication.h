#pragma once

#include "BaseApplication.h"

namespace xy
{
	class DummyApplication : public BaseApplication
	{
	public:
		virtual int Initialize();
		virtual void Finalize();
	};
}