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
    char *line;       /* Ligne saisie par l'utilisateur */
    size_t len;       /* Taille de la ligne */
    int status;       /* Statut de la lecture */
} t_input;

/* Parsing */
typedef struct s_parse
{
    char *command;   /* la commande ex: "ls" */
    char **argv;     /* tableau d’arguments ex: {"ls", "-l", NULL} */
} t_parse;

/* Exécution */
typedef struct s_execute
{
    int result;       /* 0 = succès, -1 = erreur */
} t_execute;

/* Structure globale pour le shell */
typedef struct s_shell
{
    t_prompt prompt;
    t_input input;
    t_parse parse;
    t_execute exec;
    int running;      /* Flag pour savoir si le shell continue */
} t_shell;

/* ===================== Prototypes ===================== */

/* Prompt */
void display_prompt(t_prompt *prompt);

/* Lecture de la commande */
int read_input(t_input *input);

/* Parsing simple */
void parse_command(t_input *input, t_parse *parse);

/* Exécution d'une commande simple */
void execute_command(t_parse *parse, t_execute *exec);

/* Libération de la mémoire */
void free_input(t_input *input);
void free_parse(t_parse *parse);

char *get_env_value(const char *name);

#endif /* SHELL_H */
