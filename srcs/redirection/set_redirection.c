#include "../../incs/minishell.h"

extern t_sig_info	g_sig_info;

static void	set_status(char *cmd_i, t_redir *redir)
{
	if (str_equal(cmd_i, ">>", 3))
		redir->status = RRDIR;
	else if (str_equal(cmd_i, "<<", 3))
		redir->status = LLDIR;
	else if (str_equal(cmd_i, ">", 2))
		redir->status = RDIR;
	else
		redir->status = LDIR;
}

static bool	reset_fds(t_redir *redir)
{
	int	rlt;

	rlt = 0;
	if (redir->status == RDIR || redir->status == RRDIR)
	{
		if (!close_fd(redir->new_out, SUCCESS))
			rlt = SYS_ERROR;
		redir->new_out = -1;
	}
	else if ((redir->status == LDIR || redir->status == LLDIR))
	{
		if (!close_fd(redir->new_in, SUCCESS))
			rlt = SYS_ERROR;
		redir->new_in = -1;
	}
	if (rlt == SYS_ERROR)
		return (false);
	return (true);
}

static bool	check_new_fd(char *filename, t_redir *redir)
{
	if ((redir->status == RDIR || redir->status == RRDIR)
		&& redir->new_out == SYS_ERROR)
		redir->perror = true;
	if ((redir->status == LDIR)
		&& redir->new_in == SYS_ERROR)
		redir->perror = true;
	if (!redir->perror)
		return (true);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(filename, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	g_sig_info.exit_status = EXIT_FAILURE;
	perror(NULL);
	return (false);
}

bool	set_redirection(char **cmd, int i, t_redir *redir, t_set *set)
{
	set_status(cmd[i], redir);
	if (!reset_fds(redir))
		return (false);
	if (redir->status == RDIR)
		redir->new_out = open(cmd[i + 1], redir->r_flags, redir->permissions);
	else if (redir->status == RRDIR)
		redir->new_out = open(cmd[i + 1], redir->rr_flags, redir->permissions);
	else if (redir->status == LDIR)
		redir->new_in = open(cmd[i + 1], redir->l_flags);
	else
		redir->new_in = set->heredoc_lst->fds[0];
	return (check_new_fd(cmd[i + 1], redir));
}
