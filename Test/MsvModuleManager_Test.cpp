

#include "pch.h"

#include "mmodule/MsvModuleManager.h"

#include "mmodule/Mocks/MsvModule_Mock.h"
#include "mmodule/Mocks/MsvModuleConfigurator_Mock.h"


using namespace ::testing;


class MsvModuleManagerTestWrapper:
	public MsvModuleManager
{
public:
	MsvModuleManagerTestWrapper(std::shared_ptr<MsvLogger> spLogger):
		MsvModuleManager(spLogger)
	{

	}

	const std::map<int32_t, std::pair<std::shared_ptr<IMsvModuleConfigurator>, std::shared_ptr<IMsvModule>>>& GetModules() const
	{
		return m_modules;
	}
};

class MsvModuleManager_Test:
	public MsvModule_TestBase
{
public:
	virtual void SetUp()
	{
		InitializeLogging();

		m_spStaticModuleMock.reset(new (std::nothrow) MsvModule_Mock());
		EXPECT_NE(m_spStaticModuleMock, nullptr);

		m_spStaticModuleConfiguratorMock.reset(new (std::nothrow) MsvModuleConfigurator_Mock());
		EXPECT_NE(m_spStaticModuleConfiguratorMock, nullptr);

		m_spDynamicModuleMock.reset(new (std::nothrow) MsvModule_Mock());
		EXPECT_NE(m_spDynamicModuleMock, nullptr);

		m_spDynamicModuleConfiguratorMock.reset(new (std::nothrow) MsvModuleConfigurator_Mock());
		EXPECT_NE(m_spDynamicModuleConfiguratorMock, nullptr);

		m_spModuleManager.reset(new (std::nothrow) MsvModuleManagerTestWrapper(m_spLogger));
		EXPECT_NE(m_spModuleManager, nullptr);

		//add modules to module manager
		m_spModuleManager->AddModule(static_cast<int32_t>(ModuleId::MSV_TEST_STATIC_MODULE), m_spStaticModuleMock, m_spStaticModuleConfiguratorMock);
		m_spModuleManager->AddModule(static_cast<int32_t>(ModuleId::MSV_TEST_DYNAMIC_MODULE), m_spDynamicModuleMock, m_spDynamicModuleConfiguratorMock);
	}

	virtual void TearDown()
	{
		m_spStaticModuleMock.reset();
		m_spStaticModuleConfiguratorMock.reset();
		m_spDynamicModuleMock.reset();
		m_spDynamicModuleConfiguratorMock.reset();

		m_spModuleManager.reset();

		UninitializeLogging();
	}

	void SetInitializeExpectations(bool staticInstalled, bool staticEnabled, bool dynamicInstalled, bool dynamicEnabled, MsvErrorCode dynamicInstalledErrorCode, MsvErrorCode dynamicEnabledErrorCode)
	{
		//set static module configurator
		EXPECT_CALL(*m_spStaticModuleConfiguratorMock, IsInstalled(Matcher<bool&>(_)))
			.WillOnce(DoAll(SetArgReferee<0>(staticInstalled), Return(MSV_SUCCESS)));
		EXPECT_CALL(*m_spStaticModuleConfiguratorMock, IsEnabled(Matcher<bool&>(_)))
			.WillOnce(DoAll(SetArgReferee<0>(staticEnabled), Return(MSV_SUCCESS)));

		//set dynamic module configurator
		EXPECT_CALL(*m_spDynamicModuleConfiguratorMock, IsInstalled(Matcher<bool&>(_)))
			.WillOnce(DoAll(SetArgReferee<0>(dynamicInstalled), Return(dynamicInstalledErrorCode)));
		if (MSV_SUCCEEDED(dynamicInstalledErrorCode))
		{
			EXPECT_CALL(*m_spDynamicModuleConfiguratorMock, IsEnabled(Matcher<bool&>(_)))
				.WillOnce(DoAll(SetArgReferee<0>(dynamicEnabled), Return(dynamicEnabledErrorCode)));
		}

		if (staticInstalled && staticEnabled && MSV_SUCCEEDED(dynamicInstalledErrorCode))
		{
			//set static module (will be initialized)
			EXPECT_CALL(*m_spStaticModuleMock, Initialize())
				.WillOnce(Return(MSV_SUCCESS));
		}

		if (dynamicInstalled && dynamicEnabled && MSV_SUCCEEDED(dynamicInstalledErrorCode) && MSV_SUCCEEDED(dynamicEnabledErrorCode))
		{
			//set dynamic module (will be initialized)
			EXPECT_CALL(*m_spDynamicModuleMock, Initialize())
				.WillOnce(Return(MSV_SUCCESS));
		}
	}

	void InitializeModuleManager()
	{
		SetInitializeExpectations(true, true, true, true, MSV_SUCCESS, MSV_SUCCESS);

		//initialize
		EXPECT_EQ(m_spModuleManager->Initialize(), MSV_SUCCESS);
		EXPECT_TRUE(m_spModuleManager->Initialized());
	}

	void StartModuleManager(uint32_t runningChecks, uint32_t stopCalls, MsvErrorCode dynamicStartErrorCode, MsvErrorCode dynamicStopErrorCode, MsvErrorCode dynamicUnitializeErrorCode)
	{
		InitializeModuleManager();

		//set static module (will be initialized, started, stopped and uninitialized)
		EXPECT_CALL(*m_spStaticModuleMock, Initialized())
			.Times(MSV_FAILED(dynamicUnitializeErrorCode) ? 3 : 2)
			.WillRepeatedly(Return(true));
		EXPECT_CALL(*m_spStaticModuleMock, Start())
			.WillOnce(Return(MSV_SUCCESS));
		EXPECT_CALL(*m_spStaticModuleMock, Running())
			.Times(runningChecks)
			.WillRepeatedly(Return(true));
		EXPECT_CALL(*m_spStaticModuleMock, Stop())
			.Times(stopCalls)
			.WillRepeatedly(Return(MSV_SUCCESS));
		EXPECT_CALL(*m_spStaticModuleMock, Uninitialize())
			.Times(MSV_FAILED(dynamicUnitializeErrorCode) ? 2 : 1)
			.WillRepeatedly(Return(MSV_SUCCESS));

		//set dynamic module (will be initialized, started, stopped and uninitialized)
		EXPECT_CALL(*m_spDynamicModuleMock, Initialized())
			.Times(MSV_FAILED(dynamicUnitializeErrorCode) ? 3 : 2)
			.WillRepeatedly(Return(true));
		EXPECT_CALL(*m_spDynamicModuleMock, Start())
			.WillOnce(Return(dynamicStartErrorCode));
		EXPECT_CALL(*m_spDynamicModuleMock, Running())
			.Times(runningChecks)
			.WillRepeatedly(Return(true));
		EXPECT_CALL(*m_spDynamicModuleMock, Stop())
			.Times(stopCalls)
			.WillOnce(Return(dynamicStopErrorCode));
		EXPECT_CALL(*m_spDynamicModuleMock, Uninitialize())
			.Times(MSV_FAILED(dynamicUnitializeErrorCode) ? 2 : 1)
			.WillRepeatedly(Return(dynamicUnitializeErrorCode));

		//start
		EXPECT_EQ(m_spModuleManager->Start(), dynamicStartErrorCode);
		EXPECT_EQ(m_spModuleManager->Running(), MSV_SUCCEEDED(dynamicStartErrorCode));
	}

	void StopModuleManager(MsvErrorCode dynamicUnitializeErrorCode)
	{
		StartModuleManager(1, 1, MSV_SUCCESS, MSV_SUCCESS, dynamicUnitializeErrorCode);

		EXPECT_EQ(m_spModuleManager->Stop(), MSV_SUCCESS);
		EXPECT_FALSE(m_spModuleManager->Running());
	}

	void AddAndSetModule(bool managerInitialized, bool managerRunning, bool otherInstalled, bool otherEnabled, MsvErrorCode otherInstalledErrorCode, MsvErrorCode otherEnabledErrorCode,
								uint32_t initializedCalls, bool initializedFirst, bool inititalizedSecond,
								MsvErrorCode inititalizeErrorCode,
								uint32_t runningCalls, bool runningFirst, bool runningSecond,
								MsvErrorCode startErrorCode,
								MsvErrorCode resultErrorCode, bool moduleAdded)
	{
		std::shared_ptr<MsvModule_Mock> spOtherModuleMock(new (std::nothrow) MsvModule_Mock());
		EXPECT_NE(spOtherModuleMock, nullptr);
		std::shared_ptr<MsvModuleConfigurator_Mock> spOtherModuleConfiguratorMock(new (std::nothrow) MsvModuleConfigurator_Mock());
		EXPECT_NE(spOtherModuleConfiguratorMock, nullptr);

		if (managerRunning)
		{
			StartModuleManager(1, 1, MSV_SUCCESS, MSV_SUCCESS, MSV_SUCCESS);
		}
		else if (managerInitialized)
		{
			InitializeModuleManager();
		}

		//set other module configurator
		EXPECT_CALL(*spOtherModuleConfiguratorMock, IsInstalled(Matcher<bool&>(_)))
			.WillOnce(DoAll(SetArgReferee<0>(otherInstalled), Return(otherInstalledErrorCode)));
		if (MSV_SUCCEEDED(otherInstalledErrorCode))
		{
			EXPECT_CALL(*spOtherModuleConfiguratorMock, IsEnabled(Matcher<bool&>(_)))
				.WillOnce(DoAll(SetArgReferee<0>(otherEnabled), Return(otherEnabledErrorCode)));
		}

		//set other module
		EXPECT_CALL(*spOtherModuleMock, Initialized())
			.Times(initializedCalls)
			.WillOnce(Return(initializedFirst))
			.WillOnce(Return(inititalizedSecond))
			.WillOnce(Return(false));	//in uninitialized
		if (managerInitialized && otherInstalled && otherEnabled && !initializedFirst && MSV_SUCCEEDED(otherInstalledErrorCode) && MSV_SUCCEEDED(otherEnabledErrorCode))
		{
			EXPECT_CALL(*spOtherModuleMock, Initialize())
				.WillOnce(Return(inititalizeErrorCode));
		}
		EXPECT_CALL(*spOtherModuleMock, Running())
			.Times(runningCalls)
			.WillOnce(Return(runningFirst))
			.WillOnce(Return(runningSecond));	//in stop
		if (managerRunning && otherInstalled && otherEnabled && inititalizedSecond && !runningFirst && MSV_SUCCEEDED(otherInstalledErrorCode) && MSV_SUCCEEDED(otherEnabledErrorCode))
		{
			EXPECT_CALL(*spOtherModuleMock, Start())
				.WillOnce(Return(startErrorCode));
			if (MSV_FAILED(startErrorCode))
			{
				EXPECT_CALL(*spOtherModuleMock, Uninitialize())
					.WillOnce(Return(MSV_SUCCESS));
			}
		}

		EXPECT_EQ(m_spModuleManager->AddModule(static_cast<int32_t>(ModuleId::MSV_TEST_OTHER_MODULE), spOtherModuleMock, spOtherModuleConfiguratorMock), resultErrorCode);

		//check modules
		const std::map<int32_t, std::pair<std::shared_ptr<IMsvModuleConfigurator>, std::shared_ptr<IMsvModule>>>& modules = std::static_pointer_cast<MsvModuleManagerTestWrapper>(m_spModuleManager)->GetModules();
		EXPECT_EQ(modules.find(static_cast<int32_t>(ModuleId::MSV_TEST_OTHER_MODULE)) != modules.end(), moduleAdded);
		if (moduleAdded)
		{
			EXPECT_EQ(modules.find(static_cast<int32_t>(ModuleId::MSV_TEST_OTHER_MODULE))->second.second, spOtherModuleMock);
		}

		//stop and uninitialize after test
		if (managerRunning)
		{
			EXPECT_EQ(m_spModuleManager->Stop(), MSV_SUCCESS);
		}
		if (managerInitialized)
		{
			EXPECT_EQ(m_spModuleManager->Uninitialize(), MSV_SUCCESS);
		}
	}

	//mocks
	std::shared_ptr<MsvModuleConfigurator_Mock> m_spStaticModuleConfiguratorMock;
	std::shared_ptr<MsvModule_Mock> m_spStaticModuleMock;
	std::shared_ptr<MsvModuleConfigurator_Mock> m_spDynamicModuleConfiguratorMock;
	std::shared_ptr<MsvModule_Mock> m_spDynamicModuleMock;

	//tested classes
	std::shared_ptr<IMsvModuleManager> m_spModuleManager;
};


TEST_F(MsvModuleManager_Test, ItShouldBeUnitilized_AfterCreation)
{
	EXPECT_FALSE(m_spModuleManager->Initialized());
}

TEST_F(MsvModuleManager_Test, ItShouldBeStopped_AfterCreation)
{
	EXPECT_FALSE(m_spModuleManager->Running());
}


/*-----------------------------------------------------------------------------------------------------
**											Initialize Tests
**---------------------------------------------------------------------------------------------------*/


TEST_F(MsvModuleManager_Test, ItShouldReturnInfo_WhenInitializedAndInitializing)
{
	InitializeModuleManager();

	//initialize second time
	EXPECT_EQ(m_spModuleManager->Initialize(), MSV_ALREADY_INITIALIZED_INFO);
	EXPECT_TRUE(m_spModuleManager->Initialized());

	//uninitialize after test
	EXPECT_EQ(m_spModuleManager->Uninitialize(), MSV_SUCCESS);
}

TEST_F(MsvModuleManager_Test, ItShouldFailed_WhenGetInstalledFlagFailed)
{
	SetInitializeExpectations(true, true, true, true, MSV_NOT_FOUND_ERROR, MSV_SUCCESS);

	//set static module (will be initialized and uninitialized)
	EXPECT_CALL(*m_spStaticModuleMock, Initialized())
		.WillOnce(Return(true));
	EXPECT_CALL(*m_spStaticModuleMock, Uninitialize())
		.WillOnce(Return(MSV_SUCCESS));

	//set dynamic module
	EXPECT_CALL(*m_spDynamicModuleMock, Initialized())
		.WillOnce(Return(false));

	//initialize
	EXPECT_EQ(m_spModuleManager->Initialize(), MSV_NOT_FOUND_ERROR);
	EXPECT_FALSE(m_spModuleManager->Initialized());
}

TEST_F(MsvModuleManager_Test, ItShouldFailed_WhenGetEnabledFlagFailed)
{
	SetInitializeExpectations(true, true, true, true, MSV_SUCCESS, MSV_NOT_FOUND_ERROR);

	//set static module (will be initialized and uninitialized)
	EXPECT_CALL(*m_spStaticModuleMock, Initialized())
		.WillOnce(Return(true));
	EXPECT_CALL(*m_spStaticModuleMock, Uninitialize())
		.WillOnce(Return(MSV_SUCCESS));

	//set dynamic module
	EXPECT_CALL(*m_spDynamicModuleMock, Initialized())
		.WillOnce(Return(false));

	//initialize
	EXPECT_EQ(m_spModuleManager->Initialize(), MSV_NOT_FOUND_ERROR);
	EXPECT_FALSE(m_spModuleManager->Initialized());
}

TEST_F(MsvModuleManager_Test, ItShouldNotInitializeModule_WhenItIsNotInstalledOrEnabled)
{
	SetInitializeExpectations(false, true, true, false, MSV_SUCCESS, MSV_SUCCESS);

	//initialize
	EXPECT_EQ(m_spModuleManager->Initialize(), MSV_SUCCESS);
	EXPECT_TRUE(m_spModuleManager->Initialized());

	//uninitialize after test
	EXPECT_EQ(m_spModuleManager->Uninitialize(), MSV_SUCCESS);
}


/*-----------------------------------------------------------------------------------------------------
**											Uninitialize Tests
**---------------------------------------------------------------------------------------------------*/


TEST_F(MsvModuleManager_Test, ItShouldReturnInfo_WhenNotInitializedAndUninitializing)
{
	EXPECT_EQ(m_spModuleManager->Uninitialize(), MSV_NOT_INITIALIZED_INFO);
}

TEST_F(MsvModuleManager_Test, ItShouldFailed_WhenRunningAndUninitializing)
{
	StartModuleManager(1, 1, MSV_SUCCESS, MSV_SUCCESS, MSV_SUCCESS);

	EXPECT_EQ(m_spModuleManager->Uninitialize(), MSV_STILL_RUNNING_ERROR);

	//stop and uninitialize after test
	EXPECT_EQ(m_spModuleManager->Stop(), MSV_SUCCESS);
	EXPECT_EQ(m_spModuleManager->Uninitialize(), MSV_SUCCESS);
}

TEST_F(MsvModuleManager_Test, ItShouldFailed_WhenUnitializeAnyModuleFailed)
{
	StopModuleManager(MSV_NOT_INITIALIZED_ERROR);

	EXPECT_EQ(m_spModuleManager->Uninitialize(), MSV_NOT_INITIALIZED_ERROR);
	EXPECT_TRUE(m_spModuleManager->Initialized());
}

TEST_F(MsvModuleManager_Test, ItShouldBeUninitialized_WhenUninitializeSucceeded)
{
	StopModuleManager(MSV_SUCCESS);

	EXPECT_EQ(m_spModuleManager->Uninitialize(), MSV_SUCCESS);
	EXPECT_FALSE(m_spModuleManager->Initialized());
}


/*-----------------------------------------------------------------------------------------------------
**											Start Tests
**---------------------------------------------------------------------------------------------------*/


TEST_F(MsvModuleManager_Test, ItShouldFailedToStart_WhenNotInitialized)
{
	EXPECT_EQ(m_spModuleManager->Start(), MSV_NOT_INITIALIZED_ERROR);
}

TEST_F(MsvModuleManager_Test, ItShouldReturnInfo_WhenRunningAndStarting)
{
	StartModuleManager(1, 1, MSV_SUCCESS, MSV_SUCCESS, MSV_SUCCESS);

	//start second time
	EXPECT_EQ(m_spModuleManager->Start(), MSV_ALREADY_RUNNING_INFO);
	EXPECT_TRUE(m_spModuleManager->Running());

	//stop and uninitialize after test
	EXPECT_EQ(m_spModuleManager->Stop(), MSV_SUCCESS);
	EXPECT_EQ(m_spModuleManager->Uninitialize(), MSV_SUCCESS);
}

TEST_F(MsvModuleManager_Test, ItShouldFailed_WhenStartAnyModuleFailed)
{
	StartModuleManager(1, 1, MSV_NOT_INITIALIZED_ERROR, MSV_NOT_INITIALIZED_ERROR, MSV_SUCCESS);

	//uninitialize after test
	EXPECT_EQ(m_spModuleManager->Uninitialize(), MSV_SUCCESS);
}


/*-----------------------------------------------------------------------------------------------------
**											Stop Tests
**---------------------------------------------------------------------------------------------------*/


TEST_F(MsvModuleManager_Test, StopShouldReturnInfo_WhenNotInitialized)
{
	EXPECT_EQ(m_spModuleManager->Stop(), MSV_NOT_RUNNING_INFO);
}

TEST_F(MsvModuleManager_Test, StopShouldReturnInfo_WhenNotRunning)
{
	InitializeModuleManager();

	EXPECT_EQ(m_spModuleManager->Stop(), MSV_NOT_RUNNING_INFO);
}

TEST_F(MsvModuleManager_Test, StopShouldFailed_WhenStopAnyModuleFailed)
{
	StartModuleManager(2, 2, MSV_SUCCESS, MSV_NOT_INITIALIZED_ERROR, MSV_SUCCESS);

	EXPECT_EQ(m_spModuleManager->Stop(), MSV_NOT_INITIALIZED_ERROR);
	EXPECT_TRUE(m_spModuleManager->Running());
}

TEST_F(MsvModuleManager_Test, ItShouldBeStopped_WhenStopSucceeded)
{
	StopModuleManager(MSV_SUCCESS);
}


/*-----------------------------------------------------------------------------------------------------
**											AddModule Tests
**---------------------------------------------------------------------------------------------------*/


TEST_F(MsvModuleManager_Test, ItShouldFailed_WhenAddingInvalidModule)
{
	EXPECT_EQ(m_spModuleManager->AddModule(static_cast<int32_t>(ModuleId::MSV_TEST_OTHER_MODULE), nullptr, m_spDynamicModuleConfiguratorMock), MSV_INVALID_DATA_ERROR);
}

TEST_F(MsvModuleManager_Test, ItShouldFailed_WhenAddingInvalidModuleConfigurator)
{
	std::shared_ptr<MsvModule_Mock> spOtherModuleMock(new (std::nothrow) MsvModule_Mock());
	EXPECT_NE(spOtherModuleMock, nullptr);

	EXPECT_EQ(m_spModuleManager->AddModule(static_cast<int32_t>(ModuleId::MSV_TEST_OTHER_MODULE), spOtherModuleMock, nullptr), MSV_INVALID_DATA_ERROR);
}

TEST_F(MsvModuleManager_Test, AddModuleShouldFailed_WhenIsInstalledFailed)
{
	AddAndSetModule(true, false, true, true, MSV_CLOSE_ERROR, MSV_SUCCESS,
						 0, true, true,
						 MSV_SUCCESS,
						 0, true, true,
						 MSV_SUCCESS,
						 MSV_CLOSE_ERROR, false);
}

TEST_F(MsvModuleManager_Test, AddModuleShouldFailed_WhenIsEnanledFailed)
{
	AddAndSetModule(true, false, true, true, MSV_CLOSE_ERROR, MSV_CLOSE_ERROR,
						 0, true, true,
						 MSV_SUCCESS,
						 0, true, true,
						 MSV_SUCCESS,
						 MSV_CLOSE_ERROR, false);
}

TEST_F(MsvModuleManager_Test, AddModuleShouldSucceded_WhenModuleIsNotInstalled)
{
	AddAndSetModule(true, false, false, true, MSV_SUCCESS, MSV_SUCCESS,
						 1, false, true,
						 MSV_SUCCESS,
						 0, true, true,
						 MSV_SUCCESS,
						 MSV_SUCCESS, true);
}

TEST_F(MsvModuleManager_Test, AddModuleShouldSucceded_WhenModuleIsNotEnabled)
{
	AddAndSetModule(true, false, true, false, MSV_SUCCESS, MSV_SUCCESS,
						 1, false, true,
						 MSV_SUCCESS,
						 0, true, true,
						 MSV_SUCCESS,
						 MSV_SUCCESS, true);
}

TEST_F(MsvModuleManager_Test, AddModuleShouldInitializeModule_WhenInitialized)
{
	AddAndSetModule(true, false, true, true, MSV_SUCCESS, MSV_SUCCESS,
						 2, false, true,
						 MSV_SUCCESS,
						 0, true, true,
						 MSV_SUCCESS,
						 MSV_SUCCESS, true);
}

TEST_F(MsvModuleManager_Test, AddModuleShouldSucceeded_WhenModuleIsInitialized)
{
	AddAndSetModule(true, false, true, true, MSV_SUCCESS, MSV_SUCCESS,
						 2, true, true,
						 MSV_SUCCESS,
						 0, true, true,
						 MSV_SUCCESS,
						 MSV_SUCCESS, true);
}

TEST_F(MsvModuleManager_Test, AddModuleShouldFailed_WhenInitializeFailed)
{
	AddAndSetModule(true, false, true, true, MSV_SUCCESS, MSV_SUCCESS,
						 1, false, false,
						 MSV_CLOSE_ERROR,
						 0, true, true,
						 MSV_SUCCESS,
						 MSV_CLOSE_ERROR, false);
}

TEST_F(MsvModuleManager_Test, AddModuleShouldStartModule_WhenStarted)
{
	AddAndSetModule(true, true, true, true, MSV_SUCCESS, MSV_SUCCESS,
						 3, false, true,
						 MSV_SUCCESS,
						 2, true, false,
						 MSV_SUCCESS,
						 MSV_SUCCESS, true);
}

TEST_F(MsvModuleManager_Test, AddModuleShouldFailed_WhenStartingButNotInitialized)
{
	AddAndSetModule(true, true, true, true, MSV_SUCCESS, MSV_SUCCESS,
						 2, false, false,
						 MSV_SUCCESS,
						 0, true, true,
						 MSV_SUCCESS,
						 MSV_NOT_INITIALIZED_ERROR, false);
}

TEST_F(MsvModuleManager_Test, AddModuleShouldSucceeded_WhenModuleIsRunning)
{
	AddAndSetModule(true, true, true, true, MSV_SUCCESS, MSV_SUCCESS,
						 3, true, true,
						 MSV_SUCCESS,
						 2, true, false,
						 MSV_SUCCESS,
						 MSV_SUCCESS, true);
}

TEST_F(MsvModuleManager_Test, AddModuleShouldFailed_WhenModuleStartFailed)
{
	AddAndSetModule(true, true, true, true, MSV_SUCCESS, MSV_SUCCESS,
						 2, true, true,
						 MSV_SUCCESS,
						 1, false, false,
						 MSV_CLOSE_ERROR,
						 MSV_CLOSE_ERROR, false);
}