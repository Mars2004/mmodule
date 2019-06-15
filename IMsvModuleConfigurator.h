/**************************************************************************************************//**
* @addtogroup	MMODULE
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Module Configurator Interface
* @details		Contains definition of @ref IMsvModuleConfigurator interface.
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


#ifndef MARSTECH_IMODULECONFIGURATOR_H
#define MARSTECH_IMODULECONFIGURATOR_H


#include "merror/MsvError.h"


/**************************************************************************************************//**
* @brief		MarsTech Module Configurator Interface.
* @details	Module configurator interface which can configure one module.
******************************************************************************************************/
class IMsvModuleConfigurator
{
public:
	/**************************************************************************************************//**
	* @brief		Virtual destructor.
	******************************************************************************************************/
	virtual ~IMsvModuleConfigurator() {}

	/**************************************************************************************************//**
	* @brief			Set enabled flag.
	* @details		Sets enabled flag to enabled (true) or disabled (false).
	* @param[in]	enabled					Flag if enabled (true) or not (false).
	* @retval		other_error_code		When failed (error code of active configuration).
	* @retval		MSV_SUCCESS				On success.
	******************************************************************************************************/
	virtual MsvErrorCode Enabled(bool enabled) = 0;

	/**************************************************************************************************//**
	* @brief			Get enabled flag.
	* @details		Gets enabled flag. It is enabled when true and disabled when false.
	* @param[out]	enabled					Flag if enabled (true) or not (false).
	* @retval		other_error_code		When failed (error code of active configuration).
	* @retval		MSV_SUCCESS				On success.
	******************************************************************************************************/
	virtual MsvErrorCode IsEnabled(bool& enabled) const = 0;

	/**************************************************************************************************//**
	* @brief			Set installed flag.
	* @details		Sets installed flag to installed (true) or not installed (false).
	* @param[in]	installed				Flag if installed (true) or not (false).
	* @retval		other_error_code		When failed (error code of active configuration).
	* @retval		MSV_SUCCESS				On success.
	******************************************************************************************************/
	virtual MsvErrorCode Installed(bool installed) = 0;

	/**************************************************************************************************//**
	* @brief			Get installed flag.
	* @details		Gets installed flag. It is installed when true and not installed when false.
	* @param[out]	installed					Flag if installed (true) or not (false).
	* @retval		other_error_code		When failed (error code of active configuration).
	* @retval		MSV_SUCCESS				On success.
	******************************************************************************************************/
	virtual MsvErrorCode IsInstalled(bool& installed) const = 0;
};


#endif // !MARSTECH_IMODULECONFIGURATOR_H

/** @} */	//End of group MMODULE.