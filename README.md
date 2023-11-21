# ESMP-Project

## Update Log
- [20233/11/22] Jiacheng Luo: Initial screen design
  
  (a) Due to compilation issues, all files unrelated to the stm32 project were moved to the `Project > calc` directory.

  (b) The initial screen is as shown in the figure (if you need to add components in the future, you can add their logo on a new line), using a button as the home key and a button for calibrating the resistive touch screen.
  
  Learn more through the code in `utils.c` .

<img src="https://s2.loli.net/2023/11/22/ofV9Iah4S5bUiEj.jpg" alt="QQ20231122043549.jpg" style="zoom: 25%;" />



```c
  void draw_initial_screen(uint8_t* username) {
    LCD_Clear(WHITE);
    POINT_COLOR = BLACK;
    LCD_ShowString((240 - 8 * 12) / 2, 50, 200, 24, 24, (uint8_t*) "23:59:59"); // show text in middle of the line
    LCD_ShowString((240 - 14 * 8) / 2, 85, 200, 16, 16, (uint8_t*) "2023/11/22 WED");
    char buffer[1024];
    sprintf(buffer, "user: [%s]", username);
    LCD_ShowString((240 - strlen(buffer) * 8) / 2, 110, 200, 16, 16, (uint8_t*) buffer);
    LCD_ShowPicture(10, 150, 60, 57, (uint16_t *) gImage_chat);
    LCD_ShowPicture(90, 150, 60, 60, (uint16_t *) gImage_calc);
    LCD_ShowPicture(170, 150, 60, 59, (uint16_t *) gImage_pic);
}
```
