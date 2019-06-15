

#ifndef MARSTECH_MODULECONFIGURATOR_MOCK_H
#define MARSTECH_MODULECONFIGURATOR_MOCK_H


#include "../IMsvModuleConfigurator.h"

MSV_DISABLE_ALL_WARNINGS

#include <gmock\gmock.h>

MSV_ENABLE_WARNINGS


class MsvModuleConfigurator_Mock:
	public IMsvModuleConfigurator
{
public:
	MOCK_METHOD1(Enabled, MsvErrorCode(bool enabled));
	MOCK_CONST_METHOD1(IsEnabled, MsvErrorCode(bool& enabled));

	MOCK_METHOD1(Installed, MsvErrorCode(bool installed));
	MOCK_CONST_METHOD1(IsInstalled, MsvErrorCode(bool& installed));
};


#endif // MARSTECH_MODULECONFIGURATOR_MOCK_H
