/*
 * MyShell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 * 
 */
#ifndef MYSHELL_H_
#define MYSHELL_H_

// Include your relevant functions declarations here they must start with the 
// extern keyword such as in the following example:
// extern void display_help(void);
#define BUFFER_LEN 256
extern char *userName;
extern char currentpath[BUFFER_LEN];
extern char oldPath[BUFFER_LEN];
extern char buffer[BUFFER_LEN];
#endif /* MYSHELL_H_ */
