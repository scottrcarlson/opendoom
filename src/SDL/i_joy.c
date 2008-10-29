/* Emacs style mode select   -*- C++ -*-
 *-----------------------------------------------------------------------------
 *
 *
 *  PrBoom: a Doom port merged with LxDoom and LSDLDoom
 *  based on BOOM, a modified and improved DOOM engine
 *  Copyright (C) 1999 by
 *  id Software, Chi Hoang, Lee Killough, Jim Flynn, Rand Phares, Ty Halderman
 *  Copyright (C) 1999-2000 by
 *  Jess Haas, Nicolas Kalkhof, Colin Phipps, Florian Schulze
 *  Copyright 2005, 2006 by
 *  Florian Schulze, Colin Phipps, Neil Stevens, Andrey Budko
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 *  02111-1307, USA.
 *
 * DESCRIPTION:
 *   Joystick handling for Linux
 *
 *-----------------------------------------------------------------------------
 */

#ifndef lint
#endif /* lint */

#include <stdlib.h>

#include "SDL.h"
#include "doomdef.h"
#include "doomtype.h"
#include "m_argv.h"
#include "d_event.h"
#include "d_main.h"
#include "i_joy.h"
#include "lprintf.h"

int joyleft;
int joyright;
int joyup;
int joydown;

int usejoystick;

#ifdef HAVE_SDL_JOYSTICKGETAXIS
static SDL_Joystick *joystick;
#endif


void I_PollAccelerometer(void)
{
  const unsigned int report_len = 16;
  unsigned char report[report_len];
  struct accel_3d_t accel;
  accel.val[0] = 0.0;
  accel.val[1] = 0.0;
  accel.val[2] = 0.0;

  int file_event;
  file_event = open("/dev/input/event2", O_RDONLY);
  while (rel!=0)
    {
      int read_len = read(file_event, report, report_len);
      /* this was a blocking read */
      if (read_len < 0)
	{
	  //                        perror("read");                                                                                              
	  //                        continue;                                                                                                    
	}

      else {
	rel = *(unsigned short int *)(report + 8);
	/*                                                                                                                     
	 * Neo sends three reports on X, Y, and Z with rel = 2                                                                 
	 * and another one (as a separator) with rel = 0                                                                       
	 */
	if (rel == 2)
	  {
	    unsigned short int axis_ind = *(short int *)(report + 10);
	    /* receives signed acceleration in milli-G */
	    int val_mg = *(int *)(report + 12);
	    /* convert acceleration to G */
	    float val_g = (float)val_mg / 1000.0;

	    /* save to accel on the axis */
	    accel.val[axis_ind] = val_g;
	  }
	else if (rel == 0) /* separator report */
	  {
	    /* is touchscreen pressed? */
	    //  int read_len = read(neo->screen_desc, report, report_len);                                                   
	    /* this was a non-blocking read */
	    //if (read_len < 0)                                                                                            
	    //{                                                                                                            
	    //      perror("read");                                                                                        
	    //      continue;                                                                                              
	    //}                                                                                                            
	    //         pressed = (read_len > 0);                                                                                    
	    /* call back is called */
	    //         neo->handle_recv(pressed, accel);                                                                            
	    //if (accel.val[0]>-0.10 && accel.val[1]<0.10)
	    //  lastkey = sc_K; /* forward */
	    //else if (accel.val[0]<-0.12 && accel.val[1]>0.12)
	    //  lastkey = sc_J; /* backward */
	    //else if (accel.val[0]>-0.12 && ((0.04+accel.val[0]*-1)<accel.val[1]))
	    //  lastkey = sc_H; /* left */
	    //else if (accel.val[1]<0.12 &&  (0.03+accel.val[1]<(accel.val[0]*-1)))
	    //  lastkey = sc_L; /* right */
	  }
      }
    }
  close(file_event);
  ev.type = ev_joystick;
  ev.data1 = accel.val[0];
  ev.data2 = accel.val[1];
  ev.data3 = accel.val[2];
  D_PostEvent(&ev);
}

static void I_EndJoystick(void)
{
  lprintf(LO_DEBUG, "I_EndJoystick : closing joystick\n");
}

void I_PollJoystick(void)
{
#ifdef HAVE_SDL_JOYSTICKGETAXIS
  event_t ev;
  Sint16 axis_value;

  if (!usejoystick || (!joystick)) return;
  ev.type = ev_joystick;
  ev.data1 =
    (SDL_JoystickGetButton(joystick, 0)<<0) |
    (SDL_JoystickGetButton(joystick, 1)<<1) |
    (SDL_JoystickGetButton(joystick, 2)<<2) |
    (SDL_JoystickGetButton(joystick, 3)<<3);
  axis_value = SDL_JoystickGetAxis(joystick, 0) / 3000;
  if (abs(axis_value)<10) axis_value=0;
  ev.data2 = axis_value;
  axis_value = SDL_JoystickGetAxis(joystick, 1) / 3000;
  if (abs(axis_value)<10) axis_value=0;
  ev.data3 = axis_value;

  D_PostEvent(&ev);
#endif
}

void I_InitJoystick(void)
{
#ifdef HAVE_SDL_JOYSTICKGETAXIS
  const char* fname = "I_InitJoystick : ";
  int num_joysticks;

  if (!usejoystick) return;
  SDL_InitSubSystem(SDL_INIT_JOYSTICK);
  num_joysticks=SDL_NumJoysticks();
  if (M_CheckParm("-nojoy") || (usejoystick>num_joysticks) || (usejoystick<0)) {
    if ((usejoystick > num_joysticks) || (usejoystick < 0))
      lprintf(LO_WARN, "%sinvalid joystick %d\n", fname, usejoystick);
    else
      lprintf(LO_INFO, "%suser disabled\n", fname);
    return;
  }
  joystick=SDL_JoystickOpen(usejoystick-1);
  if (!joystick)
    lprintf(LO_ERROR, "%serror opening joystick %s\n", fname, SDL_JoystickName(usejoystick-1));
  else {
    atexit(I_EndJoystick);
    lprintf(LO_INFO, "%sopened %s\n", fname, SDL_JoystickName(usejoystick-1));
    joyup = 32767;
    joydown = -32768;
    joyright = 32767;
    joyleft = -32768;
  }
#endif
}
