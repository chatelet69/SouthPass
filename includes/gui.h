#ifndef _GUI_HEADS_FILE
#define _GUI_HEADS_FILE

#include <gtk/gtk.h>

static void widgetClickEvent(GtkWidget *, gpointer );
void buttonClickEvent(GtkButton *, gpointer );
static void activate(GtkApplication *, gpointer );
int showGuiApp(int, char **);

#endif