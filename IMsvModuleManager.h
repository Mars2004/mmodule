/**************************************************************************************************//**
* @addtogroup	MMODULE
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Module Manager Interface
* @details		Contains definition of @ref IMsvModuleManager interface.
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


#ifndef MARSTECH_IMODULEMANAGER_H
#define MARSTECH_IMODULEMANAGER_H


#include "IMsvModule.h"
#include "IMsvModuleConfigurator.h"

MSV_DISABLE_ALL_WARNINGS

#include <memory>

MSV_ENABLE_WARNINGS

/**************************************************************************************************//**
* @brief		MarsTech Module Manager Interface.
* @details	Module manager interface which can manage all modules.
* @note		Module manager can also be a module.
******************************************************************************************************/
class IMsvModuleManager:
	public IMsvModule
{
public:
	/**************************************************************************************************//**
	* @brief		Virtual destructor.
	******************************************************************************************************/
	virtual ~IMsvModuleManager() {}

	/**************************************************************************************************//**
	* @brief			Add module.
	* @details		Adds module to module manager. It will be managed by module manager.
	* @param[in]	moduleId							DLL module ID.
	* @param[in]	spModule							Shared pointer to module.
	* @param[in]	spModuleConfigurator			Shared pointer to module configurator.
	* @retval		MSV_INVALID_DATA_ERROR		When module or its configurator are not valid (empty shared_ptr).
	* @retval		MSV_ALREADY_EXISTS_ERROR	When module ID has been already inserted. Module is not inserted.
	* @retval		other_error_code				When failed (error code of any action with module or its configurator).
	* @retval		MSV_SUCCESS						On success.
	******************************************************************************************************/
	virtual MsvErrorCode AddModule(int32_t moduleId, std::shared_ptr<IMsvModule> spModule, std::shared_ptr<IMsvModuleConfigurator> spModuleConfigurator) = 0;
};


#endif // !MARSTECH_IMODULEMANAGER_H

/** @} */	//End of group MMODULE.
