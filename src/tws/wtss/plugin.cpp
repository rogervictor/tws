/*
  Copyright (C) 2014 National Institute For Space Research (INPE) - Brazil.
 
  This file is part of the TerraLib GeoWeb Services.
 
  TerraLib GeoWeb Services is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 3 as
  published by the Free Software Foundation.
 
  TerraLib GeoWeb Services is distributed  "AS-IS" in the hope that it will be useful,
  but WITHOUT ANY WARRANTY OF ANY KIND; without even the implied warranty
  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.
 
  You should have received a copy of the GNU Lesser General Public License along
  with TerraLib Web Services. See COPYING. If not, see <http://www.gnu.org/licenses/lgpl-3.0.html>.
 */

/*!
  \file tws/wtss/plugin.cpp

  \brief Add a plugin interface for dynamic loading of the Web Time Series Data Service module.

  \author Gilberto Ribeiro de Queiroz
 */

// TWS
#include "../core/service_operations_manager.hpp"
#include "wtss.hpp"

// TerraLib
#include <terralib/plugin/Plugin.h>

// STL
#include <memory>

class Plugin : public te::plugin::Plugin
{
  public:
  
    Plugin(const te::plugin::PluginInfo& pluginInfo)
      : te::plugin::Plugin(pluginInfo)
    {
    }
  
    ~Plugin()
    {
    }
  
    void startup()
    {
      if(m_initialized)
        return;

      tws::wtss::register_operations();
      
      tws::wtss::initialize_operations();
      
      m_initialized = true;
    }
  
    void shutdown()
    {
      if(!m_initialized)
        return;

      m_initialized = false;
    }
};

#define export_macro

PLUGIN_CALL_BACK_DECLARATION(export_macro)

PLUGIN_CALL_BACK_IMPL(Plugin)

