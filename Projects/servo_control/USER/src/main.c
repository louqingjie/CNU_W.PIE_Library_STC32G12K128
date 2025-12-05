#include "main.h"
/*帧头帧尾，内部调用，无需关心*/
#define COMM_HEADER_1 0xAB
#define COMM_HEADER_2 0xBC
#define COMM_END_1 0xCD
#define COMM_END_2 0xDE
/*命令码*/
#define Init_Order 0xAA        // 初始化模式
#define Duty_Change_Order 0xBB // 修改占空比
#define Freq_Change_Order 0xCC // 修改频率
#define Dir_Change_Order 0xDD  // 修改方向 1为正 0为负 设置一次即可
#define Zero_Order 0xEE        // 0命令
/*内部调用变量，无需关心，请勿定义同名变量*/
uint16_t control_data[8] = {0};
uint16_t motor_dir[8]    = {0};
uint8_t  control_command = 0x00;
/**************************************************************************************************************************
 * @brief  板间通信函数，用于主控给拓展版发送
 * @exampleCode
 * ExpansionBoradControl(Init_Order, 50, 50, 50, 50, 10000, 10000, 10000);//初始化模式
 * @explain  初始化模式后是各个引脚的频率，50为舵机或摩擦轮，10000为电机
 *           修改占空比的模式后参数写设置的占空比，以此类推，写NULL则维持之前状态，该引脚的动力源相关参数不被改变
 * @param[in]  control_cmd 发送的内容
 * @param[in]  data_pxx  xx引脚的频率/占空比/方向
 ***************************************************************************************************************************/
void ExpansionBoradControl(uint8_t control_cmd, uint16_t data_p60, uint16_t data_p62, uint16_t data_p64,
                           uint16_t data_p66, uint16_t data_p74, uint16_t data_p75, uint16_t data_p76,
                           uint16_t data_p77)
{
    uint8_t i = 0;
    // 通信数据帧
    uint8_t control_frame_pack[21] = {0};
    // 帧头帧尾
    control_frame_pack[0]  = COMM_HEADER_1;
    control_frame_pack[1]  = COMM_HEADER_2;
    control_frame_pack[19] = COMM_END_1;
    control_frame_pack[20] = COMM_END_2;
    // 指令
    control_frame_pack[2] = control_cmd;
    // 数据
    control_frame_pack[3]  = (uint8_t)((data_p60 >> 8) & 0xFF);
    control_frame_pack[4]  = (uint8_t)(data_p60 & 0xFF);
    control_frame_pack[5]  = (uint8_t)((data_p62 >> 8) & 0xFF);
    control_frame_pack[6]  = (uint8_t)(data_p62 & 0xFF);
    control_frame_pack[7]  = (uint8_t)((data_p64 >> 8) & 0xFF);
    control_frame_pack[8]  = (uint8_t)(data_p64 & 0xFF);
    control_frame_pack[9]  = (uint8_t)((data_p66 >> 8) & 0xFF);
    control_frame_pack[10] = (uint8_t)(data_p66 & 0xFF);
    control_frame_pack[11] = (uint8_t)((data_p74 >> 8) & 0xFF);
    control_frame_pack[12] = (uint8_t)(data_p74 & 0xFF);
    control_frame_pack[13] = (uint8_t)((data_p75 >> 8) & 0xFF);
    control_frame_pack[14] = (uint8_t)(data_p75 & 0xFF);
    control_frame_pack[15] = (uint8_t)((data_p76 >> 8) & 0xFF);
    control_frame_pack[16] = (uint8_t)(data_p76 & 0xFF);
    control_frame_pack[17] = (uint8_t)((data_p77 >> 8) & 0xFF);
    control_frame_pack[18] = (uint8_t)(data_p77 & 0xFF);

    // 发送
    // UART_PutBuff(UART_1, control_frame_pack, 21);
    for (i = 0; i < 21; i++)
        UART_PutChar(UART_1, control_frame_pack[i]);
}

void main()
{
    Board_Init(); // 开发板初始化
    PWM_Init(PWMB_CH1_P74, 50, 750);
    PWM_Init(PWMB_CH4_P03, 50, 750);
    ExpansionBoradControl(Init_Order, 50, 50, 50, 50, 10000, 10000, 10000, 10000);
    while (1) {
        ExpansionBoradControl(Duty_Change_Order, 750, 750, 750, 750, 0, 0, 0, 0);
		Ms_Delay(10);
		PWM_SET_Duty(PWMB_CH1_P74, 750);
		PWM_SET_Duty(PWMB_CH4_P03, 750);
    }
}