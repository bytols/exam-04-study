#include <stdio.h>
#include <malloc.h>
#include <ctype.h>

typedef struct node {
    enum {
        ADD,
        MULTI,
        VAL
    }   type;
    int val;
    struct node *l;
    struct node *r;
}   node;

node    *new_node(node n)
{
    node *ret = calloc(1, sizeof(n));
    if (!ret)
        return (NULL);
    *ret = n;
    return (ret);
}

void    destroy_tree(node *n)
{
    if (!n)
        return ;
    if (n->type != VAL)
    {
        destroy_tree(n->l);
        destroy_tree(n->r);
    }
    free(n);
}

void    unexpected(char c)
{
    if (c)
        printf("Unexpected token '%c'\n", c);
    else
        printf("Unexpected end of file\n");
}

int accept(char **s, char c)
{
    if (**s == c)
    {
        (*s)++;
        return (1);
    }
    return (0);
}
node *valor(char **s);
node *base_mult(char **s);
node * base_add(char **s);
int expect(char **s, char c);

node *valor(char **s)
{
    if (isdigit((unsigned char) **s))
    {
        node n = {.type = VAL, .val = **s - '0', .l = NULL, .r = NULL};
        (*s)++;
        return new_node(n);
    }
    if (accept(s, '('))
    {
        printf("cai aqui? %c\n", **s);
        node *e = base_add(s);
        if (!e)
            return (NULL);
        if (!expect(s, ')'))
        {
            destroy_tree(e);
            return (NULL);
        }
        return (e);
    }
    printf("cai aqui 2\n");
    unexpected(**s);
    return NULL;
}

node *base_mult(char **s)
{
    node *left;

    left = valor(s);
    while (accept(s, '*'))
    {
        node *right = valor(s);
        if(!right)
        {
            destroy_tree(left);
            return NULL;
        }
        node n = {.type = MULTI, .l = left, .r = right};
        left = new_node(n);
        if (!left)
            return (NULL);
    }
    return (left);
}

node * base_add(char **s)
{
    node *left;

    left = base_mult(s);
    while (accept(s, '+'))
    {
        node *right = base_mult(s);
        if(!right)
        {
            destroy_tree(left);
            return NULL;
        }
        node n = {.type = ADD, .l = left, .r = right};
        left = new_node(n);
        if (!left)
            return (NULL);
    }
    return (left);
}

int expect(char **s, char c)
{
    printf("%c e %c\n" , **s, c);
    if (accept(s, c))
        return (1);
    unexpected(**s);
    return (0);
}

node    *parse_expr(char *s)
{
    node *ret;

    ret = base_add(&s); 
    if (!ret)
        return (NULL);   
    if (*s) 
    {
        printf("cai aqui 1\n");
        unexpected(*s);
        destroy_tree(ret);
        return (NULL);
    }
    return (ret);
}

int eval_tree(node *tree)
{
    switch (tree->type)
    {
        case ADD:
            return (eval_tree(tree->l) + eval_tree(tree->r));
        case MULTI:
            return (eval_tree(tree->l) * eval_tree(tree->r));
        case VAL:
            return (tree->val);
    }
    return (0);
}

int main(int argc, char **argv)
{
    if (argc != 2)
        return (1);
    printf("aqui está a line %s\n", argv[1]);
    node *tree = parse_expr(argv[1]);
    if (!tree)
        return (1);
    printf("%d\n", eval_tree(tree));
    destroy_tree(tree);
}

