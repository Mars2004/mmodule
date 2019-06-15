

#ifndef MARSTECH_MODULEMANAGER_MOCK_H
#define MARSTECH_MODULEMANAGER_MOCK_H


#include "../IMsvModuleManager.h"

MSV_DISABLE_ALL_WARNINGS

#include <gmock\gmock.h>

MSV_ENABLE_WARNINGS


class MsvModuleManager_Mock:
	public IMsvModuleManager
{
public:
	MOCK_METHOD0(Initialize, MsvErrorCode());
	MOCK_METHOD0(Uninitialize, MsvErrorCode());
	MOCK_CONST_METHOD0(Initialized, bool());

	MOCK_METHOD0(Start, MsvErrorCode());
	MOCK_METHOD0(Stop, MsvErrorCode());
	MOCK_CONST_METHOD0(Running, bool());

	MOCK_METHOD3(AddModule, MsvErrorCode(int32_t moduleId, std::shared_ptr<IMsvModule> spModule, std::shared_ptr<IMsvModuleConfigurator> spModuleConfigurator));
};


#endif // MARSTECH_MODULEMANAGER_MOCK_H
