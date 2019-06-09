#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SDL2/SDL.h>

#include "Bone.h"

Bone * root, * p;
char * currentName = NULL;
int nameIndex = 0;
char names[MAX_BONECOUNT][20];

void keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods){
    if(key == GLFW_KEY_LEFT){
        p = boneFindByName(root, currentName);

        if(p)
            p->a += 0.1;
    }

    if(key == GLFW_KEY_RIGHT){
        p = boneFindByName(root, currentName);

        if(p)
            p->a -= 0.1;
    }

    if (key == GLFW_KEY_UP){
        p = boneFindByName(root, currentName);
	    if (p)
		    p->l += 0.1;
    }

    if (key == GLFW_KEY_DOWN){
         p = boneFindByName(root, currentName);
	    if (p)
		    p->l -= 0.1;
    }
}

void mouseCallback(GLFWwindow * window, int button, int action, int mods){
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);
        root->x = (float)xPos - 200.0;
        root->y = 200.0 - (float)yPos;
        printf("x:%f\ty:%f\n", xPos, yPos);
    }
}

void scrollCallback(GLFWwindow * window, double xOffset, double yOffset){
    if((nameIndex < MAX_BONECOUNT) && (names[nameIndex][0] != 0))
        nameIndex++;
    else
        nameIndex =0;
        

    currentName = names[nameIndex];
}

void main(int argc, char **argv){

    int i;
    root = boneLoadStructure(argv[1]);

    names[0][0] = '\0';

    boneListNames(root, names);
    currentName = names[nameIndex];

    for (i = 0; (i < MAX_BONECOUNT) && (names[i][0] != '\0'); i++){
        printf("Bone name: %s\n", names[i]);
    }
    

    if (argc < 2)
	{
		fprintf(stderr, "This program require a filename as parameter\n");
		return;
	}

    if(!glfwInit()){
       glfwTerminate();
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    GLFWwindow * window = glfwCreateWindow(400, 400, "Skelenimator", NULL, NULL);
    if(!window){
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    glViewport(0,0, 400, 400);

    glMatrixMode(GL_PROJECTION);
    glOrtho(-200, 200, -200, 200, -1, 1);
    glMatrixMode(GL_MODELVIEW);

  

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();

        drawBone(root, currentName);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }      

    glfwDestroyWindow(window);
    glfwTerminate();

}
