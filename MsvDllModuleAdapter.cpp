/**************************************************************************************************//**
* @addtogroup	MMODULE
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech DLL Module Adapter Implementation
* @details		Contains implementation of @ref MsvDllModuleAdapter.
* @author		Martin Svoboda
* @date			19.05.2019
* @copyright	GNU General Public License (GPLv3).
******************************************************************************************************/


/*
This file is part of MarsTech Module Library.

MarsTech Dependency Injection is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

MarsTech Promise Like Syntax is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar. If not, see <https://www.gnu.org/licenses/>.
*/


#include "MsvDllModuleAdapter.h"


/********************************************************************************************************************************
*															Constructors and destructors
********************************************************************************************************************************/


MsvDllModuleAdapter::MsvDllModuleAdapter(const char* moduleId, std::shared_ptr<IMsvDllFactory> spDllFactory, std::shared_ptr<MsvLogger> spLogger):
	m_moduleId(moduleId),
	m_spDllFactory(spDllFactory),
	m_spLogger(spLogger)
{

}


MsvDllModuleAdapter::~MsvDllModuleAdapter()
{
	Stop();
	Uninitialize();
}


/********************************************************************************************************************************
*															IMsvModule public methods
********************************************************************************************************************************/


MsvErrorCode MsvDllModuleAdapter::Initialize()
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	MSV_LOG_INFO(m_spLogger, "Initializing DLL module {}.", m_moduleId);

	if (Initialized())
	{
		MSV_LOG_INFO(m_spLogger, "DLL Module {} has been already initialized.", m_moduleId);
		return MSV_ALREADY_INITIALIZED_INFO;
	}

	MsvErrorCode errorCode = m_spDllFactory->GetDllObject<IMsvDllModule>(m_moduleId.c_str(), m_spModule);
	if (MSV_FAILED(errorCode))
	{
		MSV_LOG_ERROR(m_spLogger, "Get DLL module {} failed with error: {0:x}", m_moduleId, errorCode);
		return errorCode;
	}

	if (!m_spModule)
	{
		MSV_LOG_ERROR(m_spLogger, "Loadde DLL module {} is empty - failed with error: {0:x}", m_moduleId, MSV_INVALID_DATA_ERROR);
		return MSV_INVALID_DATA_ERROR;
	}

	m_spModule->SetDllFactory(m_spDllFactory);
	
	if (MSV_FAILED(errorCode = m_spModule->Initialize()))
	{
		MSV_LOG_ERROR(m_spLogger, "Initialize DLL module {} failed with error: {0:x}", m_moduleId, errorCode);
		m_spModule.reset();
	}

	return errorCode;
}

MsvErrorCode MsvDllModuleAdapter::Uninitialize()
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	MSV_LOG_INFO(m_spLogger, "Uninitializing DLL module {}.", m_moduleId);

	if (!Initialized())
	{
		MSV_LOG_INFO(m_spLogger, "DLL module {} has not been initialized.", m_moduleId);
		return MSV_NOT_INITIALIZED_INFO;
	}

	MsvErrorCode errorCode = m_spModule->Uninitialize();

	if (MSV_FAILED(errorCode))
	{
		MSV_LOG_ERROR(m_spLogger, "Uninitialize DLL module {} failed with error: {0:x}", m_moduleId, errorCode);
	}

	m_spModule.reset();

	return errorCode;
}

bool MsvDllModuleAdapter::Initialized() const
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	return m_spModule && m_spModule->Initialized();
}

MsvErrorCode MsvDllModuleAdapter::Start()
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	MSV_LOG_INFO(m_spLogger, "Starting DLL module {}.", m_moduleId);

	if (!Initialized())
	{
		MSV_LOG_ERROR(m_spLogger, "DLL module {} has not been initialized - failed with error: {0:x}", m_moduleId, MSV_NOT_INITIALIZED_ERROR);
		return MSV_NOT_INITIALIZED_ERROR;
	}

	MsvErrorCode errorCode = m_spModule->Start();

	if (MSV_FAILED(errorCode))
	{
		MSV_LOG_ERROR(m_spLogger, "Start DLL module {} failed with error: {0:x}", m_moduleId, errorCode);
	}

	return errorCode;
}

MsvErrorCode MsvDllModuleAdapter::Stop()
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	MSV_LOG_INFO(m_spLogger, "Stopping DLL module {}.", m_moduleId);

	if (!Running())
	{
		MSV_LOG_INFO(m_spLogger, "DLL module {} has not been started.", m_moduleId);
		return MSV_NOT_RUNNING_INFO;
	}

	MsvErrorCode errorCode = m_spModule->Stop();

	if (MSV_FAILED(errorCode))
	{
		MSV_LOG_ERROR(m_spLogger, "Stop DLL module {} failed with error: {0:x}", m_moduleId, errorCode);
	}

	return errorCode;
}

bool MsvDllModuleAdapter::Running() const
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	return m_spModule && m_spModule->Running();
}


/** @} */	//End of group MMODULE.
