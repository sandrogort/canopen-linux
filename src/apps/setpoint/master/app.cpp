#include "app.h"

#include <cstdint>
#include <iostream>

CO_NODE Node;

static void SetSetPoint(void *p_arg, uint32_t p_setpoint)
{
    CO_NODE  *node;
    CO_OBJ   *od_setpoint;
    uint32_t  setpoint;

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

        COObjRdValue(od_setpoint, node, (void *)&setpoint, sizeof(setpoint));

        setpoint = p_setpoint;

        COObjWrValue(od_setpoint , node, (void *)&setpoint  , sizeof(setpoint)  );
    }
}

void COPdoAsyncUpdate(CO_RPDO *pdo)
{
    (void)pdo;

    static uint8_t oldBusy = 0;
    static bool firstBusy = true;

    /* Optional: place here some code, which is called
     * when a PDO is written after it is received.
     */
    if (pdo->Identifier == 0x181 && pdo->ObjNum == 2) {
        CO_OBJ   *od_busy;
        uint8_t   busy;

        od_busy = CODictFind(&pdo->Node->Dict, CO_DEV(0x2300, 0));
        COObjRdValue(od_busy, pdo->Node, (void *)&busy, sizeof(busy)); 
        if (busy == false && firstBusy) {
            firstBusy = false;
            SetSetPoint(pdo->Node, 42);
        } else if (oldBusy == 1 && busy == 0) {
            int random = rand() % 100;
            SetSetPoint(pdo->Node, random);
        } 
        oldBusy = busy;   
    }
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