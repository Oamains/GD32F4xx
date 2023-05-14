#include "gd32f4xx.h"
#include "lcd.h"
#include "image2lcd.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK Mini STM32������
//FLASHͼƬ��ʾ		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/3/26
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
/**
 * ��8λ���ݻ��16λ��ɫ
 * mode:0,��λ��ǰ,��λ�ں�.
 * 1,��λ��ǰ,��λ�ں�.
 * str:����
 * @param mode
 * @param str
 * @return
 */
uint16_t image_getcolor(uint8_t mode, uint8_t *str) {
    uint16_t color;
    if (mode) {
        color = ((uint16_t) *str++) << 8;
        color |= *str;
    } else {
        color = *str++;
        color |= ((uint16_t) *str) << 8;
    }
    return color;
}

/**
 * ��Һ���ϻ�ͼ����֧�֣������ң����ϵ��� or ���ϵ��£������� ��ɨ�跽ʽ����
 * xsta,ysta,width,height:��ͼ����
 * *p:ͼ������
 * @param xsta
 * @param ysta
 * @param width
 * @param height
 * @param scan
 * @param p
 */
void image_show(uint16_t xsta, uint16_t ysta, uint16_t width, uint16_t height, uint8_t scan, uint8_t *p) {
    uint32_t i;
    uint32_t len = 0;

    LCD_Scan_Dir(L2R_U2D);//���ϵ���,������
    LCD_Set_Window(xsta, ysta, width, height);
    LCD_SetCursor(xsta, ysta);//���ù��λ��

    LCD_WriteRAM_Prepare();    //��ʼд��GRAM
    len = width * height;            //д������ݳ���
    for (i = 0; i < len; i++) {
        LCD_WriteRAM(image_getcolor(scan & (1 << 4), p));
        p += 2;
    }
    LCD_Set_Window(0, 0, lcddev.width, lcddev.height);
}

/**
 * ��ָ����λ����ʾһ��ͼƬ
 * �˺���������ʾimage2lcd������ɵ�����16λ���ɫͼƬ.
 * ����:
 *      1,�ߴ粻�ܳ�����Ļ������.
 *      2,��������ʱ���ܹ�ѡ:��λ��ǰ(MSB First)
 *      3,�������ͼƬ��Ϣͷ����
 * ע��:���STM32,����ѡ��image2lcd��"��λ��ǰ(MSB First)"ѡ��,����imginfo�����ݽ�����ȷ!!
 * @param x ָ��λ��
 * @param y ָ��λ��
 * @param imgx  ͼƬ����(�������ͼƬ��Ϣͷ,"4096ɫ/16λ���ɫ/18λ���ɫ/24λ���ɫ/32λ���ɫ����ͼ������ͷ)
 */
void image_display(uint16_t x, uint16_t y, uint8_t *imgx) {
    HEADCOLOR *imginfo;
    uint8_t ifosize = sizeof(HEADCOLOR);//�õ�HEADCOLOR�ṹ��Ĵ�С
    imginfo = (HEADCOLOR *) imgx;
    image_show(x, y, imginfo->w, imginfo->h, imginfo->scan, imgx + ifosize);
}















