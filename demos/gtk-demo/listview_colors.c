/* Lists/Colors
 *
 * This demo displays a named colors.
 *
 * It is using a GtkGridView, and shows
 * how to sort the data in various ways.
 *
 * The dataset used here has 9283 items.
 */

#include <gtk/gtk.h>

#define GTK_TYPE_COLOR (gtk_color_get_type ())
G_DECLARE_FINAL_TYPE (GtkColor, gtk_color, GTK, COLOR, GObject)

/* This is our object. It's just a color */
typedef struct _GtkColor GtkColor;
struct _GtkColor
{
  GObject parent_instance;

  char *name;
  GdkRGBA *color;
  int h, s, v;
};

enum {
  PROP_0,
  PROP_NAME,
  PROP_COLOR,
  PROP_RED,
  PROP_GREEN,
  PROP_BLUE,
  PROP_HUE,
  PROP_SATURATION,
  PROP_VALUE,

  N_PROPS
};

static void
gtk_color_snapshot (GdkPaintable *paintable,
                    GdkSnapshot  *snapshot,
                    double        width,
                    double        height)
{
  GtkColor *self = GTK_COLOR (paintable);

  gtk_snapshot_append_color (snapshot, self->color, &GRAPHENE_RECT_INIT (0, 0, width, height));
}

static int
gtk_color_get_intrinsic_width (GdkPaintable *paintable)
{
  return 32;
}

static int
gtk_color_get_intrinsic_height (GdkPaintable *paintable)
{
  return 32;
}

static void
gtk_color_paintable_init (GdkPaintableInterface *iface)
{
  iface->snapshot = gtk_color_snapshot;
  iface->get_intrinsic_width = gtk_color_get_intrinsic_width;
  iface->get_intrinsic_height = gtk_color_get_intrinsic_height;
}

/*
 * Finally, we define the type. The important part is adding the paintable
 * interface, so GTK knows that this object can indeed be drawm.
 */
G_DEFINE_TYPE_WITH_CODE (GtkColor, gtk_color, G_TYPE_OBJECT,
                         G_IMPLEMENT_INTERFACE (GDK_TYPE_PAINTABLE,
                                                gtk_color_paintable_init))

static GParamSpec *properties[N_PROPS] = { NULL, };

static void
gtk_color_get_property (GObject    *object,
                        guint       property_id,
                        GValue     *value,
                        GParamSpec *pspec)
{
  GtkColor *self = GTK_COLOR (object);

  switch (property_id)
    {
    case PROP_NAME:
      g_value_set_string (value, self->name);
      break;

    case PROP_COLOR:
      g_value_set_boxed (value, self->color);
      break;

    case PROP_RED:
      g_value_set_float (value, self->color->red);
      break;

    case PROP_GREEN:
      g_value_set_float (value, self->color->green);
      break;

    case PROP_BLUE:
      g_value_set_float (value, self->color->blue);
      break;

    case PROP_HUE:
      g_value_set_int (value, self->h);
      break;

    case PROP_SATURATION:
      g_value_set_int (value, self->s);
      break;

    case PROP_VALUE:
      g_value_set_int (value, self->v);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
gtk_color_set_property (GObject      *object,
                        guint         property_id,
                        const GValue *value,
                        GParamSpec   *pspec)
{
  GtkColor *self = GTK_COLOR (object);

  switch (property_id)
    {
    case PROP_NAME:
      self->name = g_value_dup_string (value);
      break;

    case PROP_COLOR:
      self->color = g_value_dup_boxed (value);
      break;

    case PROP_HUE:
      self->h = g_value_get_int (value);
      break;

    case PROP_SATURATION:
      self->s = g_value_get_int (value);
      break;

    case PROP_VALUE:
      self->v = g_value_get_int (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
gtk_color_finalize (GObject *object)
{
  GtkColor *self = GTK_COLOR (object);

  g_free (self->name);
  g_clear_pointer (&self->color, gdk_rgba_free);

  G_OBJECT_CLASS (gtk_color_parent_class)->finalize (object);
}

static void
gtk_color_class_init (GtkColorClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

  gobject_class->get_property = gtk_color_get_property;
  gobject_class->set_property = gtk_color_set_property;
  gobject_class->finalize = gtk_color_finalize;

  properties[PROP_NAME] =
    g_param_spec_string ("name", NULL, NULL, NULL, G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
  properties[PROP_COLOR] =
    g_param_spec_boxed ("color", NULL, NULL, GDK_TYPE_RGBA, G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
  properties[PROP_RED] =
    g_param_spec_float ("red", NULL, NULL, 0, 1, 0, G_PARAM_READABLE);
  properties[PROP_GREEN] =
    g_param_spec_float ("green", NULL, NULL, 0, 1, 0, G_PARAM_READABLE);
  properties[PROP_BLUE] =
    g_param_spec_float ("blue", NULL, NULL, 0, 1, 0, G_PARAM_READABLE);
  properties[PROP_HUE] =
    g_param_spec_int ("hue", NULL, NULL, 0, 360, 0, G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
  properties[PROP_SATURATION] =
    g_param_spec_int ("saturation", NULL, NULL, 0, 100, 0, G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
  properties[PROP_VALUE] =
    g_param_spec_int ("value", NULL, NULL, 0, 100, 0, G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);

  g_object_class_install_properties (gobject_class, N_PROPS, properties);
}

static void
gtk_color_init (GtkColor *self)
{
}

static GtkColor *
gtk_color_new (const char *name,
               float r, float g, float b,
               int h, int s, int v)
{
  GtkColor *result;
  GdkRGBA color = { r, g, b, 1.0 };

  result = g_object_new (GTK_TYPE_COLOR,
                         "name", name,
                         "color", &color,
                         "hue", h,
                         "saturation", s,
                         "value", v,
                         NULL);

  return result;
}

static GListModel *
create_colors_model (void)
{
  GListStore *result;
  GtkColor *color;
  GBytes *data;
  char **lines;
  guint i;

  result = g_list_store_new (GTK_TYPE_COLOR);
  data = g_resources_lookup_data ("/listview_colors/color.names.txt", 0, NULL);
  lines = g_strsplit (g_bytes_get_data (data, NULL), "\n", 0);

  for (i = 0; lines[i]; i++)
    {
      const char *name;
      char **fields;
      int red, green, blue;
      int h, s, v;

      if (lines[i][0] == '#' || lines[i][0] == '\0')
        continue;

      fields = g_strsplit (lines[i], " ", 0);
      name = fields[1];
      red = atoi (fields[3]);
      green = atoi (fields[4]);
      blue = atoi (fields[5]);
      h = atoi (fields[9]);
      s = atoi (fields[10]);
      v = atoi (fields[11]);
 
      color = gtk_color_new (name, red / 255., green / 255., blue / 255., h, s, v);
      g_list_store_append (result, color);
      g_object_unref (color);

      g_strfreev (fields);
    }
  g_strfreev (lines);

  g_bytes_unref (data);

  return G_LIST_MODEL (result);
}

static char *
get_rgb_markup (gpointer this,
                GtkColor *color)
{
  if (!color)
    return NULL;

  return g_strdup_printf ("<b>R:</b> %d <b>G:</b> %d <b>B:</b> %d",
                          (int)(color->color->red * 255), 
                          (int)(color->color->green * 255),
                          (int)(color->color->blue * 255));
}

static char *
get_hsv_markup (gpointer this,
                GtkColor *color)
{
  if (!color)
    return NULL;

  return g_strdup_printf ("<b>H:</b> %d <b>S:</b> %d <b>V:</b> %d",
                          color->h, 
                          color->s,
                          color->v);
}

static void
setup_listitem_cb (GtkListItemFactory *factory,
                   GtkListItem        *list_item)
{
  GtkWidget *box, *picture, *name_label, *rgb_label, *hsv_label;;
  GtkExpression *color_expression, *expression;
  GtkExpression *params[1];

  box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_list_item_set_child (list_item, box);

  expression = gtk_constant_expression_new (GTK_TYPE_LIST_ITEM, list_item);
  color_expression = gtk_property_expression_new (GTK_TYPE_LIST_ITEM, expression, "item");

  expression = gtk_property_expression_new (GTK_TYPE_COLOR,
                                            gtk_expression_ref (color_expression),
                                            "name");
  name_label = gtk_label_new (NULL);
  gtk_expression_bind (expression, name_label, "label", NULL);
  gtk_container_add (GTK_CONTAINER (box), name_label);

  expression = gtk_expression_ref (color_expression);
  picture = gtk_picture_new ();
  gtk_expression_bind (expression, picture, "paintable", NULL);
  gtk_container_add (GTK_CONTAINER (box), picture);

  params[0] = gtk_expression_ref (color_expression);
  expression = gtk_cclosure_expression_new (G_TYPE_STRING,
                                            NULL,
                                            1, params,
                                            (GCallback)get_rgb_markup,
                                            NULL, NULL);

  rgb_label = gtk_label_new (NULL);
  gtk_label_set_use_markup (GTK_LABEL (rgb_label), TRUE);
  gtk_expression_bind (expression, rgb_label, "label", NULL);
  gtk_container_add (GTK_CONTAINER (box), rgb_label);

  params[0] = gtk_expression_ref (color_expression);
  expression = gtk_cclosure_expression_new (G_TYPE_STRING,
                                            NULL,
                                            1, params,
                                            (GCallback)get_hsv_markup,
                                            NULL, NULL);

  hsv_label = gtk_label_new (NULL);
  gtk_label_set_use_markup (GTK_LABEL (hsv_label), TRUE);
  gtk_expression_bind (expression, hsv_label, "label", NULL);
  gtk_container_add (GTK_CONTAINER (box), hsv_label);

  gtk_expression_unref (color_expression);
}

static GtkWidget *window = NULL;

GtkWidget *
do_listview_colors (GtkWidget *do_widget)
{
  if (window == NULL)
    {
      GtkWidget *header, *gridview, *sw;
      GtkListItemFactory *factory;
      GListModel *model;
      GtkNoSelection *selection;

      window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
      gtk_window_set_title (GTK_WINDOW (window), "Colors");
      header = gtk_header_bar_new ();
      gtk_header_bar_set_show_title_buttons (GTK_HEADER_BAR (header), TRUE);
      gtk_window_set_titlebar (GTK_WINDOW (window), header);

      gtk_window_set_default_size (GTK_WINDOW (window), 600, 400);
      gtk_window_set_display (GTK_WINDOW (window),
                              gtk_widget_get_display (do_widget));
      g_signal_connect (window, "destroy",
                        G_CALLBACK (gtk_widget_destroyed), &window);

      sw = gtk_scrolled_window_new (NULL, NULL);
      gtk_container_add (GTK_CONTAINER (window), sw);

      factory = gtk_signal_list_item_factory_new ();
      g_signal_connect (factory, "setup", G_CALLBACK (setup_listitem_cb), NULL);

      gridview = gtk_grid_view_new_with_factory (factory);
      gtk_scrollable_set_hscroll_policy (GTK_SCROLLABLE (gridview), GTK_SCROLL_NATURAL);
      gtk_scrollable_set_vscroll_policy (GTK_SCROLLABLE (gridview), GTK_SCROLL_NATURAL);

      gtk_grid_view_set_max_columns (GTK_GRID_VIEW (gridview), 12);

      model = G_LIST_MODEL (gtk_sort_list_model_new (create_colors_model (), NULL));
      selection = gtk_no_selection_new (model);
      gtk_grid_view_set_model (GTK_GRID_VIEW (gridview), G_LIST_MODEL (selection));
      gtk_container_add (GTK_CONTAINER (sw), gridview);
      g_object_unref (selection);

      g_object_unref (model);
    }

  if (!gtk_widget_get_visible (window))
    gtk_widget_show (window);
  else
    gtk_widget_destroy (window);

  return window;
}