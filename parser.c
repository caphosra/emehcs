enum ExprType {
    E_DEFINE,
    E_LAMBDA,
    E_APP,
    E_VAR,
    E_NUM
};

struct Expr {
    enum ExprType type;
    union {
        char* name;
        char* var;
        struct {
            struct Expr* left;
            struct Expr* right;
        };
        int num;
    };
};
