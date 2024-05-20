/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_checker.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jholland <jholland@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 22:49:25 by jholland          #+#    #+#             */
/*   Updated: 2024/05/20 20:25:45 by jholland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_FREE_LOG_H
# define MALLOC_FREE_LOG_H


/**
 * @brief A list that stores the status of the allocation status of all
 * the pointers allocated or freed with the mfl_malloc and mfl_free functions,
 * including its size, memory direction, status, and custom hint comment
 * set when allocated.
 * @param ptr The direction of the allocated or freed pointer
 * @param size The size of the allocated memory given to mfl_malloc
 * @param freed A status flag. 0 = Allocated by mfl_malloc but not freed,
 * 1 = Allocated by mfl_malloc and freed by mfl_free, 2 = Tried to double free,
 * -1 = Non-allocated by mfl_malloc
 * @param comment A custom hint message to improve the tracking
 */
typedef struct s_mfl_list
{
	void				*ptr;
	unsigned int		size;
	struct s_mfl_list	*next;
	int					freed;
	const char			*comment;
}	t_mfl_list;

/**
 * @brief This global pointer declaration has to exist and be initialized
 * at the beginning of the main file. And should be freed with mfl_clear
 * after printing it with mfl_print at the end of the main function.
 */
extern t_mfl_list	*g_mfl_list;

/**
 * @brief Allocates memory with malloc(size) and saves the reference in
 * a new last node of a list where it can be tracked using mfl_print
 * 
 * @param list Reference of the list in which the new pointer
 * will be attached
 * @param size Byte-size that will be allocatedd with malloc
 * @param comment Hint comment to display when tracking
 * @return void* 
 */
void	*mfl_malloc(t_mfl_list **list, unsigned int size, const char *comment);

/**
 * @brief Tries to free a memory allocation pointed by ptr made with
 * the mfl_malloc function. If it is registered as allocated, it normally
 * frees ptr. If is registered as already freed in its last ocurrence,
 * avoids freeing it and will mark it as potential double free,
 * without crashing.
 * If it was not previously registered, it regiters it as a try to free a
 * non-allocated memory, not freeing it and without crashing.
 * @param list Reference of the list in which ptr should be registered.
 * @param ptr The pointer which is tried to free.
 */
void	mfl_free(t_mfl_list **list, void *ptr);

/**
 * @brief Tracks the status of all memory allocation and freeings set
 * by the mfl_malloc and mfl_free functions, including references, sizes
 * and hint comments.
 * You should avoid mixing mfl_malloc and mfl_free with the original malloc
 * and free in order to avoid missing allocation tracking or false
 * free statuses.
 * Red statuses mean that at that point your project may have crashed
 * with the original free function. Yellow statuses mean that you should
 * look for memory leaks. Green statuses may not have to be reviewed.
 * @param list A pointer to the allocations list.
 */
void	mfl_print(t_mfl_list *list);

/**
 * @brief Clears the full list from the memory
 * 
 * @param list The reference to the list to be cleared
 */
void	mfl_clear(t_mfl_list **list);
#endif