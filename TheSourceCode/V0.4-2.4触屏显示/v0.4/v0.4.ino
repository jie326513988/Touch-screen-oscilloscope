#include <TimerOne.h> //定时器1
#include <TouchScreen.h> //触摸库
#include <LCDWIKI_GUI.h> //核心图形库
#include <LCDWIKI_KBV.h> //特定于硬件的库
LCDWIKI_KBV my_lcd(ILI9341, A3, A2, A1, A0, A4); //model,cs,cd,wr,rd,resett
#define YP A3  // must be an analog pin, use "An" notation! CS
#define XM A2  // must be an analog pin, use "An" notation! RS/cd
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin
//x+ x-的电阻值
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
// 我们有一个状态栏，显示FONA是否正常工作
#define STATUS_X 10
#define STATUS_Y 65
//触屏校准、设置相关
int16_t px, py; //p.x,p.y转换后的值
//搞点颜色
#define BLACK     0x0000          //黑色    0,   0,   0
#define NAVY      0x000F          //深蓝色  0,   0, 127
#define DGREEN    0x03E0          //深绿色  0,  127,  0
#define DCYAN     0x03EF          //深青色  0,  127, 127       
#define MAROON    0x7800          //深红色  127,   0,   0      
#define PURPLE    0x780F          //紫色    127,   0, 127      
#define OLIVE     0x7BE0          //橄榄绿  127, 127,   0      
#define LGRAY     0xC618          //灰白色  192, 192, 192      
#define DGRAY     0x7BEF          //深灰色  127, 127, 127      
#define BLUE      0x001F          //蓝色    0,   0, 255        
#define GREEN     0x07E0          //绿色    0, 255,   0        
#define CYAN      0x07FF          //青色    0, 255, 255        
#define RED       0xF800          //红色    255,   0,   0      
#define MAGENTA   0xF81F          //品红    255,   0, 255      
#define YELLOW    0xFFE0          //黄色    255, 255, 0        
#define WHITE     0xFFFF          //白色    255, 255, 255      
#define PINK      0xD2F5          //粉红色
#define fc1 PURPLE                //变量名 紫色
#define fc2 DCYAN                 //数值 深青色
//三个按钮的X轴坐标
#define incremental 60                       //间隔
#define add_key_x 45                         //加按钮
#define reduce_key_x add_key_x+incremental   //减按钮
#define hold_key_x reduce_key_x+incremental  //保存按钮
//显示、触屏标志位
boolean coordinates = 0;        //显示坐标的标志位
uint8_t display_state = 1;      //显示界面的标志位
uint8_t display_touch = 1;      //显示界面触摸的标志位
#define MainDisplay      1      //主界面
//时间操作
uint32_t key_time = 0;         //按键延迟的对比时间
//波形处理
#define VCC_REFER  5.0                //基准电压
uint8_t ad_mid;                       //中间电压的AD值
float v_max, v_min, v_mid, vpp;       //最大电压 最小电压 中间电压 电压宽度
uint32_t Freq;                        //频率
int re1, re2, re_diff;                //第一个上升沿的位置，第二个上升沿位置，位置差值
#define SAMPLING_FREQUENCY 500        //采样次数
uint8_t Buffer[SAMPLING_FREQUENCY];   //采样缓存数组
uint16_t sta;                         //波形变换
uint8_t Y1[SAMPLING_FREQUENCY / 2];   //处理后，上一次的波形存储
uint8_t Y0[SAMPLING_FREQUENCY / 2];   //处理后，现在的波形存储
volatile uint16_t ad_count = 0;       //中断次数记录
volatile uint16_t sampling_time = 40; //采样时间
boolean hold = 0;                     //保持按钮标志位

void setup()
{
  Serial.begin(115200);

  my_lcd.Init_LCD();         //屏幕初始化
  my_lcd.Fill_Screen(BLACK); //设置黑色背景
  my_lcd.Set_Rotation(1);    //设置屏幕的旋转旋转90度，即横屏显示

  //ADMUX = 0xe6;  //1110 0110 ,基准电压选择1.1v 选择A6作为输入
  //ADMUX = 0x66;  //0110 0110 ,基准电压选择vcc的电压 选择A6作为输入
  bitWrite(ADCSRA, ADPS2, 0);
  bitWrite(ADCSRA, ADPS1, 1);
  bitWrite(ADCSRA, ADPS0, 0);

  Serial.print("ADMUX "); read_register(ADMUX);     //0110 0101
  Serial.print("ADCSRA ");  read_register(ADCSRA);  //1000 0011
  Serial.print("ADCSRB ");  read_register(ADCSRB);  //0000 0000
  Serial.print("TCCR1A ");  read_register(TCCR1A);  //0000 0001
  Serial.print("TCCR1B ");  read_register(TCCR1B);  //0000 0011
  Serial.print("TCCR1C ");  read_register(TCCR1C);  //0000 0000
  Timer1.attachInterrupt(time1_sampling); // 设置中断的运行函数
  analogWrite(11, 126); //11引脚输出50%490HZ的方波
}

void loop()
{
  touch();
  main_display();

  /*Serial.print("v_max:");
    Serial.println(v_max);
    Serial.print("v_min:");
    Serial.println(v_min);
    Serial.print("v_mid:");
    Serial.println(v_mid);
    Serial.print("vpp:");
    Serial.println(vpp);
    Serial.println("------------");
  */
}
//读取指定寄存器的值，b=寄存器名称
void read_register(byte b)
{
  byte every[8];
  for (uint8_t i = 0; i < 8; i++)
  {
    every[i] = bitRead(b, i);
  }
  Serial.print(b);
  Serial.print(" ");
  for (int8_t i = 7; i >= 0; i--)
  {
    Serial.print(every[i]);
    if (i == 4) Serial.print(" ");
  }
  Serial.println();
  Serial.println();
}
//中断采样
void time1_sampling()
{
  ADMUX = 0x66;                   //设置基准电压为5V，启用A6端口输入
  //开始采样
  bitSet(ADCSRA, ADSC);           //ADSC写1启动AD转换
  while (!(ADCSRA & 1 << ADIF));  //转换完成ADIF会置1
  Buffer[ad_count] = ADCH;        //ADC转换得到的值
  ad_count++;
  if (ad_count == SAMPLING_FREQUENCY) Timer1.stop();
}
