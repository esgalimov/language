#include "tree.h"
#include "tree_debug.h"

int tree_ctor_(tree_t * tree, var_info info)
{
    ASSERT(tree != NULL);

    tree->root = NULL;
    tree->info = info;
    tree->status = 0;

    return STATUS_OK;
}

int tree_dtor(tree_t * tree)
{
    ASSERT(tree != NULL);

    nodes_dtor(tree->root);

    tree->status = 0;

    #ifdef LOG_MODE
        fprintf(log_file, "<pre>\nTree %p \"%s\" at %s at %s(%d): DESTRUCTED\n</pre>\n",
                    tree, tree->info.name, tree->info.func, tree->info.file, tree->info.line);
    #endif

    tree->info.file = NULL;
    tree->info.func = NULL;
    tree->info.line = 0;

    return STATUS_OK;
}

void nodes_dtor(tree_node_t * node)
{
    if (node == NULL) return;
    if (node != node->left)
        nodes_dtor(node->left);
    if (node != node->right)
        nodes_dtor(node->right);

    free(node->name);
    free(node);
}

int link_root(tree_t * tree, tree_node_t * root)
{
    ASSERT(tree != NULL);

    if (root == NULL) return LINK_NULL_ROOT;

    tree->root = root;

    return STATUS_OK;
}

tree_node_t* create_node(node_type type, size_t line, size_t pos, elem_t value, tree_node_t* left, tree_node_t* right)
{
    tree_node_t* node = (tree_node_t *) calloc(1, sizeof(tree_node_t));

    if (node == NULL)
        return NULL;

    node->type = type;
    node->value = value;
    node->pos = pos;
    node->line = line;

    if (left != NULL)
    {
        left->parent = node;
        node->left = left;
    }
    if (right != NULL)
    {
        right->parent = node;
        node->right = right;
    }

    return node;
}

int link_node(tree_node_t * parent, tree_node_t * child, link_mode mode)
{
    if (parent == NULL || child == NULL) return NODE_LINK_ERROR;
    if (mode != LEFT && mode != RIGHT)   return MODE_LINK_ERROR;

    if (mode == LEFT)
        parent->left = child;
    else
        parent->right = child;

    child->parent = parent;

    return STATUS_OK;
}
