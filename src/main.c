/*

	LINEAR
	SCOTT A HANNEMAN

 */

#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#define MY_UUID {0x8D, 0x2E, 0x6B, 0x25, 0x3A, 0x40, 0x4C, 0xDF, 0xA3, 0x69, 0xDE, 0x9F, 0xF4, 0x4A, 0x81, 0x88}
PBL_APP_INFO(MY_UUID, "LINEAR", "SCOTT A HANNEMAN", 0x1, 0x0, RESOURCE_ID_IMAGE_MENU_ICON, APP_INFO_WATCH_FACE);

int xPosSlider = 0;

Window window;

const int DAY_IMAGE_RESOURCE_IDS[] = {
  RESOURCE_ID_IMAGE_DAY_1,
  RESOURCE_ID_IMAGE_DAY_2,
  RESOURCE_ID_IMAGE_DAY_3,
  RESOURCE_ID_IMAGE_DAY_4,
  RESOURCE_ID_IMAGE_DAY_5,
  RESOURCE_ID_IMAGE_DAY_6,
  RESOURCE_ID_IMAGE_DAY_7,
  RESOURCE_ID_IMAGE_DAY_8,
  RESOURCE_ID_IMAGE_DAY_9,
  RESOURCE_ID_IMAGE_DAY_10,
  RESOURCE_ID_IMAGE_DAY_11,
  RESOURCE_ID_IMAGE_DAY_12,
  RESOURCE_ID_IMAGE_DAY_13,
  RESOURCE_ID_IMAGE_DAY_14,
  RESOURCE_ID_IMAGE_DAY_15,
  RESOURCE_ID_IMAGE_DAY_16,
  RESOURCE_ID_IMAGE_DAY_17,
  RESOURCE_ID_IMAGE_DAY_18,
  RESOURCE_ID_IMAGE_DAY_19,
  RESOURCE_ID_IMAGE_DAY_20,
  RESOURCE_ID_IMAGE_DAY_21,
  RESOURCE_ID_IMAGE_DAY_22,
  RESOURCE_ID_IMAGE_DAY_23,
  RESOURCE_ID_IMAGE_DAY_24,
  RESOURCE_ID_IMAGE_DAY_25,
  RESOURCE_ID_IMAGE_DAY_26,
  RESOURCE_ID_IMAGE_DAY_27,
  RESOURCE_ID_IMAGE_DAY_28,
  RESOURCE_ID_IMAGE_DAY_29,
  RESOURCE_ID_IMAGE_DAY_30,
  RESOURCE_ID_IMAGE_DAY_31
};

BmpContainer background_image_container;
BmpContainer slider_image_container;
BmpContainer rightWall_image_container;
BmpContainer leftWall_image_container;
BmpContainer arm_image_container;
BmpContainer day_image_container;

void set_container_image(BmpContainer *bmp_container, const int resource_id, GPoint origin) {

  layer_remove_from_parent(&bmp_container->layer.layer);
  bmp_deinit_container(bmp_container);

  bmp_init_container(resource_id, bmp_container);

  GRect frame = layer_get_frame(&bmp_container->layer.layer);
  frame.origin.x = origin.x;
  frame.origin.y = origin.y;
  layer_set_frame(&bmp_container->layer.layer, frame);

  layer_add_child(&window.layer, &bmp_container->layer.layer);
}

void update_display(PblTm *current_time) {
	
  int minute = current_time->tm_min;
  int hour = current_time->tm_hour;
  int dayNum = current_time->tm_mday;
	
  dayNum = dayNum - 1; //subtract 1 for array proper arrayref.
	
  /* FYI:
  int tm_sec;
  int tm_min;
  int tm_hour;
  int tm_mday;
  int tm_mon;
  int tm_year;
  int tm_wday;
  int tm_yday;
  int tm_isdst;
  */
	
	int minAdd = ((minute/10)*4);
	
	if((minute%10) >= 2){
		minAdd = minAdd + 1;
	}
	if((minute%10) >= 5){
		minAdd = minAdd + 1;
	}
	if((minute%10) >= 7){
		minAdd = minAdd + 1;
	}
	
	if (clock_is_24h_style()) {
		xPosSlider = (((hour*24)+minAdd)*-1); //must be negitive to move image to left.
		set_container_image(&slider_image_container, RESOURCE_ID_IMAGE_SLIDER, GPoint(xPosSlider, 96));
	} else {
		hour = hour % 12;
		xPosSlider = (((hour*24)+minAdd)*-1); //must be negitive to move image to left.
		set_container_image(&slider_image_container, RESOURCE_ID_IMAGE_SLIDER_12H, GPoint(xPosSlider, 96));
	}
	
  set_container_image(&leftWall_image_container, RESOURCE_ID_IMAGE_SIDEWALL, GPoint(0, 0));
  set_container_image(&rightWall_image_container, RESOURCE_ID_IMAGE_SIDEWALL, GPoint(144-7, 0));
  set_container_image(&arm_image_container, RESOURCE_ID_IMAGE_ARM, GPoint(89, 108));
  set_container_image(&day_image_container, DAY_IMAGE_RESOURCE_IDS[dayNum], GPoint(0, 72));
}

void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "LINEAR");
  window_stack_push(&window, true /* Animated */);
  window_set_background_color(&window, GColorBlack);

  resource_init_current_app(&APP_RESOURCES); //??? DO I NEED THIS???

  // Set up a layer for the static watch face background
  bmp_init_container(RESOURCE_ID_IMAGE_BACKGROUND, &background_image_container);
  layer_add_child(&window.layer, &background_image_container.layer.layer);
	
  // Avoids a blank screen on watch start.
  PblTm tick_time;
  get_time(&tick_time);
  update_display(&tick_time);
	
}

void handle_minute_tick(AppContextRef ctx, PebbleTickEvent *t) {
  (void)t;
	
  update_display(t->tick_time);
	
}

void handle_deinit(AppContextRef ctx) {
  (void)ctx;

  bmp_deinit_container(&background_image_container);
  bmp_deinit_container(&slider_image_container);
  bmp_deinit_container(&arm_image_container);
  bmp_deinit_container(&rightWall_image_container);
  bmp_deinit_container(&leftWall_image_container);
  bmp_deinit_container(&day_image_container);
}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .deinit_handler = &handle_deinit,

    .tick_info = {
      .tick_handler = &handle_minute_tick,
      .tick_units = MINUTE_UNIT
    }

  };
  app_event_loop(params, &handlers);
}