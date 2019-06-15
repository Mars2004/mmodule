/**************************************************************************************************//**
* @addtogroup	MMODULE
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Module Configurator Implementation
* @details		Contains implementation @ref MsvModuleConfigurator of @ref IMsvModuleConfigurator interface.
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


#ifndef MARSTECH_MODULECONFIGURATOR_H
#define MARSTECH_MODULECONFIGURATOR_H


#include "IMsvModuleConfigurator.h"

#include "mconfig/mactivecfg/IMsvActiveConfig.h"


/**************************************************************************************************//**
* @brief		MarsTech Module Configurator Implementation.
* @details	Module configurator implementation which can configure one module.
* @note		This iplementation is in header file only -> it should be possible to include this header
*				file to your project without linking this library.
******************************************************************************************************/
class MsvModuleConfigurator:
	public IMsvModuleConfigurator
{
public:
	/**************************************************************************************************//**
	* @brief			Constructor.
	* @param[in]	spActiveCfg				Shared pointer to active config.
	* @param[in]	enabledCfgId			Config ID for enabled flag.
	* @param[in]	installedCfgId			Config ID for installed flag.
	******************************************************************************************************/
	MsvModuleConfigurator(std::shared_ptr<IMsvActiveConfig> spActiveCfg, int32_t enabledCfgId, int32_t installedCfgId):
		m_enabledCfgId(enabledCfgId),
		m_installedCfgId(installedCfgId),
		m_spActiveCfg(spActiveCfg)
	{

	}

	/**************************************************************************************************//**
	* @brief		Virtual destructor.
	******************************************************************************************************/
	virtual ~MsvModuleConfigurator()
	{
		
	}

	/*-----------------------------------------------------------------------------------------------------
	**											IMsvModuleConfigurator public methods
	**---------------------------------------------------------------------------------------------------*/
public:
	/**************************************************************************************************//**
	* @copydoc IMsvModuleConfigurator::Enabled(bool enabled)
	******************************************************************************************************/
	virtual MsvErrorCode Enabled(bool enabled) override
	{
		//locking is not neccessary (should be done by active config)
		return m_spActiveCfg->SetValue(m_enabledCfgId, enabled);
	}

	/**************************************************************************************************//**
	* @copydoc IMsvModuleConfigurator::IsEnabled(bool& enabled) const
	******************************************************************************************************/
	virtual MsvErrorCode IsEnabled(bool& enabled) const override
	{
		//locking is not neccessary (should be done by active config)
		return m_spActiveCfg->GetValue(m_enabledCfgId, enabled);
	}

	/**************************************************************************************************//**
	* @copydoc IMsvModuleConfigurator::Installed(bool installed)
	******************************************************************************************************/
	virtual MsvErrorCode Installed(bool installed) override
	{
		//locking is not neccessary (should be done by active config)
		return m_spActiveCfg->SetValue(m_installedCfgId, installed);
	}

	/**************************************************************************************************//**
	* @copydoc IMsvModuleConfigurator::IsInstalled(bool& installed) const
	******************************************************************************************************/
	virtual MsvErrorCode IsInstalled(bool& installed) const override
	{
		//locking is not neccessary (should be done by active config)
		return m_spActiveCfg->GetValue(m_installedCfgId, installed);
	}

protected:
	/**************************************************************************************************//**
	* @brief			Config ID for enabled flag.
	* @details		It used to get/set enabled flag of module.
	******************************************************************************************************/
	int32_t m_enabledCfgId;

	/**************************************************************************************************//**
	* @brief			Config ID for installed flag.
	* @details		It used to get/set installed flag of module.
	******************************************************************************************************/
	int32_t m_installedCfgId;

	/**************************************************************************************************//**
	* @brief			Active config.
	* @details		It used to get/set module configuration. 
	******************************************************************************************************/
	std::shared_ptr<IMsvActiveConfig> m_spActiveCfg;
};


#endif // !MARSTECH_MODULECONFIGURATOR_H

/** @} */	//End of group MMODULE.