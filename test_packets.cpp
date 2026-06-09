#include <stdio.h>
#define _WIN32_WINNT 0x0501
#include <windows.h>
#include "csprotocol.h"

int main() {
    printf("CS_NC_MAPLEAVE_ACK = %x\n", CS_NC_MAPLEAVE_ACK);
    printf("CS_NC_STARTMOVE_ACK = %x\n", CS_NC_STARTMOVE_ACK);
    printf("CS_NC_SYNCMOVE_ACK = %x\n", CS_NC_SYNCMOVE_ACK);
    printf("CS_NC_ENDMOVE_ACK = %x\n", CS_NC_ENDMOVE_ACK);
    printf("CS_BT_PREATTACK_ACK = %x\n", CS_BT_PREATTACK_ACK);
    printf("CS_BT_ATTACK_ACK = %x\n", CS_BT_ATTACK_ACK);
    return 0;
}
