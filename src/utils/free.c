#include "minishell.h"

void	free_all(t_tokens *tokens, t_shell *shell, char *input_buffer)
{
	if (shell->last_path)
		free(shell->last_path);
	if (shell->envp)
		free_paths(shell->envp);
	if (tokens)
		lstclear(&tokens);
	if (shell->original_stdin)
		close(shell->original_stdin);
	if (shell->original_stdout)
		close(shell->original_stdout);
	if (input_buffer && shell->fds->in)
		close(shell->fds->in);
	if (input_buffer && shell->fds->out)
		close(shell->fds->out);
	if (input_buffer)
		free(input_buffer);
	// if (shell->p && shell->p->fd[1])
	// 	close(shell->p->fd[1]);
	// if (shell->p && shell->p->fd[0])
	// 	close(shell->p->fd[0]);
}