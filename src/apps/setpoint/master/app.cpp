#include "app.h"

#include <cstdint>

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

  SetSetPoint(&Node, 42);

  /* In the background loop we execute elapsed action
   * callback functions.
   */
  while (1) {
      CONodeProcess(&Node);
      COTmrProcess(&Node.Tmr);
  }
}