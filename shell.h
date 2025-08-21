#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

/* ===================== Structures ===================== */

/* Affichage du prompt */
typedef struct s_prompt
{
    char *text;
} t_prompt;

/* Lecture de la ligne de commande */
typedef struct s_input
{
    char *line;
    size_t len;
    int status;
} t_input;

/* Parsing */
typedef struct s_parse
{
    char *command;
    char **argv;
} t_parse;

/* Exécution */
typedef struct s_execute
{
    int result;
} t_execute;

/* Structure globale pour le shell */
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
