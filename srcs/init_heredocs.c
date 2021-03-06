#include "../incs/minishell.h"
#include <unistd.h>

extern t_sig_info	g_sig_info;

void	close_heredocs(t_doclist *hdocs)
{
	t_doclist	*tmp;

	tmp = hdocs;
	while (tmp)
	{	
		if (is_open_fd(tmp->fds[0]))
		{
			if (close(tmp->fds[0]) == SYS_ERROR)
				g_sig_info.sys_error = true;
		}
		if (is_open_fd(tmp->fds[1]))
		{
			if (close(tmp->fds[1]) == SYS_ERROR)
				g_sig_info.sys_error = true;
		}
		tmp = tmp->next;
	}
}

static bool	create_heredoc(char *delim, t_set *set, bool *no_prnt)
{
	int	fds[2];

	if (pipe(fds) == SYS_ERROR)
	{
		g_sig_info.sys_error = true;
		return (FAILURE);
	}
	if (fds[0] == _POSIX_OPEN_MAX || fds[1] == _POSIX_OPEN_MAX)
	{
		close(fds[0]);
		close(fds[1]);
		ft_putendl_fd("minishell: maximum here-document count exceeded",
			STDERR_FILENO);
		*no_prnt = true;
		return (FAILURE);
	}
	if (!ft_doclstadd_back(&set->heredoc_lst, ft_doclstnew(fds, delim)))
		return (FAILURE);
	return (SUCCESS);
}

static bool	open_heredocs(t_tree *l, t_set *set, int *rlt)
{
	int		i;
	bool	no_prnt;

	i = 0;
	no_prnt = false;
	if (*rlt == FAILURE)
		return (FAILURE);
	if (!l)
		return (SUCCESS);
	open_heredocs(l->left, set, rlt);
	if (l->node.av && has_heredoc(l->node.av))
	{
		while (l->node.av[i])
		{
			if (str_equal(l->node.av[i], "<<", 3))
			{
				i++;
				if (create_heredoc(l->node.av[i], set, &no_prnt) == FAILURE)
					return (minishell_error(NULL, rlt, no_prnt));
			}
			i++;
		}
	}
	open_heredocs(l->right, set, rlt);
	return (*rlt);
}

static bool	write_heredocs(t_doclist *heredoc_lst)
{
	char	*delim;	

	while (heredoc_lst)
	{
		delim = heredoc_lst->delim;
		if (!handle_heredoc(heredoc_lst->fds, delim))
			return (false);
		heredoc_lst = heredoc_lst->next;
	}
	return (true);
}

bool	init_heredocs(t_tree *parent, t_set *set, int *rlt)
{
	if (open_heredocs(parent, set, rlt) == FAILURE)
		return (FAILURE);
	if (!write_heredocs(set->heredoc_lst))
	{
		close_heredocs(set->heredoc_lst);
		return (minishell_error(NULL, rlt, false));
	}
	set->tmp_hdocs = set->heredoc_lst;
	return (SUCCESS);
}
