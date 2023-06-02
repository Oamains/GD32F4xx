
#include "porting/lv_port_indev.h"
#if LV_BUILD_EXAMPLES && LV_USE_BTN

static void btn1_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
        static uint8_t cnt = 0;
        cnt++;

        /*Get the first child of the button which is the label and change its text*/
        lv_obj_t * label = lv_obj_get_child(btn, 0);
        lv_label_set_text_fmt(label, "Button: %d", cnt);
    }
}
static void btn2_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
        static uint8_t cnt = 0;
        cnt++;

        /*Get the first child of the button which is the label and change its text*/
        lv_obj_t * label = lv_obj_get_child(btn, 0);
        lv_label_set_text_fmt(label, "Button: %d", cnt);
    }
}
static void btn3_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
        static uint8_t cnt = 0;
        cnt++;

        /*Get the first child of the button which is the label and change its text*/
        lv_obj_t * label = lv_obj_get_child(btn, 0);
        lv_label_set_text_fmt(label, "Button: %d", cnt);
    }
}

/**
 * Create a button with a label and react on click event.
 */
void lv_example_get_started_1(void)
{
    //lv_style_set_text_font(&my_style, LV_STATE_DEFAULT, &lv_font_montserrat_28);
    
    lv_obj_t * btn1 = lv_btn_create(lv_scr_act());     /*Add a button the current screen*/
    lv_obj_set_pos(btn1, 60, 35);                            /*Set its position*/
    lv_obj_set_size(btn1, 120, 50);                          /*Set its size*/
    lv_obj_add_event_cb(btn1, btn1_event_cb, LV_EVENT_ALL, NULL);           /*Assign a callback to the button*/

    lv_obj_t * label1 = lv_label_create(btn1);          /*Add a label to the button*/
    lv_label_set_text(label1, "Button1");                     /*Set the labels text*/
    lv_obj_center(label1);


    lv_obj_t * btn2 = lv_btn_create(lv_scr_act());     /*Add a button the current screen*/
    lv_obj_set_pos(btn2, 60, 100);                            /*Set its position*/
    lv_obj_set_size(btn2, 120, 50);                          /*Set its size*/
    lv_obj_add_event_cb(btn2, btn2_event_cb, LV_EVENT_ALL, NULL);           /*Assign a callback to the button*/

    lv_obj_t * label2 = lv_label_create(btn2);          /*Add a label to the button*/
    lv_label_set_text(label2, "Button2");                     /*Set the labels text*/
    //lv_label_set_text_fmt();
    lv_obj_center(label2);


    lv_obj_t * btn3 = lv_btn_create(lv_scr_act());     /*Add a button the current screen*/
    lv_obj_set_pos(btn3, 60, 170);                            /*Set its position*/
    lv_obj_set_size(btn3, 120, 50);                          /*Set its size*/
    lv_obj_add_event_cb(btn3, btn3_event_cb, LV_EVENT_ALL, NULL);           /*Assign a callback to the button*/

    lv_obj_t * label3 = lv_label_create(btn3);          /*Add a label to the button*/
    lv_label_set_text(label3, "Button3");                     /*Set the labels text*/
    lv_obj_center(label3);


    lv_group_t *g=lv_group_create();
    lv_group_add_obj(g,btn1);
    lv_group_add_obj(g,btn2);
    lv_group_add_obj(g,btn3);
    lv_indev_set_group(indev_keypad,g);

}

#endif

