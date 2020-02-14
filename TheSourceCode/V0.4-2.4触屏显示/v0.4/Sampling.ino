//采样
void sampling()
{
  //初始化
  ADMUX = 0x65;                //设置基准电压为5V，启用A5端口输入
  //bitWrite(ADCSRA, ADATE, 1);  //ADC自动触发启用
  /*ADCSRB = B00000101;          //定时计数1比较匹配B Timer/Counter1 Compare Match B
  OCR1B = 100;
  TCCR1A = B00000000;          //定时器1设置CTC模式 ，正常端口操作，OC1A/OC1B断开
  TCCR1B = B00001010;          //分频比为1 中断频率=16000000/分频比/OCR1B*/
  //开始采样
  for (uint16_t i = 0; i < SAMPLING_FREQUENCY; i++)
  {
    bitSet(ADCSRA, ADSC);           //ADSC写1启动AD转换
    while (!(ADCSRA & 1 << ADIF)); //转换完成ADIF会置1
    Buffer[i] = ADCH;              //ADC转换得到的值
    //delayMicroseconds(50);
  }
  //TCCR1B = B00001000;          //关闭中断
  //bitWrite(ADCSRA, ADATE, 0);  //ADC自动触发关闭
}
