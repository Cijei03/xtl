#include "../include/xtlp-100.h"

#ifndef __STDC_WANT_LIB_EXT2__
	#define __STDC_WANT_LIB_EXT2__ 1
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "xtlp-general.h"

#define delete__private(ptr) if (ptr != NULL) { free(ptr); ptr = NULL; }

typedef struct xtlpSectionMatchResult__private_t
{
	char* Name;
	uint32_t Identations;
} xtlpSectionMatchResult__private;

typedef struct xtlpInstance__private_t
{
	size_t ID;
	const char* Name;
} xtlpInstance__private;

typedef struct xtlpPairMatchResult__private_t
{
	xtlpPair Pair;
	uint32_t Identations;
} xtlpPairMatchResult__private;

static xtlpSectionMatchResult__private xtlpCheckForSectionMatch__private(const char* Line)
{
	const size_t LineLength = strlen(Line);
	int SectionCondition = 0;
	xtlpSectionMatchResult__private Cache = {};

	// Check for tabs.
	for (size_t i = 0; i < LineLength; i++)
	{
		if (Line[i] == '\t')
		{
			Cache.Identations++;
		}
		else if (Line[i] == '<')
		{
			SectionCondition = i + 1;
		}
		else
		{
			break;
		}
	}

	if (!SectionCondition)
	{
		return Cache;
	}

	const size_t SectionNameLen = LineLength - SectionCondition;

	Cache.Name = (char*)calloc(SectionNameLen + 1, 1);
	strcpy(Cache.Name, &Line[SectionCondition]);

	return Cache;
}

static xtlpPairMatchResult__private xtlpCheckForPairMatch__private(const char* Line)
{
	typedef enum Condition_t
	{
		PAIR_CONDITION_EQUALITY_SIGN,
		PAIR_CONDITION_APOSTROPHE_SIGN_OPEN,
		PAIR_CONDITION_APOSTROPHE_SIGN_CLOSE
	} Condition;

	const size_t LineLength = strlen(Line);
	size_t PairConditions[3] = {};
	xtlpPairMatchResult__private Cache = {};

	for (size_t i = 0; i < LineLength; i++)
	{
		if (Line[i] == '\t')
		{
			Cache.Identations++;
		}
		else if (Line[i] == '=')
		{
			PairConditions[PAIR_CONDITION_EQUALITY_SIGN] = i;
		}
		else if (Line[i] == "'"[0] && PairConditions[PAIR_CONDITION_APOSTROPHE_SIGN_OPEN])
		{
			PairConditions[PAIR_CONDITION_APOSTROPHE_SIGN_CLOSE] = i;
			break;
		}
		else if (Line[i] == "'"[0])
		{
			PairConditions[PAIR_CONDITION_APOSTROPHE_SIGN_OPEN] = i;
		}
	}

	if (PairConditions[PAIR_CONDITION_EQUALITY_SIGN] == PairConditions[PAIR_CONDITION_APOSTROPHE_SIGN_OPEN] - 1 && PairConditions[PAIR_CONDITION_APOSTROPHE_SIGN_CLOSE])
	{
		const size_t KeyNameLen = PairConditions[PAIR_CONDITION_EQUALITY_SIGN] - Cache.Identations;
		Cache.Pair.Key = (char*)calloc(KeyNameLen + 1, 1);
		memcpy(Cache.Pair.Key, &Line[Cache.Identations], KeyNameLen);

		const size_t ValueLen = PairConditions[PAIR_CONDITION_APOSTROPHE_SIGN_CLOSE] - PairConditions[PAIR_CONDITION_APOSTROPHE_SIGN_OPEN] - 1;
		Cache.Pair.Value = (char*)calloc(ValueLen + 1, 1);
		memcpy(Cache.Pair.Value, &Line[PairConditions[PAIR_CONDITION_APOSTROPHE_SIGN_OPEN] + 1], ValueLen);
	}

	return Cache;
}

static xtlpNode* xtlpSearchForNode__private(xtlpNode* Stack, const size_t StackLength, const char* ContextID)
{
	xtlpNode* Free = NULL;

	const size_t ContextLen = strlen(ContextID);

	for (size_t i = 0; i < StackLength; i++)
	{
		if (ContextLen == strlen(Stack[i].private__))
		{
			if (strcmp(Stack[i].private__, ContextID) == 0)
			{
				return &Stack[i];
			}
		}
	}

	return Free;
}

static xtlpInstance__private* xtlpSearchForInstance__private(xtlpInstance__private** InstancesBase, size_t* InstanceBaseLength, const char* Name)
{
	for (size_t i = 0; i < *InstanceBaseLength; i++)
	{
		if (strcmp((*InstancesBase)[i].Name, Name) == 0)
		{
			return &(*InstancesBase)[i];
		}
	}

	*InstanceBaseLength = *InstanceBaseLength + 1;
	*InstancesBase = realloc(*InstancesBase, sizeof(xtlpInstance__private) * *InstanceBaseLength);

	(*InstancesBase)[*InstanceBaseLength - 1].ID = 0;
	(*InstancesBase)[*InstanceBaseLength - 1].Name = Name;
	return &(*InstancesBase)[*InstanceBaseLength - 1];
}

static void xtlpMakePtrs__private(xtlpNode* Stack, xtlpNode* Node)
{
	size_t id = 0;
	while (Node->Childs[id] != NULL)
	{
		Node->Childs[id] = &Stack[(size_t)Node->Childs[id] - 1];
		xtlpMakePtrs__private(Stack, Node->Childs[id]);
		id++;
	}
}

xtlpNodes xtlpMakeStack(const char* FileName, xtlpNodesStack* StackAddress)
{
	FILE* FileHandle = fopen(FileName, "r");

	if (FileHandle == NULL)
	{
		return NULL;
	}
	char* LineBuffer = NULL;
	char* Context[SIZE_WIDTH] = {};
	xtlpInstance__private* InstancesBase = NULL;
	xtlpNodes UserAvailableStack = NULL;
	xtlpNode** Stack = (xtlpNode**)&StackAddress->private__0;
	size_t* StackLength = (size_t*)&StackAddress->private__1;
	size_t UserAvailableStackLength = 1;
	size_t InstanceBaseLength = 0;

	while (1)
	{
		// Get line from file.
		{
			size_t xd;
			getline(&LineBuffer, &xd, FileHandle);
			if (feof(FileHandle) != 0)
			{
				break;
			}
		}

		xtlpSectionMatchResult__private m = xtlpCheckForSectionMatch__private(LineBuffer);
		
		if (m.Name)
		{
			// Firstly process it without any white space characters.
			{
				char Name[SIZE_WIDTH] = {};
				sscanf(m.Name, "%s", Name);
				free(m.Name);

				if (!strlen(Name))
				{
					Name[0] = 'n';
					Name[1] = 'u';
					Name[2] = 'l';
					Name[3] = 'l';
				}
				m.Name = strdup(Name);
			}

			if (m.Identations > conlen(Context))
			{
				Context[conlen(Context) + 1] = m.Name;
			}
			else
			{
				memset(&Context[m.Identations], 0, sizeof(char*) * conlen(Context) - m.Identations);
				Context[m.Identations] = m.Name;
			};

			char* ContextString = xtlpMakeContextString(Context, conlen(Context));
			xtlpInstance__private* Instance = xtlpSearchForInstance__private(&InstancesBase, &InstanceBaseLength, m.Name);
			Instance->ID++;
			char* InstancedContextString = calloc(strlen(ContextString) + 3, 1);
			sprintf(InstancedContextString, "%s@%u", ContextString, Instance->ID);

			*StackLength = *StackLength + 1;
			*Stack = realloc(*Stack, sizeof(xtlpNode) * *StackLength);
			xtlpNode* Node = &(*Stack)[*StackLength - 1];

			Node->Childs = calloc(sizeof(void*), 1);
			Node->Pairs = calloc(sizeof(xtlpPair), 1);
			Node->Name = m.Name;
			Node->private__ = strdup(InstancedContextString);

			if (m.Identations == 0)
			{
				UserAvailableStackLength++;
				UserAvailableStack = realloc(UserAvailableStack, sizeof(void*) * UserAvailableStackLength);
				UserAvailableStack[UserAvailableStackLength - 1] = NULL;
				UserAvailableStack[UserAvailableStackLength - 2] = (void*)(*StackLength - 1);
			}
			else
			{
				const xtlpInstance__private* ParentInstance = xtlpSearchForInstance__private(&InstancesBase, &InstanceBaseLength, Context[conlen(Context) - 1]);
				char* ParentConString = xtlpMakeContextString(Context, conlen(Context) - 1);
				char* InstancedParentConString = calloc(strlen(ParentConString) + 3, 1);
				sprintf(InstancedParentConString, "%s@%u", ParentConString, ParentInstance->ID);
				xtlpNode* ParentNode = xtlpSearchForNode__private(*Stack, *StackLength, InstancedParentConString);

				size_t ChildsCount = 1;
				while (ParentNode->Childs[ChildsCount - 1] != NULL)
				{
					ChildsCount++;
				}
				const size_t NewChildsCount = ChildsCount + 1;
				ParentNode->Childs = realloc(ParentNode->Childs, sizeof(void*) * NewChildsCount);
				ParentNode->Childs[NewChildsCount - 1] = NULL;
				ParentNode->Childs[NewChildsCount - 2] = (xtlpNode*)(Node - *Stack + 1);

				free(ParentConString);
				free(InstancedParentConString);
			}

			free(ContextString);
			free(InstancedContextString);
		}
		else
		{
			xtlpPairMatchResult__private m = xtlpCheckForPairMatch__private(LineBuffer);

			if (m.Pair.Key != NULL && m.Pair.Value != NULL)
			{
				char* ContextString = xtlpMakeContextString(Context, m.Identations);
				char* InstancedContextString = calloc(strlen(ContextString) + 3, 1);
				sprintf(InstancedContextString, "%s@%u", ContextString, xtlpSearchForInstance__private(&InstancesBase, &InstanceBaseLength, Context[conlen(Context) - 1])->ID);

				xtlpNode* Node = xtlpSearchForNode__private(*Stack, *StackLength, InstancedContextString);

				size_t len = 1;
				while (Node->Pairs[len - 1].Key != NULL)
				{
					len++;
				}
				const size_t NewLen = len + 1;
				Node->Pairs = realloc(Node->Pairs, sizeof(xtlpPair) * NewLen);
				memset(&Node->Pairs[NewLen - 1], 0, sizeof(xtlpPair));
				Node->Pairs[NewLen - 2] = m.Pair;

				free(ContextString);
				free(InstancedContextString);
			}
		}
	}

	delete__private(InstancesBase);
	delete__private(LineBuffer);
	fclose(FileHandle);

	for (int i = 0; i < UserAvailableStackLength - 1; i++)
	{
		UserAvailableStack[i] = &(*Stack)[(size_t)UserAvailableStack[i]];
		xtlpMakePtrs__private(*Stack, UserAvailableStack[i]);
	}
	UserAvailableStack[UserAvailableStackLength - 1] = NULL;

	return UserAvailableStack;
}

static void xtlpClearNode__private(const xtlpNode Node)
{
	for (int pid = 0; Node.Pairs[pid].Key != NULL; pid++)
	{
		free(Node.Pairs[pid].Key);
		free(Node.Pairs[pid].Value);
	}
	free(Node.Pairs);
	free(Node.Name);
	free(Node.Childs);
	free(Node.private__);
}

void xtlpRemoveStack(xtlpNodesStack* Stack, xtlpNodes Nodes)
{
	xtlpNode* S = *(xtlpNode**)&Stack->private__0;
	for (size_t id = 0; id < *((size_t*)&Stack->private__1); id++)
	{
		xtlpClearNode__private(S[id]);
	}
	free(S);
	free(Nodes);
}