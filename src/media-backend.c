/*
 * media-backend.c - Source for HazeMediaBackend
 * Copyright © 2006-2009 Collabora Ltd.
 * Copyright © 2006-2009 Nokia Corporation
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "config.h"
#include "media-backend.h"

#include "media/backend-iface.h"

#include "debug.h"

static void media_backend_iface_init(PurpleMediaBackendIface *iface);

G_DEFINE_TYPE_WITH_CODE (HazeMediaBackend,
    haze_media_backend,
    G_TYPE_OBJECT,
    G_IMPLEMENT_INTERFACE (PURPLE_TYPE_MEDIA_BACKEND,
      media_backend_iface_init)
    )

/* properties */
enum
{
  PROP_CONFERENCE_TYPE = 1,
  PROP_MEDIA,
  LAST_PROPERTY
};

/* private structure */
struct _HazeMediaBackendPrivate
{
  gchar *conference_type;
  PurpleMedia *media;
};

static void
haze_media_backend_init (HazeMediaBackend *self)
{
  HazeMediaBackendPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE (self,
      HAZE_TYPE_MEDIA_BACKEND, HazeMediaBackendPrivate);

  self->priv = priv;
}

static void
haze_media_backend_get_property (GObject    *object,
                                 guint       property_id,
                                 GValue     *value,
                                 GParamSpec *pspec)
{
  HazeMediaBackend *backend = HAZE_MEDIA_BACKEND (object);
  HazeMediaBackendPrivate *priv = backend->priv;

  switch (property_id)
    {
    case PROP_CONFERENCE_TYPE:
      g_value_set_string (value, priv->conference_type);
      break;
    case PROP_MEDIA:
      g_value_set_object (value, priv->media);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
haze_media_backend_set_property (GObject      *object,
                                 guint         property_id,
                                 const GValue *value,
                                 GParamSpec   *pspec)
{
  HazeMediaBackend *backend = HAZE_MEDIA_BACKEND (object);
  HazeMediaBackendPrivate *priv = backend->priv;

  switch (property_id)
    {
    case PROP_CONFERENCE_TYPE:
      g_free (priv->conference_type);
      priv->conference_type = g_value_dup_string (value);
      break;
    case PROP_MEDIA:
      g_assert (priv->media == NULL);
      priv->media = g_value_dup_object (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void haze_media_backend_dispose (GObject *object);
static void haze_media_backend_finalize (GObject *object);

static void
haze_media_backend_class_init (HazeMediaBackendClass *haze_media_backend_class)
{
  GObjectClass *object_class = G_OBJECT_CLASS (haze_media_backend_class);

  g_type_class_add_private (haze_media_backend_class,
      sizeof (HazeMediaBackendPrivate));

  object_class->get_property = haze_media_backend_get_property;
  object_class->set_property = haze_media_backend_set_property;

  object_class->dispose = haze_media_backend_dispose;
  object_class->finalize = haze_media_backend_finalize;

  g_object_class_override_property(object_class, PROP_CONFERENCE_TYPE,
      "conference-type");
  g_object_class_override_property(object_class, PROP_MEDIA, "media");
}

void
haze_media_backend_dispose (GObject *object)
{
  HazeMediaBackend *self = HAZE_MEDIA_BACKEND (object);
  HazeMediaBackendPrivate *priv = self->priv;

  DEBUG ("called");

  if (priv->media != NULL)
    {
      g_object_unref (priv->media);
      priv->media = NULL;
    }

  if (G_OBJECT_CLASS (haze_media_backend_parent_class)->dispose)
    G_OBJECT_CLASS (haze_media_backend_parent_class)->dispose (object);
}

void
haze_media_backend_finalize (GObject *object)
{
  HazeMediaBackend *self = HAZE_MEDIA_BACKEND (object);
  HazeMediaBackendPrivate *priv = self->priv;

  g_free(priv->conference_type);

  G_OBJECT_CLASS (haze_media_backend_parent_class)->finalize (object);
}

static gboolean
haze_media_backend_add_stream (PurpleMediaBackend *self,
    const gchar *sid, const gchar *who,
    PurpleMediaSessionType type, gboolean initiator,
    const gchar *transmitter,
    guint num_params, GParameter *params)
{
  return FALSE;
}

static void
haze_media_backend_add_remote_candidates (PurpleMediaBackend *self,
                                          const gchar *sid,
                                          const gchar *who,
                                          GList *remote_candidates)
{

}

static gboolean
haze_media_backend_codecs_ready (PurpleMediaBackend *self,
                                 const gchar *sid)
{
  return FALSE;
}

static GList *
haze_media_backend_get_codecs (PurpleMediaBackend *self,
                               const gchar *sid)
{
  return NULL;
}

static GList *
haze_media_backend_get_local_candidates (PurpleMediaBackend *self,
                                         const gchar *sid,
                                         const gchar *who)
{
  return NULL;
}

static gboolean
haze_media_backend_set_remote_codecs (PurpleMediaBackend *self,
                                      const gchar *sid,
                                      const gchar *who,
                                      GList *codecs)
{
  return FALSE;
}

static gboolean
haze_media_backend_set_send_codec (PurpleMediaBackend *self,
                                   const gchar *sid,
                                   PurpleMediaCodec *codec)
{
  return FALSE;
}

static void
media_backend_iface_init (PurpleMediaBackendIface *iface)
{
#define IMPLEMENT(x) iface->x = haze_media_backend_##x
  IMPLEMENT(add_stream);
  IMPLEMENT(add_remote_candidates);
  IMPLEMENT(codecs_ready);
  IMPLEMENT(get_codecs);
  IMPLEMENT(get_local_candidates);
  IMPLEMENT(set_remote_codecs);
  IMPLEMENT(set_send_codec);
#undef IMPLEMENT
}
