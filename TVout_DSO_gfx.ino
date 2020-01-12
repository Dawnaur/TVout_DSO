/******************************************************************************/
/*        ________                                                            */
/*        \______ \  _____  __  _  __ ____  _____    __ __ _______            */
/*         |    |  \ \__  \ \ \/ \/ //    \ \__  \  |  |  \\_  __ \           */
/*         |    `   \ / __ \_\     /|   |  \ / __ \_|  |  / |  | \/           */
/*        /_______  /(____  / \/\_/ |___|  /(____  /|____/  |__|              */
/*                \/      \/             \/      \/                           */
/*                                                                            */
/* Project:        TVout_DSO                                                  */
/* File:           TVout_DSO_gfx.ino                                          */
/* Started:        dec. 2019                                                  */
/* Last updated:   12 jan. 2020                                               */
/* License:        MIT (https://opensource.org/licenses/MIT)                  */
/* Comment:        Project started with a challenge from ElectroBidouilleur,  */
/*                 highly inspired from Wullschj's project                    */
/*                                                                            */
/******************************************************************************/

#include <TVout.h>
#include "TVout_DSO.h"

static void	gfx_pixel_put(int x, int y, char c)
{
	if ((y > GFX_GRAPH_LOWER_LIMIT) && (y < GFX_GRAPH_UPPER_LIMIT))
		TV.set_pixel(x, y, c);
}

static int	gfx_compute_y(int val)
{
	return (GFX_GRAPH_OFFSET_Y - val/g_voltage_scale - g_offset);
}

void		gfx_draw_v_scale()
{
	int	y;

	// Clear old scale
	TV.draw_rect(0, GFX_GRAPH_LOWER_LIMIT, GFX_GRAPH_OFFSET_X - 2, GFX_GRAPH_HEIGHT + 1, 0, 0);

	for (unsigned int i = 0; i <= 20; i++)
	{
		y = gfx_compute_y(1024 * i / 20);

		gfx_pixel_put(GFX_GRAPH_OFFSET_X - 2, y, 1);

		if ((i % 2 != 0) && (g_voltage_scale < 8) && (y > GFX_GRAPH_LOWER_LIMIT + 3) && (y < GFX_GRAPH_UPPER_LIMIT - 3))
			TV.print(GFX_GRAPH_OFFSET_X - 18, y - 3, i / 4.0, 2);

		// Toutes les 2 graduations
		if(!(i & 0x1))
		{
			gfx_pixel_put(GFX_GRAPH_OFFSET_X - 3, y, 1);
			if ((i % 4 != 0) && (g_voltage_scale <= 8) && (y > GFX_GRAPH_LOWER_LIMIT + 3) && (y < GFX_GRAPH_UPPER_LIMIT - 3))
				TV.print(GFX_GRAPH_OFFSET_X - 15, y - 3, i / 4.0, 1);
		}

		// Toutes les 4 graduations ( + affichage valeur)
		if(!(i & 0x3))
		{
			gfx_pixel_put(GFX_GRAPH_OFFSET_X - 4, y, 1);
			gfx_pixel_put(GFX_GRAPH_OFFSET_X - 5, y, 1);
			if((y > GFX_GRAPH_LOWER_LIMIT + 3) && (y < GFX_GRAPH_UPPER_LIMIT - 3))
				TV.print(GFX_GRAPH_OFFSET_X - 9, y - 3, i / 4);
			else if ((y > GFX_GRAPH_LOWER_LIMIT) && (y < GFX_GRAPH_UPPER_LIMIT - 3))
				TV.print(GFX_GRAPH_OFFSET_X - 9, y - 1, i / 4);
			else if ((y > GFX_GRAPH_LOWER_LIMIT + 3) && (y < GFX_GRAPH_UPPER_LIMIT))
				TV.print(GFX_GRAPH_OFFSET_X - 9, y - 3, i / 4);
		}
	}
}

void		gfx_clear_graph()
{
	TV.draw_rect(GFX_GRAPH_OFFSET_X - 1, GFX_GRAPH_OFFSET_Y - GFX_GRAPH_HEIGHT, GFX_GRAPH_WIDTH + 1, GFX_GRAPH_HEIGHT + 1, 1, 0);
}

void		gfx_refresh_column(int i, int val)
{
	static unsigned char	s_pixel_position[GFX_GRAPH_WIDTH];

	gfx_pixel_put(i + GFX_GRAPH_OFFSET_X, s_pixel_position[i], 0); // Effacement de l'ancien point
	s_pixel_position[i] = gfx_compute_y(val);
	gfx_pixel_put(i + GFX_GRAPH_OFFSET_X, s_pixel_position[i], 1); // Affichage du nouveau point
}
