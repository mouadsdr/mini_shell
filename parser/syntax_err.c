/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_err.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msodor <msodor@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 13:26:53 by msodor            #+#    #+#             */
/*   Updated: 2023/05/29 13:26:47 by msodor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	rm_extra(t_elems **elems)
{
	t_elems	*head;

	head = *elems;
	while (head)
	{
		if (head->type == DQUOTE || head->type == QUOTE)
			token_del(elems, head);
		head = head->next;
	}
}

void	join_in_quote(t_elems **elems)
{
	t_elems	*head;

	head = *elems;
	while (head && head->next)
	{
		if (head->state == IN_DQUOTE && head->next->state == IN_DQUOTE \
		&& head->next->type != VAR && head->type != VAR)
		{
			head->content = ft_strjoin(head->content, head->next->content);
			head->len += head->next->len;
			head->type = WORD;
			token_del(elems, head->next);
			join_in_quote(&head);
		}
		else if (head->state == IN_QUOTE && head->next->state == IN_QUOTE)
		{
			head->content = ft_strjoin(head->content, head->next->content);
			head->len += head->next->len;
			head->type = WORD;
			token_del(elems, head->next);
			join_in_quote(&head);
		}
		head = head->next;
	}
}

void	join_cmd(t_elems **elems)
{
	t_elems	*head;

	rm_extra(elems);
	head = *elems;
	while (head && head->next != NULL)
	{
		if (head->type == WORD && head->next->type == WORD)
		{
			head->content = ft_strjoin(head->content, head->next->content);
			token_del(elems, head->next);
		}
		head = head->next;
	}
}

void	quotes_syntax(t_elems *elems)
{
	int	dcount;
	int	count;

	count = 0;
	dcount = 0;
	while (elems)
	{
		if (elems->type == DQUOTE)
			dcount++;
		else if (elems->type == QUOTE)
			count++;
		elems = elems->next;
	}
	if ((count % 2) != 0 || (dcount % 2) != 0)
		printf("syntax error: unclosed quotes.\n");
}

// void	redir_syntax(t_elems *elems)
// {
// 	while (elems && elems->next)
// 	{
// 		if (elems->type == REDIR_IN || elems->type == REDIR_OUT \
// 		|| elems->type == HERE_DOC || elems->type == AREDIR_OUT)
// 		{
// 			if (elems->next->type != WORD && elems->next->type != _SPACE)
// 				printf("syntax error: near unexpected token\n");
// 		}
// 		elems = elems->next;
// 	}
// }
