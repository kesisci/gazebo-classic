/*
 * Copyright (C) 2017 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/
#ifndef GAZEBO_COMMON_FUELMODELDATABASE_HH_
#define GAZEBO_COMMON_FUELMODELDATABASE_HH_

#include <functional>
#include <map>
#include <memory>
#include <string>

#include "gazebo/util/system.hh"

namespace gazebo
{
  namespace common
  {
    /// \brief Forward declare private data class.
    class FuelModelDatabasePrivate;

    /// \addtogroup gazebo_common Common
    /// \{

    /// \class FuelModelDatabase FuelModelDatabase.hh common/common.hh
    /// \brief Connects to an Igniiton Fuel model database, and has utility
    /// functions to find models.
    class GZ_COMMON_VISIBLE FuelModelDatabase
    {
      /// \brief Constructor.
      /// \param[in] _server The server name.
      /// E.g.: https://api.ignitionfuel.org
      public: FuelModelDatabase(const std::string &_server);

      /// \brief Destructor
      public: virtual ~FuelModelDatabase();

      /// \brief Get the dictionary of all model names via a callback.
      ///
      /// This is a non-blocking function. Your callback will be executed from
      /// a separate thread.
      /// \param[in] _func Callback function that receives the list of models.
      /// The parameter of the callback is a map, where the key is the unique
      /// name (containing the full path in the server, owner and model name)
      /// and the value is the model name.
      /// E.g.: https://api.ignitionfuel.org/caguero/Beer -> Beer
      public: virtual void Models(
        std::function<void(const std::map<std::string, std::string> &)> &_func);

      /// \brief Get the dictionary of all model names.
      ///
      /// This is a blocking function.
      /// \return The list of models.
      /// The key of the returned map is the unique name (containing the full
      /// path in the server, owner and model name) and the value is the
      /// model name.
      /// E.g.: https://api.ignitionfuel.org/caguero/Beer -> Beer
      public: virtual std::map<std::string, std::string> Models();

      /// \brief Private data.
      private: std::unique_ptr<FuelModelDatabasePrivate> dataPtr;
    };
  }
}
#endif
