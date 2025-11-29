#include "main.h"
#include "MATH.H"
uint16_t i;
void     main()
{
    Board_Init();
    PWM_Init(PWMB_CH3_P33, 1000, 10000);
    while (1) {
        //======= AP Disconnected 747 =======
        for (i = 250; i < 700; i++) {
            PWM_SET_Frequency(PWMB_CH3_P33, i, 5000);
            Ms_Delay(1);
        }
        PWM_SET_Duty(PWMB_CH3_P33, 10000);
        Ms_Delay(10);
        for (i = 250; i < 700; i++) {
            PWM_SET_Frequency(PWMB_CH3_P33, i, 5000);
            Ms_Delay(1);
        }
        PWM_SET_Duty(PWMB_CH3_P33, 10000);
        Ms_Delay(500);

        // ====== Master Warning =======
        PWM_SET_Frequency(PWMB_CH3_P33, 300, 3000);
        Ms_Delay(210);
        PWM_SET_Frequency(PWMB_CH3_P33, 1000, 5000);
        Ms_Delay(210);
        PWM_SET_Frequency(PWMB_CH3_P33, 300, 3000);
        Ms_Delay(210);
        PWM_SET_Frequency(PWMB_CH3_P33, 1000, 5000);
        Ms_Delay(500);

        //======= AP Disconnected =======
        for (i = 200; i < 220; i++) {
            PWM_SET_Frequency(PWMB_CH3_P33, i, 5000);
            Ms_Delay(3);
        }
        for (i = 220; i < 240; i++) {
            PWM_SET_Frequency(PWMB_CH3_P33, i, 5000);
            Ms_Delay(4);
        }
        for (i = 240; i < 260; i++) {
            PWM_SET_Frequency(PWMB_CH3_P33, i, 5000);
            Ms_Delay(5);
        }
        for (i = 260; i < 300; i++) {
            PWM_SET_Frequency(PWMB_CH3_P33, i, 5000);
            Ms_Delay(10);
        }
        PWM_SET_Frequency(PWMB_CH3_P33, 10000, 10000);
        Ms_Delay(50);
        for (i = 200; i < 220; i++) {
            PWM_SET_Frequency(PWMB_CH3_P33, i, 5000);
            Ms_Delay(3);
        }
        for (i = 220; i < 240; i++) {
            PWM_SET_Frequency(PWMB_CH3_P33, i, 5000);
            Ms_Delay(4);
        }
        for (i = 240; i < 260; i++) {
            PWM_SET_Frequency(PWMB_CH3_P33, i, 5000);
            Ms_Delay(5);
        }
        for (i = 260; i < 300; i++) {
            PWM_SET_Frequency(PWMB_CH3_P33, i, 5000);
            Ms_Delay(10);
        }
        PWM_SET_Frequency(PWMB_CH3_P33, 10000, 10000);
        Ms_Delay(500);

        // ====== Pull UP ======
        for (i = 500; i < 1000; i++) {
            PWM_SET_Frequency(PWMB_CH3_P33, i, 5000);
            Us_Delay(800);
        }
        for (i = 500; i < 1000; i++) {
            PWM_SET_Frequency(PWMB_CH3_P33, i, 5000);
            Us_Delay(800);
        }
        PWM_SET_Duty(PWMB_CH3_P33, 10000);
        Ms_Delay(800);
        for (i = 500; i < 1000; i++) {
            PWM_SET_Frequency(PWMB_CH3_P33, i, 5000);
            Us_Delay(800);
        }
        for (i = 500; i < 1000; i++) {
            PWM_SET_Frequency(PWMB_CH3_P33, i, 5000);
            Us_Delay(800);
        }
        PWM_SET_Duty(PWMB_CH3_P33, 10000);
        Ms_Delay(800);

        // ====== Overspeed ======
        // PWM_SET_Frequency(PWMB_CH3_P33, 1800, 5000);
        // for (i = 8000; i < 5000; i -= 2) {
        //     PWM_SET_Duty(PWMB_CH3_P33, i);
        // }
        // Ms_Delay(20);
        // PWM_SET_Duty(PWMB_CH3_P33, 10000);
        // Ms_Delay(10);
        // PWM_SET_Frequency(PWMB_CH3_P33, 2000, 5000);
        // for (i = 8000; i < 5000; i -= 2) {
        //     PWM_SET_Duty(PWMB_CH3_P33, i);
        // }
        // Ms_Delay(20);
        // PWM_SET_Duty(PWMB_CH3_P33, 10000);
        // Ms_Delay(40);
    }
}