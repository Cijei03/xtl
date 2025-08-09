#pragma once

typedef struct xtlpPair_t
{
	char* Key;
	char* Value;
} xtlpPair;

typedef struct xtlpNode_t
{
	char* Name;
	xtlpPair* Pairs;
	struct xtlpNode_t** Childs;
	void* private__;
} xtlpNode;

typedef struct xtlpNodesStack_t
{
	void* private__0;
	void* private__1;
} xtlpNodesStack;

typedef xtlpNode** xtlpNodes;

xtlpNodes xtlpMakeStack(const char* FileName, xtlpNodesStack* StackAddress);
void xtlpRemoveStack(xtlpNodesStack* Stack, xtlpNodes Nodes);