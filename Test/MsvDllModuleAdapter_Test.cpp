

#include "pch.h"

#include "mmodule/MsvDllModuleAdapter.h"

#include "mmodule/Mocks/MsvDllModule_Mock.h"
#include "mdllfactory/Mocks/MsvDllFactory_Mock.h"


using namespace ::testing;


class MsvDllModuleAdapter_Test:
	public MsvModule_TestBase
{
public:
	virtual void SetUp()
	{
		InitializeLogging();

		m_spDllFactoryMock.reset(new (std::nothrow) MsvDllFactory_Mock());
		EXPECT_NE(m_spDllFactoryMock, nullptr);

		m_spDynamicModuleMock.reset(new (std::nothrow) MsvDllModule_Mock());
		EXPECT_NE(m_spDynamicModuleMock, nullptr);

		m_spDllModuleAdapter.reset(new (std::nothrow) MsvDllModuleAdapter(MSV_DYNAMIC_MODULE_ID, m_spDllFactoryMock, m_spLogger));
		EXPECT_NE(m_spDllModuleAdapter, nullptr);
	}

	virtual void TearDown()
	{
		m_spDllModuleAdapter.reset();

		m_spDynamicModuleMock.reset();
		m_spDllFactoryMock.reset();		

		UninitializeLogging();
	}

	//mocks
	std::shared_ptr<MsvDllFactory_Mock> m_spDllFactoryMock;
	std::shared_ptr<MsvDllModule_Mock> m_spDynamicModuleMock;

	//tested classes
	std::shared_ptr<IMsvModule> m_spDllModuleAdapter;
};


TEST_F(MsvDllModuleAdapter_Test, ItShouldBeUnitilized_AfterCreation)
{
	EXPECT_FALSE(m_spDllModuleAdapter->Initialized());
}

TEST_F(MsvDllModuleAdapter_Test, ItShouldBeStopped_AfterCreation)
{
	EXPECT_FALSE(m_spDllModuleAdapter->Running());
}


/*-----------------------------------------------------------------------------------------------------
**											Initialize Tests
**---------------------------------------------------------------------------------------------------*/


TEST_F(MsvDllModuleAdapter_Test, ItShouldSuccessfullyInitialize_WhenNoError)
{
	//set dll factory (dynamic module will be loaded from it)
	std::string moduleId;
	EXPECT_CALL(*m_spDllFactoryMock, GetDllObject(_, _))
		.WillOnce(DoAll(SaveArg<0>(&moduleId), SetArgReferee<1>(m_spDynamicModuleMock), Return(MSV_SUCCESS)));

	//set dynamic module (dll factory will be set and module will be initialized)
	EXPECT_CALL(*m_spDynamicModuleMock, SetDllFactory(Matcher<std::shared_ptr<IMsvDllFactory>>(m_spDllFactoryMock)));
	EXPECT_CALL(*m_spDynamicModuleMock, Initialize())
		.WillOnce(Return(MSV_SUCCESS));

	//set dynamic module (will be initialized)
	EXPECT_CALL(*m_spDynamicModuleMock, Initialize())
		.WillOnce(Return(MSV_SUCCESS));
	EXPECT_CALL(*m_spDynamicModuleMock, Initialized())
		.WillOnce(Return(true));

	//initialize
	EXPECT_EQ(m_spDllModuleAdapter->Initialize(), MSV_SUCCESS);

	//check if right ID has been used
	EXPECT_EQ(moduleId, MSV_DYNAMIC_MODULE_ID);
}

TEST_F(MsvDllModuleAdapter_Test, ItShouldReturnInfo_WhenInitializedAndInitializing)
{
	//set dll factory (dynamic module will be loaded from it)
	std::string moduleId;
	EXPECT_CALL(*m_spDllFactoryMock, GetDllObject(_, _))
		.WillOnce(DoAll(SaveArg<0>(&moduleId), SetArgReferee<1>(m_spDynamicModuleMock), Return(MSV_SUCCESS)));

	//set dynamic module (dll factory will be set and module will be initialized)
	EXPECT_CALL(*m_spDynamicModuleMock, SetDllFactory(Matcher<std::shared_ptr<IMsvDllFactory>>(m_spDllFactoryMock)));
	EXPECT_CALL(*m_spDynamicModuleMock, Initialize())
		.WillOnce(Return(MSV_SUCCESS));

	//set dynamic module (will be initialized)
	EXPECT_CALL(*m_spDynamicModuleMock, Initialize())
		.WillOnce(Return(MSV_SUCCESS));
	EXPECT_CALL(*m_spDynamicModuleMock, Initialized())
		.Times(2)
		.WillRepeatedly(Return(true));

	//initialize
	EXPECT_EQ(m_spDllModuleAdapter->Initialize(), MSV_SUCCESS);
	EXPECT_EQ(m_spDllModuleAdapter->Initialize(), MSV_ALREADY_INITIALIZED_INFO);

	EXPECT_EQ(moduleId, MSV_DYNAMIC_MODULE_ID);
}

TEST_F(MsvDllModuleAdapter_Test, ItShouldFailed_WhenGetDllFailed)
{
	//set dll factory (dynamic module will be loaded from it)
	std::string moduleId;
	EXPECT_CALL(*m_spDllFactoryMock, GetDllObject(_, _))
		.WillOnce(DoAll(SaveArg<0>(&moduleId), SetArgReferee<1>(nullptr), Return(MSV_ALLOCATION_ERROR)));

	//initialize
	EXPECT_EQ(m_spDllModuleAdapter->Initialize(), MSV_ALLOCATION_ERROR);

	//check if right ID has been used
	EXPECT_EQ(moduleId, MSV_DYNAMIC_MODULE_ID);
}

TEST_F(MsvDllModuleAdapter_Test, ItShouldFailed_WhenGetDllReturnedNull)
{
	//set dll factory (dynamic module will be loaded from it)
	std::string moduleId;
	EXPECT_CALL(*m_spDllFactoryMock, GetDllObject(_, _))
		.WillOnce(DoAll(SaveArg<0>(&moduleId), SetArgReferee<1>(nullptr), Return(MSV_SUCCESS)));

	//initialize
	EXPECT_EQ(m_spDllModuleAdapter->Initialize(), MSV_INVALID_DATA_ERROR);

	//check if right ID has been used
	EXPECT_EQ(moduleId, MSV_DYNAMIC_MODULE_ID);
}


/*-----------------------------------------------------------------------------------------------------
**											Uninitialize Tests
**---------------------------------------------------------------------------------------------------*/


TEST_F(MsvDllModuleAdapter_Test, ItShouldReturnInfo_WhenNotInitializedAndUninitializing)
{
	EXPECT_EQ(m_spDllModuleAdapter->Uninitialize(), MSV_NOT_INITIALIZED_INFO);
}

TEST_F(MsvDllModuleAdapter_Test, ItShouldSuccessfullyUninitialize_WhenNoError)
{
	//set dll factory (dynamic module will be loaded from it)
	std::string moduleId;
	EXPECT_CALL(*m_spDllFactoryMock, GetDllObject(_, _))
		.WillOnce(DoAll(SaveArg<0>(&moduleId), SetArgReferee<1>(m_spDynamicModuleMock), Return(MSV_SUCCESS)));

	//set dynamic module (dll factory will be set and module will be initialized)
	EXPECT_CALL(*m_spDynamicModuleMock, SetDllFactory(Matcher<std::shared_ptr<IMsvDllFactory>>(m_spDllFactoryMock)));
	EXPECT_CALL(*m_spDynamicModuleMock, Initialize())
		.WillOnce(Return(MSV_SUCCESS));

	//set dynamic module (will be initialized)
	EXPECT_CALL(*m_spDynamicModuleMock, Initialize())
		.WillOnce(Return(MSV_SUCCESS));
	EXPECT_CALL(*m_spDynamicModuleMock, Initialized())
		.Times(2)
		.WillRepeatedly(Return(true));
	EXPECT_CALL(*m_spDynamicModuleMock, Uninitialize())
		.WillOnce(Return(MSV_SUCCESS));

	//initialize
	EXPECT_EQ(m_spDllModuleAdapter->Initialize(), MSV_SUCCESS);

	//check if right ID has been used
	EXPECT_EQ(moduleId, MSV_DYNAMIC_MODULE_ID);

	EXPECT_EQ(m_spDllModuleAdapter->Uninitialize(), MSV_SUCCESS);
}

TEST_F(MsvDllModuleAdapter_Test, ItShouldFailed_WhenDllModuleUninitializeFailed)
{
	//set dll factory (dynamic module will be loaded from it)
	std::string moduleId;
	EXPECT_CALL(*m_spDllFactoryMock, GetDllObject(_, _))
		.WillOnce(DoAll(SaveArg<0>(&moduleId), SetArgReferee<1>(m_spDynamicModuleMock), Return(MSV_SUCCESS)));

	//set dynamic module (dll factory will be set and module will be initialized)
	EXPECT_CALL(*m_spDynamicModuleMock, SetDllFactory(Matcher<std::shared_ptr<IMsvDllFactory>>(m_spDllFactoryMock)));
	EXPECT_CALL(*m_spDynamicModuleMock, Initialize())
		.WillOnce(Return(MSV_SUCCESS));

	//set dynamic module (will be initialized)
	EXPECT_CALL(*m_spDynamicModuleMock, Initialize())
		.WillOnce(Return(MSV_SUCCESS));
	EXPECT_CALL(*m_spDynamicModuleMock, Initialized())
		.Times(2)
		.WillRepeatedly(Return(true));
	EXPECT_CALL(*m_spDynamicModuleMock, Running())
		.Times(2)
		.WillOnce(Return(false))
		.WillOnce(Return(true));
	EXPECT_CALL(*m_spDynamicModuleMock, Uninitialize())
		.WillOnce(Return(MSV_ALLOCATION_ERROR));

	//initialize
	EXPECT_EQ(m_spDllModuleAdapter->Initialize(), MSV_SUCCESS);

	//check if right ID has been used
	EXPECT_EQ(moduleId, MSV_DYNAMIC_MODULE_ID);

	EXPECT_EQ(m_spDllModuleAdapter->Uninitialize(), MSV_ALLOCATION_ERROR);
}


/*-----------------------------------------------------------------------------------------------------
**											Start Tests
**---------------------------------------------------------------------------------------------------*/


TEST_F(MsvDllModuleAdapter_Test, ItShouldFailedToStart_WhenNotInitialized)
{
	EXPECT_EQ(m_spDllModuleAdapter->Start(), MSV_NOT_INITIALIZED_ERROR);
}

TEST_F(MsvDllModuleAdapter_Test, ItShouldSuccessfullyStart_WhenNoError)
{
	//set dll factory (dynamic module will be loaded from it)
	std::string moduleId;
	EXPECT_CALL(*m_spDllFactoryMock, GetDllObject(_, _))
		.WillOnce(DoAll(SaveArg<0>(&moduleId), SetArgReferee<1>(m_spDynamicModuleMock), Return(MSV_SUCCESS)));

	//set dynamic module (dll factory will be set and module will be initialized)
	EXPECT_CALL(*m_spDynamicModuleMock, SetDllFactory(Matcher<std::shared_ptr<IMsvDllFactory>>(m_spDllFactoryMock)));
	EXPECT_CALL(*m_spDynamicModuleMock, Initialize())
		.WillOnce(Return(MSV_SUCCESS));

	//set dynamic module (will be initialized)
	EXPECT_CALL(*m_spDynamicModuleMock, Initialize())
		.WillOnce(Return(MSV_SUCCESS));
	EXPECT_CALL(*m_spDynamicModuleMock, Initialized())
		.Times(2)
		.WillRepeatedly(Return(true));
	EXPECT_CALL(*m_spDynamicModuleMock, Running())
		.Times(2)
		.WillOnce(Return(false))
		.WillOnce(Return(true));
	EXPECT_CALL(*m_spDynamicModuleMock, Start())
		.WillOnce(Return(MSV_SUCCESS));

	//initialize
	EXPECT_EQ(m_spDllModuleAdapter->Initialize(), MSV_SUCCESS);

	//check if right ID has been used
	EXPECT_EQ(moduleId, MSV_DYNAMIC_MODULE_ID);

	EXPECT_EQ(m_spDllModuleAdapter->Start(), MSV_SUCCESS);
	//EXPECT_TRUE(m_spDllModuleAdapter->Running());
}

TEST_F(MsvDllModuleAdapter_Test, ItShouldFailedToStart_WhenDllModuleStartFailed)
{
	//set dll factory (dynamic module will be loaded from it)
	std::string moduleId;
	EXPECT_CALL(*m_spDllFactoryMock, GetDllObject(_, _))
		.WillOnce(DoAll(SaveArg<0>(&moduleId), SetArgReferee<1>(m_spDynamicModuleMock), Return(MSV_SUCCESS)));

	//set dynamic module (dll factory will be set and module will be initialized)
	EXPECT_CALL(*m_spDynamicModuleMock, SetDllFactory(Matcher<std::shared_ptr<IMsvDllFactory>>(m_spDllFactoryMock)));
	EXPECT_CALL(*m_spDynamicModuleMock, Initialize())
		.WillOnce(Return(MSV_SUCCESS));

	//set dynamic module (will be initialized)
	EXPECT_CALL(*m_spDynamicModuleMock, Initialize())
		.WillOnce(Return(MSV_SUCCESS));
	EXPECT_CALL(*m_spDynamicModuleMock, Initialized())
		.Times(2)
		.WillRepeatedly(Return(true));
	EXPECT_CALL(*m_spDynamicModuleMock, Running())
		.Times(2)
		.WillRepeatedly(Return(false));
	EXPECT_CALL(*m_spDynamicModuleMock, Start())
		.WillOnce(Return(MSV_ALLOCATION_ERROR));

	//initialize
	EXPECT_EQ(m_spDllModuleAdapter->Initialize(), MSV_SUCCESS);

	//check if right ID has been used
	EXPECT_EQ(moduleId, MSV_DYNAMIC_MODULE_ID);

	EXPECT_EQ(m_spDllModuleAdapter->Start(), MSV_ALLOCATION_ERROR);
	EXPECT_FALSE(m_spDllModuleAdapter->Running());
}


/*-----------------------------------------------------------------------------------------------------
**											Stop Tests
**---------------------------------------------------------------------------------------------------*/


TEST_F(MsvDllModuleAdapter_Test, ItShouldReturnInfo_WhenNotInitializedAndStopping)
{
	EXPECT_EQ(m_spDllModuleAdapter->Stop(), MSV_NOT_RUNNING_INFO);
}

TEST_F(MsvDllModuleAdapter_Test, ItShouldSuccessfullyStop_WhenNoError)
{
	//set dll factory (dynamic module will be loaded from it)
	std::string moduleId;
	EXPECT_CALL(*m_spDllFactoryMock, GetDllObject(_, _))
		.WillOnce(DoAll(SaveArg<0>(&moduleId), SetArgReferee<1>(m_spDynamicModuleMock), Return(MSV_SUCCESS)));

	//set dynamic module (dll factory will be set and module will be initialized)
	EXPECT_CALL(*m_spDynamicModuleMock, SetDllFactory(Matcher<std::shared_ptr<IMsvDllFactory>>(m_spDllFactoryMock)));
	EXPECT_CALL(*m_spDynamicModuleMock, Initialize())
		.WillOnce(Return(MSV_SUCCESS));

	//set dynamic module (will be initialized)
	EXPECT_CALL(*m_spDynamicModuleMock, Initialize())
		.WillOnce(Return(MSV_SUCCESS));
	EXPECT_CALL(*m_spDynamicModuleMock, Running())
		.Times(2)
		.WillOnce(Return(true))
		.WillOnce(Return(false));
	EXPECT_CALL(*m_spDynamicModuleMock, Stop())
		.WillOnce(Return(MSV_SUCCESS));

	//initialize
	EXPECT_EQ(m_spDllModuleAdapter->Initialize(), MSV_SUCCESS);

	//check if right ID has been used
	EXPECT_EQ(moduleId, MSV_DYNAMIC_MODULE_ID);

	EXPECT_EQ(m_spDllModuleAdapter->Stop(), MSV_SUCCESS);
}

TEST_F(MsvDllModuleAdapter_Test, ItShouldFailed_WhenDllModuleStopFailed)
{
	//set dll factory (dynamic module will be loaded from it)
	std::string moduleId;
	EXPECT_CALL(*m_spDllFactoryMock, GetDllObject(_, _))
		.WillOnce(DoAll(SaveArg<0>(&moduleId), SetArgReferee<1>(m_spDynamicModuleMock), Return(MSV_SUCCESS)));

	//set dynamic module (dll factory will be set and module will be initialized)
	EXPECT_CALL(*m_spDynamicModuleMock, SetDllFactory(Matcher<std::shared_ptr<IMsvDllFactory>>(m_spDllFactoryMock)));
	EXPECT_CALL(*m_spDynamicModuleMock, Initialize())
		.WillOnce(Return(MSV_SUCCESS));

	//set dynamic module (will be initialized)
	EXPECT_CALL(*m_spDynamicModuleMock, Initialize())
		.WillOnce(Return(MSV_SUCCESS));
	EXPECT_CALL(*m_spDynamicModuleMock, Initialized())
		.Times(2)
		.WillRepeatedly(Return(true));
	EXPECT_CALL(*m_spDynamicModuleMock, Running())
		.Times(2)
		.WillOnce(Return(true))
		.WillOnce(Return(false));
	EXPECT_CALL(*m_spDynamicModuleMock, Stop())
		.WillOnce(Return(MSV_ALLOCATION_ERROR));

	//initialize
	EXPECT_EQ(m_spDllModuleAdapter->Initialize(), MSV_SUCCESS);

	//check if right ID has been used
	EXPECT_EQ(moduleId, MSV_DYNAMIC_MODULE_ID);

	EXPECT_EQ(m_spDllModuleAdapter->Stop(), MSV_ALLOCATION_ERROR);
}
