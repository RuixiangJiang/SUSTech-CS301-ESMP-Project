# ESMP-Project

## Update Log
- [20233/11/22] Jiacheng Luo: Initial screen design
  
  (a) Due to compilation issues, all files unrelated to the stm32 project were moved to the `Project > calc` directory.

  (b) The initial screen is as shown in the figure (if you need to add components in the future, you can add their logo on a new line), using a button as the home key and a button for calibrating the resistive touch screen.
  
  Learn more through the code in `utils.c` .

<img src="https://raw.githubusercontent.com/Maystern/picbed/main/278EE8593A315DE9F5F38BB3234D9035.jpg" alt="278EE8593A315DE9F5F38BB3234D9035" style="zoom:25%;" />

```c
  // some code in utils.c (note there are some modifications in the later version after 2023/11/21)
  // used for learning the screen's structure
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

***TODO (Due: 2023/11/24)***: (a) Generate corresponding configuration files for each user to achieve 'plug and play' (b) Implement touch screen press to jump, press button home key to return. (c) Realize the complete function of calculator assembly (30% work of the project) 

- [2023/11/22] Jiacheng Luo: (a) you can find configs below `// config for the users` in  `main.c`, and use your own information to build the project. (b) after run on the stm32 board, you can click the three logos, if you try, you will find that the screen will point to the corresponding part.   

i.e.

```c
... // some code in the "main.c"

/* USER CODE BEGIN PTD */
// config for the users
unsigned char user_name[20] = "Alice";
struct TIME_SETTING time_setting = {2023, 11, 23, 0, 5, 11};
uint16_t mark_seed = 0x1674; // used to check if the RTC is initialized (if you want to reset the RTC, change this value)
/* USER CODE END PTD */

...
```

***TODO (Due: 2023/11/24)***: (c) Realize the complete function of calculator assembly (30% work of the project) ***

