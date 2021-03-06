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
  \file tws/core/utils.cpp

  \brief Utility functions.

  \author Gilberto Ribeiro de Queiroz
 */

// TWS
#include "utils.hpp"
#include "../build_config.hpp"
//#include "../plugin.hpp"
#include "http_server_builder.hpp"
#include "service_operations_manager.hpp"

// Boost
#include <boost/filesystem.hpp>

// rapidJSON
#include <rapidjson/document.h>
#include <rapidjson/filestream.h>

void
tws::core::init_terralib_web_services()
{
  service_operations_manager::init();

  http_server_builder::instance();

  //tws::plugin::init_plugin_support();

  //tws::plugin::load_all(true);
}

void
tws::core::shutdown_terralib_web_services()
{
  //tws::plugin::unload_all();

  //tws::plugin::shutdown_plugin_support();
}

std::string
tws::core::find_in_app_path(const std::string& p)
{
// 1st: look for an environment variable defined by macro TWS_DIR_VAR_NAME
  const char* tws_env = getenv(TWS_DIR_VAR_NAME);

  if(tws_env != nullptr)
  {
    boost::filesystem::path tws_path = tws_env;

    boost::filesystem::path eval_path = tws_path / p;

    if(boost::filesystem::exists(eval_path))
      return eval_path.string();
  }

// 2nd: look in the neighborhood of the executable
  boost::filesystem::path tws_path = boost::filesystem::current_path();

  boost::filesystem::path eval_path = tws_path / p;

  if(boost::filesystem::exists(eval_path))
    return eval_path.string();

  tws_path /= "..";

  eval_path = tws_path / p;

  if(boost::filesystem::exists(eval_path))
    return eval_path.string();

// 3rd: look into the codebase path
  tws_path = TWS_CODEBASE_PATH;

  eval_path = tws_path / p;

  if(boost::filesystem::exists(eval_path))
    return eval_path.string();

// 4th: look into install prefix-path
  tws_path = TWS_INSTALL_PREFIX_PATH;

  eval_path = tws_path / p;

  if(boost::filesystem::exists(eval_path))
    return eval_path.string();

  return "";
}

rapidjson::Document* tws::core::open_json_file(const std::string &path)
{
  if(!boost::filesystem::is_regular(path))
  {
    boost::format err_msg("input file '%1%' doesn't exist.");

    throw tws::file_exists_error() << tws::error_description((err_msg % path).str());
  }

  FILE* pfile = fopen(path.c_str(), "r");

  if(pfile == nullptr)
  {
    boost::format err_msg("error opening input file '%1%'.");

    throw tws::file_open_error() << tws::error_description((err_msg % path).str());
  }

  try
  {
    rapidjson::FileStream istr(pfile);

    rapidjson::Document* doc = new rapidjson::Document();

    doc->ParseStream<0>(istr);

    if(doc->HasParseError())
    {
      boost::format err_msg("error parsing input file '%1%': %2%.");

      throw tws::parse_error() << tws::error_description((err_msg % path % doc->GetParseError()).str());
    }

    if(!doc->IsObject() || doc->IsNull())
    {
      boost::format err_msg("error parsing input file '%1%': unexpected file format.");

      throw tws::parse_error() << tws::error_description((err_msg % path).str());
    }

    fclose(pfile);

    return doc;
  }
  catch(...)
  {
    fclose(pfile);
    throw;
  }
}

