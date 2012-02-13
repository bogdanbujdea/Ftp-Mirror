/*
    <This software is a FTP client used to download a copy of a FTP server.>
    Copyright (C) 2012  <Bujdea Bogdan> <costinel.bujdea@info.uaic.ro>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <string>
#include <string.h>

enum ExceptionType
    {
        UNKNOWN_EXCEPTION,
        SEND_EXCEPTION,
        RECV_EXCEPTION,
	FILE_CREATE_EXCEPTION
    };

class Exception
{

public:
    
    ExceptionType Type;
    int ErrorCode;
    char *Message;
    Exception();
    Exception(char *message);
    Exception(char *message, ExceptionType exType);
    Exception(char *message, ExceptionType exType, int errorCode);
    virtual ~Exception();
   
};
#endif // EXCEPTION_H
