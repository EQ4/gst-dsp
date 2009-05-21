/*
 * Copyright (C) 2009 Felipe Contreras
 * Copyright (C) 2008-2009 Nokia Corporation.
 *
 * Authors:
 * Felipe Contreras <felipe.contreras@nokia.com>
 * Marco Ballesio <marco.ballesio@nokia.com>
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

#ifndef DMM_BUFFER_H
#define DMM_BUFFER_H

#include <stdlib.h> /* for calloc, free */
#include <unistd.h> /* for getpagesize */

#include "dsp_bridge.h"
#include "log.h"

#define ARM_BUFFER_ALIGNMENT 128
#define ROUND_UP(num, scale) (((num) + ((scale) - 1)) & ~((scale) - 1))

typedef struct
{
	int handle;
	void *node;
	void *data;
	void *allocated_data;
	size_t size;
	void *reserve;
	void *map;
	bool need_copy;
} dmm_buffer_t;

static inline dmm_buffer_t *
dmm_buffer_new(int handle,
	       void *node)
{
	dmm_buffer_t *b;
	b = calloc(1, sizeof(*b));

	pr_debug(NULL, "%p", b);
	b->handle = handle;
	b->node = node;

	return b;
}

static inline void
dmm_buffer_free(dmm_buffer_t *b)
{
	pr_debug(NULL, "%p", b);
	if (b) {
		if (b->map) {
			dsp_unmap(b->handle, b->node, b->map);
			dsp_unreserve(b->handle, b->node, b->reserve);
		}
		free(b->allocated_data);
		free(b);
	}
}

static inline void
dmm_buffer_map(dmm_buffer_t *b)
{
	size_t to_reserve;
	size_t page_size;
	pr_debug(NULL, "%p", b);
	page_size = getpagesize();
	to_reserve = ROUND_UP(b->size, page_size) + page_size;
	dsp_reserve(b->handle, b->node, to_reserve, &b->reserve);
	dsp_map(b->handle, b->node, b->data, b->size, b->reserve, &b->map, 0);
}

static inline void
dmm_buffer_unmap(dmm_buffer_t *b)
{
	pr_debug(NULL, "%p", b);
	if (b->map) {
		dsp_unmap(b->handle, b->node, b->map);
		dsp_unreserve(b->handle, b->node, b->reserve);
	}
}

static inline void
dmm_buffer_flush(dmm_buffer_t *b,
		 size_t len)
{
	pr_debug(NULL, "%p", b);
	dsp_flush(b->handle, b->node, b->data, len, 0);
}

static inline void
dmm_buffer_invalidate(dmm_buffer_t *b,
		      size_t len)
{
	pr_debug(NULL, "%p", b);
	dsp_invalidate(b->handle, b->node, b->data, len);
}

static inline void
dmm_buffer_allocate(dmm_buffer_t *b,
		    size_t size)
{
	pr_debug(NULL, "%p", b);
	free(b->allocated_data);
#ifdef ARM_BUFFER_ALIGNMENT
	posix_memalign(&b->allocated_data, ARM_BUFFER_ALIGNMENT, ROUND_UP(size, ARM_BUFFER_ALIGNMENT));
	b->data = b->allocated_data;
#else
	b->data = b->allocated_data = malloc(size);
#endif
	b->size = size;
	dmm_buffer_map(b);
}

static inline void
dmm_buffer_use(dmm_buffer_t *b,
	       void *data,
	       size_t size)
{
	pr_debug(NULL, "%p", b);
	b->data = data;
	b->size = size;
	dmm_buffer_map(b);
}

#endif /* DMM_BUFFER_H */
