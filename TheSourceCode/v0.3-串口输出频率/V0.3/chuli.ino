void count_freq()
{
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
    Freq = 1000000 / (re_diff * 8.4);
  else
    Freq = 0;
}
