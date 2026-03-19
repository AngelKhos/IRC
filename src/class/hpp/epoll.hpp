/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   epoll.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: authomas <authomas@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 17:06:21 by authomas          #+#    #+#             */
/*   Updated: 2026/03/19 17:10:39 by authomas         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

class Epoll
{
    public:

        Epoll();
        void wait();
        void set_write();
        void set_read();

        // to do

    private:
        int fd;
};