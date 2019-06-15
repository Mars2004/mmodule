# MarsTech Module Library
 - [Installation](#installation)
	 - [Dependencies](#dependencies)
	 - [Configuration](#configuration)
 - [Module Manager](#module-manager)
 - [MarsTech Module](#marstech-module)
 - [MarsTech DLL Module Adapter](#marstech-dll-module-adapter)
 - [MarsTech Module Configurator](#marstech-module-configurator)
 - [Usage Example](#usage-example)
 - [Source Code Documentation](#source-code-documentation)
 - [License](#license)

MarsTech module library contains module interface, DLL module adapter and module manager which manage all created modules.

## Installation
Download all [MarsTech](https://github.com/Mars2004) dependencies and put them to a same directory. Create a new subdirectory "3rdParty" and put there "3rdParty" dependencies ([inih](https://github.com/jtilly/inih), [SQLite3](https://www.sqlite.org/index.html), [spdlog](https://github.com/gabime/spdlog)).

### Dependencies

 - [MarsTech Headers](https://github.com/Mars2004/mheaders)
 - [MarsTech Dependency Injection](https://github.com/Mars2004/mdi)
 - [MarsTech Error Handling](https://github.com/Mars2004/merror)
 - [MarsTech Logging](https://github.com/Mars2004/mlogging)
 - [MarsTech Config](https://github.com/Mars2004/mconfig)
 - [MarsTech Threading](https://github.com/Mars2004/mthreading)
 - [MarsTech Dll Factory](https://github.com/Mars2004/mdllfactory)
 - [spdlog](https://github.com/gabime/spdlog)
 - [inih](https://github.com/jtilly/inih)
 - [SQLite3](https://www.sqlite.org/index.html)

### Configuration
No build configuration is needed - just build whole solution.

## Module Manager
Module manager manages all created modules. It can initialize, uninitialize, start and stop modules.

**Example:**
~~~cpp
#include "mmodule/MsvModuleManager.h"

//module IDs (should be defined in separate header)
enum class MsvModuleId: int32_t
{
	MSV_EXAMPLE_STATIC_MODULE_1				= 0,
	MSV_EXAMPLE_STATIC_MODULE_2,
	MSV_EXAMPLE_DYNAMIC_MODULE_1,
	MSV_EXAMPLE_DYNAMIC_MODULE_2
};

std::shared_ptr<IMsvModuleManager> spModuleManager(new MsvModuleManager());

//add modules to module manager (example at https://github.com/Mars2004/msys/blob/master/Example/MsvMainInitializer.cpp)
//spModuleManager->AddModule(static_cast<int32_t>(MSV_EXAMPLE_STATIC_MODULE_1), spModule, spModuleConfigurator);

MsvErrorCode errorCode = spModuleManager->Initialize();
if (MSV_FAILED(errorCode))
{
	MSV_LOG_ERROR(m_spLogger, "Initialize module module manager failed with error: {0:x}", errorCode);
	return errorCode;
}

if (MSV_FAILED(errorCode = m_spModuleManager->Start()))
{
	MSV_LOG_ERROR(m_spLogger, "Start module manager failed with error: {0:x}", errorCode);
	return errorCode;
}

//some stop request synchronization (e.g. MsvEvent)

if (MSV_FAILED(errorCode = spModuleManager->Stop()))
{
	MSV_LOG_ERROR(m_spLogger, "Stop module manager failed with error: {0:x}", errorCode);
}

if (MSV_FAILED(errorCode = m_spModuleManager->Uninitialize()))
{
	MSV_LOG_ERROR(m_spLogger, "Uninitialize module manager failed with error: {0:x}", errorCode);
}
~~~

## MarsTech Module
It is just interface. When you want to create module just inherit from IMsvModule interface and implement Initialize, Uninitialize, Start and Stop methods.

**Example:**
~~~cpp
#include "mmodule/IMsvModule.h"
#include "mheaders/MsvObject.h"
#include "merror/MsvErrorCodes.h"

class MyModule:
	public MsvObject<IMsvModule>
{
	MyModule():
		MsvObject<IMsvModule>()
	{
	}
	
	virtual MsvErrorCode Initialize() override
	{
		return MSV_SUCCESS;
	}

	virtual MsvErrorCode Uninitialize() override
	{
		return MSV_SUCCESS;
	}

	virtual MsvErrorCode Start() override
	{
		return MSV_SUCCESS;
	}

	virtual MsvErrorCode Stop() override
	{
		return MSV_SUCCESS;
	}
};
~~~

## MarsTech DLL Module Adapter
There is also implementation for modules in DLLs. These DLLs must implement GetDllObject function to be able to load by [MarsTech Dll Factory](https://github.com/Mars2004/mdllfactory).

**Example:**
~~~cpp
#include "mmodule/MsvDllModuleAdapter.h"

//example from (https://github.com/Mars2004/msys/blob/master/modules/MsvModules.cpp)
std::shared_ptr<IMsvModule> spDllModule(new MsvDllModuleAdapter(moduleId, spDllFactory, spLogger));
~~~

## MarsTech Module Configurator
Module manager needs to know if modules are installed and enabled. There is module configurator which usese [MarsTech Active Config](https://github.com/Mars2004/mconfig) to check if each module is installed and enabled.
It is possible to inherit from MsvModuleConfigurator and implement more configuration get and set methods.

**Example:**
~~~cpp
#include "mmodule/MsvModuleConfigurator.h"

//example from usage example at https://github.com/Mars2004/msys/blob/master/Example/MsvMainInitializer.cpp
std::shared_ptr<IMsvModuleConfigurator> spModuleConfigurator(new MsvModuleConfigurator(spActiveCfg, static_cast<int32_t>(enabledCfgId), static_cast<int32_t>(installedCfgId)));
~~~

## Usage Example
There is also an [usage example](https://github.com/Mars2004/msys/tree/master/Example) which uses the most of [MarsTech](https://github.com/Mars2004) projects and libraries.
Its source codes and readme can be found at:
 - [https://github.com/Mars2004/msys/tree/master/Example](https://github.com/Mars2004/msys/tree/master/Example)

## Source Code Documentation
You can find generated source code documentation at [https://www.marstech.cz/projects/mmodule/1.0.1/doc](https://www.marstech.cz/projects/mmodule/1.0.1/doc).

## License
This project is released under GNU General Public License version 3. If you can not or do not want to accept GNU GPLv3 license and you would like to use this project under another license, please contact me on [info@marstech.cz](mailto:info@marstech.cz) or visit [www.marstech.cz](https://www.marstech.cz/en).
