#pragma once
#include "Lexical.h"

namespace Error {
	enum errortype {
		lexical_error, ident_redefine, ident_undefine, paranum_error,
		paratype_error, condition_error, void_return_error, unvoid_unreturn_error,
		array_error, const_assign_error, no_semi_error, no_rparent_error,
		no_rbrack_error, no_while_error, const_type_error
	};

	void error_print(errortype type);
}