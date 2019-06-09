#include "Bone.h"

#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

Bone * boneAddChild(Bone * root, float x, float y, float angle, float length, uint8_t flags, char * name){
    Bone * tmp;
    int i;

    if(!root){
        if(!(root = (Bone *)malloc(sizeof(Bone)))){
            return NULL;
        }

        root->parent = NULL;
    }else if(root->childCount < MAX_CHCOUNT){
        if(!(tmp = (Bone *)malloc(sizeof(Bone)))){
            return NULL;
        }

        tmp->parent = root;
        root->child[root->childCount++] = tmp;
        root = tmp;
    }else{
        return NULL;
    }

    root->x = x;
    root->y = y;
    root->a = angle;
    root->l = length;
    root->flags = flags;
    root->childCount = 0;

    if(name){
        strcpy(root->name, name);
    }else{
        strcpy(root->name, "Bone");
    }

    for (i = 0; i < MAX_CHCOUNT; i++)
    {
        root->child[i] = NULL;
    }

    return root;
}

Bone * boneFreeTree(Bone * root){
    int i;
    if(!root){
        return NULL;
    }

    for (i = 0; i < root->childCount; i++)
    {
        boneFreeTree(root->child[i]);
    }
    
    free(root);

    return NULL;
}

void boneDumpTree(Bone * root, uint8_t level){
    int i;
    if(!root){
        return;
    }

    for (i = 0; i < level; i++)
    {
        printf("#");
    }

    printf(" %4.4f %4.4f %4.4f %4.4f %d %d %s\n", root->x, root->y,
        root->a, root->l, root->childCount, root->flags, root->name);
    
    for (i = 0; i < root->childCount; i++)
    {
        boneDumpTree(root->child[i], level + 1);
    }
    
}

Bone * boneLoadStructure(char * path){
    Bone * root, *tmp;

    FILE * file;
    float x, y, angle, length;

    int depth, actualLevel, flags;

    char name[20], depthStr[20], buffer[512];

    if (!(file = fopen(path, "r")))
	{
		fprintf(stderr, "Can't open file %s for reading\n", path);
		return NULL;
	}

    root = NULL;
    tmp = NULL;
    actualLevel = 0;

    while (!feof(file)){
        fgets(buffer, 512, file);
        sscanf(buffer, "%s %f %f %f %f %d %s\n", depthStr, &x, &y, &angle, &length, &flags, name);\

        if (strlen(buffer) < 3)
			continue;

        depth = strlen(depthStr) - 1;
		if (depth < 0 || depth > MAX_CHCOUNT)
		{
			fclose(file);
			fprintf(stderr, "Wrong bone depth (%s)\n", depthStr);
			return NULL;
		}

        for (; actualLevel > depth; actualLevel--)
			tmp = tmp->parent;

        if (!root && !depth)
		{
			root = boneAddChild(NULL, x, y, angle, length, flags, name);
			tmp = root;
		}
		else
			tmp = boneAddChild(tmp, x, y, angle, length, flags, name);

        actualLevel++;
    }

    fclose(file);
    return root;
}

void drawBone(Bone * root, char * currentName){
    int i;

    glPushMatrix();

    glTranslatef(root->x, root->y, 0.0);
    glRotatef((root->a * 180.0)/ M_PI, 0.0, 0.0, 1.0);
    
    glBegin(GL_LINES);

    if (!strcmp(root->name, currentName))
	    glColor3f(0.0, 0.0, 1.0);
    else
	    glColor3f(1.0, 0.0, 0.0);

    glVertex2f(0, 0);

    if (!strcmp(root->name, currentName))
	    glColor3f(1.0, 1.0, 0.0);
    else
	    glColor3f(0.0, 1.0, 0.0);
	
    glVertex2f(root->l, 0);
	glEnd();

    glTranslatef(root->l, 0.0, 0.0);

    for (i = 0; i < root->childCount; i++){
        drawBone(root->child[i], currentName);
    }
    
    glPopMatrix();
}

Bone * boneFindByName(Bone * root, char * name){
    int i;
    Bone * p;

    if(!root){
        return NULL;
    }

    if(!strcmp(root->name, name)){
        return root;
    }

    for (i = 0; i < root->childCount; i++){
        p = boneFindByName(root->child[i], name);

        if(p){
            return p;
        }
    }

    return NULL;
}

void boneListNames(Bone * root, char names[MAX_BONECOUNT][20]){
    int i, present;

    if(!root){
        return;
    }

    present = 0;
    for (i = 0; (i < MAX_BONECOUNT) && (names[i][0] != '\0'); i++){
        if(!strcmp(names[i], root->name)){
            present = 1;
            return;
        }
    }

    if(!present && (i < MAX_BONECOUNT)){
        strcpy(names[i], root->name);

        if(i + 1 < MAX_BONECOUNT)
            names[i+1][0] = '\0';
    }

    for (i = 0; i < root->childCount; i++){
        boneListNames(root->child[i], names);
    }
    
    
}