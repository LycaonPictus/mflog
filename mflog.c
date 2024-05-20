/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mflog.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jholland <jholland@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:24:29 by jholland          #+#    #+#             */
/*   Updated: 2024/05/20 20:26:50 by jholland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mflog.h"
#include <stdlib.h>
#include <stdio.h>

static t_mfl_list	*add_node(t_mfl_list **list, void *ptr, const char *comment)
{
	t_mfl_list	*new;
	t_mfl_list	*node;

	new = malloc(sizeof(t_mfl_list));
	new->freed = -1;
	new->size = 0;
	new->ptr = ptr;
	new->comment = comment;
	new->next = NULL;
	if (!*list)
		(*list) = new;
	else
	{
		node = *list;
		while (node->next)
			node = node->next;
		node->next = new;
	}
	return (new);
}

void	*mfl_malloc(t_mfl_list **list, unsigned int size, const char *comment)
{
	void		*ptr;
	t_mfl_list	*new;

	ptr = malloc(size);
	new = add_node(list, ptr, comment);
	new->size = size;
	new->ptr = malloc(size);
	new->comment = comment;
	new->freed = 0;
	return (new->ptr);
}

void	mfl_print(t_mfl_list *list)
{
	while (list)
	{
		if (list->freed == -1 && list->ptr)
			printf("\033[0;31m[NO_ALLOC]\033[0m\t");
		else if (list->freed == -1 && !list->ptr)
			printf("\033[0;32m[NLLFREED]\033[0m\t");
		else if (list->freed == 0)
			printf("\033[0;33m[MEM_LEAK]\033[0m\t");
		else if (list->freed == 1)
			printf("\033[0;32m[FREED_OK]\033[0m\t");
		else if (list->freed == 2)
			printf("\033[0;31m[DBL_FREE]\033[0m\t");
		printf("%-14p\t%3u bytes\t%s\n", list->ptr, list->size, list->comment);
		list = list->next;
	}
}

void	mfl_free(t_mfl_list **list, void *ptr)
{
	t_mfl_list	*last_freed;
	t_mfl_list	*node;

	last_freed = NULL;
	node = *list;
	while (node)
	{
		if (node->ptr == ptr)
		{
			if (node->freed == 0)
			{
				free(node->ptr);
				node->freed = 1;
				return ;
			}
			else if (node->freed == 1)
				last_freed = node;
		}
		node = node->next;
	}
	if (last_freed)
		last_freed->freed = 2;
	else if (ptr)
		add_node(list, ptr, ptr ? "???" : "SAFE FREE");
	else
		add_node(list, ptr, "SAFE FREE");
}

void	mfl_clear(t_mfl_list **list)
{
	t_mfl_list	*node;
	t_mfl_list	*next;

	node = *list;
	if (!node)
		return ;
	while (node)
	{
		next = node->next;
		free(node);
		node = next;
	}
	*list = NULL;
}
