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

#include "drv_nvm_file.h"

#include <iostream>

/******************************************************************************
* PRIVATE TYPE DEFINITION
******************************************************************************/

/******************************************************************************
* PRIVATE DEFINES
******************************************************************************/

/******************************************************************************
* PRIVATE VARIABLES
******************************************************************************/

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

static void     DrvNvmInit  (void);
static uint32_t DrvNvmRead  (uint32_t start, uint8_t *buffer, uint32_t size);
static uint32_t DrvNvmWrite (uint32_t start, uint8_t *buffer, uint32_t size);

/******************************************************************************
* PUBLIC VARIABLE
******************************************************************************/

const CO_IF_NVM_DRV Linux_File_NvmDriver = {
    DrvNvmInit,
    DrvNvmRead,
    DrvNvmWrite
};

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

static void DrvNvmInit(void)
{
    std::cout << __func__ << " not implemented" << std::endl;
}

static uint32_t DrvNvmRead(uint32_t start, uint8_t *buffer, uint32_t size)
{
    std::cout << __func__ << " not implemented" << std::endl;

    return (0u);
}

static uint32_t DrvNvmWrite(uint32_t start, uint8_t *buffer, uint32_t size)
{
    std::cout << __func__ << " not implemented" << std::endl;

    return (0u);
}
