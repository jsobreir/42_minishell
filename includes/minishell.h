#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <signal.h>
# include <unistd.h>
# include <stdlib.h> 
# include "./libft/libft.h"
# include "./libft/ft_printf/ft_printf.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>

# define MAX_PATH_SIZE 4096 // From Google search about path size limits in Unix

//token types
# define OUTPUT 1
# define CMD 2
# define PIPE 3
# define REDIRECT_IN 4
# define REDIRECT_OUT 5
# define INPUT 6
# define APPEND_IN 7
# define APPEND_OUT 8
# define LIMITER 9
# define ARG 10

typedef struct s_tokens
{
	char			*token;
	char			**env;
	int				type;
	struct s_tokens	*next;
	struct s_tokens	*prev;
}			t_tokens;

// Init
void	init_tokens(t_tokens *token);

// Parser
void	create_tokens(t_tokens **tokens, char *input);
void	redirect_in(t_tokens **temp);
void	redirect_out(t_tokens **temp);
void	append_out(t_tokens **temp);
void	append_in(t_tokens **temp);
void	command(t_tokens **temp);
int		is_symbol(char *token);
void	loop_assigning(t_tokens **temp, int type);

//separate types
void	process_tokens(t_tokens **tokens);
char	**join_cmd_args(t_tokens **tokens);
char	*process_full_path(char *cmd, char **envp);
char	*get_cmd(char **paths, char *cmd);

// Executor

// Builtins
int	ft_pwd(char *cwd);
int	exec_cmd(t_tokens *tokens);

// Free

// Utils
t_tokens	*find_last(t_tokens *lst);
void		add_back_list(t_tokens **lst, t_tokens *new);
t_tokens	*new_node(char *content);
void		lstclear(t_tokens **lst);
int			count_pipes(t_tokens **tokens);

// testing
void		print_tokens(t_tokens **begin_list);

// Error handling
void	badfork(int pid, char *strerr);
void	badpath(char *path, char *cmd);
void	badopen(int fd, char *file);
void	free_array(char **arr);

#endif