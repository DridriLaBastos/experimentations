#include <stddef.h>

#include "log.h"
#include "assembly_generator.h"
#include "./generators/generator_api.h"

static RegId InternalAssemblyGenerate(const AstNode* root)
{
    if (root == NULL)
    {
        return INVALID_REG_ID;
    }
    
    const RegId left = InternalAssemblyGenerate(root->left);
    const RegId right = InternalAssemblyGenerate(root->right);
    
    switch (root->type)
    {
        case NODE_TYPE_ADD: return AsGenApi_Add(left,right);
        case NODE_TYPE_SUB: return AsGenApi_Sub(left,right);
        case NODE_TYPE_MUL: return AsGenApi_Mul(left,right);
        case NODE_TYPE_DIV: return AsGenApi_Div(left,right);
        case NODE_TYPE_INTLIST: return AsGenApi_Load(root->intValue);
    
        default:
            LOG_ERROR("Unknown node type %d",root->type);
            break;
    }
    
    return INVALID_REG_ID;
}

void AsGen_Generate(const AstNode* root)
{
    AsGenApi_Preamble();
    RegId reg = InternalAssemblyGenerate(root);
    AsGenApi_Postamble(reg);
}