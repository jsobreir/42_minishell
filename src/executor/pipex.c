#include "minishell.h"

static void	parent_process(int *new_fd)
{
 	dup2(new_fd[0], STDIN_FILENO);
	close(new_fd[0]);
	close(new_fd[1]);
}

static void	prepare_exec(t_tokens *tokens, t_shell *shell)
{
	int		res;
	char	**cmds;
	char	*path;

	cmds = put_cmds(tokens);
	res = ft_isbuiltin(tokens, shell); // Pq esta verificacao aqui?
	if (res)
	{
		child_cleanup(tokens, shell);
		free_paths(cmds);
		exit(0);
	}
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

void	do_pipe(t_tokens *tokens, t_shell *shell, t_pipe *p)
{
	if (p->pid[p->i] == 0)
	{
		signal(SIGINT, SIG_DFL); //TODO: dar mute aos outros sinais tambem
		if (p->i == 0 && shell->fd_in != STDIN_FILENO)
			dup2(shell->fd_in, STDIN_FILENO);
		if (p->i == shell->n_pipes && shell->fd_out != STDOUT_FILENO)
			dup2(shell->fd_out, STDOUT_FILENO);
		else if (p->i != shell->n_pipes)
			dup2(p->fd[1], STDOUT_FILENO);
		close(p->fd[1]);
		close(p->fd[0]);
		prepare_exec(tokens, shell);
	}
	else
		signal(SIGINT, signore);
	parent_process(p->fd);
}
