

#ifndef MARSTECH_MODULE_MOCK_H
#define MARSTECH_MODULE_MOCK_H


#include "../IMsvModule.h"

MSV_DISABLE_ALL_WARNINGS

#include <gmock\gmock.h>

MSV_ENABLE_WARNINGS


class MsvModule_Mock:
	public IMsvModule
{
public:
	MOCK_METHOD0(Initialize, MsvErrorCode());
	MOCK_METHOD0(Uninitialize, MsvErrorCode());
	MOCK_CONST_METHOD0(Initialized, bool());

	MOCK_METHOD0(Start, MsvErrorCode());
	MOCK_METHOD0(Stop, MsvErrorCode());
	MOCK_CONST_METHOD0(Running, bool());
};


#endif // MARSTECH_MODULE_MOCK_H
