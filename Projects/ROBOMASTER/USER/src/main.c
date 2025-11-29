#include "main.h"
#include "MATH.H"
// ========================= 参数区 =========================
uint16_t maxSpeed                   = 4000;
uint16_t ultraSpeed                 = 8000;
uint16_t deadBandOfLeft             = 10;                   // 左摇杆中心死区
uint16_t deadBandOfRight            = 10;                   // 右摇杆中心死区
uint16_t midDutyOfServo[4]          = {750, 750, 750, 750}; // 分别为机械臂L，机械臂R，夹爪转角，夹爪夹角
uint16_t maxChangeDutyOfServo[4]    = {200, 200, 200, 200};
uint16_t singleChangeDutyOfServo[2] = {10, 10};

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
// 自定义变量
int     dutyOfServo[4];
int     dutyOfMotor[4];
uint8_t valueOfKey[3][4];
int     valueOfRoker[2][2];
void    All_Init();
uint8_t Get_Dir(int rawdata);
void    Main_Countrol(int *dutyOfMotor, int *dutyOfServo);
void    ExpansionBoradControl(uint8_t control_cmd, uint16_t data_p60, uint16_t data_p62, uint16_t data_p64,
                              uint16_t data_p66, uint16_t data_p74, uint16_t data_p75, uint16_t data_p76,
                              uint16_t data_p77);

void main()
{
    All_Init();
    while (1) {
        if (RcKeyValueRead(KEY_OFFSET_UP))
            GPIO_Write_Bit(GPIO_P3, GPIO_Pin_7, 0);
        else
            GPIO_Write_Bit(GPIO_P3, GPIO_Pin_7, 1);
        // 摇杆读数读取
        valueOfRoker[0][0] = RcRockerValueRead(ROCKER_LEFT_HORIZONTAL);
        valueOfRoker[0][1] = RcRockerValueRead(ROCKER_LEFT_VERTICAL);
        valueOfRoker[1][0] = RcRockerValueRead(ROCKER_RIGHT_HORIZONTAL);
        valueOfRoker[1][1] = RcRockerValueRead(ROCKER_RIGHT_VERTICAL);

        // 按键读数获取
        valueOfKey[0][0] = RcKeyValueRead(KEY_OFFSET_UP);
        valueOfKey[0][1] = RcKeyValueRead(KEY_OFFSET_DOWN);
        valueOfKey[0][2] = RcKeyValueRead(KEY_OFFSET_LEFT);
        valueOfKey[0][3] = RcKeyValueRead(KEY_OFFSET_RIGHT);
        valueOfKey[1][0] = RcKeyValueRead(KEY_OFFSET_A);
        valueOfKey[1][1] = RcKeyValueRead(KEY_OFFSET_B);
        valueOfKey[1][2] = RcKeyValueRead(KEY_OFFSET_C);
        valueOfKey[1][3] = RcKeyValueRead(KEY_OFFSET_D);
        valueOfKey[2][0] = RcKeyValueRead(KEY_OFFSET_Rocker11);
        valueOfKey[2][1] = RcKeyValueRead(KEY_OFFSET_Rocker21);

        // 左侧轮控制值计算
        if (valueOfKey[2][0]) {
            dutyOfMotor[0] = -(int)(((float)valueOfRoker[0][1] / 2047) * ultraSpeed);
            dutyOfMotor[1] = -(int)(((float)valueOfRoker[0][1] / 2047) * ultraSpeed);
        } else {
            dutyOfMotor[0] = -(int)(((float)valueOfRoker[0][1] / 2047) * maxSpeed);
            dutyOfMotor[1] = -(int)(((float)valueOfRoker[0][1] / 2047) * maxSpeed);
        }

        // 右侧轮控制值计算
        if (valueOfKey[2][1]) {
            dutyOfMotor[2] = (int)(((float)valueOfRoker[1][1] / 2047) * ultraSpeed);
            dutyOfMotor[3] = (int)(((float)valueOfRoker[1][1] / 2047) * ultraSpeed);
        } else {
            dutyOfMotor[2] = (int)(((float)valueOfRoker[1][1] / 2047) * maxSpeed);
            dutyOfMotor[3] = (int)(((float)valueOfRoker[1][1] / 2047) * maxSpeed);
        }

        // 机械臂控制值计算
        if (valueOfKey[0][0]) {
            dutyOfServo[0] = midDutyOfServo[0] + maxChangeDutyOfServo[0];
            dutyOfServo[1] = midDutyOfServo[1] - maxChangeDutyOfServo[1];
        }
        if (valueOfKey[0][1]) {
            dutyOfServo[0] = midDutyOfServo[0] - maxChangeDutyOfServo[0];
            dutyOfServo[1] = midDutyOfServo[1] + maxChangeDutyOfServo[1];
        }
        if (valueOfKey[0][2]) {
            dutyOfServo[0] -= singleChangeDutyOfServo[0];
            dutyOfServo[1] += singleChangeDutyOfServo[1];
        }
        if (valueOfKey[0][3]) {
            dutyOfServo[0] += singleChangeDutyOfServo[0];
            dutyOfServo[1] -= singleChangeDutyOfServo[1];
        }

        // 夹爪转角控制值计算
        dutyOfServo[2] = midDutyOfServo +
                         (int)(((float)valueOfRoker[0][0] / 2047) * maxChangeDutyOfServo[2]);

        // 夹爪夹角控制值计算
        dutyOfServo[3] = midDutyOfServo +
                         (int)(((float)valueOfRoker[1][0] / 2047) * maxChangeDutyOfServo[3]);

        // 显示操纵数据
        ShowStringData("MOT_L", 5, dutyOfMotor[0], 0, 0);
        ShowStringData("MOT_R", 5, dutyOfMotor[1], 1, 0);
        ShowStringData("ARM_H", 5, dutyOfServo[0] - midDutyOfServo[0], 2, 0);
        ShowStringData("CLW_T", 5, dutyOfServo[2] - midDutyOfServo[2], 3, 0);
        ShowStringData("CLW_D", 5, dutyOfServo[3] - midDutyOfServo[3], 4, 0);
        Main_Countrol(dutyOfMotor, dutyOfServo);
    }
}

uint8_t Get_Dir(int rawdata)
{
    if (rawdata >= 0)
        return 1;
    else
        return 0;
}

void All_Init()VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV;
{
    Board_Init();
    GPIO_Init(GPIO_P3, GPIO_Pin_4, GPIO_OUT_PP);
    GPIO_Write_Bit(GPIO_P3, GPIO_Pin_4, 0);
    remote_control_init();
    GPIO_Write_Bit(GPIO_P3, GPIO_Pin_4, 1);
    UART_Init(UART_1, UART1_RX_P30, UART1_TX_P31, 230400, TIM1);
    PWM_Init(PWMB_CH3_P33, 1000, 10000);
    ExpansionBoradControl(Init_Order,
                          50, 50,
                          50, 50,
                          10000, 10000,
                          10000, 10000);
    PWM_Init(PWMB_CH1_P74, 50, midDutyOfServo[2]);
    PWM_Init(PWMB_CH4_P03, 50, midDutyOfServo[3]);
    // 舵机1、2、3、4；电机1、2、3、4Dk,RU \ ...                                                                                                                                                                                                                                \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
}

void Main_Countrol(int *dutyOfMotor, int *dutyOfServo)
{
    ExpansionBoradControl(Dir_Change_Order,
                          1, 1,
                          1, 1,
                          Get_Dir(dutyOfMotor[0]), Get_Dir(dutyOfMotor[1]),
                          Get_Dir(dutyOfMotor[2]), Get_Dir(dutyOfMotor[3]));
    Ms_Delay(10);
    ExpansionBoradControl(Duty_Change_Order,
                          (uint16_t)abs(dutyOfServo[0]), (uint16_t)abs(dutyOfServo[1]),
                          750, 750, (uint16_t)abs(dutyOfMotor[0]), (uint16_t)abs(dutyOfMotor[1]),
                          (uint16_t)abs(dutyOfMotor[2]), (uint16_t)abs(dutyOfMotor[3]));
    Ms_Delay(10);
    PWM_SET_Duty(PWMB_CH1_P74, dutyOfServo[2]);
    PWM_SET_Duty(PWMB_CH4_P03, dutyOfServo[3]);
}
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
