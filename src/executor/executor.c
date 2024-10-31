#include "minishell.h"

/// @brief 
/// @param token 
/// @param shell 
/// @return 
int	ft_isbuiltin(t_tokens *token, t_shell *shell)
{
	if (ft_strncmp(token->token, "pwd", 3) == 0)
	{
		if (ft_pwd(token, shell) != 0)
			return (1);
	}
	else if (ft_strncmp(token->token, "cd", 2) == 0)
	{
		if ((ft_cd(token, shell) != 0))
			return (1);
	}
	else if (ft_strncmp(token->token, "echo", 4) == 0)
	{
		if ((ft_echo(token, shell) != 0))
			return (1);
	}
	else if (ft_strncmp(token->token, "env", 3) == 0)
	{
		if ((ft_env(shell, token) != 0))
			return (1);
	}
	else if (ft_strncmp(token->token, "export", 6) == 0)
	{
		if ((ft_export(token, shell) != 0))
			return (1);
	}
	else if (ft_strncmp(token->token, "unset", 6) == 0)
	{
		if ((ft_unset(token, shell) != 0))
			return (1);
	}
	else
		return (0);
	return (0);
}

/// @brief Function that executes a cmd.
/// @return 
int	exec_cmd(t_tokens *tokens, t_shell *shell, int executable)
{
	int		pid;
	int		status;
	char	*path;
	char	**cmds;

	if (ft_isbuiltin(tokens, shell))
		return (0); // Is a builtin
	else
	{
		pid = fork();
		if (pid < 0)
		{
			// TODO error when badfork
			return (1);
		}
		if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			cmds = put_cmds(tokens);
			if (!cmds)
				return (1);
			if (shell->fd_in != STDIN_FILENO)
			{
				dup2(shell->fd_in, STDIN_FILENO);
				close(shell->fd_in);
			}
			if (shell->fds->fd != STDOUT_FILENO)
			{
				dup2(shell->fds->fd, STDOUT_FILENO);
				close(shell->fds->fd);
			}
			if (executable)
				handle_executable(tokens, shell);
			else
			{
				path = get_path(tokens->token, shell->envp);
				if (!path || execve(path, cmds, shell->envp) == -1)
				{
					child_cleanup(tokens, shell);
					free_paths(cmds);
					if (path)
						free(path);
					exit(10);
				}
			}
		}
		else
		{
			signal(SIGINT, signore);
			wait(&status);
			if (WIFEXITED(status))
			{
				if (WEXITSTATUS(status) == 10)
					do_error(tokens, shell, ERROR_CMD);
				else
					shell->exit_code = 0;				
			}
		}
	}
	return (0);
}

void	handle_executable(t_tokens *tokens, t_shell *shell)
{
	char *path;
	char    cwd[MAX_PATH_SIZE];
	char	*token;
	char	*dup;
	char	**args;

	token = tokens->token;
	getcwd(cwd, sizeof(cwd));
	dup = ft_strdup(&token[1]);
	path = ft_strjoin(cwd, dup);
	free(dup);
	args = malloc(sizeof(char *)* 2);
	args[0] = path;
	args[1] = NULL;
	if (!path || execve(path, args, shell->envp) == -1)
	{
		free(path);
		free(args);
	}
}

static void	handle_dir_file(t_tokens *tokens, t_shell *shell)
{
	char *token;
	int	file;

	token = tokens->token;
	file = is_file(tokens->token);
	if (!strncmp(token, ".", ft_strlen(token)))
		do_error(tokens, shell, ERROR_FAR);
	else if (!strncmp(token, "~", ft_strlen(token)))
		do_error(tokens, shell, ERROR_TILD);
	else if (file == 1)
		exec_cmd(tokens, shell, 1);
	else if (file == 2)
		do_error(tokens, shell, IS_DIR);
	else if (file == 4)
		do_error(tokens, shell, P_DENY);
	else if (!is_file(tokens->token))
		do_error(tokens, shell, ERROR_OPEN);
}

void	execute(t_tokens *tokens, t_shell *shell)
{
	t_tokens	*temp;
	t_pipe		p;
	int			pid[shell->n_pipes];

	temp = tokens;
	p.pid = pid;
	if (temp->type == DIR_FILE)
		return (handle_dir_file(temp, shell));
	if (shell->n_pipes)
	{
		p.i = -1;
		while (++p.i <= shell->n_pipes)
		{
			if (pipe(p.fd) == -1)
				;
			p.pid[p.i] = fork();
			do_pipe(temp, shell, &p);
			set_next_pipe(&temp);
		}
		wait_allchildren(tokens, shell, p.pid);
		dup2(shell->original_stdin, STDIN_FILENO);
	}
	else
		exec_cmd(tokens, shell, 0);
}
