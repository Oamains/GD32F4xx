#include "lcd.h"
#include "font.h"
//////////////////////////////////////////////////////////////////////////////////	 
uint8_t DFT_SCAN_DIR;
//////////////////////////////////////////////////////////////////////////////////	 

//LCD的画笔颜色和背景色	   
uint16_t POINT_COLOR = 0x0000;    //画笔颜色
uint16_t BACK_COLOR = 0xFFFF;  //背景色

//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;

//写寄存器函数
//regval:寄存器值
void LCD_WR_REG(uint8_t REG) {
    uint8_t i;
    LCD_CS = 0;
    LCD_RS = 0;
    for (i = 0; i < 8; i++) {
        if (REG & 0x80)
            LCD_SDA = 1;
        else
            LCD_SDA = 0;
        REG <<= 1;
        LCD_SCL = 0;
        LCD_SCL = 1;
    }//送低8位
    LCD_CS = 1;
}

//写LCD数据
//data:要写入的值
void LCD_WR_DATA(uint16_t DATA) {
    uint8_t i;
    LCD_CS = 0;
    LCD_RS = 1;
    for (i = 0; i < 8; i++) {
        if (DATA & 0x80)
            LCD_SDA = 1;
        else
            LCD_SDA = 0;
        DATA <<= 1;
        LCD_SCL = 0;
        LCD_SCL = 1;
    }//送低8位
    LCD_CS = 1;
}

//写寄存器
//LCD_Reg:寄存器地址
//LCD_RegValue:要写入的数据
void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue) {
    LCD_WR_REG(LCD_Reg);
    LCD_WR_DATA(LCD_RegValue);
}

//开始写GRAM
void LCD_WriteRAM_Prepare(void) {
    LCD_WR_REG(lcddev.wramcmd);
}

//LCD写GRAM
//RGB_Code:颜色值
void LCD_WriteRAM(uint16_t DAT) {
    uint8_t i;
    LCD_CS = 0;
    LCD_RS = 1;
    for (i = 0; i < 16; i++) {
        if (DAT & 0x8000)
            LCD_SDA = 1;
        else
            LCD_SDA = 0;
        DAT <<= 1;
        LCD_SCL = 0;
        LCD_SCL = 1;
    }
    LCD_CS = 1;
}

//当mdk -O1时间优化时需要设置
//延时i
void opt_delay(uint8_t i) {
    while (i--);
}

//设置光标位置
//Xpos:横坐标
//Ypos:纵坐标
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos) {
    LCD_WR_REG(lcddev.setxcmd);
    LCD_WR_DATA(Xpos >> 8);
    LCD_WR_DATA(Xpos & 0XFF);

    LCD_WR_REG(lcddev.setycmd);
    LCD_WR_DATA(Ypos >> 8);
    LCD_WR_DATA(Ypos & 0XFF);
}

//设置LCD的自动扫描方向
//注意:其他函数可能会受到此函数设置的影响(尤其是9341/6804这两个奇葩),
//所以,一般设置为L2R_U2D即可,如果设置为其他扫描方式,可能导致显示不正常.
//dir:0~7,代表8个方向(具体定义见lcd.h)
//9320/9325/9328/4531/4535/1505/b505/8989/5408/9341等IC已经实际测试	   	   
void LCD_Scan_Dir(uint8_t dir) {
    uint16_t regval = 0;
    uint8_t dirreg = 0;
    uint16_t temp;
    switch (dir)//方向转换
    {
        case 0:
            dir = 6;
            break;
        case 1:
            dir = 7;
            break;
        case 2:
            dir = 4;
            break;
        case 3:
            dir = 5;
            break;
        case 4:
            dir = 1;
            break;
        case 5:
            dir = 0;
            break;
        case 6:
            dir = 3;
            break;
        case 7:
            dir = 2;
            break;
    }
    switch (dir) {
        case L2R_U2D://从左到右,从上到下
            regval |= (0 << 7) | (0 << 6) | (0 << 5);
            break;
        case L2R_D2U://从左到右,从下到上
            regval |= (1 << 7) | (0 << 6) | (0 << 5);
            break;
        case R2L_U2D://从右到左,从上到下
            regval |= (0 << 7) | (1 << 6) | (0 << 5);
            break;
        case R2L_D2U://从右到左,从下到上
            regval |= (1 << 7) | (1 << 6) | (0 << 5);
            break;
        case U2D_L2R://从上到下,从左到右
            regval |= (0 << 7) | (0 << 6) | (1 << 5);
            break;
        case U2D_R2L://从上到下,从右到左
            regval |= (0 << 7) | (1 << 6) | (1 << 5);
            break;
        case D2U_L2R://从下到上,从左到右
            regval |= (1 << 7) | (0 << 6) | (1 << 5);
            break;
        case D2U_R2L://从下到上,从右到左
            regval |= (1 << 7) | (1 << 6) | (1 << 5);
            break;
    }
    dirreg = 0X36;
    regval |= 0x08;
    LCD_WriteReg(dirreg, regval);

    LCD_WR_REG(lcddev.setxcmd);
    LCD_WR_DATA(0);
    LCD_WR_DATA(0);
    LCD_WR_DATA((lcddev.width - 1) >> 8);
    LCD_WR_DATA((lcddev.width - 1) & 0XFF);
    LCD_WR_REG(lcddev.setycmd);
    LCD_WR_DATA(0);
    LCD_WR_DATA(0);
    LCD_WR_DATA((lcddev.height - 1) >> 8);
    LCD_WR_DATA((lcddev.height - 1) & 0XFF);


}

//画点
//x,y:坐标
//POINT_COLOR:此点的颜色
void LCD_DrawPoint(uint16_t x, uint16_t y) {
    LCD_SetCursor(x, y);        //设置光标位置
    LCD_WriteRAM_Prepare();    //开始写入GRAM
    LCD_WriteRAM(POINT_COLOR);
}

//设置LCD显示方向（6804不支持横屏显示）
//dir:0,竖屏；1,横屏
void LCD_Display_Dir(uint8_t dir) {
    if (dir == 0)            //竖屏
    {
        lcddev.dir = 0;    //竖屏
        lcddev.width = 240;
        lcddev.height = 320;

        lcddev.wramcmd = 0X2C;
        lcddev.setxcmd = 0X2A;
        lcddev.setycmd = 0X2B;
        DFT_SCAN_DIR = U2D_R2L;

    } else                //横屏
    {
        lcddev.dir = 1;    //横屏
        lcddev.width = 320;
        lcddev.height = 240;

        lcddev.wramcmd = 0X2C;
        lcddev.setxcmd = 0X2A;
        lcddev.setycmd = 0X2B;
        DFT_SCAN_DIR = L2R_U2D;

    }
    LCD_Scan_Dir(DFT_SCAN_DIR);    //默认扫描方向
}

void Lcd_Gpio_Init() {
    //SCL
    rcu_periph_clock_enable(RCU_GPIOF);
    gpio_mode_set(GPIOF, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_7);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);

    //SDA
    rcu_periph_clock_enable(RCU_GPIOG);
    gpio_mode_set(GPIOF, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_9);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);


    rcu_periph_clock_enable(RCU_GPIOB);
    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_15);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_15);

    rcu_periph_clock_enable(RCU_GPIOB);
    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_11);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11);

    rcu_periph_clock_enable(RCU_GPIOD);
    gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_4);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4);

    rcu_periph_clock_enable(RCU_GPIOC);
    gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_7);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
}

//初始化lcd
//该初始化函数可以初始化各种ILI93XX液晶,但是其他函数是基于ILI9320的!!!
//在其他型号的驱动芯片上没有测试! 
void LCD_Init(void) {
    Lcd_Gpio_Init();
    //LCD复位
    LCD_RESET = 1;
    delay_1ms(1);
    LCD_RESET = 0;
    delay_1ms(80);
    LCD_RESET = 1;
    delay_1ms(10);
    //---------------------------------------------------------------------------------------------------//
    LCD_WR_REG(0x11);
    delay_1ms(120); //Delay 120ms
    //------------------------------display and color format setting--------------------------------//
    LCD_WR_REG(0x36);
    LCD_WR_DATA(0x00);
    LCD_WR_REG(0x3a);
    LCD_WR_DATA(0x05);
    //--------------------------------ST7789V Frame rate setting----------------------------------//
    LCD_WR_REG(0xb2);
    LCD_WR_DATA(0x0c);
    LCD_WR_DATA(0x0c);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x33);
    LCD_WR_DATA(0x33);
    LCD_WR_REG(0xb7);
    LCD_WR_DATA(0x35);
    //---------------------------------ST7789V Power setting--------------------------------------//
    LCD_WR_REG(0xbb);
    LCD_WR_DATA(0x28);
    LCD_WR_REG(0xc0);
    LCD_WR_DATA(0x2c);
    LCD_WR_REG(0xc2);
    LCD_WR_DATA(0x01);
    LCD_WR_REG(0xc3);
    LCD_WR_DATA(0x0b);
    LCD_WR_REG(0xc4);
    LCD_WR_DATA(0x20);
    LCD_WR_REG(0xc6);
    LCD_WR_DATA(0x0f);
    LCD_WR_REG(0xd0);
    LCD_WR_DATA(0xa4);
    LCD_WR_DATA(0xa1);
    //--------------------------------ST7789V gamma setting---------------------------------------//
    LCD_WR_REG(0xe0);
    LCD_WR_DATA(0xd0);
    LCD_WR_DATA(0x01);
    LCD_WR_DATA(0x08);
    LCD_WR_DATA(0x0f);
    LCD_WR_DATA(0x11);
    LCD_WR_DATA(0x2a);
    LCD_WR_DATA(0x36);
    LCD_WR_DATA(0x55);
    LCD_WR_DATA(0x44);
    LCD_WR_DATA(0x3a);
    LCD_WR_DATA(0x0b);
    LCD_WR_DATA(0x06);
    LCD_WR_DATA(0x11);
    LCD_WR_DATA(0x20);
    LCD_WR_REG(0xe1);
    LCD_WR_DATA(0xd0);
    LCD_WR_DATA(0x02);
    LCD_WR_DATA(0x07);
    LCD_WR_DATA(0x0a);
    LCD_WR_DATA(0x0b);
    LCD_WR_DATA(0x18);
    LCD_WR_DATA(0x34);
    LCD_WR_DATA(0x43);
    LCD_WR_DATA(0x4a);
    LCD_WR_DATA(0x2b);
    LCD_WR_DATA(0x1b);
    LCD_WR_DATA(0x1c);
    LCD_WR_DATA(0x22);
    LCD_WR_DATA(0x1f);
    LCD_WR_REG(0x29);
    LCD_WR_REG(0x2c);


    LCD_Display_Dir(0);            //默认为竖屏
    LCD_BL = 1;                    //点亮背光
    LCD_Clear(WHITE);
}

//清屏函数
//color:要清屏的填充色
void LCD_Clear(uint16_t color) {
    uint32_t index = 0;
    uint32_t totalpoint = lcddev.width;
    totalpoint *= lcddev.height;    //得到总点数
    LCD_SetCursor(0x00, 0x0000);    //设置光标位置
    LCD_WriteRAM_Prepare();     //开始写入GRAM
    for (index = 0; index < totalpoint; index++) {
        LCD_WriteRAM(color);
    }
}

//在指定区域内填充单个颜色
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color) {
    uint16_t i, j;
    uint16_t xlen = 0;
    xlen = ex - sx + 1;
    for (i = sy; i <= ey; i++) {
        LCD_SetCursor(sx, i);                    //设置光标位置
        LCD_WriteRAM_Prepare();                //开始写入GRAM
        for (j = 0; j < xlen; j++)LCD_WriteRAM(color);    //设置光标位置
    }
}

//在指定区域内填充指定颜色块			 
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD_Color_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color) {
    uint16_t height, width;
    uint16_t i, j;
    width = ex - sx + 1;        //得到填充的宽度
    height = ey - sy + 1;        //高度
    for (i = 0; i < height; i++) {
        LCD_SetCursor(sx, sy + i);    //设置光标位置
        LCD_WriteRAM_Prepare();     //开始写入GRAM
        for (j = 0; j < width; j++)LCD_WriteRAM(color[i * height + j]);//写入数据
    }
}

//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    delta_x = x2 - x1; //计算坐标增量
    delta_y = y2 - y1;
    uRow = x1;
    uCol = y1;
    if (delta_x > 0)incx = 1; //设置单步方向
    else if (delta_x == 0)incx = 0;//垂直线
    else {
        incx = -1;
        delta_x = -delta_x;
    }
    if (delta_y > 0)incy = 1;
    else if (delta_y == 0)incy = 0;//水平线
    else {
        incy = -1;
        delta_y = -delta_y;
    }
    if (delta_x > delta_y)distance = delta_x; //选取基本增量坐标轴
    else distance = delta_y;
    for (t = 0; t <= distance + 1; t++)//画线输出
    {
        LCD_DrawPoint(uRow, uCol);//画点
        xerr += delta_x;
        yerr += delta_y;
        if (xerr > distance) {
            xerr -= distance;
            uRow += incx;
        }
        if (yerr > distance) {
            yerr -= distance;
            uCol += incy;
        }
    }
}

//画矩形	  
//(x1,y1),(x2,y2):矩形的对角坐标
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    LCD_DrawLine(x1, y1, x2, y1);
    LCD_DrawLine(x1, y1, x1, y2);
    LCD_DrawLine(x1, y2, x2, y2);
    LCD_DrawLine(x2, y1, x2, y2);
}

//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r) {
    int a, b;
    int di;
    a = 0;
    b = r;
    di = 3 - (r << 1);             //判断下个点位置的标志
    while (a <= b) {
        LCD_DrawPoint(x0 + a, y0 - b);             //5
        LCD_DrawPoint(x0 + b, y0 - a);             //0
        LCD_DrawPoint(x0 + b, y0 + a);             //4
        LCD_DrawPoint(x0 + a, y0 + b);             //6
        LCD_DrawPoint(x0 - a, y0 + b);             //1
        LCD_DrawPoint(x0 - b, y0 + a);
        LCD_DrawPoint(x0 - a, y0 - b);             //2
        LCD_DrawPoint(x0 - b, y0 - a);             //7
        a++;
        //使用Bresenham算法画圆
        if (di < 0)di += 4 * a + 6;
        else {
            di += 10 + 4 * (a - b);
            b--;
        }
    }
}

//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16
//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar(uint16_t x, uint16_t y, uint8_t num, uint8_t size, uint8_t mode) {
    uint8_t temp, t1, t;
    uint16_t y0 = y;
    uint16_t colortemp = POINT_COLOR;
    //设置窗口
    num = num - ' ';//得到偏移后的值
    if (!mode) //非叠加方式
    {
        for (t = 0; t < size; t++) {
            if (size == 12)temp = asc2_1206[num][t];  //调用1206字体
            else temp = asc2_1608[num][t];         //调用1608字体
            for (t1 = 0; t1 < 8; t1++) {
                if (temp & 0x80)POINT_COLOR = colortemp;
                else POINT_COLOR = BACK_COLOR;
                LCD_DrawPoint(x, y);
                temp <<= 1;
                y++;
                if (x >= lcddev.width) {
                    POINT_COLOR = colortemp;
                    return;
                }//超区域了
                if ((y - y0) == size) {
                    y = y0;
                    x++;
                    if (x >= lcddev.width) {
                        POINT_COLOR = colortemp;
                        return;
                    }//超区域了
                    break;
                }
            }
        }
    } else//叠加方式
    {
        for (t = 0; t < size; t++) {
            if (size == 12)temp = asc2_1206[num][t];  //调用1206字体
            else temp = asc2_1608[num][t];         //调用1608字体
            for (t1 = 0; t1 < 8; t1++) {
                if (temp & 0x80)LCD_DrawPoint(x, y);
                temp <<= 1;
                y++;
                if (x >= lcddev.height) {
                    POINT_COLOR = colortemp;
                    return;
                }//超区域了
                if ((y - y0) == size) {
                    y = y0;
                    x++;
                    if (x >= lcddev.width) {
                        POINT_COLOR = colortemp;
                        return;
                    }//超区域了
                    break;
                }
            }
        }
    }
    POINT_COLOR = colortemp;
}

//m^n函数
//返回值:m^n次方.
uint32_t LCD_Pow(uint8_t m, uint8_t n) {
    uint32_t result = 1;
    while (n--)result *= m;
    return result;
}

//显示数字,高位为0,则不显示
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//color:颜色 
//num:数值(0~4294967295);	 
void LCD_ShowNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size) {
    uint8_t t, temp;
    uint8_t enshow = 0;
    for (t = 0; t < len; t++) {
        temp = (num / LCD_Pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1)) {
            if (temp == 0) {
                LCD_ShowChar(x + (size / 2) * t, y, ' ', size, 0);
                continue;
            } else enshow = 1;

        }
        LCD_ShowChar(x + (size / 2) * t, y, temp + '0', size, 0);
    }
}

//显示数字,高位为0,还是显示
//x,y:起点坐标
//num:数值(0~999999999);	 
//len:长度(即要显示的位数)
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
void LCD_ShowxNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint8_t mode) {
    uint8_t t, temp;
    uint8_t enshow = 0;
    for (t = 0; t < len; t++) {
        temp = (num / LCD_Pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1)) {
            if (temp == 0) {
                if (mode & 0X80)LCD_ShowChar(x + (size / 2) * t, y, '0', size, mode & 0X01);
                else LCD_ShowChar(x + (size / 2) * t, y, ' ', size, mode & 0X01);
                continue;
            } else enshow = 1;

        }
        LCD_ShowChar(x + (size / 2) * t, y, temp + '0', size, mode & 0X01);
    }
}

//显示字符串
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//*p:字符串起始地址		  
void LCD_ShowString(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, uint8_t *p) {
    uint8_t x0 = x;
    width += x;
    height += y;
    while ((*p <= '~') && (*p >= ' '))//判断是不是非法字符!
    {
        if (x >= width) {
            x = x0;
            y += size;
        }
        if (y >= height)break;//退出
        LCD_ShowChar(x, y, *p, size, 0);
        x += size / 2;
        p++;
    }
}

//设置窗口
void LCD_Set_Window(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height) {
    width = sx + width - 1;
    height = sy + height - 1;

    LCD_WR_REG(lcddev.setxcmd);
    LCD_WR_DATA(sx >> 8);
    LCD_WR_DATA(sx & 0XFF);
    LCD_WR_DATA(width >> 8);
    LCD_WR_DATA(width & 0XFF);
    LCD_WR_REG(lcddev.setycmd);
    LCD_WR_DATA(sy >> 8);
    LCD_WR_DATA(sy & 0XFF);
    LCD_WR_DATA(height >> 8);
    LCD_WR_DATA(height & 0XFF);
}




















