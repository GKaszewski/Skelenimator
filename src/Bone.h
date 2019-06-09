#ifndef BONE_H_INCLUDED
#define BONE_H_INCLUDED

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_CHCOUNT 8
#define BONE_ABS_ANGLE 0x01
#define BONE_ABS_POSITION 0x02
#define BONE_ABS (BONE_ABS_ANGLE | BONE_ABS_POSITION)
#define MAX_BONECOUNT 20

typedef struct _Bone{
    char name[20];
    float x, y, a, l;
    uint8_t flags;
    uint8_t childCount;

    struct _Bone * child[MAX_CHCOUNT], * parent;
} Bone;

Bone * boneAddChild(Bone * root, float x, float y, float angle, float length, uint8_t flags, char * name);
Bone * boneFreeTree(Bone * root);
Bone * boneFindByName(Bone * root, char * name);
void boneDumpTree(Bone * root, uint8_t level);

Bone * boneLoadStructure(char * path);
void drawBone(Bone * root, char * currentName);
void boneListNames(Bone * root, char names[MAX_BONECOUNT][20]);
#endif