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

// Macros for setting and clearing register bits
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

// Varlables de memorisation de l'etat des boutons
boolean					g_btn_scale_pressed;
boolean					g_btn_sample_pressed;
boolean					g_restart_acquisition;

// Variables d'etat des boutons
unsigned char			g_button_set;
unsigned char			g_button_plus;
unsigned char			g_button_mode;

// Variables globales de parametrages
unsigned int			g_setting;
unsigned int			g_sampling_mode;
volatile boolean		g_fast_mode;
volatile unsigned int	g_run_mode;
boolean					g_print_stats;
unsigned int			g_trigger_value;
volatile boolean		g_trigger_run;
volatile unsigned long	g_sample_time;
int						g_offset;
int						g_voltage_scale;

// Variables de statistiques
int						g_min_value; // TODO use volatile for single acquisition
int						g_max_value;
long					g_avg_value;


const char				k_run_mode_str[][7] = {"RUN   ", "SINGLE", "HOLD  "};
const char				k_setting_str[][8] = {"V SCALE", "H SCALE", " TRIG  ", " STAT  "};
const int				k_sampling_values[] = {100UL, 100UL, 200UL, 500UL, 1000UL, 2000UL, 5000UL, 10000UL, 20000UL};
const int				k_sampling_fast_values[] = {1, 2, 4, 10, 20, 40, 100, 200, 400};

// Display
TVout					TV;

/******************************************************************************/
/*                                                                            */
/*                             FUNCTION PROTOTYPES                            */
/*                                                                            */
/******************************************************************************/
void	gfx_clear_graph();
void	gfx_draw_v_scale();
void	change_voltage_scale();
void	gfx_refresh_column(int i, int val);

#endif /* TVOUT_DSO_GFX_H */
