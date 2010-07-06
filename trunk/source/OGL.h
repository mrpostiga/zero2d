/**
 *  This file is part of Zero2D.
 *
 *  Zero2D is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Zero2D is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Zero2D.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef OGL_H
#define OGL_H

// GLEE does all the work for us. The only thing it leaves out is GLU.
#include <GL/glee.h>
#if defined(__APPLE__) || defined(__APPLE_CC__)
    #define GL_GLEXT_LEGACY
	#include <OpenGL/glu.h>
#else
    #include <GL/glu.h>
#endif

#endif
