/**************************************************************************************************//**
* @defgroup		MMODULE MarsTech Module Library
* @brief			MarsTech Module Library
* @details		Contains implementation and all definitions of MarsTech Module library. Implements module
*					manager to manage modules.
* @copyright	GNU General Public License (GPLv3).
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Module Interface
* @details		Contains definition of @ref IMsvModule interface.
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


#ifndef MARSTECH_IMODULE_H
#define MARSTECH_IMODULE_H


#include "merror/MsvError.h"


/**************************************************************************************************//**
* @brief		MarsTech Module Interface.
* @details	Module interface representing module. Inherit from it and implement child as module which
*				can be managed by module manager.
******************************************************************************************************/
class IMsvModule
{
public:
	/**************************************************************************************************//**
	* @brief		Virtual destructor.
	******************************************************************************************************/
	virtual ~IMsvModule() {}

	/**************************************************************************************************//**
	* @brief			Initialize module.
	* @details		Initializes module and prepares it to start.
	* @retval		other_error_code					When failed.
	* @retval		MSV_SUCCESS							On success.
	******************************************************************************************************/
	virtual MsvErrorCode Initialize() = 0;

	/**************************************************************************************************//**
	* @brief			Uninitialize module.
	* @details		Uninitializes module and prepares it to release.
	* @retval		other_error_code					When failed.
	* @retval		MSV_SUCCESS							On success.
	******************************************************************************************************/
	virtual MsvErrorCode Uninitialize() = 0;

	/**************************************************************************************************//**
	* @brief			Initialize check.
	* @details		Returns flag if module is initialized (true) or not (false).
	* @retval		true		When initialized.
	* @retval		false		When not initialized.
	******************************************************************************************************/
	virtual bool Initialized() const = 0;

	/**************************************************************************************************//**
	* @brief			Start module.
	* @details		Starts module and executes it.
	* @retval		other_error_code					When failed.
	* @retval		MSV_SUCCESS							On success.
	******************************************************************************************************/
	virtual MsvErrorCode Start() = 0;

	/**************************************************************************************************//**
	* @brief			Stop module.
	* @details		Stops module and its execution.
	* @retval		other_error_code					When failed.
	* @retval		MSV_SUCCESS							On success.
	******************************************************************************************************/
	virtual MsvErrorCode Stop() = 0;

	/**************************************************************************************************//**
	* @brief			Running check.
	* @details		Returns flag if module is running (true) or not (false).
	* @retval		true		When running.
	* @retval		false		When not running.
	******************************************************************************************************/
	virtual bool Running() const = 0;
};


#endif // !MARSTECH_IMODULE_H

/** @} */	//End of group MMODULE.
