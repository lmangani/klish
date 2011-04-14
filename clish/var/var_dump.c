/*
 * var_dump.c
 */
#include "private.h"
#include "lub/dump.h"
/*--------------------------------------------------------- */
void clish_var_dump(const clish_var_t *this)
{
	lub_dump_printf("var(%p)\n", this);
	lub_dump_indent();

	lub_dump_printf("name     : %s\n", this->name);
	lub_dump_printf("dynamic  : %s\n",
		this->dynamic ? "true" : "false");
	lub_dump_printf("value    : %s\n", this->value);

	lub_dump_undent();
}

/*--------------------------------------------------------- */
