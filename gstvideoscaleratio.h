
#ifndef __GST_VIDEOSCALERATIO_H__
#define __GST_VIDEOSCALERATIO_H__

#include <gst/gst.h>
#include <gst/video/video.h>

#include "config.h"

G_BEGIN_DECLS
/* */
#define GST_TYPE_VIDEOSCALERATIO \
    (gst_videoscaleratio_get_type ())
#define GST_VIDEOSCALERATIO(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),GST_TYPE_VIDEOSCALERATIO,GstVideoScaleRatio))
    typedef struct
{
  GstBin element;

  /* properties */
  gint width;
  gint height;

  GstPad *sinkpad, *srcpad;
  GstElement *videoscale;
  GstElement *capsfilter;

} GstVideoScaleRatio;

typedef struct
{
  GstBinClass parent_class;
} GstVideoScaleRatioClass;

G_END_DECLS
/* */
#endif /* __GST_VIDEOSCALERATIO_H__ */
