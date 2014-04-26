#include <pebble.h>

#define TITLE_SIZE 64
#define CONTENT_SIZE 128

#define NUM_MENU_SECTIONS 2
#define NUM_MENU_0 2
#define NUM_MENU_1 1
// Variables
static Window *window;
static Window *mwindow;
static TextLayer *title_big_layer;
static TextLayer *title_layer;
static TextLayer *content_layer;
static SimpleMenuLayer *menu_layer;
static SimpleMenuSection menu_sections[NUM_MENU_SECTIONS];
static SimpleMenuItem menu_item_0[NUM_MENU_0];
static SimpleMenuItem menu_item_1[NUM_MENU_1];
static char title_buffer[TITLE_SIZE];
static char content_buffer[CONTENT_SIZE];
bool front = false;
bool menu = false;
bool title_screen = true;
static int id;
static int deck_id = 0;

//added for logo
static BitmapLayer *image_layer;
static GBitmap *image;

// Communication
enum {
    TITLE_KEY = 0,
    CONTENT_KEY = 1,
};

static void menu_select_callback(int index, void *ctx) {
    deck_id = index;
    id = 0;
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
    Tuplet value = TupletInteger(2, id);
    Tuplet value2 = TupletInteger(3, deck_id);
    dict_write_tuplet(iter, &value);
    dict_write_tuplet(iter, &value2);
    app_message_outbox_send();
    
    layer_mark_dirty(simple_menu_layer_get_layer(menu_layer));
    window_stack_pop(true);
}
void process_tuple(Tuple *t){
    int key = t -> key;
    int value = t -> value->int32;
    char string_value[128];
    strcpy(string_value, t->value->cstring);
    
    switch(key) {
        case TITLE_KEY:
            snprintf(title_buffer, TITLE_SIZE, " - %s - ", string_value);
            text_layer_set_text(title_big_layer, (char*) &title_buffer);
            layer_set_hidden((Layer*)content_layer, true);
            layer_set_hidden((Layer*)title_layer, true);
            layer_set_hidden((Layer*)title_big_layer, false);
            layer_set_hidden(bitmap_layer_get_layer(image_layer), true); //also will hide the layer idk why it works
            break;
        case CONTENT_KEY:
            snprintf(content_buffer, CONTENT_SIZE, "---------------------- %s", string_value);
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
    title_screen = false;
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
    
    // doesn't matter what to send, just trigger the callback
    Tuplet value = TupletInteger(2, id);
    Tuplet value2 = TupletInteger(3, deck_id);
    dict_write_tuplet(iter, &value);
    dict_write_tuplet(iter, &value2);
    if(id > 0) id--;
    app_message_outbox_send();
}

void down_click_handler(ClickRecognizerRef recognizer, void *context)
{
    title_screen = false;
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
    
    // doesn't matter what to send, just trigger the callback
    Tuplet value = TupletInteger(2, id);
    Tuplet value2 = TupletInteger(3, deck_id);
    dict_write_tuplet(iter, &value);
    dict_write_tuplet(iter, &value2);
    id++;
    app_message_outbox_send();
}

void select_click_handler(ClickRecognizerRef recognizer, void *context)
{
    // TODO: make a menu
    title_screen = false;
    window_stack_push(mwindow, true);
}

void back_click_handler(ClickRecognizerRef recognizer, void *context)
{
    window_stack_pop(true);
}

void click_config_provider(void *context)
{
    window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
    window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

void click_config_provider_menu(void *context)
{
    window_single_click_subscribe(BUTTON_ID_BACK, back_click_handler);
}


// Shakes

void accel_tap_handler(AccelAxisType axis, int32_t direction) {
    if(!title_screen)
    {
    text_layer_set_text(title_layer, (char*) &title_buffer);
    text_layer_set_text(content_layer, (char*) &content_buffer);
    layer_set_hidden((Layer*)content_layer, false);
    layer_set_hidden((Layer*)title_big_layer, true);
    layer_set_hidden(bitmap_layer_get_layer(image_layer), true); //will turn off the layer
    layer_set_hidden((Layer*)title_layer, false);
    vibes_double_pulse();
    }
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
    /* did not touch this code because it is used for the word page */
    title_big_layer = init_text_layer(GRect(5, 56, 134, 30), GColorBlack, GColorClear, "RESOURCE_ID_GOTHIC_24_BOLD", GTextAlignmentCenter);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(title_big_layer));
    layer_set_hidden((Layer*)title_big_layer, true);
    
    /* This will create the image_layer and apply it to the window */
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);
    image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_LOGO);
    image_layer = bitmap_layer_create(bounds);
    bitmap_layer_set_bitmap(image_layer, image);
    bitmap_layer_set_alignment(image_layer, GAlignCenter);
    layer_add_child(window_layer, bitmap_layer_get_layer(image_layer));
    
    title_layer = init_text_layer(GRect(5, 0, 134, 30), GColorBlack, GColorClear, "RESOURCE_ID_GOTHIC_18_BOLD", GTextAlignmentCenter);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(title_layer));
    
    content_layer = init_text_layer(GRect(5, 30, 134, 138), GColorBlack, GColorClear, "RESOURCE_ID_GOTHIC_18", GTextAlignmentCenter);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(content_layer));
}

void window_unload(Window *window){
    text_layer_destroy(title_big_layer);
    text_layer_destroy(title_layer);
    text_layer_destroy(content_layer);
    bitmap_layer_destroy(image_layer); //remove logo layer
}

void mwindow_load(Window *menu_win)
{
    menu_item_0[0] = (SimpleMenuItem){
        .title = "SAT",
        .callback = menu_select_callback,
    };
    menu_item_0[1] = (SimpleMenuItem){
        .title = "GRE",
        .callback = menu_select_callback,
    };
    menu_item_1[0] = (SimpleMenuItem){
        .title = "My Cards",
        .callback = menu_select_callback,
    };
    menu_sections[0] = (SimpleMenuSection){
        .title = "Flashebble Cards",
        .num_items = NUM_MENU_0,
        .items = menu_item_0,
    };
    menu_sections[1] = (SimpleMenuSection){
        // Menu sections can also have titles as well
        .title = "User Cards",
        .num_items = NUM_MENU_1,
        .items = menu_item_1,
    };
    menu_layer = simple_menu_layer_create(GRect(0,0,144,168), menu_win, menu_sections, NUM_MENU_SECTIONS, NULL);
    layer_add_child(window_get_root_layer(menu_win), simple_menu_layer_get_layer(menu_layer));
}

void mwindow_unload(Window *menu_win)
{
    simple_menu_layer_destroy(menu_layer);
}


static void init(void) {
    window = window_create();
    id = 0;
    WindowHandlers handlers = {
        .load = window_load,
        .unload = window_unload
    };
    mwindow = window_create();
    WindowHandlers mhandlers = {
        .load = mwindow_load,
        .unload = mwindow_unload
    };
    window_set_window_handlers(window, handlers);
    window_set_window_handlers(mwindow, mhandlers);
    
    app_message_register_inbox_received(in_received_handler);
    app_message_register_inbox_dropped(in_dropped_handler);
    
    app_message_register_outbox_failed(out_failed_handler);
    app_message_open(TITLE_SIZE, CONTENT_SIZE);
    
    
    
    window_set_click_config_provider(window, click_config_provider);
    window_set_click_config_provider(mwindow, click_config_provider_menu);
    
    accel_tap_service_subscribe(&accel_tap_handler);
    const bool animated = true;
    window_stack_push(window, animated);
}

static void deinit(void) {
    accel_tap_service_unsubscribe();
    window_destroy(window);
    window_destroy(mwindow);
}

int main(void) {
    init();
    
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);
    
    app_event_loop();
    deinit();
}
