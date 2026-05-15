#include <stdio.h>
#define _WIN32_WINNT 0x0501
#include <windows.h>
#include "csprotocol.h"
int main() {
    printf("CS_BT_PREATTACK_ACK = 0x%X\n", CS_BT_PREATTACK_ACK);
    printf("CS_BT_ATTACK_ACK = 0x%X\n", CS_BT_ATTACK_ACK);
    printf("CS_BT_NPCPREATTACK_ACK = 0x%X\n", CS_BT_NPCPREATTACK_ACK);
    printf("CS_BT_NPCATTACK_ACK = 0x%X\n", CS_BT_NPCATTACK_ACK);
    printf("CS_NC_MAPLEAVE_ACK = 0x%X\n", CS_NC_MAPLEAVE_ACK);
    printf("CS_NC_STARTMOVE_ACK = 0x%X\n", CS_NC_STARTMOVE_ACK);
    printf("CS_NC_SYNCMOVE_ACK = 0x%X\n", CS_NC_SYNCMOVE_ACK);
    printf("CS_NC_ENDMOVE_ACK = 0x%X\n", CS_NC_ENDMOVE_ACK);
    return 0;
}
