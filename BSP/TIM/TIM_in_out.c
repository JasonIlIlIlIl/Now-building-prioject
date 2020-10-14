
#include "TIM_in_out.h"

/**********TIM18 初始化***********
 * 外部使用TIM_config()函数作为接口**
 * 无刷电机调速使用
 *  EM_speedx(int 32 spedd numbee )
 * ******************************/




void TIM_config(void);
void PWMINPUT_Config(void);
void PWMINPUT_Config(void);

void EM_speed1(int );
void EM_speed2(int );
void EM_speed3(int );
void EM_speed4(int );

void TIM_config()
{
	PWMINPUT_Config();
    PWMINPUT_Config();
}




void PWMINPUT_Config(void)
{
	 NVIC_InitTypeDef                NVIC_InitStructure ;
	 TIM_TimeBaseInitTypeDef         TIM_TimeBaseStructure;
	 TIM_ICInitTypeDef               TIM_ICInitStructure;
	 GPIO_InitTypeDef                GPIO_InitStructure;
   //GPIO_CLK ENABLE	
	 RCC_AHB1PeriphClockCmd(PWMINPUT_PORT_CLK,ENABLE);
	 //TIM CLK ENABLE
	 RCC_APB2PeriphClockCmd(PWMINPUT_TIM_CLK,ENABLE);

	
	//GPIO Define 
	 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
     GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	 GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	 GPIO_InitStructure.GPIO_Pin   = PWMINPUT_PIN8;
	 GPIO_Init(PWMINPUT_PORT,& GPIO_InitStructure);
	 GPIO_InitStructure.GPIO_Pin   = PWMINPUT_PIN9;
	 GPIO_Init(PWMINPUT_PORT,& GPIO_InitStructure);
	 GPIO_InitStructure.GPIO_Pin   = PWMINPUT_PIN10;
	 GPIO_Init(PWMINPUT_PORT,& GPIO_InitStructure);
	 GPIO_InitStructure.GPIO_Pin   = PWMINPUT_PIN11;
	 GPIO_Init(PWMINPUT_PORT,& GPIO_InitStructure);
	 
	//Alter Function link 
	 GPIO_PinAFConfig(PWMINPUT_PORT,PWMINPUT_PINSOURCE8  ,PWMINPUT_GPIO_AF);
	 GPIO_PinAFConfig(PWMINPUT_PORT,PWMINPUT_PINSOURCE9  ,PWMINPUT_GPIO_AF);
	 GPIO_PinAFConfig(PWMINPUT_PORT,PWMINPUT_PINSOURCE10 ,PWMINPUT_GPIO_AF);
	 GPIO_PinAFConfig(PWMINPUT_PORT,PWMINPUT_PINSOURCE11 ,PWMINPUT_GPIO_AF);
	
	//Timebase init 
	 TIM_TimeBaseStructure.TIM_Period            = ADVANCE_TIM_PERIOD;
	 TIM_TimeBaseStructure.TIM_Prescaler         = ADVANCE_TIM_PSC ;
	 TIM_TimeBaseStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
     TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;
     TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;	
	 TIM_TimeBaseInit(PWMINPUT_TIM, &TIM_TimeBaseStructure);
  
	
	 //Input catch init 
	  //TIM2_CH1输入捕获初始化
     TIM_ICInitStructure.TIM_Channel =PWMINPUT_PIN8_CHANNEL1 ;
     TIM_ICInitStructure.TIM_ICFilter = 0x00;                            //不滤波
     TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;        //上升沿捕获
     TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;                //输入器不分频
     TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;    //映射到IC1
     TIM_ICInit(TIM1, &TIM_ICInitStructure);
    
    TIM_ICInitStructure.TIM_Channel = PWMINPUT_PIN9_CHANNEL2 ;
    TIM_ICInitStructure.TIM_ICFilter = 0x00;                            //不滤波
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;        //上升沿捕获
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;                //输入器不分频
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;    //映射到IC2
    TIM_ICInit(TIM1, &TIM_ICInitStructure);
    
    TIM_ICInitStructure.TIM_Channel = PWMINPUT_PIN10_CHANNEL3;
    TIM_ICInitStructure.TIM_ICFilter = 0x00;                            //不滤波
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;        //上升沿捕获
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;                //输入器不分频
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;    //映射到IC3
    TIM_ICInit(TIM1, &TIM_ICInitStructure);
    
    TIM_ICInitStructure.TIM_Channel = PWMINPUT_PIN11_CHANNEL4;
    TIM_ICInitStructure.TIM_ICFilter = 0x00;                            //不滤波
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;        //上升沿捕获
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;                //输入器不分频
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;    //映射到IC4
    TIM_ICInit(TIM1, &TIM_ICInitStructure);
    
    
    //中断分组初始化
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel                     =   TIM1_CC_IRQn ;
    NVIC_InitStructure.NVIC_IRQChannelCmd                  =   ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority   =   1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority          =   1;
    NVIC_Init(&NVIC_InitStructure);    
    
    TIM_ITConfig(TIM1, TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4, ENABLE);            //更新中断和CC1IE捕获中断
    
    TIM_Cmd(TIM1, ENABLE);
    printf("TIM1 all 4CH Init SCU\n");
}
/*********************************************************************************/

//PWM OUT configuration 

void PWMOUTPUT_Config(void)
{
    //printf("PWMOUT");
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
                                                             
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);  
 
    GPIO_InitStructure.GPIO_Mode       = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType      = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd       = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed      = GPIO_Speed_50MHz;
	
    GPIO_InitStructure.GPIO_Pin        = PWMOTPUT_PIN6;
	GPIO_Init(PWMOTPUT_PORT,& GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin        = PWMOTPUT_PIN7;
	GPIO_Init(PWMOTPUT_PORT,& GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin        = PWMOTPUT_PIN8;
    GPIO_Init(PWMOTPUT_PORT,& GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin        = PWMOTPUT_PIN9;
	GPIO_Init(PWMOTPUT_PORT,& GPIO_InitStructure);
	 
	 
	GPIO_PinAFConfig(PWMOTPUT_PORT,PWMOTPUT_PINSOURCE6  ,PWMOTPUT_GPIO_AF);
	GPIO_PinAFConfig(PWMOTPUT_PORT,PWMOTPUT_PINSOURCE7  ,PWMOTPUT_GPIO_AF);
	GPIO_PinAFConfig(PWMOTPUT_PORT,PWMOTPUT_PINSOURCE8  ,PWMOTPUT_GPIO_AF);
	GPIO_PinAFConfig(PWMOTPUT_PORT,PWMOTPUT_PINSOURCE9  ,PWMOTPUT_GPIO_AF);
	
	TIM_TimeBaseStructure.TIM_Period = ADVANCE_TIM_PERIOD1  ; 
	TIM_TimeBaseStructure.TIM_Prescaler =ADVANCE_TIM_PSC ; 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); 
 
	TIM_OCInitStructure.TIM_OCMode      =       TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OutputState =       TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_Pulse       =       0; 
	TIM_OCInitStructure.TIM_OCPolarity  =       TIM_OCPolarity_High; 
	TIM_OC1Init(TIM8, &TIM_OCInitStructure); 
	
	TIM_OCInitStructure.TIM_OCMode      =       TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OutputState =       TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_Pulse       =       0; 
	TIM_OCInitStructure.TIM_OCPolarity  =       TIM_OCPolarity_High; 
	TIM_OC2Init(TIM8, &TIM_OCInitStructure); 
	
	
	TIM_OCInitStructure.TIM_OCMode      =       TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState =       TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_Pulse       =       0; 
	TIM_OCInitStructure.TIM_OCPolarity  =       TIM_OCPolarity_High; 
	TIM_OC3Init(TIM8, &TIM_OCInitStructure);  
	
	
	TIM_OCInitStructure.TIM_OCMode      =       TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OutputState =       TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_Pulse       =       0; 
	TIM_OCInitStructure.TIM_OCPolarity  =       TIM_OCPolarity_High; 
	TIM_OC4Init(TIM8, &TIM_OCInitStructure);  
 
	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);  
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);  
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);  
	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);  
	TIM_CtrlPWMOutputs(TIM8,ENABLE);	
	
	TIM_ARRPreloadConfig(TIM8, ENABLE); 
	
	TIM_Cmd(TIM8, ENABLE); 
	printf("PWM OUT Init complete!");
}

void EM_speed1(int  para)
{
    TIM_SetCompare1(TIM8,para);
}
void EM_speed2(int  para)
{
    TIM_SetCompare2(TIM8,para);
}
void EM_speed3(int  para)
{
    TIM_SetCompare3(TIM8,para);
}
void EM_speed4(int para)
{
    TIM_SetCompare4(TIM8,para);
}
