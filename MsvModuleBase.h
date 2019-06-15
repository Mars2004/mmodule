/**************************************************************************************************//**
* @addtogroup	MMODULE
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Module Base Implementation
* @details		Contains implementation @ref MsvModuleBase of @ref IMsvModule interface.
* @author		Martin Svoboda
* @date			26.05.2019
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


#ifndef MARSTECH_MODULEBASE_H
#define MARSTECH_MODULEBASE_H


#include "IMsvModule.h"
#include "mlogging/mlogging.h"

MSV_DISABLE_ALL_WARNINGS

#include <mutex>

MSV_ENABLE_WARNINGS


/**************************************************************************************************//**
* @brief		MarsTech Module Base.
* @details	Dll module base which implements  @ref Initialized and @ref Running.
*				You can inherit from it or implement your own methods in your IMsvModule implementation.
* @note		This iplementation is in header file only -> it should be possible to include this header
*				file to your project without linking this library.
******************************************************************************************************/
class MsvModuleBase:
	public IMsvModule
{
public:
	/**************************************************************************************************//**
	* @brief		Constructor.
	******************************************************************************************************/
	MsvModuleBase(std::shared_ptr<IMsvLoggerProvider> spLoggerProvider, const char* loggerName):
		m_initialized(false),
		m_running(false),
		m_spLogger(spLoggerProvider->GetLogger(loggerName))
	{
		
	}

	/**************************************************************************************************//**
	* @brief		Virtual destructor.
	******************************************************************************************************/
	virtual ~MsvModuleBase() {}

	/*-----------------------------------------------------------------------------------------------------
	**											IMsvModule public methods
	**---------------------------------------------------------------------------------------------------*/
public:
	/**************************************************************************************************//**
	* @copydoc IMsvModule::Initialized() const
	******************************************************************************************************/
	virtual bool Initialized() const override
	{
		std::lock_guard<std::recursive_mutex> lock(m_lock);

		return m_initialized;
	}

	/**************************************************************************************************//**
	* @copydoc IMsvModule::Running() const
	******************************************************************************************************/
	virtual bool Running() const override
	{
		std::lock_guard<std::recursive_mutex> lock(m_lock);

		return m_running;
	}

protected:
	/**************************************************************************************************//**
	* @brief		Module mutex.
	* @details	Locks this object for thread safety access.
	******************************************************************************************************/
	mutable std::recursive_mutex m_lock;

	/**************************************************************************************************//**
	* @brief		Initialize flag.
	* @details	Flag if module is initialized (true) or not (false).
	* @see		Initialize
	* @see		Uninitialize
	* @see		Initialized
	******************************************************************************************************/
	bool m_initialized;

	/**************************************************************************************************//**
	* @brief		Running flag.
	* @details	Flag if module is running (true) or not (false).
	* @see		Start
	* @see		Stop
	* @see		Running
	******************************************************************************************************/
	bool m_running;

	/**************************************************************************************************//**
	* @brief		Logger.
	* @details	Shared pointer to logger for logging.
	******************************************************************************************************/
	std::shared_ptr<MsvLogger> m_spLogger;
};


#endif // !MARSTECH_MODULEBASE_H

/** @} */	//End of group MMODULE.
