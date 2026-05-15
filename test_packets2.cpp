#include <stdio.h>
#define _WIN32_WINNT 0x0501
#include <windows.h>
#include "csprotocol.h"
int main() {
    printf("OFFSET_CS_BT=0x%X\n", OFFSET_CS_BT);
    printf("CS_BT_PREATTACK_ACK=0x%X\n", CS_BT_PREATTACK_ACK);
    return 0;
}
