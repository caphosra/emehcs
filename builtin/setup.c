#include "builtin.h"
#include "evaluation.h"

void setup_builtin(Environment* env) {
    BUILTIN("+", add);
    BUILTIN("if", branch);
    BUILTIN("define", define);
    BUILTIN("display", display);
    BUILTIN("=", eq);
    BUILTIN("equal?", is_equal);
    BUILTIN(">", gt);
    BUILTIN(">=", gte);
    BUILTIN("list?", is_list);
    BUILTIN("lambda", lambda);
    BUILTIN("let", let);
    BUILTIN("let*", let_ex);
    BUILTIN("list", list);
    BUILTIN("<", lt);
    BUILTIN("<=", lte);
    BUILTIN("-", minus);
    BUILTIN("*", mul);
    BUILTIN("quote", quote);
    BUILTIN("cons", cons);
    BUILTIN("car", car);
    BUILTIN("cdr", cdr);
}
