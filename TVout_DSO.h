/******************************************************************************/
/*        ________                                                            */
/*        \______ \  _____  __  _  __ ____  _____    __ __ _______            */
/*         |    |  \ \__  \ \ \/ \/ //    \ \__  \  |  |  \\_  __ \           */
/*         |    `   \ / __ \_\     /|   |  \ / __ \_|  |  / |  | \/           */
/*        /_______  /(____  / \/\_/ |___|  /(____  /|____/  |__|              */
/*                \/      \/             \/      \/                           */
/*                                                                            */
/* Project:        TVout_DSO                                                  */
/* File:           TVout_DSO.h                                                */
/* Started:        dec. 2019                                                  */
/* Last updated:   12 jan. 2020                                               */
/* License:        MIT (https://opensource.org/licenses/MIT)                  */
/* Comment:        Project started with a challenge from ElectroBidouilleur,  */
/*                 highly inspired from Wullschj's project                    */
/*                                                                            */
/******************************************************************************/

#ifndef TVOUT_DSO_GFX_H
# define TVOUT_DSO_GFX_H

# include <TVout.h>

/******************************************************************************/
/*                                                                            */
/*                                   MACROS                                   */
/*                                                                            */
/******************************************************************************/

// needs to be modified for ADC of less or more than 10 bits
# define	CONVERT_TO_VOLT(X)	(X * 0.0048828125)
# define	RANGE(X, MIN, MAX)	(X < MIN ? MIN : (X > MAX ? MAX : X))

// Macros used to set or clear register bits
# ifndef cbi
#  define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
# endif
# ifndef sbi
#  define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
# endif

/******************************************************************************/
/*                                                                            */
/*                               GLOBAL VARIABLES                             */
/*                                                                            */
/******************************************************************************/

unsigned long			g_start_time;
unsigned int			g_time_division;
volatile unsigned int	g_time_iteration;
unsigned int			g_ecart_mesure;
volatile unsigned long	g_echeance_mesure;
volatile unsigned long	g_echeance_btn;
volatile unsigned int	g_measure_column;

// Flags for button actions
boolean					g_btn_scale_pressed;
boolean					g_btn_sample_pressed;
boolean					g_restart_acquisition;

// Button states
unsigned char			g_button_set;
unsigned char			g_button_plus;
unsigned char			g_button_mode;

// Settings
unsigned int			g_setting;
unsigned int			g_sampling_mode;
volatile boolean		g_fast_mode;
volatile unsigned int	g_run_mode;
boolean					g_print_stats;
unsigned int			g_trigger_value;
volatile boolean		g_trigger_run;
boolean					g_trigger_edge;
volatile unsigned long	g_sample_time;
int						g_offset;
int						g_voltage_scale;

// Stats
volatile unsigned int	g_min_value; // TODO use volatile for single acquisition
volatile unsigned int	g_max_value;
volatile long			g_avg_value;

// Values and strings tables
const char				k_run_mode_str[][7] = {"RUN   ", "SINGLE", "HOLD  "};
const char				k_setting_str[][8] = {"V SCALE", "H SCALE", "TRIGGER", " STAT  "};
const int				k_sampling_values[] = {100UL, 100UL, 200UL, 500UL, 1000UL, 2000UL, 5000UL, 10000UL, 20000UL};
const int				k_sampling_fast_values[] = {1, 2, 4, 10, 20, 40, 100, 200, 400};

// Display
TVout					TV;

/******************************************************************************/
/*                                                                            */
/*                             FUNCTION PROTOTYPES                            */
/*                                                                            */
/******************************************************************************/

/* Clears the graph area by drawing a black rectangle */
void	gfx_clear_graph();

/* Draws the vertical scale aside the graph */
void	gfx_draw_v_scale();

/* Compute the new voltage scale and offset to center the signal */
void	change_voltage_scale();

/* Change time scale by delaying acquisitions */
void	change_time_scale();

/* Check changes on buttons */
void	poll_buttons();

/* Prints menu with mode, informations, and settings */
void	print_menu();

/* Launch an acquisition and draw the result */
void	acq_measure();

/* Refresh the column by removing old pixel and putting the new one */
void	gfx_refresh_column(int i, int val);

#endif /* TVOUT_DSO_GFX_H */
