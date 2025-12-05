// 步兵机器人操作代码
#include "main.h"
#include "MATH.H"
// ========================= 参数区 =========================
uint16_t maxSpeed                   = 4000;
uint16_t ultraSpeed                 = 8000;
uint16_t deadBandOfLeft             = 20;                   // 左摇杆中心死区
uint16_t deadBandOfRight            = 20;                   // 右摇杆中心死区
uint16_t midDutyOfServo[4]          = {750, 750, 750, 750}; // 分别为机械臂抬升、夹爪仰角、倾斜角、开合舵机的占空比中值
uint16_t maxChangeDutyOfServo[4]    = {500, 500, 500, 120}; // 分别为机械臂抬升、夹爪仰角、倾斜角、开合舵机的最大占空比
uint16_t singleChangeDutyOfServo[4] = {10, 10, 10, 10};     // 按下按键单次占空比改变量
float    changeRateOfServo[2]       = {0.01, 0.01};         // 左右摇杆水平灵敏度

// 限幅宏
#define LIMIT_VALUE(x, min, max) \
    do {                         \
        if ((x) < (min))         \
            (x) = (min);         \
        else if ((x) > (max))    \
            (x) = (max);         \
    } while (0)

// 扩展板通信用
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
float                floatDutyOfServo[4];                                             // 分别为夹爪旋转角，夹爪抬头所用的微操占空比
uint16_t             dutyOfServo[4];                                                  // 分别为机械臂抬升、夹爪仰角、倾斜角、开合舵机的占空比
int                  dutyOfMotor[4];                                                  // 四个底盘电机
uint8_t              valueOfKey[3][4], valueOfRKey, lastValueOfRKey, statusOfArm; // 上下左右、ABCD、左右摇杆
uint8_t              i, j;                                                            // 循环用变量
int                  valueOfRoker[2][2];                                              // 左右摇杆的水平、垂直轴取值，[-2047, 2047]
int16_t              baseSpeed, turnSpeed;
static const uint8_t keyOffsets[3][4] = {
    {KEY_OFFSET_UP, KEY_OFFSET_DOWN, KEY_OFFSET_LEFT, KEY_OFFSET_RIGHT},
    {KEY_OFFSET_A, KEY_OFFSET_B, KEY_OFFSET_C, KEY_OFFSET_D},
    {KEY_OFFSET_Rocker11, KEY_OFFSET_Rocker21, 0, 0} // 实际只有2个
};

void    All_Init();
void    Read_Controller_Inputs();
void    Calculate_Motor_Controls();
void    Calculate_Servo_Controls();
uint8_t Get_Dir(int rawdata);
void    Main_Countrol(int *dutyOfMotor, uint16_t *dutyOfServo);
void    ExpansionBoradControl(uint8_t control_cmd, uint16_t data_p60, uint16_t data_p62, uint16_t data_p64,
                              uint16_t data_p66, uint16_t data_p74, uint16_t data_p75, uint16_t data_p76,
                              uint16_t data_p77);

void main()
{
    All_Init();
    while (1) {
        // 测试手柄连接状态
        if (RcKeyValueRead(KEY_OFFSET_UP))
            GPIO_Write_Bit(GPIO_P3, GPIO_Pin_7, 0);
        else
            GPIO_Write_Bit(GPIO_P3, GPIO_Pin_7, 1);

        Read_Controller_Inputs();   // 统一读取输入
        Calculate_Motor_Controls(); // 计算电机控制
        Calculate_Servo_Controls(); // 计算舵机控制
        for (i = 0; i < 4; i++)
            LIMIT_VALUE(dutyOfMotor[i], -10000, 10000);
        for (i = 0; i < 4; i++)
            LIMIT_VALUE(floatDutyOfServo[i], midDutyOfServo[i] - maxChangeDutyOfServo[i], midDutyOfServo[i] + maxChangeDutyOfServo[i]);
        for (i = 0; i < 4; i++)
            dutyOfServo[i] = floatDutyOfServo[i];

        // 发送控制函数
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

uint8_t Pure_Key_Value(int8_t keyValue)
{
    if (keyValue == 1)
        return 1;
    else
        return 0;
}
void All_Init()
{
    Board_Init();
    GPIO_Init(GPIO_P3, GPIO_Pin_4, GPIO_OUT_PP);
    GPIO_Write_Bit(GPIO_P3, GPIO_Pin_4, 0);
    remote_control_init();
    GPIO_Write_Bit(GPIO_P3, GPIO_Pin_4, 1);
    UART_Init(UART_1, UART1_RX_P30, UART1_TX_P31, 230400, TIM1);
    ExpansionBoradControl(Init_Order,
                          50, 50,
                          50, 50,
                          10000, 10000,
                          10000, 10000);
    Ms_Delay(20);
    PWM_Init(PWMB_CH1_P74, 50, midDutyOfServo[0]); // 云台水平舵机
    PWM_Init(PWMB_CH4_P03, 50, midDutyOfServo[1]); // 云台垂直舵机
    for (i = 0; i < 4; i++) {
        dutyOfServo[i]      = midDutyOfServo[i];
        floatDutyOfServo[i] = midDutyOfServo[i];
    }
}

void Read_Controller_Inputs()
{
    // 摇杆读数读取
    valueOfRoker[0][0] = RcRockerValueRead(ROCKER_LEFT_HORIZONTAL);
    valueOfRoker[0][1] = RcRockerValueRead(ROCKER_LEFT_VERTICAL);
    valueOfRoker[1][0] = RcRockerValueRead(ROCKER_RIGHT_HORIZONTAL);
    valueOfRoker[1][1] = RcRockerValueRead(ROCKER_RIGHT_VERTICAL);
    // 死区过滤
    if (abs(valueOfRoker[0][0]) <= deadBandOfLeft)
        valueOfRoker[0][0] = 0;
    if (abs(valueOfRoker[0][1]) <= deadBandOfLeft)
        valueOfRoker[0][1] = 0;
    if (abs(valueOfRoker[1][0]) <= deadBandOfRight)
        valueOfRoker[1][0] = 0;
    if (abs(valueOfRoker[1][1]) <= deadBandOfRight)
        valueOfRoker[1][1] = 0;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            if (i == 2 && j >= 2)
                break; // 第三行只有2个按键
            valueOfKey[i][j] = RcKeyValueRead(keyOffsets[i][j]);
        }
    }
    valueOfRKey = RcKeyValueRead(KEY_OFFSET_1);
}

void Calculate_Motor_Controls()
{
    // 左侧轮电机控制值计算

    if (valueOfKey[2][0]) {
        baseSpeed = (int)((float)valueOfRoker[0][1] * ultraSpeed / 2047);
        turnSpeed = -(int)((float)valueOfRoker[0][0] * ultraSpeed / 2047);

    } else {
        baseSpeed = (int)((float)valueOfRoker[0][1] * maxSpeed / 2047);
        turnSpeed = -(int)((float)valueOfRoker[0][0] * maxSpeed / 2047);
    }

    if (valueOfKey[0][0] == 1)
        baseSpeed = ultraSpeed;
    if (valueOfKey[0][1] == 1)
        baseSpeed = -ultraSpeed;
    if (valueOfKey[0][2] == 1)
        turnSpeed = -ultraSpeed;
    if (valueOfKey[0][3] == 1)
        turnSpeed = ultraSpeed;

    dutyOfMotor[0] = -baseSpeed - turnSpeed;
    dutyOfMotor[1] = -baseSpeed - turnSpeed;
    dutyOfMotor[2] = baseSpeed - turnSpeed;
    dutyOfMotor[3] = baseSpeed - turnSpeed;
}

void Calculate_Servo_Controls()
{
    floatDutyOfServo[0] += Pure_Key_Value(valueOfKey[1][1]) * singleChangeDutyOfServo[0];
    floatDutyOfServo[0] -= Pure_Key_Value(valueOfKey[1][2]) * singleChangeDutyOfServo[0];

    if (valueOfRKey && !lastValueOfRKey) {  // 检测上升沿
        statusOfArm = !statusOfArm; // 翻转状态
    }

    if (statusOfArm) {
        floatDutyOfServo[0] -= valueOfRoker[1][1] * changeRateOfServo[0];
        floatDutyOfServo[1] -= valueOfRoker[1][1] * changeRateOfServo[0];
    } else {
        floatDutyOfServo[1] -= valueOfRoker[1][1] * changeRateOfServo[0];
    }

    lastValueOfRKey = valueOfRKey;

    floatDutyOfServo[2] -= valueOfRoker[1][0] * changeRateOfServo[1];

    if (valueOfKey[1][3] == 1)
        floatDutyOfServo[3] = midDutyOfServo[3] + maxChangeDutyOfServo[3];
    if (valueOfKey[1][0] == 1)
        floatDutyOfServo[3] = midDutyOfServo[3] - maxChangeDutyOfServo[3];

    if (valueOfKey[2][1] == 1) {
        dutyOfServo[1] = midDutyOfServo[1];
        dutyOfServo[2] = midDutyOfServo[2];
    }
}

void Main_Countrol(int *dutyOfMotor, uint16_t *dutyOfServo)
{
    ExpansionBoradControl(Dir_Change_Order,
                          1, 1,
                          1, 1,
                          Get_Dir(dutyOfMotor[0]), Get_Dir(dutyOfMotor[1]),
                          Get_Dir(dutyOfMotor[2]), Get_Dir(dutyOfMotor[3]));
    Ms_Delay(10);
    ExpansionBoradControl(Duty_Change_Order,
                          dutyOfServo[0], dutyOfServo[1],
                          dutyOfServo[2], dutyOfServo[3],
                          (uint16_t)abs(dutyOfMotor[0]), (uint16_t)abs(dutyOfMotor[1]),
                          (uint16_t)abs(dutyOfMotor[2]), (uint16_t)abs(dutyOfMotor[3]));
    Ms_Delay(10);
}
/**************************************************************************************************************************
 * @brief  板间通信函数，用于主控给拓展版发送
 * @exampleCode
 * ExpansionBoradControl(Init_Order, 50, 50, 50, 50, 10000, 10000, 10000);//初始化模式
 * @explain  初始化模式后是各个引脚的频率，50为舵机或摩擦轮，10000为电机
 *           修改占空比的模式后参数写设置的占空比，以此类推，写NULL则维持之前状态，该引脚的动力源相关参数不被改变
 * @param[in]  control_cmd 发送的内容
 * @param[in]  data_pxx  xx引脚的频率/占空比
 ***************************************************************************************************************************/

/// @brief 板间通信函数，用于主控给拓展版发送
/// @param control_cmd
/// @param data_p60 机械臂抬升舵机
/// @param data_p62 机械臂仰角舵机
/// @param data_p64 无
/// @param data_p66 无
/// @param data_p74 左前电机
/// @param data_p75 左后电机
/// @param data_p76 右前电机
/// @param data_p77 右后电机
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