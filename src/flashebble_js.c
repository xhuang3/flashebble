#include <pebble.h>


// Variables
static Window *window;
static TextLayer *title_big_layer;
static TextLayer *title_layer;
static TextLayer *content_layer;
static char title_buffer[64];
static char content_buffer[64];
bool front = false;
static int id;



// Communication
enum {
  TITLE_KEY = 0,
  CONTENT_KEY = 1,
};

void process_tuple(Tuple *t){
  int key = t -> key;
  int value = t -> value->int32;
  char string_value[32];
  strcpy(string_value, t->value->cstring);
  
  switch(key) {
    case TITLE_KEY:
      snprintf(title_buffer, 64, "%s", string_value);
      text_layer_set_text(title_big_layer, (char*) &title_buffer);
      layer_set_hidden((Layer*)content_layer, true);
      layer_set_hidden((Layer*)title_layer, true);
      layer_set_hidden((Layer*)title_big_layer, false);
      break;
    case CONTENT_KEY:
      snprintf(content_buffer, 64, "%s", string_value);
      break;
  }
}

void out_sent_handler(DictionaryIterator *iter, void *context) {
}


void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Failed to Send!");
}

void in_received_handler(DictionaryIterator *iter, void *context) {
  Tuple *t = dict_read_first(iter);
  if(t){
    process_tuple(t);
  }
  
  while(t != NULL){
    t = dict_read_next(iter);
    if(t)
    {
      process_tuple(t);
    }
  }
}

void in_dropped_handler(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Dropped!");
}



// Buttons
void up_click_handler(ClickRecognizerRef recognizer, void *context)
{
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  
  // doesn't matter what to send, just trigger the callback
  Tuplet value = TupletInteger(2, id);
  dict_write_tuplet(iter, &value);
  if(id > 0) id--;
  app_message_outbox_send();
}
 
void down_click_handler(ClickRecognizerRef recognizer, void *context)
{
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  
  // doesn't matter what to send, just trigger the callback
  Tuplet value = TupletInteger(2, id);
  dict_write_tuplet(iter, &value);
  id++;
  app_message_outbox_send();
}
 
void select_click_handler(ClickRecognizerRef recognizer, void *context)
{
  // TODO: make a menu
}

void click_config_provider(void *context)
{
    window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
    window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}



// Shakes

void accel_tap_handler(AccelAxisType axis, int32_t direction) {
  text_layer_set_text(title_layer, (char*) &title_buffer);
  text_layer_set_text(content_layer, (char*) &content_buffer);
  layer_set_hidden((Layer*)content_layer, false);
  layer_set_hidden((Layer*)title_big_layer, true);
  layer_set_hidden((Layer*)title_layer, false);
}




// Set up
static TextLayer* init_text_layer(GRect location, GColor colour, GColor background, const char *res_id, GTextAlignment alignment)
{
  TextLayer *layer = text_layer_create(location);
  text_layer_set_text_color(layer, colour);
  text_layer_set_background_color(layer, background);
  text_layer_set_font(layer, fonts_get_system_font(res_id));
  text_layer_set_text_alignment(layer, alignment);
  text_layer_set_overflow_mode(layer, GTextOverflowModeWordWrap);
 
  return layer;
}

void window_load(Window *window)
{
  title_big_layer = init_text_layer(GRect(5, 60, 144, 90), GColorBlack, GColorClear, "RESOURCE_ID_GOTHIC_18", GTextAlignmentCenter);
  text_layer_set_text(title_big_layer, "Welcome to Flashebble!");
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(title_big_layer));
  
  title_layer = init_text_layer(GRect(5, 0, 144, 30), GColorBlack, GColorClear, "RESOURCE_ID_GOTHIC_18", GTextAlignmentLeft);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(title_layer));
  
  content_layer = init_text_layer(GRect(5, 30, 144, 90), GColorBlack, GColorClear, "RESOURCE_ID_GOTHIC_18", GTextAlignmentLeft);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(content_layer));
}

void window_unload(Window *window){
  text_layer_destroy(title_layer);
  text_layer_destroy(content_layer);
}

static void init(void) {
  window = window_create();
  id = 0;
  WindowHandlers handlers = {
    .load = window_load,
    .unload = window_unload
  };
  window_set_window_handlers(window, handlers);

  app_message_register_inbox_received(in_received_handler);
  app_message_register_inbox_dropped(in_dropped_handler);
  
  app_message_register_outbox_failed(out_failed_handler);
  app_message_open(512, 512);
  
  
  
  window_set_click_config_provider(window, click_config_provider);
  accel_tap_service_subscribe(&accel_tap_handler);
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  accel_tap_service_unsubscribe();
  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
