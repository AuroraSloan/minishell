#include "../../incs/minishell.h"

extern t_sig_info	g_sig_info;

static bool	free_cmd_path(char *cmd_path)
{
	free(cmd_path);
	cmd_path = NULL;
	return (FAILURE);
}

static int	command_not_found(char *cmd)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putendl_fd(": command not found" , STDERR_FILENO);
	g_sig_info.exit_status = 127;
	return (SUCCESS);
}

static bool run_gnu_parent(int c_pid, t_set *set, char *cmd_path)
{
	g_sig_info.child = true;
	if (!wait_options(c_pid, false))
	{
		if (g_sig_info.signal == SIGINT)
			mod_termios_attr(set, true);
		return (free_cmd_path(cmd_path));
	}
	mod_termios_attr(set, true);
	free(cmd_path);
	if (g_sig_info.sys_error)
		return (FAILURE);
	return (SUCCESS);
}

static bool	run_gnu_cmd(char **cmd, t_set *set)
{
	pid_t		c_pid;
	char		*cmd_path;
	extern char	**environ;

	if (create_cmd_path(cmd, &cmd_path) == FAILURE)
		return (FAILURE);
	if (!cmd_path)
		return (command_not_found(cmd[0]));
	c_pid = fork();
	if (c_pid == SYS_ERROR)
		return (free_cmd_path(cmd_path));
	else if (c_pid == 0)
	{
		if (!mod_termios_attr(set, false))
			exit(exec_cmd_error(cmd[0], cmd_path, true));
		if (execve(cmd_path, cmd, environ) == SYS_ERROR)
			exit(exec_cmd_error(cmd[0], cmd_path, false));
	}
	else
		return (run_gnu_parent(c_pid, set, cmd_path));
	return (SUCCESS);
}

char	**get_cmd(t_node *node, t_set *set, t_redir *redir, bool *touch)
{
	if (has_redirection(node))
	{
		if (!ms_redirection(node, redir, set->heredoc_lst))
			return (NULL);
		return (create_new_cmd(node, touch));
	}
	else
		return (node->av);
}

bool	execute_simple_cmd(t_node node, t_set *set, t_redir *redir)
{
	int		rlt;
	bool	touch;
	char	**cmd;
	t_node	*exp_node;

	touch = false;
	rlt = SUCCESS;
	exp_node = expansion_node(&node);
	if (!exp_node)
		return (expansion_node_conclude(NULL, FAILURE));
	if (!exp_node->av)
		return (expansion_node_conclude(exp_node, SUCCESS));
	cmd = get_cmd(exp_node, set, redir, &touch);
	if (!cmd && !touch)
	{
		expansion_node_conclude(exp_node, FAILURE);
		return (end_redirection(NULL, redir, FAILURE));
	}
	else if (!touch)
	{
		if (is_buildin(cmd[0]))
			rlt = run_builtin_cmd(cmd, set, true);
		else
			rlt = run_gnu_cmd(cmd, set);
	}
	if (has_redirection(exp_node))
		rlt = end_redirection(cmd, redir, rlt);
	return (expansion_node_conclude(exp_node, rlt));
}
