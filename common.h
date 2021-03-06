/*
 * Copyright (C) 2002 David De Grave <david@ledav.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _COMMON_H
#define _COMMON_H

#include <stdbool.h>

#define between(c, min, max)	(((min) <= (c)) && ((c) <= (max)))	/* Check if (c) is between min and max all included	*/

extern bool		isnumber(				/* Check if given string contains a number			*/
				 const char *);			/* <- Input string (must be !NULL)				*/

#endif
