#include "../../incs/minishell.h"

extern t_sig_info	g_sig_info;

void	exit_error(char *arg, char *msg)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	if (arg)
	{
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putendl_fd(msg, STDERR_FILENO);
}

bool	ft_exit(char **av, t_set *set, bool print_exit)
{
	int	status;
	int	flg;
	int	i;

	if (print_exit)
		ft_putendl_fd("exit", STDERR_FILENO);
	if (str_equal(av[1], "--", 3))
		i = 1;
	else
		i = 0;
	if (!av[i + 1])
		ms_exit(set, g_sig_info.exit_status, print_exit);
	status = ft_atol(av[i + 1], &flg) % 256;
	if (flg)
	{
		exit_error(av[i + 1], "numeric argument required");
		ms_exit(set, 255, print_exit);
	}
	if (av[i + 2])
	{
		set->exit_done = true;
		exit_error(NULL, "too many arguments");
		g_sig_info.exit_status = EXIT_FAILURE;
		return (SUCCESS);
	}
	ms_exit(set, status, print_exit);
	return (SUCCESS);
}
