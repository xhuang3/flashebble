#include <pebble.h>

static Window *window;
static TextLayer *title_layer;
static TextLayer *content_layer;
static char title_buffer[64];
static char content_buffer[64];

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
      snprintf(title_buffer, sizeof("Location: couldbereallylongname"), "%s", string_value);
      text_layer_set_text(title_layer, (char*) &title_buffer);
      break;
    case CONTENT_KEY:
      snprintf(content_buffer, sizeof("Location: couldbereallylongname"), "%s", string_value);
      text_layer_set_text(content_layer, (char*) &content_buffer);
      break;
  }
}

void out_sent_handler(DictionaryIterator *iter, void *context) {
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
  // outgoing message delivered
}


void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Failed to Send!");
  // outgoing message delivered
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


static TextLayer* init_text_layer(GRect location, GColor colour, GColor background, const char *res_id, GTextAlignment alignment)
{
  TextLayer *layer = text_layer_create(location);
  text_layer_set_text_color(layer, colour);
  text_layer_set_background_color(layer, background);
  text_layer_set_font(layer, fonts_get_system_font(res_id));
  text_layer_set_text_alignment(layer, alignment);
 
  return layer;
}

void window_load(Window *window)
{
  title_layer = init_text_layer(GRect(5, 0, 144, 30), GColorBlack, GColorClear, "RESOURCE_ID_GOTHIC_18", GTextAlignmentLeft);
  text_layer_set_text(title_layer, "Welcome");
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(title_layer));
  
  content_layer = init_text_layer(GRect(5, 30, 144, 30), GColorBlack, GColorClear, "RESOURCE_ID_GOTHIC_18", GTextAlignmentLeft);
  text_layer_set_text(content_layer, "This is the Flashebble!");
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(content_layer));
}

void window_unload(Window *window){
  text_layer_destroy(title_layer);
  text_layer_destroy(content_layer);
}

static void init(void) {
  window = window_create();
  WindowHandlers handlers = {
    .load = window_load,
    .unload = window_unload
  };
  window_set_window_handlers(window, handlers);
  /**
  app_message_register_inbox_received(in_received_handler);
  app_message_register_inbox_dropped(in_dropped_handler);
  app_message_register_outbox_sent(out_sent_handler);
  app_message_register_outbox_failed(out_failed_handler);
  
  const uint32_t inbound_size = 64;
  const uint32_t outbound_size = 64;
  app_message_open(inbound_size, outbound_size);
  
  fetch_msg();
  
  */
  app_message_register_inbox_received(in_received_handler);
  app_message_open(512, 512);
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
