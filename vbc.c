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
    if (**s)
    {
        (*s)++;
        return (1);
    }
    return (0);
}

int expect(char **s, char c)
{
    if (accept(s, c))
        return (1);
    unexpected(**s);
    return (0);
}

int find_valid_start(char *s)
{
    int i;
    int parenthesis;

    i = 0;
    parenthesis = -1;
    while(s[i])
    {
        if (s[i] == '(')
            parenthesis = i;
        i++;
    }
    if (parenthesis == -1)
        return (0);
    i = 0;
    while(s[i])
    {
        if (s[i] == ')')
            return (parenthesis);
        i++;
    }
    unexpected('\0');
    return (-1);
}


node    *parse_expr(char *s)
{
    int i;
    int start;
    int end;
    node *ret;

    ret = NULL;
    i = 0;
    start = 0;
    end = 0;
    while (s[i])
    {
        i = 0;
        start = find_valid_start(s);
        if (start == -1)
            return (NULL);
        while(s[start] && s[start] != ')')
            start++;
        end = s[start];
        i++;
    }
    if (*s) 
    {
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

