//以最快的方式读取AD的方法
#define VCC_REFER  5.0                //基准电压
uint8_t ad_mid;                       //中间电压的AD值
float v_max, v_min, v_mid, vpp;       //最大电压 最小电压 中间电压 电压宽度
uint32_t Freq;                        //频率
int re1, re2, re_diff;                //第一个上升沿的位置，第二个上升沿位置，位置差值
#define SAMPLING_FREQUENCY 500        //采样次数
uint8_t Buffer[SAMPLING_FREQUENCY];   //采样缓存数组
volatile uint16_t sampling_time = 40; //采样时间
void setup()
{
  Serial.begin(115200);
  //ADMUX = 0xe5;  //1110 0101 , //基准电压选择1.1v 选择A5作为输入
  ADMUX = 0x66;  //0110 0101 , //基准电压选择vcc的电压 选择A6作为输入
  //ADC采样频率 16000000/4 = 4000000, 13.5/4000000 = 3.375us
  bitWrite(ADCSRA, ADPS2, 0);
  bitWrite(ADCSRA, ADPS1, 1);
  bitWrite(ADCSRA, ADPS0, 0);
  /*
              7     6     5      4    3     2      1      0

    ADCSRA   ADEN  ADSC  ADATE  ADIF ADIE  ADPS2  ADPS1  ADPS0
    ADMUX    REFS1 REFS0 ADLAR  0    MUX3  MUX2   MUX1   MUX0
  */
  Serial.print("ADMUX "); read_register(ADMUX);  //1110 0101
  Serial.print("ADCSRA ");  read_register(ADCSRA); //1000 0010
  analogWrite(11, 126); //11引脚输出50%490HZ的方波
}

void loop()
{
  for (uint16_t i = 0; i < SAMPLING_FREQUENCY; i++)
  {
    bitWrite(ADCSRA, ADSC, 1);      //ADSC写1启动AD转换
    while (!(ADCSRA & 1 << ADIF)); //转换完成ADIF会置1
    Buffer[i] = ADCH;              //ADC转换得到的值
    //delayMicroseconds(4);
  }
  v_max = Buffer[0];
  v_min = Buffer[0];
  //找到最大最小值
  for (uint16_t i = 0; i < SAMPLING_FREQUENCY; i++)
  {
    if (Buffer[i] > v_max) v_max = Buffer[i];
    if (Buffer[i] < v_min) v_min = Buffer[i];
  }
  ad_mid = (v_max + v_min) / 2;         //计算中间的AD值
  v_max = v_max * (VCC_REFER / 255.0);  //计算最大电压
  v_min = v_min * (VCC_REFER / 255.0);  //计算最小电压
  v_mid = (v_max + v_min) / 2;          //计算中间电压
  vpp = (v_max - v_min);                //计算电压宽度
  count_freq(); //计算频率
  /*for (uint16_t i = 0; i < SAMPLING_FREQUENCY; i++)
    {
    if (Buffer[i] > v_mid) i1++;
    if (Buffer[i] < v_mid) i2++;
    }*/

  Serial.print("v_max:");
  Serial.println(v_max);
  Serial.print("v_min:");
  Serial.println(v_min);
  Serial.print("v_mid:");
  Serial.println(v_mid);
  Serial.print("vpp:");
  Serial.println(vpp);
  Serial.print("Freq:");
  Serial.println(Freq);
  Serial.println("------------");
  /*
    input_1_vcc = float(input_1_ad) * (VCC_REFER / 255.0);
    Serial.print("input_1_ad:");
    Serial.println(input_1_ad);
    Serial.print("input_1_vcc:");
    Serial.println(input_1_vcc);
    Serial.println();*/
  delay(100);
}

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
