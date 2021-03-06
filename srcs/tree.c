#include "../incs/minishell.h"

t_tree	*new_tree(int flgs, t_tree *left, t_tree *right, bool *malloc_err)
{
	t_tree	*tree;
	t_node	node;

	if (*malloc_err)
	{
		free_tree(left);
		free_tree(right);
		return (NULL);
	}
	tree = ft_calloc(1, sizeof(t_tree));
	if (!tree)
	{
		*malloc_err = true;
		free_tree(left);
		free_tree(right);
		return (NULL);
	}
	tree->left = left;
	tree->right = right;
	node.ac = 0;
	node.av = NULL;
	node.flgs = flgs;
	node.str_flgs = NULL;
	tree->node = node;
	return (tree);
}

t_tree	*new_tree_cmd(t_node node, bool *malloc_err)
{
	t_tree	*tree;

	tree = ft_calloc(1, sizeof(t_tree));
	if (!tree)
	{
		*malloc_err = true;
		free(node.av);
		free(node.str_flgs);
		return (NULL);
	}
	tree->node = node;
	return (tree);
}

void	free_tree(t_tree *l)
{
	if (l != NULL)
	{
		free_tree(l->left);
		free_tree(l->right);
		if (l->node.flgs == STR && l->node.av)
			free(l->node.av);
		if (l->node.str_flgs)
			free(l->node.str_flgs);
		free(l);
	}
}
