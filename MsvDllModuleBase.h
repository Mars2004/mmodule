/**************************************************************************************************//**
* @addtogroup	MMODULE
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech DLL Module Base Implementation
* @details		Contains implementation @ref MsvDllModuleBase of @ref IMsvDllModule interface.
* @author		Martin Svoboda
* @date			05.05.2019
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


#ifndef MARSTECH_DLLMODULEBASE_H
#define MARSTECH_DLLMODULEBASE_H


#include "IMsvDllModule.h"

#include "msys/msys/MsvSysDll_Interface.h"


/**************************************************************************************************//**
* @brief		MarsTech DLL Module Base.
* @details	Dll module base which implements @ref SetDllFactory, @ref Initialized and @ref Running.
*				You can inherit from it or implement your own methods in your IMsvDllModule implementation.
* @note		This iplementation is in header file only -> it should be possible to include this header
*				file to your project without linking this library.
******************************************************************************************************/
class MsvDllModuleBase:
	public IMsvDllModule
{
public:
	/**************************************************************************************************//**
	* @brief		Constructor.
	******************************************************************************************************/
	MsvDllModuleBase():
		m_initialized(false),
		m_running(false)
	{

	}

	/**************************************************************************************************//**
	* @brief		Virtual destructor.
	******************************************************************************************************/
	virtual ~MsvDllModuleBase() {}

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

	/*-----------------------------------------------------------------------------------------------------
	**											IMsvDllModule public methods
	**---------------------------------------------------------------------------------------------------*/
public:
	/**************************************************************************************************//**
	* @copydoc IMsvDllModule::SetDllFactory(std::shared_ptr<IMsvDllFactory> spDllFactory)
	******************************************************************************************************/
	virtual void SetDllFactory(std::shared_ptr<IMsvDllFactory> spDllFactory) override
	{
		m_spDllFactory = spDllFactory;
	}

protected:
	/**************************************************************************************************//**
	* @brief			Initialize DLL Module Base.
	* @details		Initializes DLL module base - loads MarsTech C++ SYS library and gets logger.
	* @param[in]	loggerName				Name of logger used for logging.
	* @retval		other_error_code		When failed.
	* @retval		MSV_SUCCESS				On success.
	* @warning		It should be called in initialize method of child. Ohter way @ref m_spSys and @ref m_spLogger
	*					members are not initialized (they are empty).
	******************************************************************************************************/
	virtual MsvErrorCode InitializeDllModuleBase(const char* loggerName)
	{		
		MsvErrorCode errorCode = m_spDllFactory->GetDllObject<IMsvSys_Last>(MSV_SYS_OBJECT_ID_LAST, m_spSys);
		MSV_RETURN_FAILED(errorCode);

		std::shared_ptr<IMsvLogging> spLogging;
		MSV_RETURN_FAILED(m_spSys->GetMsvLogging(spLogging));
		return spLogging->GetLogger(m_spLogger, loggerName);
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
	* @brief			DLL factory.
	* @details		DLL factory used for loading DLLs and theirs objects.
	******************************************************************************************************/
	std::shared_ptr<IMsvDllFactory> m_spDllFactory;

	/**************************************************************************************************//**
	* @brief		Logger.
	* @details	Shared pointer to logger for logging.
	******************************************************************************************************/
	std::shared_ptr<MsvLogger> m_spLogger;

	/**************************************************************************************************//**
	* @brief		MarsTech C++ SYS.
	* @details	Shared pointer to MarsTech C++ SYS.
	******************************************************************************************************/
	std::shared_ptr<IMsvSys_Last> m_spSys;
};


#endif // !MARSTECH_DLLMODULEBASE_H

/** @} */	//End of group MMODULE.
