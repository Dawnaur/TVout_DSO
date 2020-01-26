/******************************************************************************/
/*        ________                                                            */
/*        \______ \  _____  __  _  __ ____  _____    __ __ _______            */
/*         |    |  \ \__  \ \ \/ \/ //    \ \__  \  |  |  \\_  __ \           */
/*         |    `   \ / __ \_\     /|   |  \ / __ \_|  |  / |  | \/           */
/*        /_______  /(____  / \/\_/ |___|  /(____  /|____/  |__|              */
/*                \/      \/             \/      \/                           */
/*                                                                            */
/* Project:        TVout_DSO                                                  */
/* File:           TVout_DSO.ino                                              */
/* Started:        dec. 2019                                                  */
/* Last updated:   12 jan. 2020                                               */
/* License:        MIT (https://opensource.org/licenses/MIT)                  */
/* Comment:        Project started with a challenge from ElectroBidouilleur,  */
/*                 highly inspired from Wullschj's project                    */
/*                                                                            */
/******************************************************************************/

#include <TVout.h>
#include <fontALL.h>
#include "TVout_DSO_values.h"
#include "TVout_DSO.h"

extern	char *__brkval;

unsigned int	get_free_memory(void)
{
	char	top;

	return ((unsigned int)(&top - __brkval));
}
unsigned int g_free_mem;

void	setup()
{
	// set prescale to 16 -> clkADC = 1MHz -> reduces acquisition time to 13us (from 100us)
	sbi(ADCSRA,ADPS2);
	cbi(ADCSRA,ADPS1);
	cbi(ADCSRA,ADPS0);

	// Init I/O pins
	pinMode(PIN_SET, INPUT);
	pinMode(PIN_PLUS, INPUT);
	pinMode(PIN_MODE, INPUT);
	pinMode(2, INPUT);

	// Init internal variables
	g_button_set = 0;
	g_button_plus = 0;
	g_button_mode = 0;

	// Init events
	g_btn_scale_pressed = false;
	g_btn_sample_pressed = false;
	g_restart_acquisition = false;

	// Init settings
	g_run_mode = MODE_RUN;
	g_setting = SET_STAT;
	g_voltage_scale = (1024 / GFX_GRAPH_HEIGHT) + 1;
	g_echeance_mesure = micros();
	g_echeance_btn = micros();
	g_trigger_value = analogRead(PIN_TRIGGER);
	g_trigger_run = false;
	g_trigger_edge = TRIGGER_RISING_EDGE;
	g_print_stats = false;
	g_fast_mode = false;

	// Init global processing variables
	g_measure_column = 0;
	g_time_iteration = 0;
	g_sample_time = 0;
	g_offset = 0;

	// Init of timings for 20kSa/s acquisition
	g_sampling_mode = SAMPLING_20K;
	g_time_division = k_sampling_fast_values[SAMPLING_20K];
	g_ecart_mesure = k_sampling_values[SAMPLING_20K];

	// Init TV
	TV.begin(NTSC, GFX_RES_X, GFX_RES_Y);
	TV.select_font(font4x6);

	gfx_clear_graph();
	gfx_draw_v_scale();
	print_menu();

}

void	change_voltage_scale()
{
	int i;

	i = 1;
	while (i < g_voltage_scale)
		i *= 2;
	g_voltage_scale = i / 2;

	if (g_voltage_scale == 0)
		g_voltage_scale = 1024 / GFX_GRAPH_HEIGHT + 1;

	g_offset = RANGE((GFX_GRAPH_HEIGHT / 2) - (g_avg_value / g_voltage_scale), 0, GFX_GRAPH_HEIGHT - (1024 / g_voltage_scale));

	gfx_clear_graph();
	gfx_draw_v_scale();
}

void	change_time_scale()
{
	switch (g_sampling_mode)
	{
		case 0:
			g_sampling_mode = 1;
			g_time_division = k_sampling_fast_values[1];
			g_ecart_mesure = k_sampling_values[1];
			break;

		case 1:
			g_sampling_mode = 2;
			g_time_division = k_sampling_fast_values[2];
			g_ecart_mesure = k_sampling_values[2];
			break;

		case 2:
			g_sampling_mode = 3;
			g_time_division = k_sampling_fast_values[3];
			g_ecart_mesure = k_sampling_values[3];
			break;

		case 3:
			g_sampling_mode = 4;
			g_time_division = k_sampling_fast_values[4];
			g_ecart_mesure = k_sampling_values[4];
			break;

		case 4:
			g_sampling_mode = 5;
			g_time_division = k_sampling_fast_values[5];
			g_ecart_mesure = k_sampling_values[5];
			break;

		case 5:
			g_sampling_mode = 6;
			g_time_division = k_sampling_fast_values[6];
			g_ecart_mesure = k_sampling_values[6];
			break;

		case 6:
			g_sampling_mode = 7;
			g_time_division = k_sampling_fast_values[7];
			g_ecart_mesure = k_sampling_values[7];
			break;

		case 7:
			g_sampling_mode = 8;
			g_time_division = k_sampling_fast_values[8];
			g_ecart_mesure = k_sampling_values[8];
			break;

		case 8:
			g_sampling_mode = 0;
			g_time_division = k_sampling_fast_values[0];
			g_ecart_mesure = k_sampling_values[0];
			break;
	}
	gfx_clear_graph();
}

void	poll_buttons()
{
	if (digitalRead(PIN_SET) == LOW)
		g_button_set = (g_button_set << 1) | 1;
	else
		g_button_set = (g_button_set << 1) & 0xFE;

	if (digitalRead(PIN_PLUS) == LOW)
		g_button_plus = (g_button_plus << 1) | 1;
	else
		g_button_plus = (g_button_plus << 1) & 0xFE;

	if (digitalRead(PIN_MODE) == LOW)
		g_button_mode = (g_button_mode << 1) | 1;
	else
		g_button_mode = (g_button_mode << 1) & 0xFE;

	if ((g_button_set & 7) == 3)
		g_setting = (g_setting + 1) % SET_NB;

	if ((g_button_plus & 7) == 3)
	{
		switch (g_setting)
		{
			case SET_VSCALE:
				g_restart_acquisition = true;
				change_voltage_scale();
				break;

			case SET_HSCALE:
				change_time_scale();
				g_restart_acquisition = true;
				break;

			case SET_STAT:
				g_print_stats = !g_print_stats;
				if (!g_print_stats)
					TV.draw_rect(20, 10, 92, 6, 0, 0);
				break;

			case SET_TRIGGER:
				g_trigger_edge = !g_trigger_edge;
				break;
		}
	}

	if ((g_button_mode & 7) == 3)
	{
		if (g_run_mode == MODE_RUN)
		{
			g_fast_mode = true;
			g_run_mode = MODE_SINGLE;
		}
		else
		{
			g_fast_mode = false;
			g_run_mode = MODE_RUN;
		}
		g_restart_acquisition = true;
	}
}

static void	print_mode()
{
	TV.print(2, 2, k_run_mode_str[g_run_mode]);
}

static void	print_setting()
{
	TV.print(32, 2, k_setting_str[g_setting]);
	if (g_setting == SET_TRIGGER)
		TV.print(g_trigger_edge == TRIGGER_RISING_EDGE ? " /" : " \\");
	else
		TV.print("  ");
}

static void	print_stats()
{
	TV.print(20, 8, CONVERT_TO_VOLT(g_avg_value / GFX_GRAPH_WIDTH), 2);
	TV.print("V ("); 
	TV.print(CONVERT_TO_VOLT(g_min_value), 2);
	TV.print("V-"); 
	TV.print(CONVERT_TO_VOLT(g_max_value), 2);
	TV.print("V)"); 
}


static void	print_sample_rate()
{
	TV.print(78, 2, (unsigned int)(((unsigned long)GFX_GRAPH_WIDTH * 1000000UL) / g_sample_time), 10);
	TV.print(" Sa/s  ");
}

/* only for debug purposes */
static void	print_free_memory()
{
	TV.print(78, 2, g_free_mem, 10);
	TV.print(" Octets");
}

void	print_menu()
{
	print_mode();
	print_setting();
	print_sample_rate();
	print_stats();
}

void	acq_measure()
{
	static unsigned int	val = 0;
	unsigned long		time;

	time = micros();
	if (g_fast_mode == true || time > g_echeance_mesure)
	{
		if (++g_time_iteration >= g_time_division)
		{
			g_echeance_mesure += g_ecart_mesure;
			while (time > g_echeance_mesure)
			{
				g_echeance_mesure += g_ecart_mesure;
				++g_measure_column;
				g_min_value = g_min_value < val ? g_min_value : val;
				g_max_value = g_max_value > val ? g_max_value : val;
				g_avg_value += val;
			}
			g_time_iteration = 0;
			val = analogRead(PIN_CH1);
			if (g_measure_column < GFX_GRAPH_WIDTH)
				gfx_refresh_column(g_measure_column, val);
			++g_measure_column;
			g_min_value = g_min_value < val ? g_min_value : val;
			g_max_value = g_max_value > val ? g_max_value : val;
			g_avg_value += val;
			g_free_mem = get_free_memory();
			if (g_measure_column >= GFX_GRAPH_WIDTH || g_restart_acquisition)
			{
				g_trigger_run = false;
				if (g_run_mode == MODE_SINGLE && !g_restart_acquisition)
					g_run_mode = MODE_HOLD;
				if (g_restart_acquisition)
				{
					g_measure_column = GFX_GRAPH_WIDTH;
					g_restart_acquisition = 0;
				}
				detachInterrupt(digitalPinToInterrupt(2));
			}
		}
	}
}

void	loop()
{
	unsigned int	i;
	unsigned int	val;
	unsigned int	old_val = 0;
	unsigned long	time;

	g_measure_column = 0;
	if (g_run_mode != MODE_HOLD)
	{
		val = analogRead(PIN_CH1);
		old_val = val;
		while (g_measure_column < GFX_GRAPH_WIDTH)
		{
			time = micros();
			if (time > g_echeance_btn)
			{
				poll_buttons();
				while (time > g_echeance_btn)
					g_echeance_btn += 10000;
				if (g_restart_acquisition)
					g_trigger_run = false;
			}
			if (g_trigger_run == false)
			{
				old_val = val;
				g_start_time = micros();
				val = analogRead(PIN_CH1);
				g_trigger_value = analogRead(PIN_TRIGGER);
				if ((g_trigger_edge == TRIGGER_RISING_EDGE && (old_val < g_trigger_value && val >= g_trigger_value)) || 
					(g_trigger_edge == TRIGGER_FALLING_EDGE && (old_val > g_trigger_value && val <= g_trigger_value))) // ajouter timeout en mode run
				{
					g_trigger_run = true;
					g_time_iteration = 0;
					g_measure_column = 1;
					gfx_refresh_column(0, val);
					g_measure_column++;
					g_min_value = val;
					g_max_value = val;
					g_avg_value = val;
					g_echeance_mesure = g_start_time;
					if (g_fast_mode)
						attachInterrupt(digitalPinToInterrupt(2), acq_measure, RISING);
				}
			}
			else if (g_fast_mode == false)
				acq_measure();
		}
		g_trigger_run = false;
		g_sample_time = (micros() - g_start_time);
		print_menu();
	}

	i = 0;
	while (i++ < 2)
	{
		poll_buttons();
		g_trigger_value = analogRead(PIN_TRIGGER);
		delay(500UL);
	}
}

