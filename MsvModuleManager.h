/**************************************************************************************************//**
* @addtogroup	MMODULE
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Module Manager Implementation
* @details		Contains implementation @ref MsvModuleManager of @ref IMsvModuleManager interface.
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


#ifndef MARSTECH_MODULEMANAGER_H
#define MARSTECH_MODULEMANAGER_H


#include "IMsvModuleManager.h"
#include "IMsvModuleConfigurator.h"

#include "mlogging/mlogging.h"

MSV_DISABLE_ALL_WARNINGS

#include <map>
#include <mutex>

MSV_ENABLE_WARNINGS


/**************************************************************************************************//**
* @brief		MarsTech Module Manager Implementation.
* @details	Module manager implementation which can manage all modules.
******************************************************************************************************/
class MsvModuleManager:
	public IMsvModuleManager
{
public:
	/**************************************************************************************************//**
	* @brief			Constructor.
	* @param[in]	spLogger						Shared pointer to logger for logging.
	******************************************************************************************************/
	MsvModuleManager(std::shared_ptr<MsvLogger> spLogger);

	/**************************************************************************************************//**
	* @brief		Virtual destructor.
	******************************************************************************************************/
	virtual ~MsvModuleManager();

	/*-----------------------------------------------------------------------------------------------------
	**											IMsvModule public methods
	**---------------------------------------------------------------------------------------------------*/
public:
	/**************************************************************************************************//**
	* @copydoc	IMsvModule::Initialize()
	* @retval	MSV_ALREADY_INITIALIZED_INFO	When module manager has been already initialized (this is info, not error).
	******************************************************************************************************/
	virtual MsvErrorCode Initialize() override;

	/**************************************************************************************************//**
	* @copydoc	IMsvModule::Uninitialize()
	* @retval	MSV_NOT_INITIALIZED_INFO		When module manager has not been initialized (this is info, not error).
	* @retval	MSV_STILL_RUNNING_ERROR			When module manager is still running. Must be stopped before.
	******************************************************************************************************/
	virtual MsvErrorCode Uninitialize() override;

	/**************************************************************************************************//**
	* @copydoc IMsvModule::Initialized() const
	******************************************************************************************************/
	virtual bool Initialized() const override;

	/**************************************************************************************************//**
	* @copydoc	IMsvModule::Start()
	* @retval	MSV_NOT_INITIALIZED_ERROR		When module manager has not been initialized before. Must be initialized before.
	* @retval	MSV_ALREADY_RUNNING_INFO		When module manager is already running (this is info, not error).
	******************************************************************************************************/
	virtual MsvErrorCode Start() override;

	/**************************************************************************************************//**
	* @copydoc	IMsvModule::Stop()
	* @retval	MSV_NOT_RUNNING_INFO				When module manager is not running (this is info, not error).
	******************************************************************************************************/
	virtual MsvErrorCode Stop() override;

	/**************************************************************************************************//**
	* @copydoc IMsvModule::Running() const
	******************************************************************************************************/
	virtual bool Running() const override;

	/*-----------------------------------------------------------------------------------------------------
	**											IMsvModuleManager public methods
	**---------------------------------------------------------------------------------------------------*/
public:
	/**************************************************************************************************//**
	* @copydoc IMsvModuleManager::AddModule(int32_t moduleId, std::shared_ptr<IMsvModule> spModule, std::shared_ptr<IMsvModuleConfigurator> spModuleConfigurator)
	******************************************************************************************************/
	virtual MsvErrorCode AddModule(int32_t moduleId, std::shared_ptr<IMsvModule> spModule, std::shared_ptr<IMsvModuleConfigurator> spModuleConfigurator) override;

protected:
	/**************************************************************************************************//**
	* @brief		Module manager mutex.
	* @details	Locks this object for thread safety access.
	******************************************************************************************************/
	mutable std::recursive_mutex m_lock;

	/**************************************************************************************************//**
	* @brief		Initialize flag.
	* @details	Flag if module manager is initialized (true) or not (false).
	* @see		Initialize
	* @see		Uninitialize
	* @see		Initialized
	******************************************************************************************************/
	bool m_initialized;

	/**************************************************************************************************//**
	* @brief		Registered modules.
	* @details	Map of registered modules to module manager. These modules are managed by module manager.
	* @see		AddModule
	******************************************************************************************************/
	std::map<int32_t, std::pair<std::shared_ptr<IMsvModuleConfigurator>, std::shared_ptr<IMsvModule>>> m_modules;

	/**************************************************************************************************//**
	* @brief		Logger.
	* @details	Shared pointer to logger for logging.
	******************************************************************************************************/
	std::shared_ptr<MsvLogger> m_spLogger;

	/**************************************************************************************************//**
	* @brief		Running flag.
	* @details	Flag if module manager is running (true) or not (false).
	* @see		Start
	* @see		Stop
	* @see		Running
	******************************************************************************************************/
	bool m_running;
};


#endif // !MARSTECH_MODULEMANAGER_H

/** @} */	//End of group MMODULE.
