/*

   How to use a custom non-system font.

 */

#include "pebble.h"
#define ACCEL_STEP_MS 50

static Window *window;

static TextLayer *top_text;
static TextLayer *middle_text;
static TextLayer *bottom_text;
static TextLayer *fpm_text;
Layer *window_layer;
bool menu = true;
int faps = 0;
double fpm = 0;
static AppTimer *timer;
static char *rate_text;
AccelData accel;
int y_grav = 0;
bool pos = true;
double time_elapsed = 0;
static void redraw_text(){
  //sets fap_value
  
  static char body_text[50];
  snprintf(body_text, sizeof(body_text), "%u", faps);
  text_layer_set_text(middle_text, body_text);
  //set fpm value  
 
  if(faps>9000){
      text_layer_set_text(fpm_text, "YOUR A GOD!");  
  }else{
  fpm=(double)((double)faps/(double)(time_elapsed/20))*60;
  static char fapp_text[50];
  snprintf(fapp_text, sizeof(fapp_text), "%u", (int)fpm);
  text_layer_set_text(fpm_text, fapp_text);  
  }
}
static void timer_callback(void *data) {
  //faps++;
  //fpm++;
  time_elapsed++;
  accel_service_peek(&accel);
  y_grav = accel.y;
  if(pos){
    if(y_grav<0){
      pos=false;
      faps++;
    }
  }else{
    if(y_grav>0){
      pos=true;
      //faps++;
    }
  }
  
  redraw_text();
  timer = app_timer_register(ACCEL_STEP_MS, timer_callback, NULL);
}

void middle_click_handler(ClickRecognizerRef recognizer, void *context) {
  if(menu){
    menu = false;
    
    //draw title
    GFont top_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_FAP_22));
    text_layer_set_font(top_text, top_font);
    text_layer_set_text(top_text, "FAPP COUNTER");
    
    //draw middle area
     GRect move_pos = (GRect) { .origin = { -15, 45 }, .size = { 180, 180 } };
    layer_set_frame(text_layer_get_layer(middle_text),move_pos);
    
    //draw number of faps
     static char body_text[50];
     snprintf(body_text, sizeof(body_text), "%u", faps);
     text_layer_set_text(middle_text, body_text);
    
    text_layer_set_text(bottom_text, "Faps per minute:");  
    text_layer_set_text(fpm_text, "0");  
    
    accel_data_service_subscribe(0, NULL);
    timer = app_timer_register(ACCEL_STEP_MS, timer_callback, NULL);
  }
}
void config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, middle_click_handler);
}

static void init() {
  window = window_create();
  window_stack_push(window, true /* Animated */);
  window_set_click_config_provider(window, config_provider);
  window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  GFont top_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_FAP_35));
  GFont middle_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_FAP_50));
  GFont bottom_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DEFAULT_20));  
  GFont fpm_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DEFAULT_18));  
  
  top_text = text_layer_create(bounds);
  middle_text = text_layer_create(bounds);
  bottom_text = text_layer_create(bounds);
  fpm_text = text_layer_create(bounds);
  text_layer_set_text(top_text, "THE");
  text_layer_set_text(middle_text, "FAPP");
  text_layer_set_text(bottom_text,"Select to Start");
  text_layer_set_font(top_text, top_font);
  text_layer_set_font(middle_text, middle_font);
  text_layer_set_font(bottom_text, bottom_font);
  text_layer_set_font(fpm_text, fpm_font);
  text_layer_set_text_alignment(top_text, GTextAlignmentCenter);
  text_layer_set_text_alignment(middle_text, GTextAlignmentCenter);
  text_layer_set_text_alignment(bottom_text, GTextAlignmentCenter);
  text_layer_set_text_alignment(fpm_text, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(top_text));
  layer_add_child(window_layer, text_layer_get_layer(middle_text));
  layer_add_child(window_layer, text_layer_get_layer(bottom_text));
  layer_add_child(window_layer, text_layer_get_layer(fpm_text));
  GRect move_pos = (GRect) { .origin = { -15, 39 }, .size = { 180, 180 } };
  layer_set_frame(text_layer_get_layer(middle_text),move_pos);
  GRect move_pos2 = (GRect) { .origin = { -15, 105 }, .size = { 180, 180 } };
  layer_set_frame(text_layer_get_layer(bottom_text),move_pos2);
  GRect move_pos3 = (GRect) { .origin = { -15, 130 }, .size = { 180, 180 } };
  layer_set_frame(text_layer_get_layer(fpm_text),move_pos3);
  
}

static void deinit() {
  accel_data_service_unsubscribe();
  text_layer_destroy(top_text);
  text_layer_destroy(middle_text);
  text_layer_destroy(bottom_text);
  text_layer_destroy(fpm_text);
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
