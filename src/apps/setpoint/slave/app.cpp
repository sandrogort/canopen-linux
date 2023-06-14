#include "app.h"

#include <cstdint>
#include <iostream>

CO_NODE Node;

static void SetActualValue(void *p_arg)
{
    CO_NODE  *node;
    CO_OBJ   *od_setpoint;
    uint32_t  setpoint;
    CO_OBJ   *od_actualValue;
    uint32_t  actualValue;  
    CO_OBJ   *od_busy;
    uint8_t   busy;          

    /* For flexible usage (not needed, but nice to show), we use the argument
     * as reference to the CANopen node object. If no node is given, we ignore
     * the function call by returning immediatelly.
     */
    node = (CO_NODE *)p_arg;
    if (node == 0) {
        return;
    }

    /* Main functionality: when we are in operational mode, we get the current
     * clock values out of the object dictionary, increment the seconds and
     * update all clock values in the object dictionary.
     */
    if (CONmtGetMode(&node->Nmt) == CO_OPERATIONAL) {

        od_setpoint = CODictFind(&node->Dict, CO_DEV(0x2100, 0));
        od_actualValue = CODictFind(&node->Dict, CO_DEV(0x2200, 0));
        od_busy = CODictFind(&node->Dict, CO_DEV(0x2300, 0));

        COObjRdValue(od_setpoint, node, (void *)&setpoint, sizeof(setpoint));
        COObjRdValue(od_actualValue, node, (void *)&actualValue, sizeof(actualValue));
        COObjRdValue(od_busy, node, (void *)&busy, sizeof(busy));

        if (actualValue > setpoint) {
            actualValue--;
        } else if (actualValue < setpoint) {
            actualValue++;
        }

        if (actualValue == setpoint) {
            busy = 0;
        } else {
            busy = 1;
        }

        COObjWrValue(od_actualValue , node, (void *)&actualValue  , sizeof(actualValue)  );
        COObjWrValue(od_busy , node, (void *)&busy  , sizeof(busy)  );
    }
}

void COPdoSyncUpdate(CO_RPDO *pdo)
{
    (void)pdo;
    
    /* Optional: place here some code, which is called
     * right after the object dictionary update due to
     * a synchronized PDO.
     */
    SetActualValue(&Node);
}

void AppStart() {
  uint32_t ticks;

  /* Initialize the CANopen stack. Stop execution if an
   * error is detected.
   */
  CONodeInit(&Node, &AppSpec);
  if (CONodeGetErr(&Node) != CO_ERR_NONE) {
      while(1);    /* error not handled */
  }

  /* Start the CANopen node and set it automatically to
   * NMT mode: 'OPERATIONAL'.
   */
  CONodeStart(&Node);
  CONmtSetMode(&Node.Nmt, CO_OPERATIONAL);

  /* In the background loop we execute elapsed action
   * callback functions.
   */
  while (1) {
      CONodeProcess(&Node);
      COTmrProcess(&Node.Tmr);
  }
}