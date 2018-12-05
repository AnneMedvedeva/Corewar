/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   corewar.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrazhni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/24 14:23:28 by vbrazhni          #+#    #+#             */
/*   Updated: 2018/12/05 21:41:08 by vbrazhni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COREWAR_H

# define COREWAR_H

# include "op.h"
# include "vs.h"
# include "libft.h"
# include <stdint.h>

/*
** Macroses
*/

# define INDEX(X)		((X) - 1)

# define OP_CODE_LEN	1
# define ARGS_CODE_LEN	1
# define REG_LEN		1
# define IND_LEN		2

/*
** Arg's type — Arg's code
*/

static uint8_t			g_arg_code[3] = {
	T_REG,
	T_DIR,
	T_IND
};

/*
** Player
*/

/*
** id        — id number of player
** name      — champion's name
** comment   — champion's comment
** code_size — size of champion's executable code
** code      — champion's executable code
** lives_num — number of reports that player is alive during cycles_to_die
** last_live — cycle's number, on which player was assigned as alive last time
** next      — pointer to the next player
*/

typedef struct			s_player
{
	int32_t				id;
	char				*name;
	char				*comment;
	int32_t				code_size;
	uint8_t				*code;
	size_t				lives_num;
	size_t				last_live;
	struct s_player		*next;
}						t_player;

/*
** Cursor
*/

/*
** id             — id number of cursor
** carry          — special flag
** op_code        — operator's code that is placed under cursor
** lives_num      — number of executed live operator during of cycle to die
** last_live      — cycle's number when live operator was executed last time
** cycles_to_exec — number of cycles that remains to wait
** args_types     - types of op's each argument
**                  before operator execution
** pc             — address of the next operator to execute at memory
** step           — number of bytes to shift
** reg            — registers
** next           — pointer to the next cursor
*/

typedef struct			s_cursor
{
	uint32_t			id;
	t_bool				carry;
	uint8_t				op_code;
	size_t				lives_num;
	size_t				last_live;
	int					cycles_to_exec;
	uint8_t				args_types[3];
	int32_t				pc;
	uint32_t			step;
	int32_t				reg[REG_NUMBER];
	struct s_cursor		*next;
}						t_cursor;

/*
** Virtual machine
*/

/*
** arena              — memory where players are fighting
** players            — list of players
** players_num        — number of players
** last_alive         — pointer to the last player that was assigned as alive
** cursors            — list of cursors
** cursors_num        — number of cursors
** cycles             — number of cycles that was passed after start
** cycles_to_die      — game parameter
** cycles_after_check — number of cycles that was passed after last rules check
** checks_num         — game parameter
** vs                 — visualizer
** dump               — cycle's number after which dump will be created
** display_aff        — flag that reports display output of aff operator or not
** log                — number that reports about log level.
**                      If log is assigned as -1, it means that log doesn't
**                      display.
*/

typedef struct			s_vm
{
	uint8_t				arena[MEM_SIZE];
	t_player			*players[MAX_PLAYERS];
	size_t				players_num;
	t_player			*last_alive;
	t_cursor			*cursors;
	size_t				cursors_num;
	size_t				cycles;
	ssize_t				cycles_to_die;
	size_t				cycles_after_check;
	size_t				checks_num;
	t_vs				*vs;
	ssize_t				dump_cycle;
	int					dump_print_mode;
	ssize_t				show_cycle;
	int					show_print_mode;
	t_bool				display_aff;
	int					log;
}						t_vm;

/*
** Functions
*/

/*
** Init
*/

t_player				*init_player(int id);

t_cursor				*init_cursor(int32_t id, int32_t pc);

t_vm					*init_vm(void);

void					init_arena(t_vm *vm);

/*
** Parse
*/

void					parse_args(int argc, char **argv, t_vm *vm);

void					parse_vs_flag(int *argc, char ***argv, t_vm *vm);

void					parse_dump_flag(int *argc, char ***argv, t_vm *vm);

void					parse_show_flag(int *argc, char ***argv, t_vm *vm);

void					parse_aff_flag(int *argc, char ***argv, t_vm *vm);

void					parse_log_flag(int *argc, char ***argv, t_vm *vm);

t_player				*parse_champion(char *filename, int num);

static void				add_player(t_player **list, t_player *new);

void					add_cursor(t_cursor **list, t_cursor *new);

/*
** Execute
*/

void					exec(t_vm *vm);

/*
** Execute Utils
*/

void					update_op_code(t_vm *vm, t_cursor *current);

void					move_cursor(t_cursor *cursor);

/*
** Find
*/

t_player				*find_player(t_player *list, int32_t id);

/*
** Free
*/

void					delete_cursors(t_vm *vm);

void					free_players(t_player **players, size_t players_num);

/*
** Utils
*/

void					terminate(char *s);

int32_t					bytecode_to_int32_ptr(const uint8_t *bytecode,
															size_t size);

t_bool					is_filename(const char *filename);

int32_t					calc_addr(int32_t addr);

int8_t					get_byte(t_vm *vm, int32_t pc, int32_t step);

uint32_t				calc_lives_num(t_cursor *cursor);

void					cycles_to_die_check(t_vm *vm);

/*
** Print
*/

void					print_intro(t_player **players, size_t players_num);

void					print_last_alive(t_vm *vm);

void					print_help();

void					print_arena(uint8_t *arena, int print_mode);

/*
** Log
*/

/*
** Levels
*/

# define LIVE_LOG			1
# define CYCLE_LOG			2
# define OP_LOG				4
# define DEATH_LOG			8
# define PC_MOVEMENT_LOG	16

/*
** Functions
*/

void					log_cycle(size_t cycle);

void					log_pc_movements(uint8_t *arena, t_cursor *cursor);

void					log_cursor_death(t_vm *vm, t_cursor *cursor);

void					log_cycles_to_die(ssize_t cycles_to_die);

#endif
