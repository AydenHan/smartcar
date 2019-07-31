#include "car.h"
#include "encoder.h"
/**/
void EncoderInit(void)
{
    //³õÊ¼»¯±àÂëÆ÷
    ftm_quad_init(ENCODER_L_FTM);    //FTM1
    ftm_quad_init(ENCODER_R_FTM);    //FTM2
}

void EncoderClean(void)
{
	ftm_quad_clean(ENCODER_L_FTM);
	ftm_quad_clean(ENCODER_R_FTM);
}