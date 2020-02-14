void main_display()
{
  uint16_t x = 270;
  uint8_t y = 5;
  if (display_state == MainDisplay)
  {
    //画框框
    my_lcd.Set_Draw_color(RED);
    my_lcd.Draw_Fast_HLine(0, 214, 248); //画水平线
    my_lcd.Draw_Fast_VLine(248, 0, 319); //画垂直线
    //打印参数名称
    my_lcd.Set_Text_colour(fc1);
    show_string_no_fc("vmax", x, y + 0, 1, BLACK, 0);
    show_string_no_fc("vmin", x, y + 30, 1, BLACK, 0);
    show_string_no_fc("vmid", x, y + 60, 1, BLACK, 0);
    show_string_no_fc("us", x + 6, y + 90, 1, BLACK, 0);
    show_string_no_fc("F(HZ)", x, y + 120, 1, BLACK, 0);
    //show_string_no_fc("hold", x, y + 150, 1, BLACK, 0);
    //画3个按钮
    my_lcd.Set_Text_colour(255, 105, 180); //热情的粉红
    show_string_no_fc("+", add_key_x, 219, 2, BLACK, 0);
    my_lcd.Set_Text_colour(255, 140, 0); //深橙色
    show_string_no_fc("-", reduce_key_x, 219, 2, BLACK, 0);
    if (hold == 1) {
      my_lcd.Set_Text_colour(124, 252, 0); //草坪绿
      show_string_no_fc("Hold", hold_key_x, 219, 2, BLACK, 0);
    }
    else {
      my_lcd.Set_Text_colour(DGRAY); //深灰色
      show_string_no_fc("Hold", hold_key_x, 219, 2, BLACK, 0);
    }
    if (coordinates == 0) {
      my_lcd.Fill_Rect(250, 155, 69, 84, BLACK);
      my_lcd.Set_Text_colour(135, 206, 235); //天蓝色
      show_string_no_fc("By lcj", 270, 200, 1, BLACK, 0);
      show_string_no_fc("v0.4", 270, 220, 1, BLACK, 0);
    }
    else my_lcd.Fill_Rect(250, 155, 69, 84, BLACK);

    display_state = 0;
  }
  //画出波形
  draw_waveform();
  //画坐标
  my_lcd.Set_Draw_color(LGRAY); //灰白色
  my_lcd.Draw_Fast_HLine(1, 107, 247); //画水平线
  my_lcd.Draw_Fast_VLine(124, 1, 213); //画垂直线
  //打印参数变量
  my_lcd.Set_Text_Mode(0);             //设置文本的重叠模式 0不重叠，1重叠
  my_lcd.Set_Text_Size(1);             //设置字体大小
  my_lcd.Set_Text_colour(fc2);        //设置字体颜色
  my_lcd.Set_Text_Back_colour(BLACK);  //设置字体背景颜色
  my_lcd.Print_Number_Float(v_max, 1, x, y + 10, '.', 0, ' ');
  my_lcd.Print_Number_Float(v_min, 1, x, y + 40, '.', 0, ' ');
  my_lcd.Print_Number_Float(v_mid, 2, x, y + 70, '.', 0, ' ');
  my_lcd.Print_Number_Int(sampling_time, x, y + 100, 5, ' ', 10);
  my_lcd.Print_Number_Int(Freq, x, y + 130, 6, '0', 10);
  /*my_lcd.Print_Number_Int(hold, x, y + 160, 4, ' ', 10);
    my_lcd.Print_Number_Int(millis(), x, y + 180, 4, ' ', 10);
    my_lcd.Print_Number_Int(key_time, x, y + 190, 4, ' ', 10);
    my_lcd.Print_Number_Int(millis() - key_time, x, y + 200, 4, ' ', 10);*/

}
