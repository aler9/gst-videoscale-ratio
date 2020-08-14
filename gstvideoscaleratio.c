
#include "gstvideoscaleratio.h"


#define gst_videoscaleratio_parent_class parent_class
G_DEFINE_TYPE (GstVideoScaleRatio, gst_videoscaleratio, GST_TYPE_BIN);

static GstStaticPadTemplate sink_factory = GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS (GST_VIDEO_CAPS_MAKE (GST_VIDEO_FORMATS_ALL) ";"
        GST_VIDEO_CAPS_MAKE_WITH_FEATURES ("ANY", GST_VIDEO_FORMATS_ALL))
    );

static GstStaticPadTemplate src_factory = GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS (GST_VIDEO_CAPS_MAKE (GST_VIDEO_FORMATS_ALL) ";"
        GST_VIDEO_CAPS_MAKE_WITH_FEATURES ("ANY", GST_VIDEO_FORMATS_ALL))
    );

enum
{
  PROP_0,
  PROP_WIDTH,
  PROP_HEIGHT,
};

static void
gst_videoscaleratio_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec)
{
  GstVideoScaleRatio *filter = GST_VIDEOSCALERATIO (object);

  switch (prop_id) {
    case PROP_WIDTH:
      filter->width = g_value_get_int (value);
      break;

    case PROP_HEIGHT:
      filter->height = g_value_get_int (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
gst_videoscaleratio_class_init (GstVideoScaleRatioClass * klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GstElementClass *element_class = (GstElementClass *) klass;

  object_class->set_property = gst_videoscaleratio_set_property;

  g_object_class_install_property (object_class, PROP_WIDTH,
      g_param_spec_int ("width", "Width", "The width of the output frame",
          0, 99999, 0, G_PARAM_WRITABLE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (object_class, PROP_HEIGHT,
      g_param_spec_int ("height", "Height", "The height of the output frame",
          0, 99999, 0, G_PARAM_WRITABLE | G_PARAM_STATIC_STRINGS));

  gst_element_class_add_pad_template (element_class,
      gst_static_pad_template_get (&sink_factory));

  gst_element_class_add_pad_template (element_class,
      gst_static_pad_template_get (&src_factory));
}

static gboolean
gst_videoscaleratio_sink_event (GstPad * pad, GstObject * parent,
    GstEvent * event)
{
  GstVideoScaleRatio *filter = GST_VIDEOSCALERATIO (parent);

  if (GST_EVENT_TYPE (event) == GST_EVENT_CAPS) {
    GstCaps *caps;
    gst_event_parse_caps (event, &caps);
    GstVideoInfo info;
    gst_video_info_from_caps (&info, caps);

    if (filter->height > 0) {
      info.width = info.width * filter->height / info.height;
      info.height = filter->height;

    } else if (filter->width > 0) {
      info.height = info.height * filter->width / info.width;
      info.width = filter->width;
    }

    if ((info.width % 2) != 0) {
      info.width++;
    }
    if ((info.height % 2) != 0) {
      info.height++;
    }

    GstCaps *new_caps = gst_caps_copy (caps);
    gst_caps_set_simple (new_caps, "width", G_TYPE_INT, info.width, NULL);
    gst_caps_set_simple (new_caps, "height", G_TYPE_INT, info.height, NULL);
    g_object_set (G_OBJECT (filter->capsfilter), "caps", new_caps, NULL);
  }

  return gst_pad_event_default (pad, parent, event);
}

static void
gst_videoscaleratio_init (GstVideoScaleRatio * filter)
{
  GstElement *element = GST_ELEMENT (filter);

  filter->videoscale = gst_element_factory_make ("videoscale", NULL);
  g_object_set (G_OBJECT (filter->videoscale), "add-borders", FALSE, NULL);

  filter->capsfilter = gst_element_factory_make ("capsfilter", NULL);

  // link videoscale to capsfilter
  gst_bin_add_many (GST_BIN (filter), filter->videoscale, filter->capsfilter,
      NULL);
  gst_element_link_many (filter->videoscale, filter->capsfilter, NULL);

  // link sinkpad to videoscale
  GstPadTemplate *tmpl = gst_static_pad_template_get (&sink_factory);
  GstPad *pad = gst_element_get_static_pad (filter->videoscale, "sink");
  filter->sinkpad = gst_ghost_pad_new_from_template ("sink", pad, tmpl);
  gst_object_unref (pad);
  gst_pad_set_event_function (filter->sinkpad, gst_videoscaleratio_sink_event);
  gst_object_unref (tmpl);
  gst_element_add_pad (element, filter->sinkpad);

  // link capsfilter to srcpad
  tmpl = gst_static_pad_template_get (&src_factory);
  pad = gst_element_get_static_pad (filter->capsfilter, "src");
  filter->srcpad = gst_ghost_pad_new_from_template ("src", pad, tmpl);
  gst_object_unref (pad);
  gst_object_unref (tmpl);
  gst_element_add_pad (element, filter->srcpad);
}

static gboolean
plugin_init (GstPlugin * plugin)
{
  return gst_element_register (plugin, "videoscaleratio",
      GST_RANK_NONE, GST_TYPE_VIDEOSCALERATIO);
}

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    videoscaleratio,
    "Resizes video and keep aspect ratio",
    plugin_init,
    PACKAGE_VERSION, GST_LICENSE, GST_PACKAGE_NAME, GST_PACKAGE_ORIGIN)
