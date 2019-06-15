/**************************************************************************************************//**
* @addtogroup	MMODULE
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech DLL Module Adapter Implementation
* @details		Contains implementation @ref MsvDllModuleAdapter of @ref IMsvModule interface.
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


#ifndef MARSTECH_DLLMODULEADAPTER_H
#define MARSTECH_DLLMODULEADAPTER_H


#include "IMsvDllModule.h"

#include "mlogging/mlogging.h"


/**************************************************************************************************//**
* @brief		MarsTech DLL Module Adapter.
* @details	DLL module adapter which loads, initializes, starts, stops and uninitializes module
*				in dynamic/shared library.
* @note		This class is usefull for modules stored in dynamic/shared libraries.
******************************************************************************************************/
class MsvDllModuleAdapter:
	public IMsvModule
{
public:
	/**************************************************************************************************//**
	* @brief			Constructor.
	* @param[in]	moduleId				DLL module ID (ID to get module from DLL factory).
	* @param[in]	spDllFactory		Shared pointer to DLL factory.
	* @param[in]	spLogger				Shared pointer to logger for logging.
	******************************************************************************************************/
	MsvDllModuleAdapter(const char* moduleId, std::shared_ptr<IMsvDllFactory> spDllFactory, std::shared_ptr<MsvLogger> spLogger = nullptr);

	/**************************************************************************************************//**
	* @brief		Virtual destructor.
	******************************************************************************************************/
	virtual ~MsvDllModuleAdapter();

	/*-----------------------------------------------------------------------------------------------------
	**											IMsvModule public methods
	**---------------------------------------------------------------------------------------------------*/
public:
	/**************************************************************************************************//**
	* @copydoc IMsvModule::Initialize()
	******************************************************************************************************/
	virtual MsvErrorCode Initialize() override;

	/**************************************************************************************************//**
	* @copydoc IMsvModule::Uninitialize()
	******************************************************************************************************/
	virtual MsvErrorCode Uninitialize() override;

	/**************************************************************************************************//**
	* @copydoc IMsvModule::Initialized() const
	******************************************************************************************************/
	virtual bool Initialized() const override;

	/**************************************************************************************************//**
	* @copydoc IMsvModule::Start()
	******************************************************************************************************/
	virtual MsvErrorCode Start() override;

	/**************************************************************************************************//**
	* @copydoc IMsvModule::Stop()
	******************************************************************************************************/
	virtual MsvErrorCode Stop() override;

	/**************************************************************************************************//**
	* @copydoc IMsvModule::Running() const
	******************************************************************************************************/
	virtual bool Running() const override;

protected:
	/**************************************************************************************************//**
	* @brief		Module manager mutex.
	* @details	Locks this object for thread safety access.
	******************************************************************************************************/
	mutable std::recursive_mutex m_lock;

	/**************************************************************************************************//**
	* @brief			Module ID.
	* @details		DLL module ID (ID to get module from DLL factory).
	******************************************************************************************************/
	std::string m_moduleId;

	/**************************************************************************************************//**
	* @brief			DLL factory.
	* @details		DLL factory used for loading DLLs and theirs objects. Used to get DLL module.
	******************************************************************************************************/
	std::shared_ptr<IMsvDllFactory> m_spDllFactory;

	/**************************************************************************************************//**
	* @brief			DLL module.
	* @details		Real DLL module loaded from DLL.
	******************************************************************************************************/
	std::shared_ptr<IMsvDllModule> m_spModule;

	/**************************************************************************************************//**
	* @brief		Logger.
	* @details	Shared pointer to logger for logging.
	******************************************************************************************************/
	std::shared_ptr<MsvLogger> m_spLogger;
};


#endif // !MARSTECH_DLLMODULEADAPTER_H

/** @} */	//End of group MMODULE.
