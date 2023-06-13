#pragma once

#include "co_core.h"

/* Specify the EMCY-IDs for the application */
enum EMCY_CODES {
    APP_ERR_ID_EEPROM = 0,

    APP_ERR_ID_NUM            /* number of EMCY error codes in application */
};

extern struct CO_NODE_SPEC_T AppSpec;