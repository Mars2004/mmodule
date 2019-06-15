

#include "pch.h"

#include "mmodule/MsvModuleConfigurator.h"

#include "mconfig/Mocks/MsvActiveConfig_Mock.h"


using namespace ::testing;


class MsvModuleConfigurator_Test:
	public ::testing::Test
{
public:
	virtual void SetUp()
	{
		m_spActiveConfigMock.reset(new (std::nothrow) MsvActiveConfig_Mock());
		EXPECT_NE(m_spActiveConfigMock, nullptr);

		m_spModuleConfigurator.reset(new (std::nothrow) MsvModuleConfigurator(m_spActiveConfigMock, static_cast<int32_t>(ConfigId::MSV_STATIC_MODULE_ENABLED), static_cast<int32_t>(ConfigId::MSV_STATIC_MODULE_INSTALLED)));
		EXPECT_NE(m_spModuleConfigurator, nullptr);
	}

	virtual void TearDown()
	{
		m_spActiveConfigMock.reset();

		m_spModuleConfigurator.reset();
	}

	//mocks
	std::shared_ptr<MsvActiveConfig_Mock> m_spActiveConfigMock;

	//tested classes
	std::shared_ptr<IMsvModuleConfigurator> m_spModuleConfigurator;
};


/*-----------------------------------------------------------------------------------------------------
**											Enabled Tests
**---------------------------------------------------------------------------------------------------*/


TEST_F(MsvModuleConfigurator_Test, ItShouldSetTrue_WhenEnabled)
{
	EXPECT_CALL(*m_spActiveConfigMock, SetValue(static_cast<int32_t>(ConfigId::MSV_STATIC_MODULE_ENABLED), Matcher<bool>(true)))
		.WillOnce(Return(MSV_SUCCESS));

	EXPECT_EQ(m_spModuleConfigurator->Enabled(true), MSV_SUCCESS);
}

TEST_F(MsvModuleConfigurator_Test, ItShouldSetFalse_WhenDisabled)
{
	EXPECT_CALL(*m_spActiveConfigMock, SetValue(static_cast<int32_t>(ConfigId::MSV_STATIC_MODULE_ENABLED), Matcher<bool>(false)))
		.WillOnce(Return(MSV_SUCCESS));

	EXPECT_EQ(m_spModuleConfigurator->Enabled(false), MSV_SUCCESS);
}

TEST_F(MsvModuleConfigurator_Test, ItShouldFailed_WhenSetEnabledError)
{
	EXPECT_CALL(*m_spActiveConfigMock, SetValue(static_cast<int32_t>(ConfigId::MSV_STATIC_MODULE_ENABLED), Matcher<bool>(false)))
		.WillOnce(Return(MSV_NOT_FOUND_ERROR));

	EXPECT_EQ(m_spModuleConfigurator->Enabled(false), MSV_NOT_FOUND_ERROR);
}


/*-----------------------------------------------------------------------------------------------------
**											IsEnabled Tests
**---------------------------------------------------------------------------------------------------*/


TEST_F(MsvModuleConfigurator_Test, ItShouldReturnTrue_WhenEnabled)
{
	EXPECT_CALL(*m_spActiveConfigMock, GetValue(static_cast<int32_t>(ConfigId::MSV_STATIC_MODULE_ENABLED), Matcher<bool&>(_)))
		.WillOnce(DoAll(SetArgReferee<1>(true), Return(MSV_SUCCESS)));

	bool enabled = false;
	EXPECT_EQ(m_spModuleConfigurator->IsEnabled(enabled), MSV_SUCCESS);
	EXPECT_TRUE(enabled);
}

TEST_F(MsvModuleConfigurator_Test, ItShouldReturnFalse_WhenDisabled)
{
	EXPECT_CALL(*m_spActiveConfigMock, GetValue(static_cast<int32_t>(ConfigId::MSV_STATIC_MODULE_ENABLED), Matcher<bool&>(_)))
		.WillOnce(DoAll(SetArgReferee<1>(false), Return(MSV_SUCCESS)));

	bool enabled = true;
	EXPECT_EQ(m_spModuleConfigurator->IsEnabled(enabled), MSV_SUCCESS);
	EXPECT_FALSE(enabled);
}

TEST_F(MsvModuleConfigurator_Test, ItShouldFailed_WhenEnabledError)
{
	EXPECT_CALL(*m_spActiveConfigMock, GetValue(static_cast<int32_t>(ConfigId::MSV_STATIC_MODULE_ENABLED), Matcher<bool&>(_)))
		.WillOnce(Return(MSV_NOT_FOUND_ERROR));

	bool enabled = false;
	EXPECT_EQ(m_spModuleConfigurator->IsEnabled(enabled), MSV_NOT_FOUND_ERROR);
	EXPECT_FALSE(enabled);
}


/*-----------------------------------------------------------------------------------------------------
**											Installed Tests
**---------------------------------------------------------------------------------------------------*/


TEST_F(MsvModuleConfigurator_Test, ItShouldSetTrue_WhenInstalled)
{
	EXPECT_CALL(*m_spActiveConfigMock, SetValue(static_cast<int32_t>(ConfigId::MSV_STATIC_MODULE_INSTALLED), Matcher<bool>(true)))
		.WillOnce(Return(MSV_SUCCESS));

	EXPECT_EQ(m_spModuleConfigurator->Installed(true), MSV_SUCCESS);
}

TEST_F(MsvModuleConfigurator_Test, ItShouldSetFalse_WhenNotInstalled)
{
	EXPECT_CALL(*m_spActiveConfigMock, SetValue(static_cast<int32_t>(ConfigId::MSV_STATIC_MODULE_INSTALLED), Matcher<bool>(false)))
		.WillOnce(Return(MSV_SUCCESS));

	EXPECT_EQ(m_spModuleConfigurator->Installed(false), MSV_SUCCESS);
}

TEST_F(MsvModuleConfigurator_Test, ItShouldFailed_WhenSetInstalledError)
{
	EXPECT_CALL(*m_spActiveConfigMock, SetValue(static_cast<int32_t>(ConfigId::MSV_STATIC_MODULE_INSTALLED), Matcher<bool>(false)))
		.WillOnce(Return(MSV_NOT_FOUND_ERROR));

	EXPECT_EQ(m_spModuleConfigurator->Installed(false), MSV_NOT_FOUND_ERROR);
}


/*-----------------------------------------------------------------------------------------------------
**											IsInstalled Tests
**---------------------------------------------------------------------------------------------------*/


TEST_F(MsvModuleConfigurator_Test, ItShouldReturnTrue_WhenInstalled)
{
	EXPECT_CALL(*m_spActiveConfigMock, GetValue(static_cast<int32_t>(ConfigId::MSV_STATIC_MODULE_INSTALLED), Matcher<bool&>(_)))
		.WillOnce(DoAll(SetArgReferee<1>(true), Return(MSV_SUCCESS)));

	bool installed = false;
	EXPECT_EQ(m_spModuleConfigurator->IsInstalled(installed), MSV_SUCCESS);
	EXPECT_TRUE(installed);
}

TEST_F(MsvModuleConfigurator_Test, ItShouldReturnFalse_WhenNotInstalled)
{
	EXPECT_CALL(*m_spActiveConfigMock, GetValue(static_cast<int32_t>(ConfigId::MSV_STATIC_MODULE_INSTALLED), Matcher<bool&>(_)))
		.WillOnce(DoAll(SetArgReferee<1>(false), Return(MSV_SUCCESS)));

	bool installed = true;
	EXPECT_EQ(m_spModuleConfigurator->IsInstalled(installed), MSV_SUCCESS);
	EXPECT_FALSE(installed);
}

TEST_F(MsvModuleConfigurator_Test, ItShouldFailed_WhenInstalledError)
{
	EXPECT_CALL(*m_spActiveConfigMock, GetValue(static_cast<int32_t>(ConfigId::MSV_STATIC_MODULE_INSTALLED), Matcher<bool&>(_)))
		.WillOnce(Return(MSV_NOT_FOUND_ERROR));

	bool installed = false;
	EXPECT_EQ(m_spModuleConfigurator->IsInstalled(installed), MSV_NOT_FOUND_ERROR);
	EXPECT_FALSE(installed);
}
