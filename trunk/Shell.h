/*

Copyright (C) 2007  iPhoneDev Team ( http://iphone.fiveforty.net/wiki/ )

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>

*/

#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <sys/types.h>

extern "C"
{
#include <readline/history.h>
#include <readline/readline.h>
}

#include "MobileDevice.h"
#include "NormalInterface.h"
#include "RecoveryInterface.h"
#include "RestoreInterface.h"
#include "privateFunctions.h"

/* Unix version of a hidden file.  Could be different on other systems. */
#define HIDDEN_FILE(fname)	((fname)[0] == '.')


// TODO:  Add windows version of this?
#define PATH_DELIMITER_CHAR '/'
#define PATH_DELIMITER_STRING "/"

#ifndef savestring
#define savestring(x) strcpy ((char *)xmalloc (1 + strlen (x)), (x))
#endif

#define SHELL_UNKNOWN	-1
#define SHELL_NORMAL	0
#define	SHELL_RESTORE	1
#define SHELL_RECOVERY	2

#define MAX_ARGUMENTS	5

// userspace function exit values
#define SHELL_CONTINUE	-1	// continue reading from interactive shell
#define	SHELL_TERMINATE	0	// terminate application 
#define SHELL_WAIT	1	// wait for device reconnect

typedef int shell_funct (string*, shell_state*);

typedef struct {
	char *name;		// User printable name
	shell_funct *func;	// Function pointer
	char *doc;		// Documentation string
} COMMAND;

// readline
typedef struct shell_state {
	struct afc_connection *afch;
	struct afc_connection *conn;
	struct afc_connection *alt_service;
	struct am_restore_device *restore_dev;
	struct am_recovery_device *recovery_dev;
	struct am_device *dev;
	
	int shell_mode;
	
	string prompt_string;
	string remote_path;
	string local_path;
	
	COMMAND *command_array;
};

struct afc_dirent {
	uint8_t d_namlen;
	char *d_name;
};

#define D_NAMLEN(d)	((d).d_namlen)

typedef int shell_funct (string*, shell_state*);
typedef char *rl_compentry_func_t (const char *, int);

extern "C" int _rl_match_hidden_files;
extern "C" int rl_complete_with_tilde_expansion;
extern "C" int _rl_completion_case_fold;	
extern "C" char *xmalloc (unsigned long size);
extern "C" char **rl_completion_matches(const char *, rl_compentry_func_t *);
extern "C" int rl_catch_signals;
extern "C" int rl_catch_sigwinch;
extern "C" void rl_free_line_state(void);
extern "C" void rl_cleanup_after_signal(void);
extern "C" int rl_set_signals(void);

int _rl_stricmp (char *string1, char *string2);
int _rl_strnicmp (char *string1, char *string2, int count);

char *dupstr( char *s );
char **cmd_completer(const char *text, int start, int end);
char *cmd_generator(const char *text, int state);
int exec_line( char *line, struct shell_state *sh );
void initialize_readline();

// shell
bool dirExists(afc_connection *hAFC, char *path);
void processRelativePath(string *basePath, string *cdPath);
int shell(struct shell_state *sh);
int sh_help(string *args, struct shell_state *sh);
char *rl_remote_complete(const char *text, int state);

#endif //SHELL_H

/* -*- mode:c; indent-tabs-mode:nil; c-basic-offset:2; tab-width:2; */
