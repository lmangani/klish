/*
 * shell.h - private interface to the shell class
 */
#include "clish/shell.h"
#include "clish/pargv.h"
#include "clish/variable.h"
#include "lub/bintree.h"
#include "tinyrl/tinyrl.h"

/*-------------------------------------
 * PRIVATE TYPES
 *------------------------------------- */

/*-------------------------------------------------------- */

/*
 * iterate around commands
 */
typedef struct {
	const char *last_cmd;
	clish_nspace_visibility_t field;
} clish_shell_iterator_t;

/* this is used to maintain a stack of file handles */
typedef struct clish_shell_file_s clish_shell_file_t;
struct clish_shell_file_s {
	clish_shell_file_t *next;
	FILE *file;
	bool_t stop_on_error;	/* stop on error for file input  */
};

typedef struct {
	char *line;
	clish_view_t *view;
	char *viewid;
} clish_shell_pwd_t;

/* 
 * The context structure
 */
struct clish_context_s {
	clish_pargv_t *completion_pargv;
	unsigned completion_index;
	unsigned completion_pindex;
	clish_shell_iterator_t iter; /* used for iterating commands */
};

struct clish_shell_s {
	lub_bintree_t view_tree;	/* Maintain a tree of views      */
	lub_bintree_t ptype_tree;	/* Maintain a tree of ptypes     */
	const clish_shell_hooks_t *client_hooks;	/* Client callback hooks         */
	void *client_cookie;	/* Client callback cookie        */
	clish_view_t *global;	/* Reference to the global view. */
	clish_view_t *view;	/* Reference to the current view. */
	clish_command_t *startup;	/* This is the startup command   */
	clish_shell_state_t state;	/* The current state               */
	char *overview;		/* Overview text for this shell.  */
	char *viewid;		/* The current view ID string     */
	tinyrl_t *tinyrl;	/* Tiny readline instance          */
	clish_shell_file_t *current_file;	/* file currently in use for input */
	clish_shell_pwd_t **cfg_pwdv;	/* Levels for the config file structure */
	unsigned cfg_pwdc;
	konf_client_t *client;
	char * lockfile;
	pthread_t pthread;
	clish_context_t context;
	char * default_shebang;
	char * fifo_name; /* The name of temporary fifo file. */
	bool_t interactive; /* Is shell interactive. */

	/* Static params for var expanding. The refactoring is needed. */
	clish_param_t *param_depth;
	clish_param_t *param_pwd;
	clish_param_t *param_interactive;
};

/**
 * Initialise a command iterator structure
 */
void
clish_shell_iterator_init(clish_shell_iterator_t * iter,
	clish_nspace_visibility_t field);

/**
 * get the next command which is an extension of the specified line 
 */
const clish_command_t *clish_shell_find_next_completion(const clish_shell_t *
	instance, const char *line, clish_shell_iterator_t * iter);
/** 
 * Pop the current file handle from the stack of file handles, shutting
 * the file down and freeing any associated memory. The next file handle
 * in the stack becomes associated with the input stream for this shell.
 * 
 * \return
 * BOOL_TRUE - the current file handle has been replaced.
 * BOOL_FALSE - there is only one handle on the stack which cannot be replaced.
 */
bool_t clish_shell_pop_file(clish_shell_t * instance);

clish_view_t *clish_shell_find_view(clish_shell_t * instance, const char *name);
void clish_shell_insert_view(clish_shell_t * instance, clish_view_t * view);
clish_pargv_status_t
clish_shell_parse(const clish_shell_t * instance,
		  const char *line,
		  const clish_command_t ** cmd, clish_pargv_t ** pargv);
char *clish_shell_word_generator(clish_shell_t * instance,
				 const char *line,
				 unsigned offset, unsigned state);
const clish_command_t *clish_shell_resolve_command(const clish_shell_t *
						   instance, const char *line);
const clish_command_t *clish_shell_resolve_prefix(const clish_shell_t *
						  instance, const char *line);
const clish_command_t *clish_shell_getfirst_command(clish_shell_t * instance,
						    const char *line,
						    clish_nspace_visibility_t
						    field);
const clish_command_t *clish_shell_getnext_command(clish_shell_t * instance,
						   const char *line);
void clish_shell_insert_ptype(clish_shell_t * instance, clish_ptype_t * ptype);
void clish_shell_tinyrl_history(clish_shell_t * instance, unsigned int *limit);
tinyrl_t *clish_shell_tinyrl_new(FILE * instream,
				 FILE * outstream, unsigned stifle);
void clish_shell_tinyrl_delete(tinyrl_t * instance);
