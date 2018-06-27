/*
 * Battery_ADC.c
 *
 *  Created on: 2018��6��16��
 *      Author: Administrator
 */
/******************************************************************************/
#include "Battery_ADC.h"

/******************************************************************************/
void  Adc_Init(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	//ʹ��ADC1ͨ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_ADC1, ENABLE );


	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   				//����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M

	//PC1 ��Ϊģ��ͨ����������
	GPIO_InitStructure.GPIO_Pin = Battery_ADC_IN1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(Battery_ADC_IN1_PORT, &GPIO_InitStructure);

	ADC_DeInit(ADC1);  									//��λADC1

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;		//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;				//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;				//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);					//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���


	ADC_Cmd(ADC1, ENABLE);								//ʹ��ָ����ADC1

	ADC_ResetCalibration(ADC1);							//ʹ�ܸ�λУ׼

	while(ADC_GetResetCalibrationStatus(ADC1));			//�ȴ���λУ׼����

	ADC_StartCalibration(ADC1);	 						//����ADУ׼

	while(ADC_GetCalibrationStatus(ADC1));	 			//�ȴ�У׼����

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);				//ʹ��ָ����ADC1�����ת����������

}

//���ADCֵ
//ch:ͨ��ֵ 0~3
/******************************************************************************/
uint16 Get_Adc(uint8 ch)
{
  														//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);				//ʹ��ָ����ADC1�����ת����������

	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));		//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);				//�������һ��ADC1�������ת�����
}

/******************************************************************************/
uint16 Get_Adc_Average(uint8 ch,uint8 times)
{
	uint32 temp_val=0;
	uint8 t;
	for(t=0;t<times;t++)
	{
		temp_val += Get_Adc(ch);
		Delay_ms_SW(5);
	}
	return temp_val/times;
}
