void touch()
{
  TSPoint p = ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  if (p.z < 800 && p.z > 100 && p.x < 1020 && p.x > 60 && p.y < 1020 && p.y > 60)
  {
    py = map(p.x, 910, 125, 0, 240);
    px = map(p.y, 85, 895, 0, 320);
    uint16_t x = 260;
    uint8_t y = 155;
    if (coordinates == 1)
    {
      show_string("p.x", x, y, 1, fc1, BLACK, 1);
      show_string_no_fc("px", x + 30, y, 1, BLACK, 1);
      show_string_no_fc("p.y", x, y + 30 , 1, BLACK, 1);
      show_string_no_fc("py", x + 30, y + 30, 1, BLACK, 1);
      show_string_no_fc("p.z", x, y + 60, 1, BLACK, 1);

      show_number_int(p.x, x, y + 10, 1, fc2, BLACK  , 0);
      show_number_int(px, x + 30, y + 10, 1, fc2, BLACK  , 0);
      show_number_int(p.y, x, y + 40, 1, fc2, BLACK  , 0);
      show_number_int(py, x + 30, y + 40, 1, fc2, BLACK  , 0);
      show_number_int(p.z, x, y + 70, 1, fc2, BLACK  , 0);

    }
    if (int32_t(millis() - key_time) > 50)
    {
      if (px < add_key_x + 20 && px > add_key_x - 10 && py < 240 && py > 215) //按下+
      {
        sampling_time += 10;
        if (sampling_time == 25) sampling_time = 20;
        key_time = millis();
      }
      else if (px < reduce_key_x + 20 && px > reduce_key_x - 10 && py < 240 && py > 215) //按下-
      {
        sampling_time -= 10;
        if (sampling_time < 15) sampling_time = 15;
        key_time = millis();
      }
      else if (px < hold_key_x + 50 && px > hold_key_x - 10 && py < 240 && py > 215) //按下hold
      {
        hold = !hold;                //反转状态
        display_state = MainDisplay; //刷新界面
        if (hold == 0) Timer1.initialize(sampling_time); //启动中断，us
        //记录时间，1.2秒后才能按，防止按太快
        key_time = millis() ;
        key_time = key_time + 1200;
      }
      else if (px < 319 && px > x  && py < 239 && py > y) //按下坐标
      {
        coordinates = !coordinates;  //反转状态
        display_state = MainDisplay; //刷新界面
         key_time = millis() ;
        key_time = key_time + 1200;
      }
    }
  }
}
