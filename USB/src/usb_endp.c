/**
  ******************************************************************************
  * @file    usb_endp.c
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Endpoint routines
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_mem.h"
#include "hw_config.h"
#include "usb_istr.h"
#include "usb_pwr.h"
#include "Comm_FIFO.h"
#include <string.h>
#include "stm32f10x_tim.h"
#include "Common.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

static volatile uint8_t txFlg = 0;
static volatile uint32_t FrameCount = 0;

uint16_t Framelen=0;/*接收指示*/
uint8_t  data[1024]={0};/*接收缓存*/	

uint16_t FramelenTx=0;/*发送指示*/
uint8_t  Txdata[1024]={0};/*发送缓存*/	


/* Private function prototypes -----------------------------------------------*/
void TimeOut_Reload(void);

/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : EP1_IN_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP1_IN_Callback (void)
{



}
#define TIM_CNT_RELOAD						(9)

void TimeOut_Reload(void)
{
	/* 向下计数模式,装载TIMx->CNT值 */
	TIM2->CNT = TIM_CNT_RELOAD;
}

/******************************************************************************/
void TIM2_IRQHandler(void)
{
	DataTypedef Rxdata;
	uint16_t crcCal;

	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_Cmd(TIM2, DISABLE);
		if(Framelen!=0)
		{
			if((data[0]=='$')&&(data[Framelen-1]=='#'))
			{
				/*将接收的数据生成crc校验码*/
				crcCal = Common_CalculateCRC(&data[1], *(uint16_t*)&data[1]-2, 0xFFFF, 0x0000);
				/*进行校验码核对*/
				if(crcCal==*(uint16_t*)&data[*(uint16_t*)&data[1]+2-3])
				{
					Rxdata.len=*(uint16_t*)&data[1]-4;
					memcpy(Rxdata.Data,&data[3],Rxdata.len);
					Comm_FIFO_RxDataPut(&Rxdata,&RxDataFIFO);
				}
			}		
			memset(data, 0, Framelen);
			Framelen=0;
		}
		else
		{

		}

		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

/*******************************************************************************
* Function Name  : EP3_OUT_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP3_OUT_Callback(void)
{
	uint8_t USB_Rx_Cnt;
	if(Framelen==0)
	{
		/*1启动定时器*/
		TIM_Cmd(TIM2, ENABLE);
	}
	USB_Rx_Cnt=USB_SIL_Read(EP3_OUT, &data[Framelen]);
	Framelen+=USB_Rx_Cnt;
	/*清除定时器计时*/
	TimeOut_Reload();
	/* Enable the receive of data on EP3 */
	SetEPRxValid(ENDP3);
}

/*******************************************************************************
* Function Name  : SOF_Callback / INTR_SOFINTR_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SOF_Callback(void)
{
	static uint16_t TxLen=0;
    uint8_t len = 0;

    if(bDeviceState == CONFIGURED)
    {
    	if(FramelenTx==0)
    	{
    		if (Comm_RequestTX)
    		{
    			Comm_RequestTX = 0;
				memset(Txdata, 0, 1024);
    			memcpy(&Txdata[0], TxData.Data, TxData.len);
    			FramelenTx = TxData.len;
    			TxLen = 0;
			}
    	}
		else
		{	if(FramelenTx < 64)
			{
				len = FramelenTx;
			}
			else
			{
				len = 64;
			}
			UserToPMABufferCopy(&Txdata[TxLen], ENDP1_TXADDR, len);
			SetEPTxCount(ENDP1, len);
			SetEPTxValid(ENDP1);
			TxLen += len;
			
			if(FramelenTx <= 64)
			{
				/*数据发送结束，清空缓存*/
				FramelenTx = 0;
				memset((uint8_t*)&TxData, 0, sizeof(DataTypedef));
			}
			else
			{
				FramelenTx -= 64;
			}
		}
    }  
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

