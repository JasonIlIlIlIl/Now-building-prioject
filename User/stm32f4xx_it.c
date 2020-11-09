/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
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
#include "stm32f4xx_it.h"
#include "ucos_ii.h"
#include "bsp_bluetooth.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"

u8 CAPTURE_STA_TIM1CH[4] = {0};
u16 CAPTURE_VAL_TIM1CH[4];
u16 CAPTURE_UP_TIM1CH[4], CAPTURE_DOWN_TIM1CH[4];
extern int VAL[4];
/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
	/* This interrupt is generated when HSE clock fails */

  if (RCC_GetITStatus(RCC_IT_CSS) != RESET)
  {
    /* At this stage: HSE, PLL are disabled (but no change on PLL config) and HSI
       is selected as system clock source */

    /* Enable HSE */
    RCC_HSEConfig(RCC_HSE_ON);

    /* Enable HSE Ready and PLL Ready interrupts */
    RCC_ITConfig(RCC_IT_HSERDY | RCC_IT_PLLRDY, ENABLE);

    /* Clear Clock Security System interrupt pending bit */
    RCC_ClearITPendingBit(RCC_IT_CSS);

    /* Once HSE clock recover, the HSERDY interrupt is generated and in the RCC ISR
       routine the system clock will be reconfigured to its previous state (before
       HSE clock failure) */
  }
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
//void PendSV_Handler(void)
//{
//}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
#if OS_CRITICAL_METHOD == 3 /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr = 0;
#endif
    OS_ENTER_CRITICAL();  /* Tell uC/OS-II that we are starting an ISR */
    OSIntNesting++;
    OS_EXIT_CRITICAL();
    OSTimeTick();  /* Call uC/OS-II's OSTimeTick() */
    OSIntExit();  /* Tell uC/OS-II that we are leaving the ISR */	
}

//void DEBUG_USART_IRQHandler(void){
//	uint8_t ucTemp;
//	if(USART_GetITStatus(DEBUG_USART,USART_IT_RXNE)!=RESET){
//		ucTemp=USART_ReceiveData(DEBUG_USART);
//		USART_SendData(DEBUG_USART,ucTemp);
//	}
//}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

void TIM1_CC_IRQHandler(void)
{    
    if((CAPTURE_STA_TIM1CH[0]&0x80) == 0) {   			//还未捕获成功
				
        if(TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET) {    //捕获1发生捕获事件
            if(CAPTURE_STA_TIM1CH[0]&0x40) {                    //捕获到一个下降沿
                CAPTURE_STA_TIM1CH[0] |= 0x80;                    //标记成功捕获到一次高电平脉宽
                CAPTURE_DOWN_TIM1CH[0] = TIM_GetCapture1(TIM1);
                if(CAPTURE_DOWN_TIM1CH[0]>= CAPTURE_UP_TIM1CH[0]) 
                    CAPTURE_VAL_TIM1CH[0] = CAPTURE_DOWN_TIM1CH[0] - CAPTURE_UP_TIM1CH[0];
                else 
                    CAPTURE_VAL_TIM1CH[0] = 0xffff + CAPTURE_DOWN_TIM1CH[0] - CAPTURE_UP_TIM1CH[0];

                    TIM_OC1PolarityConfig(TIM1, TIM_ICPolarity_Rising);    //CC1P=0 设置为上升沿捕获
            } else {                                        //还未开始，第一次捕获上升沿
                CAPTURE_STA_TIM1CH[0] = 0;                    //清空
                CAPTURE_VAL_TIM1CH[0] = 0;
				
                CAPTURE_UP_TIM1CH[0] = TIM_GetCapture1(TIM1);
                CAPTURE_STA_TIM1CH[0]|= 0x40; 				//标记捕获到了上升沿
			          TIM_OC1PolarityConfig(TIM1, TIM_ICPolarity_Falling);    //CC1P=1 设置为下降沿捕获
            }    
            TIM_ClearFlag(TIM1, TIM_FLAG_CC1);                                //清除状态标志
        }
    } 

    if((CAPTURE_STA_TIM1CH[1]&0x80) == 0) {                    //还未捕获成功
        if(TIM_GetITStatus(TIM1, TIM_IT_CC2) != RESET) {            //捕获2发生捕获事件
            if(CAPTURE_STA_TIM1CH[1]&0x40) {                        //捕获到一个下降沿
                CAPTURE_STA_TIM1CH[1] |= 0x80;                        //标记成功捕获到一次高电平脉宽
                CAPTURE_DOWN_TIM1CH[1] = TIM_GetCapture2(TIM1);        //获取捕获2计数
                if(CAPTURE_DOWN_TIM1CH[1] >= CAPTURE_UP_TIM1CH[1])
                    CAPTURE_VAL_TIM1CH[1] = CAPTURE_DOWN_TIM1CH[1] - CAPTURE_UP_TIM1CH[1];
                else
                    CAPTURE_VAL_TIM1CH[1] =  CAPTURE_UP_TIM1CH[1] - CAPTURE_DOWN_TIM1CH[1];
                TIM_OC2PolarityConfig(TIM1, TIM_ICPolarity_Rising);    //CC1P=0 设置为上升沿捕获
            } else {                                                //还未开始，第一次捕获上升沿
                CAPTURE_STA_TIM1CH[1] = 0;                            //清空
                CAPTURE_VAL_TIM1CH[1] = 0;
                CAPTURE_UP_TIM1CH[1] = TIM_GetCapture2(TIM1);
                CAPTURE_STA_TIM1CH[1] |= 0x40;                //标记捕获到了上升沿
                TIM_OC2PolarityConfig(TIM1, TIM_ICPolarity_Falling);    //CC1P=1 设置为下降沿捕获
            }
            TIM_ClearFlag(TIM1, TIM_FLAG_CC2);                                //清除状态标志    
        }
    }
    
    if((CAPTURE_STA_TIM1CH[2]&0x80) == 0) {                    //还未捕获成功
        if(TIM_GetITStatus(TIM1, TIM_IT_CC3) != RESET) {            //捕获2发生捕获事件
            if(CAPTURE_STA_TIM1CH[2]&0x40) {                        //捕获到一个下降沿
                CAPTURE_STA_TIM1CH[2] |= 0x80;                        //标记成功捕获到一次高电平脉宽
                CAPTURE_DOWN_TIM1CH[2] = TIM_GetCapture3(TIM1);        //获取捕获2计数
                if(CAPTURE_DOWN_TIM1CH[2] >= CAPTURE_UP_TIM1CH[2])
                    CAPTURE_VAL_TIM1CH[2] = CAPTURE_DOWN_TIM1CH[2] - CAPTURE_UP_TIM1CH[2];
                else
                    CAPTURE_VAL_TIM1CH[2] = 0xffff + CAPTURE_DOWN_TIM1CH[2] - CAPTURE_UP_TIM1CH[2];
                TIM_OC3PolarityConfig(TIM1, TIM_ICPolarity_Rising);    //CC1P=0 设置为上升沿捕获
            } else {                                                //还未开始，第一次捕获上升沿
                CAPTURE_STA_TIM1CH[2] = 0;                            //清空
                CAPTURE_VAL_TIM1CH[2] = 0;
                CAPTURE_UP_TIM1CH[2] = TIM_GetCapture3(TIM1);
                CAPTURE_STA_TIM1CH[2] |= 0x40;                //标记捕获到了上升沿
                TIM_OC3PolarityConfig(TIM1, TIM_ICPolarity_Falling);    //CC1P=1 设置为下降沿捕获
            }
            TIM_ClearFlag(TIM1, TIM_FLAG_CC3);                                //清除状态标志        
        }
    } 
    if((CAPTURE_STA_TIM1CH[3]&0x80) == 0) {                    //还未捕获成功
        if(TIM_GetITStatus(TIM1, TIM_IT_CC4) != RESET) {            //捕获2发生捕获事件
            if(CAPTURE_STA_TIM1CH[3]&0x40) {                        //捕获到一个下降沿
                CAPTURE_STA_TIM1CH[3] |= 0x80;                        //标记成功捕获到一次高电平脉宽
                CAPTURE_DOWN_TIM1CH[3] = TIM_GetCapture4(TIM1);        //获取捕获2计数
                if(CAPTURE_DOWN_TIM1CH[3] >= CAPTURE_UP_TIM1CH[3])
                    CAPTURE_VAL_TIM1CH[3] = CAPTURE_DOWN_TIM1CH[3] - CAPTURE_UP_TIM1CH[3];
                else
                    CAPTURE_VAL_TIM1CH[3] = 0xffff + CAPTURE_DOWN_TIM1CH[3] - CAPTURE_UP_TIM1CH[3];
                TIM_OC4PolarityConfig(TIM1, TIM_ICPolarity_Rising);    //CC1P=0 设置为上升沿捕获
            } else {                                                //还未开始，第一次捕获上升沿
                CAPTURE_STA_TIM1CH[3] = 0;                            //清空
                CAPTURE_VAL_TIM1CH[3] = 0;
                CAPTURE_UP_TIM1CH[3] = TIM_GetCapture4(TIM1);
                CAPTURE_STA_TIM1CH[3] |= 0x40;                //标记捕获到了上升沿
                TIM_OC4PolarityConfig(TIM1, TIM_ICPolarity_Falling);    //CC1P=1 设置为下降沿捕获
            }
            TIM_ClearFlag(TIM1, TIM_FLAG_CC4);                                //清除状态标志        
        }
    }

    //处理帧数据
    if(CAPTURE_STA_TIM1CH[0]&0x80) {                                //成功捕获到了一次上升沿                                            //溢出时间总和
      		 
	//	printf("PWM  1 HIGH TIME: %d\n", CAPTURE_VAL_TIM1CH[0] );				//得到总的高电平时间
      VAL[0]=CAPTURE_VAL_TIM1CH[0];
			CAPTURE_STA_TIM1CH[0] = 0;
    } 
		
    if(CAPTURE_STA_TIM1CH[1]&0x80) {                                //成功捕获到了一次上升沿                                            //溢出时间总和
        printf("PWM  2 HIGH TIME: %d\n", CAPTURE_VAL_TIM1CH[1] );			//得到总的高电平时间
			 VAL[1]=CAPTURE_VAL_TIM1CH[1];		
//        printf("TIM1 CH2:%d\t", temp[1]);
        CAPTURE_STA_TIM1CH[1] = 0;
    } 
    if(CAPTURE_STA_TIM1CH[2]&0x80) {                                //成功捕获到了一次上升沿                                        //溢出时间总和
     //   printf("PWM  3 HIGH TIME: %d\n", CAPTURE_VAL_TIM1CH[2] );
			 VAL[2]=CAPTURE_VAL_TIM1CH[1];
//      printf("TIM1 CH3:%d\t", temp[2]);
        CAPTURE_STA_TIM1CH[2] = 0;
    } 
    if(CAPTURE_STA_TIM1CH[3]&0x80) {                                //成功捕获到了一次上升沿                                            //溢出时间总和
  //   printf("PWM 4 HIGH TIME: %d\n", CAPTURE_VAL_TIM1CH[3] );                            //得到总的高电平时间		
   //  printf("TIM1 CH4:%d\t", temp[3]);
			 VAL[3]=CAPTURE_VAL_TIM1CH[1];
       CAPTURE_STA_TIM1CH[3] = 0;
    }

}


/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


