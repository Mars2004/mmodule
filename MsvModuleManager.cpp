/**************************************************************************************************//**
* @addtogroup	MMODULE
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Module Manager Implementation
* @details		Contains implementation of @ref MsvModuleManager.
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


#include "MsvModuleManager.h"

#include "mlogging/mlogging.h"
#include "merror/MsvErrorCodes.h"


/********************************************************************************************************************************
*															Constructors and destructors
********************************************************************************************************************************/


MsvModuleManager::MsvModuleManager(std::shared_ptr<MsvLogger> spLogger):
	m_initialized(false),
	m_spLogger(spLogger),
	m_running(false)
{

}


MsvModuleManager::~MsvModuleManager()
{
	Stop();
	Uninitialize();
}


/********************************************************************************************************************************
*															IMsvModule public methods
********************************************************************************************************************************/


MsvErrorCode MsvModuleManager::Initialize()
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	MSV_LOG_INFO(m_spLogger, "Initializing module manager.");

	if (Initialized())
	{
		MSV_LOG_INFO(m_spLogger, "Module manager has been already initialized.");
		return MSV_ALREADY_INITIALIZED_INFO;
	}

	MsvErrorCode errorCode = MSV_SUCCESS;

	//initialize all modules
	std::map<int32_t, std::pair<std::shared_ptr<IMsvModuleConfigurator>, std::shared_ptr<IMsvModule>>>::iterator endIt = m_modules.end();
	for (std::map<int32_t, std::pair<std::shared_ptr<IMsvModuleConfigurator>, std::shared_ptr<IMsvModule>>>::iterator it = m_modules.begin(); it != endIt; ++it)
	{
		bool installed = false;
		bool enabled = false;
		if (MSV_FAILED(errorCode = it->second.first->IsInstalled(installed)) || MSV_FAILED(errorCode = it->second.first->IsEnabled(enabled)))
		{
			//get installed or enabled flag failed -> error
			MSV_LOG_ERROR(m_spLogger, "Get installed or enabled flag of module {} failed with error: {0:x}", it->first, errorCode);
			break;
		}
		else if (!installed || !enabled)
		{
			//module is not installed or enabled -> do not initialize it -> continue
			MSV_LOG_INFO(m_spLogger, "Module {} is not installed or enabled (installed: {}, enabled: {}).", it->first, installed, enabled);
			continue;
		}

		if (MSV_FAILED(errorCode = it->second.second->Initialize()))
		{
			//initialize module failed
			MSV_LOG_ERROR(m_spLogger, "Initialize module {} failed with error: {0:x}", it->first, errorCode);
			break;
		}
	}

	//check if initialize modules succeeded
	if (MSV_FAILED(errorCode))
	{
		//initialize failed (uninitialize all initialized modules)
		for (std::map<int32_t, std::pair<std::shared_ptr<IMsvModuleConfigurator>, std::shared_ptr<IMsvModule>>>::iterator it = m_modules.begin(); it != endIt; ++it)
		{
			if (it->second.second->Initialized())
			{
				//module is initialized -> uninitialize it
				MsvErrorCode uninitializeErrorCode = it->second.second->Uninitialize();
				if (MSV_FAILED(uninitializeErrorCode))
				{
					//uninitialize module failed -> just log and continue
					MSV_LOG_ERROR(m_spLogger, "Uninitilize module {} failed with error: {0:x}", it->first, uninitializeErrorCode);
				}
			}
		}

		//return error code received from initialize method
		return errorCode;
	}

	//all modules has been successfully initialized -> set initialized flag
	m_initialized = true;

	return errorCode;
}

MsvErrorCode MsvModuleManager::Uninitialize()
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	MSV_LOG_INFO(m_spLogger, "Uninitializing module manager.");

	if (Running())
	{
		MSV_LOG_ERROR(m_spLogger, "Module manager still running - can not be uninitialized.");
		return MSV_STILL_RUNNING_ERROR;
	}

	if (!Initialized())
	{
		MSV_LOG_INFO(m_spLogger, "Module manager has not been initialized.");
		return MSV_NOT_INITIALIZED_INFO;
	}

	MsvErrorCode errorCode = MSV_SUCCESS;

	//initialize all modules
	std::map<int32_t, std::pair<std::shared_ptr<IMsvModuleConfigurator>, std::shared_ptr<IMsvModule>>>::iterator endIt = m_modules.end();
	for (std::map<int32_t, std::pair<std::shared_ptr<IMsvModuleConfigurator>, std::shared_ptr<IMsvModule>>>::iterator it = m_modules.begin(); it != endIt; ++it)
	{
		if (it->second.second->Initialized())
		{
			MsvErrorCode unitializeErrorCode = it->second.second->Uninitialize();
			if (MSV_FAILED(unitializeErrorCode))
			{
				//initialize module failed
				MSV_LOG_ERROR(m_spLogger, "Uninitialize module {} failed with error: {0:x}", it->first, unitializeErrorCode);
				errorCode = unitializeErrorCode;
			}
		}
	}

	if (MSV_FAILED(errorCode))
	{
		//at least one module unitialize failed (it is error code of last failed module)
		return errorCode;
	}
	
	//all modules has been successfully uninitialized -> set initialized flag
	m_initialized = false;

	return errorCode;
}

bool MsvModuleManager::Initialized() const
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	return m_initialized;
}

MsvErrorCode MsvModuleManager::Start()
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	MSV_LOG_INFO(m_spLogger, "Starting module manager.");

	if (!Initialized())
	{
		MSV_LOG_ERROR(m_spLogger, "Module manager has not been initialized - failed with error: {0:x}", MSV_NOT_INITIALIZED_ERROR);
		return MSV_NOT_INITIALIZED_ERROR;
	}

	if (Running())
	{
		MSV_LOG_INFO(m_spLogger, "Module manager has been already started.");
		return MSV_ALREADY_RUNNING_INFO;
	}

	MsvErrorCode errorCode = MSV_SUCCESS;

	//start all modules
	std::map<int32_t, std::pair<std::shared_ptr<IMsvModuleConfigurator>, std::shared_ptr<IMsvModule>>>::iterator endIt = m_modules.end();
	for (std::map<int32_t, std::pair<std::shared_ptr<IMsvModuleConfigurator>, std::shared_ptr<IMsvModule>>>::iterator it = m_modules.begin(); it != endIt; ++it)
	{
		if (!it->second.second->Initialized())
		{
			//module is not initialized (probably not installed or not enabled, or failed to intialize) -> can not be started -> continue
			MSV_LOG_INFO(m_spLogger, "Module {} is not initialized - skipping.", it->first);
			continue;
		}

		if (MSV_FAILED(errorCode = it->second.second->Start()))
		{
			//start module failed
			MSV_LOG_ERROR(m_spLogger, "Start module {} failed with error: {0:x}", it->first, errorCode);
			break;
		}
	}

	//check if start modules succeeded
	if (MSV_FAILED(errorCode))
	{
		//start failed (stop all started modules)
		for (std::map<int32_t, std::pair<std::shared_ptr<IMsvModuleConfigurator>, std::shared_ptr<IMsvModule>>>::iterator it = m_modules.begin(); it != endIt; ++it)
		{
			if (it->second.second->Running())
			{
				//module is running -> stop it
				MsvErrorCode stopErrorCode = it->second.second->Stop();
				if (MSV_FAILED(stopErrorCode))
				{
					//stop module failed -> just log and continue
					MSV_LOG_ERROR(m_spLogger, "Stop module {} failed with error: {0:x}", it->first, stopErrorCode);
				}
			}
		}

		//return error code received from start method
		return errorCode;
	}

	//all modules has been successfully started -> set running flag
	m_running = true;

	return errorCode;
}

MsvErrorCode MsvModuleManager::Stop()
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	MSV_LOG_INFO(m_spLogger, "Stopping module manager.");

	if (!Running())
	{
		MSV_LOG_INFO(m_spLogger, "Module manager has not been started.");
		return MSV_NOT_RUNNING_INFO;
	}

	MsvErrorCode errorCode = MSV_SUCCESS;

	//stop all modules
	std::map<int32_t, std::pair<std::shared_ptr<IMsvModuleConfigurator>, std::shared_ptr<IMsvModule>>>::iterator endIt = m_modules.end();
	for (std::map<int32_t, std::pair<std::shared_ptr<IMsvModuleConfigurator>, std::shared_ptr<IMsvModule>>>::iterator it = m_modules.begin(); it != endIt; ++it)
	{
		if (it->second.second->Running())
		{
			MsvErrorCode stopErrorCode = it->second.second->Stop();
			if (MSV_FAILED(stopErrorCode))
			{
				//stop module failed
				MSV_LOG_ERROR(m_spLogger, "Stop module {} failed with error: {0:x}", it->first, stopErrorCode);
				errorCode = stopErrorCode;
			}
		}
	}

	if (MSV_FAILED(errorCode))
	{
		//at least one module stop failed (it is error code of last failed module)
		return errorCode;
	}

	//all modules has been successfully stopped -> set running flag
	m_running = false;

	return errorCode;
}

bool MsvModuleManager::Running() const
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	return m_running;
}


/********************************************************************************************************************************
*															IMsvModuleManager public methods
********************************************************************************************************************************/


MsvErrorCode MsvModuleManager::AddModule(int32_t moduleId, std::shared_ptr<IMsvModule> spModule, std::shared_ptr<IMsvModuleConfigurator> spModuleConfigurator)
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);

	//check if module and its configurator are valid
	if (!spModule || !spModuleConfigurator)
	{
		//module or its configurator is empty -> invalid data error
		MSV_LOG_ERROR(m_spLogger, "Invalid module {} or its configurator - failed with error: {0:x}", moduleId, MSV_INVALID_DATA_ERROR);
		return MSV_INVALID_DATA_ERROR;
	}

	//check if module is in the map and insert if not
	if (m_modules.find(moduleId) != m_modules.end())
	{
		//it is already in the map -> error
		MSV_LOG_ERROR(m_spLogger, "Module {} already exists - failed with error: {0:x}", moduleId, MSV_ALREADY_EXISTS_ERROR);
		return MSV_ALREADY_EXISTS_ERROR;
	}

	//moduleId is not in the map -> set module to right state
	MsvErrorCode errorCode = MSV_SUCCESS;

	bool installed = false;
	bool enabled = false;
	if (MSV_FAILED(errorCode = spModuleConfigurator->IsInstalled(installed)) || MSV_FAILED(errorCode = spModuleConfigurator->IsEnabled(enabled)))
	{
		//get installed or enabled flag failed -> error
		MSV_LOG_ERROR(m_spLogger, "Get installed or enabled flag of module {} failed with error: {0:x}", moduleId, errorCode);
		return errorCode;
	}
	else if (!installed || !enabled)
	{
		//module is not installed or enabled -> do not initialize it -> continue
		MSV_LOG_INFO(m_spLogger, "Module {} is not installed or enabled (installed: {}, enabled: {}).", moduleId, installed, enabled);
	}
	else
	{
		//module is installed and enabled -> check if module manager is initialized and initialize module if it is
		if (Initialized())
		{
			//module manager is initialized -> initialize module
			if (!spModule->Initialized())
			{				
				if (MSV_FAILED(errorCode = spModule->Initialize()))
				{
					MSV_LOG_ERROR(m_spLogger, "Initialize module {} failed with error: {0:x}", moduleId, errorCode);
					return errorCode;
				}
			}
		}

		//check if module manager is running and start module if it is
		if (Running())
		{
			//module manager is running -> start module
			if (!spModule->Initialized())
			{
				//module is not initialized -> can not be started -> error
				errorCode = MSV_NOT_INITIALIZED_ERROR;
				MSV_LOG_ERROR(m_spLogger, "Module {} is not initialized - start failed with error: {0:x}", moduleId, errorCode);
				return errorCode;
			}

			if (!spModule->Running())
			{
				if (MSV_FAILED(errorCode = spModule->Start()))
				{
					MSV_LOG_ERROR(m_spLogger, "Start module {} failed with error: {0:x}", moduleId, errorCode);

					MsvErrorCode unitializeErrorCode = spModule->Uninitialize();
					if (MSV_FAILED(unitializeErrorCode))
					{
						//initialize module failed
						MSV_LOG_ERROR(m_spLogger, "Uninitialize module {} failed with error: {0:x}", moduleId, unitializeErrorCode);
					}

					return errorCode;
				}
			}			
		}
	}
	
	//insert module to the mape
	m_modules[moduleId] = std::pair<std::shared_ptr<IMsvModuleConfigurator>, std::shared_ptr<IMsvModule>>(spModuleConfigurator, spModule);
	
	return MSV_SUCCESS;
}


/** @} */	//End of group MMODULE.
