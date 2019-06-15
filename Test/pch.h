//
// pch.h
// Header for standard system include files.
//

#pragma once


#include "merror/MsvErrorCodes.h"
#include "mlogging/MsvSpdLogLoggerProvider.h"

MSV_DISABLE_ALL_WARNINGS

#include "gtest/gtest.h"

MSV_ENABLE_WARNINGS


#ifndef MSV_TEST_WITH_LOGGING
#define MSV_TEST_WITH_LOGGING 0
#endif


const char* const MSV_DYNAMIC_MODULE_ID = "{92582DD7-D9A6-456A-89D5-0F7FF85E6B55}";


enum class ModuleId: int32_t
{
	MSV_TEST_STATIC_MODULE = 0,
	MSV_TEST_DYNAMIC_MODULE,
	MSV_TEST_OTHER_MODULE
};


enum class ConfigId: int32_t
{
	//static module config IDs
	MSV_STATIC_MODULE_INSTALLED = 0,
	MSV_STATIC_MODULE_ENABLED,

	//dynamic module config IDs
	MSV_DYNAMIC_MODULE_INSTALLED,
	MSV_DYNAMIC_MODULE_ENABLED,
};


class MsvModule_TestBase:
	public ::testing::Test
{
public:
	MsvModule_TestBase()
	{

	}

	virtual void InitializeLogging()
	{
#if defined(MSV_TEST_WITH_LOGGING) && MSV_TEST_WITH_LOGGING != 0
		m_spLoggerProvider.reset(new (std::nothrow) MsvLoggerProvider());
#else
		m_spLoggerProvider.reset(new (std::nothrow) MsvNullLoggerProvider());
#endif
		EXPECT_NE(m_spLoggerProvider, nullptr);
		m_spLogger = m_spLoggerProvider->GetLogger();
		EXPECT_NE(m_spLogger, nullptr);
	}

	virtual void UninitializeLogging()
	{
		m_spLogger.reset();
		m_spLoggerProvider.reset();
	}

	//logger
	std::shared_ptr<IMsvLoggerProvider> m_spLoggerProvider;
	std::shared_ptr<MsvLogger> m_spLogger;
};
