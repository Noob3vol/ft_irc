# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    debug.mk                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: thzeribi <thzeribi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/15 07:53:26 by thzeribi          #+#    #+#              #
#    Updated: 2023/01/15 08:02:05 by thzeribi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

debug: header
	$(MAKE) $(NAME) DBG=2

.PHONY: debug