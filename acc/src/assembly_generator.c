#include <stddef.h>

#include "log.h"
#include "assembly_generator.h"

RegId AsGen_Generate(const AstNode* root)
{
    if (root == NULL)
    {
        return INVALID_REG_ID;
    }

    const RegId left = AsGen_Generate(root->left);
    const RegId right = AsGen_Generate(root->right);

    switch (root->type)
    {
        case NODE_TYPE_ADD: return AsGen_Add(left,right);
        case NODE_TYPE_INTLIST: return AsGen_Load(root->intValue);

        default:
            LOG_ERROR("Unknown node type %d",root->type);
            break;
    }

    return INVALID_REG_ID;
}