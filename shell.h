#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

/* ===================== Structures ===================== */
/**
 * struct s_prompt - Represents the shell prompt
 * @text: The string to display as the prompt
 */
typedef struct s_prompt
{
    char *text;
} t_prompt;

/**
 * struct s_input - Stores user input line
 * @line: The raw input line
 * @len: Length of the input buffer
 * @status: Status flag for reading input
 */
typedef struct s_input
{
    char *line;
    size_t len;
    int status;
} t_input;

/**
 * struct s_parse - Holds parsed command and arguments
 * @command: Command to execute
 * @argv: Array of arguments
 */
typedef struct s_parse
{
    char *command;
    char **argv;
} t_parse;

/**
 * struct s_execute - Stores the execution result
 * @result: Exit status of the executed command
 */
typedef struct s_execute
{
    int result;
} t_execute;

/**
 * struct s_shell - Main shell structure
 * @prompt: Prompt structure
 * @input: Input structure
 * @parse: Parse structure
 * @exec: Execute structure
 * @running: Flag to control shell loop
 */
typedef struct s_shell
{
    t_prompt prompt;
    t_input input;
    t_parse parse;
    t_execute exec;
    int running;
} t_shell;

/* ===================== Prototypes ===================== */

/* Prompt */
void display_prompt(t_prompt *prompt);

/* Lecture de la commande */
int read_input(t_input *input);

/* Parsing simple */
void parse_command(t_input *input, t_parse *parse);

/* Exécution d'une commande simple */
int execute_command(t_parse *parse, t_execute *exec);

/* Libération de la mémoire */
void free_input(t_input *input);
void free_parse(t_parse *parse);

/* Récupération d'une variable d'environnement */
char *get_env_value(const char *name);

/* Recherche le chemin complet d'une commande */
char *find_path(char *command);

#endif /* SHELL_H */
