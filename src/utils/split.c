#include "minishell.h"

int malloc_gone_wrong(char **arr, int j)
{
	if (!arr[j])
	{
		while (j--)
			free(arr[j]);
		free(arr);
		return (1);
	}
	return (0);
}

void	split_quotes(t_split *sp, char c)
{
	sp->arr[sp->j] = malloc(ft_word_len(sp->s, sp->i, c) * sizeof(char));
	if (malloc_gone_wrong(sp->arr, sp->j))
		return ;
	sp->arr[sp->j][sp->k++] = sp->s[sp->i++];
	while (sp->s[sp->i] != c)
		sp->arr[sp->j][sp->k++] = sp->s[sp->i++];
	while (sp->s[sp->i] && sp->s[sp->i] != ' ' && sp->s[sp->i] != '|')
		sp->arr[sp->j][sp->k++] = sp->s[sp->i++];
}

void	put_word(t_split *sp, int c)
{
	if (c == 0)
	{
		sp->arr[sp->j] = malloc(ft_word_len(sp->s, sp->i, 0) * sizeof(char));
		if (malloc_gone_wrong(sp->arr, sp->j))
			return ;
		while (sp->s[sp->i] && sp->s[sp->i] != ' ' && sp->s[sp->i] != '|')
			sp->arr[sp->j][sp->k++] = sp->s[sp->i++];
	}
	else if (c == PIPE)
	{
		sp->arr[sp->j] = malloc(2 * sizeof(char));
		if (malloc_gone_wrong(sp->arr, sp->j))
			return ;
		sp->arr[sp->j][sp->k++] = sp->s[sp->i++];
	}
}

void	split_words(t_split *sp)
{
	while (sp->s[sp->i])
	{
		sp->k = 0;
		while (sp->s[sp->i] && sp->s[sp->i] == ' ')
			sp->i++;
		if (sp->s[sp->i] == '|')
			put_word(sp, PIPE);
		if (sp->s[sp->i] == '\"')
			split_quotes(sp, '\"');
		else if (sp->s[sp->i] == '\'')
			split_quotes(sp, '\'');
		else if (!sp->s[sp->i])
			break ;
		else
			put_word(sp, 0);
		sp->arr[sp->j][sp->k] = '\0';
		sp->j++;
	}
	sp->arr[sp->j] = 0;
}

char	**ft_split_adapted(char *s)
{
	int		n;
	t_split sp;

	n = countwords(s, 0, 0);
	sp.i = 0;
	sp.j = 0;
	sp.k = 0;
	sp.s = s;
	sp.arr = (char **)malloc((n + 1) * sizeof(char *));
	if (!sp.arr)
		return (NULL);
	split_words(&sp);
	if (!*(sp.arr))
		return (free(sp.arr), NULL);
	return (sp.arr);
}
