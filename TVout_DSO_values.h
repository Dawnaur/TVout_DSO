/******************************************************************************/
/*        ________                                                            */
/*        \______ \  _____  __  _  __ ____  _____    __ __ _______            */
/*         |    |  \ \__  \ \ \/ \/ //    \ \__  \  |  |  \\_  __ \           */
/*         |    `   \ / __ \_\     /|   |  \ / __ \_|  |  / |  | \/           */
/*        /_______  /(____  / \/\_/ |___|  /(____  /|____/  |__|              */
/*                \/      \/             \/      \/                           */
/*                                                                            */
/* Project:        TVout_DSO                                                  */
/* File:           TVout_DSO_values.h                                         */
/* Started:        dec. 2019                                                  */
/* Last updated:   12 jan. 2020                                               */
/* License:        MIT (https://opensource.org/licenses/MIT)                  */
/* Comment:        Project started with a challenge from ElectroBidouilleur,  */
/*                 highly inspired from Wullschj's project                    */
/*                                                                            */
/******************************************************************************/

#ifndef TVOUT_DSO_VALUES_H
# define TVOUT_DSO_VALUES_H

# define	MODE_RUN				0
# define	MODE_SINGLE				1
# define	MODE_HOLD				2
# define	MODE_NB					3

# define	SET_VSCALE				0
# define	SET_HSCALE				1
# define	SET_TRIGGER				2
# define	SET_STAT				3
# define	SET_NB					4

# define	PIN_SET					12
# define	PIN_PLUS				3
# define	PIN_MODE				4

# define	PIN_CH1					0
# define	PIN_TRIGGER				2

# define	BTN_REFRESH_INTERVAL	10000
# define	MENU_REFRESH_INTERVAL	100000

# define	SAMPLING_20K			0
# define	SAMPLING_10K			1
# define	SAMPLING_5K				2
# define	SAMPLING_2K				3
# define	SAMPLING_1K				4
# define	SAMPLING_500			5
# define	SAMPLING_200			6
# define	SAMPLING_100			7
# define	SAMPLING_NB				8

// Graphical area
# define	GFX_RES_X				120
# define	GFX_RES_Y				96
# define	GFX_GRAPH_OFFSET_X		18
# define	GFX_GRAPH_OFFSET_Y		94
# define	GFX_GRAPH_WIDTH			96
# define	GFX_GRAPH_HEIGHT		80
# define	GFX_GRAPH_UPPER_LIMIT	(GFX_GRAPH_OFFSET_Y + 1)
# define	GFX_GRAPH_LOWER_LIMIT	(GFX_GRAPH_OFFSET_Y - GFX_GRAPH_HEIGHT)
# define	GFX_SCALE_FACTOR		((1024 / GFX_GRAPH_HEIGHT) + 1)

#endif /* TVOUT_DSO_VALUES_H */
