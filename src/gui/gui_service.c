#include <gtk/gtk.h>
#include "../../includes/gui.h"
#include "../../includes/db.h"

static cairo_surface_t *surface = NULL;
MYSQL *dbc;

static void widgetClickEvent(GtkWidget *widget, gpointer data) {
    int* my_data = (int*)data;

    g_print("Coucou\n");
    //printf("Button clicked: %p, %#x\n", )
}

void entryActionEvent(GtkWidget *entry, gpointer data){
  if (data){
    GtkEntry *input = GTK_ENTRY(data);
    const gchar *text;
    text = gtk_editable_get_text(input);
    g_print("data : %s\n", (strcmp(text, "") == 0) ? "No data!" : text);

    if (strcmp(text, "") != 0) {
      char *query;
      query = malloc(sizeof("INSERT INTO users (username) VALUES ('%s')", text));
      sprintf(query, "INSERT INTO users (username) VALUES ('%s')", text);
      int resDb = putData(dbc, query);
    }
  }
}

void buttonClickEvent(GtkButton *button, gpointer data) {
    // The data is the value we passed to g_signal_connect.
    int* myData = (int*)data;
    //%#x

    printf("button_clicked: %p\n", button);
    if (data) {
      printf("data: %s\n", data);
      dbGet(dbc, data);
    }

    // Since the location that we're pointing to is a static in main,
    // we can do stuff like increment the value each time the button
    // is clicked:
    //++*myData;
}

static void preferences_activated (GSimpleAction *action, GVariant *parameter, gpointer app) {
    
}

static void clear_surface (void) {
  cairo_t *cr;

  cr = cairo_create (surface);

  cairo_set_source_rgb (cr, 1, 1, 1);
  cairo_paint (cr);

  cairo_destroy (cr);
}

static void resize_cb (GtkWidget *widget, int width, int height, gpointer data) {
    if (surface) {
        cairo_surface_destroy (surface);
        surface = NULL;
    }
        
    if (gtk_native_get_surface (gtk_widget_get_native (widget))) {
        surface = gdk_surface_create_similar_surface (gtk_native_get_surface (gtk_widget_get_native (widget)),
                                                    CAIRO_CONTENT_COLOR,
                                                    gtk_widget_get_width (widget),
                                                    gtk_widget_get_height (widget));

      /* Initialize the surface to white */
      clear_surface ();
    }
}

static void draw_cb(GtkDrawingArea *drawing_area, cairo_t *cr, int width, int height, gpointer data) {
    cairo_set_source_surface(cr, surface, 0, 0);
    cairo_paint(cr);
}

static void draw_brush(GtkWidget *widget, double x, double y) {
    cairo_t *cr;

    /* Paint to the surface, where we store our state */
    cr = cairo_create(surface);

    cairo_rectangle(cr, x - 3, y - 3, 6, 6);
    cairo_fill(cr);

    cairo_destroy(cr);

    /* Now invalidate the drawing area. */
    gtk_widget_queue_draw(widget);
}

static double start_x;
static double start_y;

static void drag_begin (GtkGestureDrag *gesture,
            double          x,
            double          y,
            GtkWidget      *area)
{
  start_x = x;
  start_y = y;

  draw_brush (area, x, y);
}

static void
drag_update (GtkGestureDrag *gesture,
             double          x,
             double          y,
             GtkWidget      *area)
{
  draw_brush (area, start_x + x, start_y + y);
}


static void
drag_end (GtkGestureDrag *gesture, double x, double y, GtkWidget *area){
  draw_brush (area, start_x + x, start_y + y);
}

static void
pressed (GtkGestureClick *gesture,
         int              n_press,
         double           x,
         double           y,
         GtkWidget       *area)
{
  clear_surface ();
  gtk_widget_queue_draw (area);
}

static void
close_window (void)
{
  if (surface)
    cairo_surface_destroy (surface);
}

static void
quit_cb (GSimpleAction *action,
         GVariant      *parameter,
         gpointer       user_data)
{
  /* ...quit... */
}

void setAppMenu(GtkApplication *app, GtkWidget *window) {
  const GActionEntry app_actions[] = {{ "quit", quit_cb }};
  GMenu *menu;
  g_action_map_add_action_entries(G_ACTION_MAP(app), app_actions, G_N_ELEMENTS(app_actions), app);
  menu = g_menu_new();
  g_menu_append(menu, "Quit", "app.quit");
  gtk_application_set_menubar(GTK_APPLICATION(app), G_MENU_MODEL(menu));
  //g_object_unref(menu);
}


static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget   *window;
    GtkWidget   *grid;
    GtkWidget   *button;
    GtkWidget   *frame;
    GtkWidget   *drawing_area;
    GtkGesture  *drag;
    GtkGesture  *press;

    int row = 0, column = 0, width = 2, height = 2;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "BozoApp");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);
    g_signal_connect (window, "destroy", G_CALLBACK (close_window), NULL);

    /*GError *error = NULL;
    GdkPixbuf *icon = gdk_pixbuf_new_from_file("./favicon.png", &error);
    if (error != NULL) {
        g_printerr("Erreur lors du chargement de l'icône : %s\n", error->message);
        g_clear_error(&error);
    } else {
        gtk_window_set_icon(GTK_WINDOW(window), icon);
    }*/

    // Grid
    grid = gtk_grid_new();
    gtk_window_set_child(GTK_WINDOW(window), grid);

    // Buttons
    button = gtk_button_new_with_label("Get users");
    g_signal_connect(button, "clicked", G_CALLBACK(buttonClickEvent), "SELECT * FROM users;");
    // On attache à la grid un nouvel élément, le bouton, avec position + dimensions
    gtk_grid_attach(GTK_GRID(grid), button, column, row, width, height);

    // Définition d'un autre bouton
    button = gtk_button_new_with_label("J'ai faim");
    g_signal_connect(button, "clicked", G_CALLBACK(buttonClickEvent), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, column+5, row, width, height);


    GtkWidget *input;
    input = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(input), 20);
    
    gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(input), column+20, row+15, width, height);
    gtk_widget_set_visible (input, 1);

    GtkWidget *screen;
    GtkCssProvider *provider;
    screen = gdk_screen_get_default ();
    provider = gtk_css_provider_new ();
    gtk_css_provider_load_from_resource (provider, "./style.css");  
    gtk_style_context_add_provider_for_screen (screen, provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    
    GtkStyleContext *context;
    context = gtk_widget_get_style_context(button);
    gtk_style_context_add_class(context,"button");

    button = gtk_button_new_with_label("Créer un user !");
    g_signal_connect(button, "clicked", G_CALLBACK(entryActionEvent), input);
    gtk_grid_attach(GTK_GRID(grid), button, column, row+8, width+1, height);

    setAppMenu(app, window);

    //gtk_css_provider_load_from_string(css_provider, "button { background-color: rgba(200, 150, 300, 0.4); }");
    //char *cssButon = "button { background-color: #dc143c; }";
    //GtkCssProvider* css_provider = gtk_css_provider_new();
    //GtkStyleProvider *styleProvider;
    //GtkStyleContext *styleContext;
    //gtk_style_context_add_provider(styleContext, styleProvider, GTK_STYLE_PROVIDER_PRIORITY_USER);

    /*frame = gtk_frame_new (NULL);
    gtk_window_set_child (GTK_WINDOW (window), frame);

    drawing_area = gtk_drawing_area_new ();
    // set a minimum size
    gtk_widget_set_size_request(drawing_area, 100, 100);

    gtk_frame_set_child(GTK_FRAME(frame), drawing_area);

    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(drawing_area), draw_cb, NULL, NULL);

    g_signal_connect_after(drawing_area, "resize", G_CALLBACK(resize_cb), NULL);

    drag = gtk_gesture_drag_new();
    gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(drag), GDK_BUTTON_PRIMARY);
    gtk_widget_add_controller(drawing_area, GTK_EVENT_CONTROLLER(drag));
    g_signal_connect(drag, "drag-begin", G_CALLBACK(drag_begin), drawing_area);
    g_signal_connect(drag, "drag-update", G_CALLBACK(drag_update), drawing_area);
    g_signal_connect(drag, "drag-end", G_CALLBACK(drag_end), drawing_area);

    press = gtk_gesture_click_new();
    gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(press), GDK_BUTTON_SECONDARY);
    gtk_widget_add_controller(drawing_area, GTK_EVENT_CONTROLLER(press));

    g_signal_connect(press, "pressed", G_CALLBACK(pressed), drawing_area);*/

    //GtkWidget *table;
    //table = gtk_text (1, 2, TRUE);
    //gtk_grid_attach(GTK_GRID(grid), table, column+15, row, width, height);
    //gtk_widget_show (table);

    gtk_window_present(GTK_WINDOW(window));
}

int showGuiApp(int argc, char **argv) {
    dbc = dbConnect();
    GtkApplication *app;
    app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    //g_object_set(gtk_settings_get_default(), "gtk-application-prefer-dark-theme", TRUE, NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    closeDb(dbc);

    return status;
}