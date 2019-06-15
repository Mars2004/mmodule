/**************************************************************************************************//**
* @addtogroup	MMODULE
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech DLL Module Interface
* @details		Contains definition of @ref IMsvDllModule interface.
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


#ifndef MARSTECH_IDLLMODULE_H
#define MARSTECH_IDLLMODULE_H


#include "IMsvModule.h"

#include "mdllfactory/IMsvDllFactory.h"


/**************************************************************************************************//**
* @brief		MarsTech DLL Module Interface.
* @details	Module interface representing DLL module. Inherit from it and implement child as module which
*				can be managed by module manager and is loaded from DLL.
******************************************************************************************************/
class IMsvDllModule:
	public IMsvModule,
	public IMsvDllObject
{
public:
	/**************************************************************************************************//**
	* @brief		Virtual destructor.
	******************************************************************************************************/
	virtual ~IMsvDllModule() {}

	/**************************************************************************************************//**
	* @brief			Set DLL factory.
	* @details		Sets DLL factory to DLL module.
	* @param[in]	spDllFactory		DLL factory used for loading DLLs and theirs objects.
	******************************************************************************************************/
	virtual void SetDllFactory(std::shared_ptr<IMsvDllFactory> spDllFactory) = 0;
};


#endif // !MARSTECH_IDLLMODULE_H

/** @} */	//End of group MMODULE.
