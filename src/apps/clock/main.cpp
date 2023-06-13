/******************************************************************************
   @author Sandro Gort

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
******************************************************************************/

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "app.h"

#include "drv_can_socketcan.h"

#include <string>
#include <cstring>
#include <vector>
#include <iostream>

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/


/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

int main(int argc, const char* argv[])                    /* main entry point for controller startup */
{
    std::string current_exec_name = argv[0]; // Name of the current exec program
    std::string arg1;

    if (argc == 2) {
        arg1 = argv[1];
    } else {
        std::cout << "Usage:" << std::endl;
        std::cout << argv[0] << " <can-interface-name>" << std::endl;
        return -1;
    }

    strcpy(CanInterfaceName, arg1.c_str());

    AppStart();                 /* ok, we are ready to start the application */
}

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/
