#pragma once

#include "IRuntimeModule.h"

namespace xy
{
	class IApplication : public IRuntimeModule
	{
	public:
		virtual bool IsExit() = 0;
	};
}