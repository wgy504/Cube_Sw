#include <includes.h>

void bsp_InitRNG(void)
{
    /* ʹ��RNGʱ��Դ */
    RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE);

    /* ʹ��RNG */
    RNG_Cmd(ENABLE);
}

