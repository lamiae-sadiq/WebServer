/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exception.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kel-baam <kel-baam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 23:39:06 by kel-baam          #+#    #+#             */
/*   Updated: 2024/03/23 23:41:19 by kel-baam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Exception.hpp"

Exception::Exception(const char* init):msg(init){};


const char* Exception::what() const throw()
{
    return msg;
}