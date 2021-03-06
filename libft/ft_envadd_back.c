#include "libft.h"

bool	ft_envadd_back(t_env **env, t_env *new)
{
	t_env	*tmp;

	if (env == NULL || new == NULL)
		return (false);
	if (*env == NULL)
	{
		(*env) = new;
		return (true);
	}
	tmp = *env;
	while ((*env)->next)
		(*env) = (*env)->next;
	(*env)->next = new;
	*env = tmp;
	return (true);
}
