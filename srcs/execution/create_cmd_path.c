#include "../../incs/minishell.h"

void	free_str_arr(char **str_arr)
{
	int	i;

	i = 0;
	while (str_arr[i])
	{
		free(str_arr[i]);
		str_arr[i++] = NULL;
	}
	if (str_arr)
		free(str_arr);
	str_arr = NULL;
}

static char	*create_path(char *cmd, char **paths)
{
	char		*abs_path;
	int			i;
	struct stat	ss;

	abs_path = NULL;
	i = 0;
	while (paths[i])
	{
		abs_path = ft_strcjoin(paths[i], cmd, '/');
		if ((stat(abs_path, &ss) == 0 && (ss.st_mode & S_IXUSR)) || !abs_path
			|| !paths[i + 1])
			break ;
		free(abs_path);
		abs_path = NULL;
		i++;
	}
	free_str_arr(paths);
	return (abs_path);
}

static char	**find_paths(void)
{
	char	*path;
	char	**paths;

	path = getenv("PATH");
	paths = NULL;
	paths = ft_split(path, ':');
	return (paths);
}

char	*create_cmd_path(t_node node)
{
	char	**paths;
	char	*cmd_path;

	if (node.av[0][0] != '/')
	{
		paths = find_paths();
		if (!paths)
			return (NULL);
		cmd_path = create_path(node.av[0], paths);
	}
	else
		cmd_path = ft_strdup(node.av[0]);
	return (cmd_path);
}