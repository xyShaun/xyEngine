#include "BaseApplication.h"

namespace xy
{
	int BaseApplication::Initialize()
	{
		m_bExit = false;

		return 0;
	}

	void BaseApplication::Finalize()
	{

	}

	void BaseApplication::Tick()
	{

	}

	bool BaseApplication::IsExit()
	{
		return m_bExit;
	}
}