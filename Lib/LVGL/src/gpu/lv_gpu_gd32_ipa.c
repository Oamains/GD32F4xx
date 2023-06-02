/**
 * @file lv_gpu_gd32_ipa.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_gpu_gd32_ipa.h"
#include "../core/lv_disp.h"
#include "../core/lv_refr.h"

#if LV_USE_GPU_GD32_IPA

#include LV_GPU_IPA_CMSIS_INCLUDE

#define IPA_IMS_HEIGHT_POS                   0       /*!< height of the image to be processed */
#define IPA_IMS_WIDTH_POS                    16      /*!< width of the image to be processed */

/*********************
 *      DEFINES
 *********************/

#if LV_COLOR_16_SWAP
    // TODO: F7 has red blue swap bit in control register for all layers and output
    #error "Can't use IPA with LV_COLOR_16_SWAP 1"
#endif

#if LV_COLOR_DEPTH == 8
    #error "Can't use IPA with LV_COLOR_DEPTH == 8"
#endif

#if LV_COLOR_DEPTH == 16
    #define LV_IPA_COLOR_FORMAT LV_IPA_RGB565
#elif LV_COLOR_DEPTH == 32
    #define LV_IPA_COLOR_FORMAT LV_IPA_ARGB8888
#else
    /*Can't use GPU with other formats*/
#endif

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void invalidate_cache(void);
static void wait_finish(void);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Turn on the peripheral and set output color mode, this only needs to be done once
 */
void lv_gpu_gd32_ipa_init(void)
{
    /*Enable IPA clock*/
#if defined(GD32F4XX)
    rcu_periph_clock_enable(RCU_IPA);
#else
# warning "LVGL can't enable the clock of IPA"
#endif

	ipa_deinit();
	ipa_pixel_format_convert_mode_set(IPA_FGTODE);
    /*set output colour mode*/
    IPA_DPCTL = LV_IPA_COLOR_FORMAT;
}

/**
 * Fill an area in the buffer with a color
 * @param buf a buffer which should be filled
 * @param buf_w width of the buffer in pixels
 * @param color fill color
 * @param fill_w width to fill in pixels (<= buf_w)
 * @param fill_h height to fill in pixels
 * @note `buf_w - fill_w` is offset to the next line after fill
 */
void lv_gpu_gd32_ipa_fill(lv_color_t * buf, lv_coord_t buf_w, lv_color_t color, lv_coord_t fill_w, lv_coord_t fill_h)
{
    invalidate_cache();

    IPA_CTL = 0x30000;
    IPA_DMADDR = (uint32_t)buf;
    /*as input color mode is same as output we don't need to convert here do we?*/
    IPA_DPV = color.full;
    IPA_DLOFF = buf_w - fill_w;
    IPA_IMS = (fill_w << IPA_IMS_WIDTH_POS) | (fill_h << IPA_IMS_HEIGHT_POS);

    /*start transfer*/
    IPA_CTL |= IPA_CTL_TEN;

    wait_finish();
}

/**
 * Fill an area in the buffer with a color but take into account a mask which describes the opacity of each pixel
 * @param buf a buffer which should be filled using a mask
 * @param buf_w width of the buffer in pixels
 * @param color fill color
 * @param mask 0..255 values describing the opacity of the corresponding pixel. It's width is `fill_w`
 * @param opa overall opacity. 255 in `mask` should mean this opacity.
 * @param fill_w width to fill in pixels (<= buf_w)
 * @param fill_h height to fill in pixels
 * @note `buf_w - fill_w` is offset to the next line after fill
 */
void lv_gpu_gd32_ipa_fill_mask(lv_color_t * buf, lv_coord_t buf_w, lv_color_t color, const lv_opa_t * mask,
                                  lv_opa_t opa, lv_coord_t fill_w, lv_coord_t fill_h)
{
#if 0
    invalidate_cache();

    /*Configure the IPA Mode, Color Mode and line output offset*/
    hipa.Init.Mode         = IPA_M2M_BLEND;
    hipa.Init.ColorMode    = IPA_OUTPUT_FORMAT;
    hipa.Init.OutputOffset = buf_w - fill_w;

    /*Configure the foreground -> The character*/
    lv_color32_t c32;
    c32.full = lv_color_to32(color);
    c32.ch.alpha = opa;
    hipa.LayerCfg[1].AlphaMode       = IPA_COMBINE_ALPHA;
    hipa.LayerCfg[1].InputAlpha      = c32.full;
    hipa.LayerCfg[1].InputColorMode  = IPA_INPUT_A8;
    hipa.LayerCfg[1].InputOffset     = 0;

    /*Configure the background -> Display buffer*/
    hipa.LayerCfg[0].AlphaMode       = IPA_NO_MODIF_ALPHA;
    hipa.LayerCfg[0].InputAlpha      = 0x00;
    hipa.LayerCfg[0].InputColorMode  = IPA_INPUT_FORMAT;
    hipa.LayerCfg[0].InputOffset     = buf_w - fill_w;

    /*IPA Initialization*/
    HAL_IPA_Init(&hipa);
    HAL_IPA_ConfigLayer(&hipa, 0);
    HAL_IPA_ConfigLayer(&hipa, 1);
    HAL_IPA_BlendingStart(&hipa, (uint32_t) mask, (uint32_t) buf, (uint32_t)buf, fill_w, fill_h);
    wait_finish();
#endif
}

/**
 * Copy a map (typically RGB image) to a buffer
 * @param buf a buffer where map should be copied
 * @param buf_w width of the buffer in pixels
 * @param map an "image" to copy
 * @param map_w width of the map in pixels
 * @param copy_w width of the area to copy in pixels (<= buf_w)
 * @param copy_h height of the area to copy in pixels
 * @note `map_w - fill_w` is offset to the next line after copy
 */
void lv_gpu_gd32_ipa_copy(lv_color_t * buf, lv_coord_t buf_w, const lv_color_t * map, lv_coord_t map_w,
                             lv_coord_t copy_w, lv_coord_t copy_h)
{
    invalidate_cache();

    IPA_CTL = 0;
    /*copy output colour mode, this register controls both input and output colour format*/
    IPA_FPCTL = LV_IPA_COLOR_FORMAT;
    IPA_FMADDR = (uint32_t)map;
    IPA_FLOFF = map_w - copy_w;
    IPA_DMADDR = (uint32_t)buf;
    IPA_DLOFF = buf_w - copy_w;
    IPA_IMS = (copy_w << IPA_IMS_WIDTH_POS) | (copy_h << IPA_IMS_HEIGHT_POS);

    /*start transfer*/
    IPA_CTL |= IPA_CTL_TEN;
    wait_finish();
}

/**
 * Blend a map (e.g. ARGB image or RGB image with opacity) to a buffer
 * @param buf a buffer where `map` should be copied
 * @param buf_w width of the buffer in pixels
 * @param map an "image" to copy
 * @param opa opacity of `map`
 * @param map_w width of the map in pixels
 * @param copy_w width of the area to copy in pixels (<= buf_w)
 * @param copy_h height of the area to copy in pixels
 * @note `map_w - fill_w` is offset to the next line after copy
 */
void lv_gpu_gd32_ipa_blend(lv_color_t * buf, lv_coord_t buf_w, const lv_color_t * map, lv_opa_t opa,
                              lv_coord_t map_w, lv_coord_t copy_w, lv_coord_t copy_h)
{
    invalidate_cache();
    IPA_CTL = 0x20000;

    IPA_BPCTL = LV_IPA_COLOR_FORMAT;
    IPA_BMADDR = (uint32_t)buf;
    IPA_BLOFF = buf_w - copy_w;

    IPA_FPCTL = (uint32_t)LV_IPA_COLOR_FORMAT
                     /*alpha mode 2, replace with foreground * alpha value*/
                     | (2 << 16)
                     /*alpha value*/
                     | (opa << 24);
    IPA_FMADDR = (uint32_t)map;
    IPA_FLOFF = map_w - copy_w;

    IPA_DMADDR = (uint32_t)buf;
    IPA_DLOFF = buf_w - copy_w;
    IPA_IMS = (copy_w << IPA_IMS_WIDTH_POS) | (copy_h << IPA_IMS_HEIGHT_POS);

    /*start transfer*/
    IPA_CTL |= IPA_CTL_TEN;
    wait_finish();
}

void lv_gpu_gd32_ipa_wait_cb(lv_disp_drv_t * drv)
{
    if(drv && drv->wait_cb) {
        while(IPA_CTL & IPA_CTL_TEN) {
            drv->wait_cb(drv);
        }
    }
    else {
        while(IPA_CTL & IPA_CTL_TEN);
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void invalidate_cache(void)
{
    lv_disp_t * disp = _lv_refr_get_disp_refreshing();
    if(disp->driver->clean_dcache_cb) disp->driver->clean_dcache_cb(disp->driver);
}

static void wait_finish(void)
{
    lv_disp_t * disp = _lv_refr_get_disp_refreshing();
    if(disp->driver->gpu_wait_cb) return;

    while(IPA_CTL & IPA_CTL_TEN) {
        if(disp->driver->wait_cb) disp->driver->wait_cb(disp->driver);
    }
}

#endif
