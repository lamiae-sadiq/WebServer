/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exception.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kel-baam <kel-baam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 13:51:55 by kel-baam          #+#    #+#             */
/*   Updated: 2024/03/16 13:53:14 by kel-baam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>

class   Exception:public  std::exception
{
    private:
        const char* msg;
    public:
        Exception(const char* init):msg(init){};
        const char* what() const throw()
        {
                return msg;
        }
};
