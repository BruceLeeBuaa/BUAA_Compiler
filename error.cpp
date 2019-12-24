#include "Lexical.h";
#include "Globalvar.h"
#include "Syntax.h"
#include "error.h"
//#include "csignal"

void Error::error_print(Error::errortype type)
{
	//raise(last_num);
	switch (type)
	{
	case lexical_error:
		printf("%d a\n", last_num);
		break;
	case ident_redefine:
		printf("%d b\n", last_num);
		break;
	case ident_undefine:
		printf("%d c\n", last_num);
		break;
	case paranum_error:
		printf("%d d\n", last_num);
		break;
	case paratype_error:
		printf("%d e\n", last_num);
		break;
	case condition_error:
		printf("%d f\n", last_num);
		break;
	case void_return_error:
		printf("%d g\n", last_num);
		break;
	case unvoid_unreturn_error:
		printf("%d h\n", last_num);
		break;
	case array_error:
		printf("%d i\n", last_num);
		break;
	case const_assign_error:
		printf("%d j\n", last_num);
		break;
	case no_semi_error:
		printf("%d k\n", last_num);
		break;
	case no_rparent_error:
		printf("%d l\n", last_num);
		break;
	case no_rbrack_error:
		printf("%d m\n", last_num);
		break;
	case no_while_error:
		printf("%d n\n", last_num);
		break;
	case const_type_error:
		printf("%d o\n", last_num);
		break;
	default:
		printf("%d other_error\n", line_num);
		break;
	}
}