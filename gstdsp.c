/*
 * Copyright (C) 2009 Felipe Contreras
 *
 * Author: Felipe Contreras <felipe.contreras@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation
 * version 2.1 of the License.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "gstdsp.h"

#include "gstdspdummy.h"

GstDebugCategory *gstdsp_debug;

static gboolean
plugin_init(GstPlugin *plugin)
{
#ifndef GST_DISABLE_GST_DEBUG
	gstdsp_debug = _gst_debug_category_new("dsp", 0, "DSP stuff");
#endif

	if (!gst_element_register(plugin, "dspdummy", GST_RANK_PRIMARY, GST_DSP_DUMMY_TYPE))
		return FALSE;

	return TRUE;
}

GstPluginDesc gst_plugin_desc = {
	.major_version = 0,
	.minor_version = 10,
	.name = "dsp",
	.description = (gchar *) "Texas Instruments DSP elements",
	.plugin_init = plugin_init,
	.version = VERSION,
	.license = "LGPL",
	.source = "none",
	.package = "none",
	.origin = "none",
};
