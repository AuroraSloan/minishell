#include "../../incs/minishell.h"

void	ft_export_error(char *arg)
{
	ft_putstr_fd("minishell: export: ", STDERR_FILENO);
	ft_putchar_fd('\'', STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
}

int	identifier_type(char *s)
{
	size_t	i;

	i = 0;
	if (!ft_isalpha(s[i]))
		return (ERROR);
	while (s[i])
	{
		if ((s[i] == '+' && s[i + 1] == '=') || s[i] == '=')
			break ;
		if (!ft_isalnum(s[i++]))
			return (ERROR);
	}
	if (i == ft_strlen(s))
		return (NOTHING);
	if (s[i] == '+' && s[i - 1] != ' ' && s[i + 2] != ' ')
		return (ADD);
	if (s[i] == '=' && s[i - 1] != ' ' && s[i + 1] != ' ')
		return (UPDATE);
	return (ERROR);
}

void	delete_env(t_env **env, char *name)
{
	t_env	*p;
	t_env	*tmp;

	if (ft_strncmp(name, (*env)->name, UINT_MAX) == 0)
	{
		tmp = *env;
		*env = (*env)->next;
		ft_envdelone(tmp, free);
		return ;
	}
	p = *env;
	while (p->next)
	{
		if (ft_strncmp(name, p->next->name, UINT_MAX) == 0)
		{
			tmp = p->next;
			p->next = p->next->next;
			ft_envdelone(tmp, free);
			return ;
		}
		p = p->next;
	}
}

void	print_name_value(t_env *env)
{
	while (env)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(env->name, STDOUT_FILENO);
		ft_putstr_fd("=\"", STDOUT_FILENO);
		ft_putstr_fd(env->value, STDOUT_FILENO);
		ft_putendl_fd("\"", STDOUT_FILENO);
		env = env->next;
	}
}

void	ft_swap_env(t_env *a, t_env *b)
{
	char	*tmp;

	tmp = a->name;
	a->name = b->name;
	b->name = tmp;
	tmp = a->value;
	a->value = b->value;
	b->value = tmp;
}
