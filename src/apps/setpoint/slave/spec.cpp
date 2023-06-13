#include "spec.h"
#include "hw.h"

/* Define some default values for our CANopen node: */
#define APP_NODE_ID       2u                  /* CANopen node ID             */
#define APP_BAUDRATE      250000u             /* CAN baudrate                */
#define APP_TMR_N         16u                 /* Number of software timers   */
#define APP_TICKS_PER_SEC 20u                 /* Timer clock frequency in Hz */
#define APP_OBJ_N         128u                /* Object dictionary max size  */

/* allocate variables for dynamic runtime value in RAM */
static uint8_t  Obj1001_00_08 = 0;

uint32_t obj1005 = 0x80; // Enable sync consumer

static uint16_t Obj1017_00_10 = 0;

static uint32_t Obj2100_00_20 = 0;
static uint32_t Obj2200_00_20 = 0;

/* allocate variables for constant values in FLASH */
const  uint32_t Obj1000_00_20 = 0x00000000L;

const  uint32_t Obj1014_00_20 = 0x00000080L;

const  uint32_t Obj1018_01_20 = 0x00000000L;
const  uint32_t Obj1018_02_20 = 0x00000000L;
const  uint32_t Obj1018_03_20 = 0x00000000L;
const  uint32_t Obj1018_04_20 = 0x00000000L;

const  uint32_t Obj1200_01_20 = CO_COBID_SDO_REQUEST();
const  uint32_t Obj1200_02_20 = CO_COBID_SDO_RESPONSE();

const  uint32_t Obj1400_01_20 = 0x00000180;

const  uint32_t Obj1600_01_20 = CO_LINK(0x2100, 0x00, 32);

const  uint32_t Obj1800_01_20 = 0x40000181;

const  uint32_t Obj1A00_01_20 = CO_LINK(0x2200, 0x00, 32);

/* define the static object dictionary */
static struct CO_OBJ_T ClockOD[APP_OBJ_N] = {
    {CO_KEY(0x1000, 0, CO_OBJ_____R_), CO_TUNSIGNED32, (CO_DATA)(&Obj1000_00_20)},
    {CO_KEY(0x1001, 0, CO_OBJ_____R_), CO_TUNSIGNED8 , (CO_DATA)(&Obj1001_00_08)},

    {CO_KEY(0x1005, 0, CO_OBJ_____RW), CO_TSYNC_ID,    (CO_DATA)(&obj1005)},

    {CO_KEY(0x1014, 0, CO_OBJ__N__R_), CO_TEMCY_ID,    (CO_DATA)(&Obj1014_00_20)},
    {CO_KEY(0x1017, 0, CO_OBJ_____RW), CO_THB_PROD,    (CO_DATA)(&Obj1017_00_10)},

    {CO_KEY(0x1018, 0, CO_OBJ_D___R_), CO_TUNSIGNED8 , (CO_DATA)(4)             },
    {CO_KEY(0x1018, 1, CO_OBJ_____R_), CO_TUNSIGNED32, (CO_DATA)(&Obj1018_01_20)},
    {CO_KEY(0x1018, 2, CO_OBJ_____R_), CO_TUNSIGNED32, (CO_DATA)(&Obj1018_02_20)},
    {CO_KEY(0x1018, 3, CO_OBJ_____R_), CO_TUNSIGNED32, (CO_DATA)(&Obj1018_03_20)},
    {CO_KEY(0x1018, 4, CO_OBJ_____R_), CO_TUNSIGNED32, (CO_DATA)(&Obj1018_04_20)},

    {CO_KEY(0x1200, 0, CO_OBJ_D___R_), CO_TUNSIGNED8 , (CO_DATA)(2)             },
    {CO_KEY(0x1200, 1, CO_OBJ__N__R_), CO_TUNSIGNED32, (CO_DATA)(&Obj1200_01_20)},
    {CO_KEY(0x1200, 2, CO_OBJ__N__R_), CO_TUNSIGNED32, (CO_DATA)(&Obj1200_02_20)},

    {CO_KEY(0x1400, 0, CO_OBJ_D___R_), CO_TUNSIGNED8 , (CO_DATA)(2)             },
    {CO_KEY(0x1400, 1, CO_OBJ_____R_), CO_TUNSIGNED32, (CO_DATA)(&Obj1400_01_20)},
    {CO_KEY(0x1400, 2, CO_OBJ_D___R_), CO_TUNSIGNED8 , (CO_DATA)(1)             },

    {CO_KEY(0x1600, 0, CO_OBJ_D___R_), CO_TUNSIGNED8 , (CO_DATA)(1)             },
    {CO_KEY(0x1600, 1, CO_OBJ_____R_), CO_TUNSIGNED32, (CO_DATA)(&Obj1600_01_20)},    

    {CO_KEY(0x1800, 0, CO_OBJ_D___R_), CO_TUNSIGNED8 , (CO_DATA)(2)             },
    {CO_KEY(0x1800, 1, CO_OBJ_____R_), CO_TUNSIGNED32, (CO_DATA)(&Obj1800_01_20)},
    {CO_KEY(0x1800, 2, CO_OBJ_D___R_), CO_TUNSIGNED8 , (CO_DATA)(1)             },

    {CO_KEY(0x1A00, 0, CO_OBJ_D___R_), CO_TUNSIGNED8 , (CO_DATA)(1)             },
    {CO_KEY(0x1A00, 1, CO_OBJ_____R_), CO_TUNSIGNED32, (CO_DATA)(&Obj1A00_01_20)},

    {CO_KEY(0x2100, 0, CO_OBJ____PRW), CO_TUNSIGNED32, (CO_DATA)(&Obj2100_00_20)}, // setpoint
    {CO_KEY(0x2200, 0, CO_OBJ____PR_), CO_TUNSIGNED32 ,(CO_DATA)(&Obj2200_00_20)}, // actual value

    CO_OBJ_DICT_ENDMARK  /* mark end of used objects */
};

/* Each software timer needs some memory for managing
 * the lists and states of the timed action events.
 */
static CO_TMR_MEM TmrMem[APP_TMR_N];

/* Each SDO server needs memory for the segmented or
 * block transfer requests.
 */
static uint8_t SdoSrvMem[CO_SSDO_N * CO_SDO_BUF_BYTE];

/* Specify the EMCY error codes with the corresponding
 * error register bit. There is a collection of defines
 * for the predefined emergency codes CO_EMCY_CODE...
 * and for the error register bits CO_EMCY_REG... for
 * readability. You can use plain numbers, too.
 */
static CO_EMCY_TBL AppEmcyTbl[APP_ERR_ID_NUM] = {
    { CO_EMCY_REG_GENERAL, CO_EMCY_CODE_HW_ERR }    /* APP_ERR_ID_EEPROM */
};

/******************************************************************************
* PUBLIC VARIABLES
******************************************************************************/

/* Collect all node specification settings in a single
 * structure for initializing the node easily.
 */
struct CO_NODE_SPEC_T AppSpec = {
    APP_NODE_ID,             /* default Node-Id                */
    APP_BAUDRATE,            /* default Baudrate               */
    &ClockOD[0],             /* pointer to object dictionary   */
    APP_OBJ_N,               /* object dictionary max length   */
    &AppEmcyTbl[0],          /* EMCY code & register bit table */
    &TmrMem[0],              /* pointer to timer memory blocks */
    APP_TMR_N,               /* number of timer memory blocks  */
    APP_TICKS_PER_SEC,       /* timer clock frequency in Hz    */
    &AppDriver,              /* select drivers for application */
    &SdoSrvMem[0]            /* SDO Transfer Buffer Memory     */
};
