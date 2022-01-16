#pragma  once

#include "IApplication.h"

namespace xy
{
	class BaseApplication : public IApplication
	{
	public:
		virtual int Initialize();
		virtual void Finalize();

		virtual void Tick();

		virtual bool IsExit();

	protected:
		bool m_bExit;
	};
}