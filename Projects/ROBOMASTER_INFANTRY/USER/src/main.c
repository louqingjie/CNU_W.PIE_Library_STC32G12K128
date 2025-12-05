// 步兵机器人操作代码
#include "main.h"
#include "MATH.H"
// ========================= 参数区 =========================
uint16_t maxSpeed                   = 4000;
uint16_t ultraSpeed                 = 8000;
uint16_t deadBandOfLeft             = 10;         // 左摇杆中心死区
uint16_t deadBandOfRight            = 10;         // 右摇杆中心死区
uint16_t midDutyOfServo[2]          = {750, 750}; // 分别为云台水平舵机、云台垂直舵机
uint16_t maxChangeDutyOfServo[2]    = {200, 200}; // 同上
uint16_t singleChangeDutyOfServo[2] = {10, 10};   // 按下按键单次占空比改变量
uint16_t singleChangeDutyOfBooster  = 100;        // 按下按键单次占空比改变量
uint16_t maxDutyOfBooster           = 1100;       // 摩擦轮最大占空比
float    changeRateOfServo[2]       = {0.01, 0.01};

#define LIMIT_VALUE(x, min, max) \
    do {                         \
        if ((x) < (min))         \
            (x) = (min);         \
        else if ((x) > (max))    \
            (x) = (max);         \
    } while (0)
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
float    floatDutyOfServo[2]; // 云台舵机
uint16_t dutyOfServo[2];
int      dutyOfMotor[5]; // 四个底盘电机，供弹电机
uint16_t dutyOfBooster = 0, expectDutyOfBooster = 0;
uint8_t  valueOfKey[3][4];
uint8_t  valueOfRKey, lastValueOfRKey, statusOfBooster = 0;
uint8_t  i, j;
int      valueOfRoker[2][2] // 左摇杆水平、竖直；左摇杆水平、竖直
    ,
    baseSpeed, turnSpeed;
static const uint8_t keyOffsets[3][4] = {
    {KEY_OFFSET_UP, KEY_OFFSET_DOWN, KEY_OFFSET_LEFT, KEY_OFFSET_RIGHT},
    {KEY_OFFSET_A, KEY_OFFSET_B, KEY_OFFSET_C, KEY_OFFSET_D},
    {KEY_OFFSET_Rocker11, KEY_OFFSET_Rocker21, 0, 0} // 实际只有2个
};

void    All_Init();
void    ReadControllerInputs();
void    CalculateMotorControls();
void    CalculateGimbalControls();
void    CalculateBoosterControl();
uint8_t Get_Dir(int rawdata);
void    Main_Countrol(int *dutyOfMotor, uint16_t *dutyOfServo, uint16_t dutyOfBooster);
void    ExpansionBoradControl(uint8_t control_cmd, uint16_t data_p60, uint16_t data_p62, uint16_t data_p64,
                              uint16_t data_p66, uint16_t data_p74, uint16_t data_p75, uint16_t data_p76,
                              uint16_t data_p77);

void main()
{
    All_Init();
    floatDutyOfServo[0] = midDutyOfServo[0];
    floatDutyOfServo[1] = midDutyOfServo[1];
    while (1) {
        // 测试手柄连接状态
        if (RcKeyValueRead(KEY_OFFSET_UP))
            GPIO_Write_Bit(GPIO_P3, GPIO_Pin_7, 0);
        else
            GPIO_Write_Bit(GPIO_P3, GPIO_Pin_7, 1);

        ReadControllerInputs();    // 统一读取输入
        CalculateMotorControls();  // 计算电机控制
        CalculateGimbalControls(); // 计算云台控制
        CalculateBoosterControl(); // 计算摩擦轮控制
        LIMIT_VALUE(dutyOfMotor[0], -10000, 10000);
        LIMIT_VALUE(dutyOfMotor[1], -10000, 10000);
        LIMIT_VALUE(dutyOfMotor[2], -10000, 10000);
        LIMIT_VALUE(dutyOfMotor[3], -10000, 10000);
        LIMIT_VALUE(dutyOfMotor[4], 0, 10000);
        LIMIT_VALUE(floatDutyOfServo[0], 250, 1250);
        LIMIT_VALUE(dutyOfServo[1], 250, 1250);

        // 平滑占空比变化
        if (expectDutyOfBooster > 500 && dutyOfBooster < 500)
            dutyOfBooster = 500;

        if (dutyOfBooster + 2 <= expectDutyOfBooster)
            dutyOfBooster += 2;
        else if (dutyOfBooster < expectDutyOfBooster)
            dutyOfBooster++;
        else if (dutyOfBooster - 2 >= expectDutyOfBooster)
            dutyOfBooster -= 2;
        else if (dutyOfBooster > expectDutyOfBooster)
            dutyOfBooster--;
        else
            dutyOfBooster = expectDutyOfBooster;

        // 发送控制函数
        Main_Countrol(dutyOfMotor, dutyOfServo, dutyOfBooster);
        Ms_Delay(10);
    }
}

// void PIT0_Activated() interrupt TMR0_VECTOR
// {
//     PIT_Timer_Clear(TIM0);
//     // 限幅

// }

uint8_t Get_Dir(int rawdata)
{
    if (rawdata >= 0)
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
                          10000, 50,
                          50, 50,
                          10000, 10000,
                          10000, 10000); // 供弹电机、空、摩擦轮L，摩擦轮R、左前轮、左后轮、右前轮、右后轮
    Ms_Delay(20);
    PWM_Init(PWMB_CH1_P74, 50, midDutyOfServo[0]); // 云台水平舵机
    PWM_Init(PWMB_CH4_P03, 50, midDutyOfServo[1]); // 云台垂直舵机
}

void ReadControllerInputs()
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

void CalculateMotorControls()
{
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

    // 供弹电机控制值计算
    if (valueOfKey[1][0])
        dutyOfMotor[4] += 50;
    if (valueOfKey[1][3])
        dutyOfMotor[4] = 0;
}

void CalculateBoosterControl()
{
    // 摩擦轮占空比计算
    if (valueOfKey[1][1])
        expectDutyOfBooster += singleChangeDutyOfBooster;
    else if (valueOfKey[1][2])
        expectDutyOfBooster -= singleChangeDutyOfBooster;

    if (valueOfRKey && !lastValueOfRKey) {  // 检测上升沿
        statusOfBooster = !statusOfBooster; // 翻转状态
    }

    if (statusOfBooster)
        expectDutyOfBooster = maxDutyOfBooster;
    else
        expectDutyOfBooster = 0;

    lastValueOfRKey = valueOfRKey;
}

void CalculateGimbalControls()
{
    // 云台舵机控制值计算
    floatDutyOfServo[0] += valueOfRoker[1][0] * changeRateOfServo[0];
    floatDutyOfServo[1] += valueOfRoker[1][1] * changeRateOfServo[1];
    dutyOfServo[0] = (uint16_t)floatDutyOfServo[0];
    dutyOfServo[1] = (uint16_t)floatDutyOfServo[1];
}

void Main_Countrol(int *dutyOfMotor, uint16_t *dutyOfServo, uint16_t dutyOfBooster)
{
    ExpansionBoradControl(Dir_Change_Order,
                          1, 1,
                          1, 1,
                          Get_Dir(dutyOfMotor[0]), Get_Dir(dutyOfMotor[1]),
                          Get_Dir(dutyOfMotor[2]), Get_Dir(dutyOfMotor[3]));
    Ms_Delay(5);
    ExpansionBoradControl(Duty_Change_Order, dutyOfMotor[4], 10000,
                          dutyOfBooster, dutyOfBooster,
                          (uint16_t)abs(dutyOfMotor[0]), (uint16_t)abs(dutyOfMotor[1]),
                          (uint16_t)abs(dutyOfMotor[2]), (uint16_t)abs(dutyOfMotor[3]));
    Ms_Delay(5);
    PWM_SET_Frequency(PWMB_CH1_P74, 50, dutyOfServo[0]);
    PWM_SET_Frequency(PWMB_CH4_P03, 50, dutyOfServo[1]);
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

/// @brief 板间通信函数，用于主 控给拓展版发送
/// @param control_cmd
/// @param data_p60 供弹电机
/// @param data_p62 空
/// @param data_p64 摩擦轮L
/// @param data_p66 摩擦轮R
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