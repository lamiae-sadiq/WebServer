/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webs.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kel-baam <kel-baam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 14:35:49 by lsadiq            #+#    #+#             */
/*   Updated: 2024/02/21 09:58:59 by kel-baam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <stdexcept>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>

// #define PORT 8080


// class Server
// {
//     public:
//         Server(std::string config_file);
//         ~Server();
//         void run();
//     private:
//         std::string config_file;
// };