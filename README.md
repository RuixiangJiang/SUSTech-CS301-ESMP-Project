# ESMP-Project

## Update Log
- [2023/11/22] Initial screen design
  
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

```c
// for each 1s, we will update the time info (some code in the utils.c)
void update_time() {
	// the screen is 320 (height) * 240 (width)
	RTC_Get();
	if (screen_state == INITIAL) {
		POINT_COLOR = BLACK;
		char now_time[20], now_data[20];
		sprintf(now_time, "%02d:%02d:%02d", calendar.hour, calendar.min, calendar.sec);
		sprintf(now_data, "%04d/%02d/%02d %s", calendar.w_year, calendar.w_month, calendar.w_date, getDayOfWeekString(calendar.week));
		LCD_ShowString((240 - strlen(now_time) * 12) / 2, 50, 200, 24, 24, (uint8_t*) now_time);
		LCD_ShowString((240 - strlen(now_data) * 8) / 2, 85, 200, 16, 16, (uint8_t*) now_data);
	}
}

```

***TODO (Due: 2023/11/24)***: (a) Generate corresponding configuration files for each user to achieve 'plug and play' (b) Implement touch screen press to jump, press button home key to return. (c) Realize the complete function of calculator assembly (30% work of the project) 

- [2023/11/22] (a) you can find configs below `// config for the users` in  `main.c`, and use your own information to build the project. (b) after run on the stm32 board, you can click the three logos, if you try, you will find that the screen will point to the corresponding part.   

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

***TODO (Due: 2023/11/24)***: (c) Realize the complete function of calculator assembly (30% work of the project)


- [2023/11/24]  finish the calculator component! (***Please double check it, if you have found some bugs, welcome to contact!**)

![image-20231124031649201](https://raw.githubusercontent.com/Maystern/picbed/main/image-20231124031649201.png)

1. Click "calculator" logo in the home screen to go into the calculator part.
2. [left]/[right] button for sliding the equation window (if the equation is very long, you can use it moving left or right to see the whole input equation).
3. You can use "change" button to switch the function of the calculator. ***i.e.*** decimal computation / equation solving / binary computation.
4. In the decimal or binary computation part, once you click the "=" button, the result will be calculated, while in the equation solving part, when you input the second "=", the equation will be solved.
