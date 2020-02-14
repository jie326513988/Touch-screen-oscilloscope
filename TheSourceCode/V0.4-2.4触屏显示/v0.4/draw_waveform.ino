//画波形
void draw_waveform()
{
  if (ad_count == SAMPLING_FREQUENCY && hold == 0)
  {
    data_processing(); //计算参数
    //波形变换处理
    for (sta = 0; sta < SAMPLING_FREQUENCY / 2; sta++)
    {
      if (Buffer[sta] < ad_mid && Buffer[sta + 2] > ad_mid)
        break;
    }
    for (uint16_t i = 0; i < SAMPLING_FREQUENCY / 2; i++)
      Y0[i] =  160  - (Buffer[i + sta] >> 1); //暂时没搞明白
    //画波形方法
    for (uint16_t x = 1; x < (SAMPLING_FREQUENCY / 2 - 2); x++)
    {
      //清除上一次波形
      my_lcd.Set_Draw_color(BLACK);
      my_lcd.Draw_Line(x, Y1[x], x, Y1[x + 1]); //画线
      //画现在的波形
      my_lcd.Set_Draw_color(YELLOW);
      my_lcd.Draw_Line(x,  Y0[x], x, Y0[x + 1]); //画线
      Y1[x] = Y0[x]; //将现在的波形存储为旧波形
    }
    //画完波形重新开始采样
    if (hold == 0) {
      ad_count = 0;
      Timer1.initialize(sampling_time);  //启动中断，us
    }
  }
}
//计算参数
void data_processing()
{
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
  /*for (uint16_t i = 0; i < SAMPLING_FREQUENCY; i++)
    {
    if (Buffer[i] > v_mid) i1++;
    if (Buffer[i] < v_mid) i2++;
    }*/
  //计算频率的方法
  //找到第一个上升沿
  for (uint16_t i = 0; i < (SAMPLING_FREQUENCY / 2 - 1); i++)
  {
    if (Buffer[i] < ad_mid && Buffer[i + 1] >= ad_mid)
    {
      re1 = i;
      break;
    }
  }
  //找到第二个上升沿
  for (uint16_t i = re1 + 1; i < (SAMPLING_FREQUENCY / 2 + re1); i++)
  {
    if (Buffer[i] < ad_mid && Buffer[i + 1] >= ad_mid)
    {
      re2 = i;
      break;
    }
  }
  re_diff = re2 - re1; //计算两个上升沿的差值
  //通过差值计算出频率
  if (re_diff > 0)
    Freq = 1000000 / (re_diff * sampling_time);
  else
    Freq = 0;
}
