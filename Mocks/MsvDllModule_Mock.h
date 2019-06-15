

#ifndef MARSTECH_DLLMODULE_MOCK_H
#define MARSTECH_DLLMODULE_MOCK_H


#include "../IMsvDllModule.h"

MSV_DISABLE_ALL_WARNINGS

#include <gmock\gmock.h>

MSV_ENABLE_WARNINGS


class MsvDllModule_Mock:
	public IMsvDllModule
{
public:
	MOCK_METHOD0(Initialize, MsvErrorCode());
	MOCK_METHOD0(Uninitialize, MsvErrorCode());
	MOCK_CONST_METHOD0(Initialized, bool());

	MOCK_METHOD0(Start, MsvErrorCode());
	MOCK_METHOD0(Stop, MsvErrorCode());
	MOCK_CONST_METHOD0(Running, bool());

	MOCK_METHOD1(SetDllFactory, void(std::shared_ptr<IMsvDllFactory> spDllFactory));
};


#endif // MARSTECH_DLLMODULE_MOCK_H
