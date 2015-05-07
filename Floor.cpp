//
//  Floor.cpp/Users/michaelmullenjr/Downloads/cse169 2/Floor.cpp
//  cse169
//
//  Created by michael mullen jr on 4/5/15.
//
//

#include "Floor.h"

Floor::Floor()
{
    state = true;
}
Floor::Floor(float t)
{
    exits=0;
    tilenumber=t;
    state=true;
    
    
    
}

void Floor::createFloor(float offsetx, float offsetz)
{
    glColor3f(0, 150, 0);
    glBegin(GL_QUADS);
    
    
    glVertex3f(-20+offsetx, -5,-20+offsetz); // vertex 1
    glVertex3f(-20+offsetx, -5,20+offsetz); // vertex 2
    glVertex3f(20+offsetx, -5,20+offsetz); // vertex 3
    glVertex3f(20+offsetx, -5,-20+offsetz); // vertex 4
    
    
    glEnd();
    
    if(buildingList.size()>0)
    {
    for(int i =0;i<buildingList.size();i++)
    {
        createTile(buildingList[i], offsetx, offsetz);
    }
    }

}

void Floor::createTile(Building* b, float offsetx, float offsetz)
{
	
    glEnable(GL_DEPTH_TEST);
   // if(tilenumber)
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    /*glEnable(GL_TEXTURE_2D);
    
    if(state)
    {
        glGenTextures(1, texture);
       // t.loadTexture("/Users/michaelmullenjr/Downloads/10984245_10153174086971095_5588850874871390930_n_phpwRaZiJ.ppm", texture[0]);
        
        state = false;
    }
    */
   // glBindTexture(GL_TEXTURE_2D, texture[0]);
    
    

	//glfwLoadTexture2D("image.tga", GLFW_BUILD_MIPMAPS_BIT);

	// Use trilinear interpolation for minification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// Use bilinear interpolation for magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glEnable(GL_TEXTURE_2D);
    //////////////FLLLOOOOOOOOOOOR
    glColor3f(1,1,1);
    
    
    
    glBegin(GL_QUADS);
    /*
    glVertex3f(-40, 3,-40); // vertex 1
    glVertex3f(-10, 3,-40); // vertex 2
    glVertex3f(-10, 3,-10); // vertex 3
    glVertex3f(-40, 3,-10); // vertex 4
    
    glVertex3f(-40, 3,-40); // vertex 1
    glVertex3f(-40, -5,-40);
    glVertex3f(-10, -5,-40); // vertex 2
    glVertex3f(-10, 3,-40);
    
   
    glVertex3f(-10, 3,-40); // vertex 2
    glVertex3f(-10, -5,-40); // vertex 2
    glVertex3f(-10, -5,-10); // vertex 3
    glVertex3f(-10, 3,-10); // vertex 3
    
    glVertex3f(-10, 3,-10); // vertex 3
    glVertex3f(-10, -5,-10); // vertex 3
    glVertex3f(-40, -5,-10); // vertex 4
    glVertex3f(-40, 3,-10); // vertex 4
    
    
    glVertex3f(-40, 3,-10); // vertex 4
    glVertex3f(-40, -5,-10); // vertex 4
    glVertex3f(-40, -5,-40); // vertex 1
    glVertex3f(-40, 3,-40);

    
    //Box 2
    glVertex3f(40, 10,-40); // vertex 1
    glVertex3f(5, 10,-40); // vertex 2
    glVertex3f(5, 10,-5); // vertex 3
    glVertex3f(40, 10,-5); // vertex 4
    
    glVertex3f(40, 10,-40); // vertex 1
    glVertex3f(40, -5,-40);
    glVertex3f(5, -5,-40); // vertex 2
    glVertex3f(5, 10,-40);
    
    
    glVertex3f(5, 10,-40); // vertex 2
    glVertex3f(5, -5,-40); // vertex 2
    glVertex3f(5, -5,-5); // vertex 3
    glVertex3f(5, 10,-5); // vertex 3
    
    glVertex3f(5, 10,-5); // vertex 3
    glVertex3f(5, -5,-5); // vertex 3
    glVertex3f(40, -5,-5); // vertex 4
    glVertex3f(40, 10,-5); // vertex 4
    
    
    glVertex3f(40, 10,-5); // vertex 4
    glVertex3f(40, -5,-5); // vertex 4
    glVertex3f(40, -5,-40); // vertex 1
    glVertex3f(40, 10,-40);
    
    //box3
    glVertex3f(40, 10,40); // vertex 1
    glVertex3f(15, 10,40); // vertex 2
    glVertex3f(15, 10,15); // vertex 3
    glVertex3f(40, 10,15); // vertex 4
    
    glVertex3f(40, 10,40); // vertex 1
    glVertex3f(40, -5,40);
    glVertex3f(15, -5,40); // vertex 2
    glVertex3f(15, 10,40);
    
    
    glVertex3f(15, 10,40); // vertex 2
    glVertex3f(15, -5,40); // vertex 2
    glVertex3f(15, -5,15); // vertex 3
    glVertex3f(15, 10,15); // vertex 3
    
    glVertex3f(15, 10,15); // vertex 3
    glVertex3f(15, -5,15); // vertex 3
    glVertex3f(40, -5,15); // vertex 4
    glVertex3f(40, 10,15); // vertex 4
    
    
    glVertex3f(40, 10,15); // vertex 4
    glVertex3f(40, -5,15); // vertex 4
    glVertex3f(40, -5,40); // vertex 1
    glVertex3f(40, 10,40);
    
    //box4
    glVertex3f(-40, 10,40); // vertex 1
    glVertex3f(-15, 10,40); // vertex 2
    glVertex3f(-15, 10,15); // vertex 3
    glVertex3f(-40, 10,15); // vertex 4
    
    glVertex3f(-40, 10,40); // vertex 1
    glVertex3f(-40, -5,40);
    glVertex3f(-15, -5,40); // vertex 2
    glVertex3f(-15, 10,40);
    
    
    glVertex3f(-15, 10,40); // vertex 2
    glVertex3f(-15, -5,40); // vertex 2
    glVertex3f(-15, -5,15); // vertex 3
    glVertex3f(-15, 10,15); // vertex 3
    
    glVertex3f(-15, 10,15); // vertex 3
    glVertex3f(-15, -5,15); // vertex 3
    glVertex3f(-40, -5,15); // vertex 4
    glVertex3f(-40, 10,15); // vertex 4
    
    
    glVertex3f(-40, 10,15); // vertex 4
    glVertex3f(-40, -5,15); // vertex 4
    glVertex3f(-40, -5,40); // vertex 1
    glVertex3f(-40, 10,40);
     */
    
    
    /////CEILING 1
   /* glVertex3f(-floorsizex, floorsizey+15, floorsizez-10);
    glVertex3f(-floorsizex+30, floorsizey+15, floorsizez-10);
    glVertex3f(-floorsizex+30, floorsizey+15, floorsizez-20);
    glVertex3f(-floorsizex, floorsizey+15, floorsizez-20);*/
    
    
  
    
    //ceiling
    glVertex3f(b->x1+offsetx, floorsizey+b->maxy, b->z1+offsetz);
    glVertex3f(b->x2+offsetx, floorsizey+b->maxy, b->z1+offsetz);
    glVertex3f(b->x2+offsetx, floorsizey+b->maxy, b->z2+offsetz);
    glVertex3f(b->x1+offsetx, floorsizey+b->maxy, b->z2+offsetz);
    

	if (b->x1 + offsetx < b->x2 + offsetx)
	{
		b->min.x = b->x1 + offsetx;
		b->max.x = b->x2 + offsetx;
	}
	else
	{
		b->max.x = b->x1 + offsetx;
		b->min.x = b->x2 + offsetx;
	}

	if (b->z1 + offsetz < b->z2 + offsetz)
	{
		b->min.z = b->z1 + offsetz;
		b->max.z = b->z2 + offsetz;
	}
	else
	{
		b->max.z = b->z1 + offsetz;
		b->min.z = b->z2 + offsetz;
	}
	
	b->min.y = floorsizey;
	b->max.y = floorsizey + b->maxy;
	
    
    //all four walls--counter clockwise creation
	glColor3f(1, 0, 0);
    glTexCoord2f(0, 0);
    glVertex3f(b->x1+offsetx, floorsizey+b->maxy, b->z1+offsetz);
    glTexCoord2f(1, 0);
    glVertex3f(b->x2+offsetx, floorsizey+b->maxy, b->z1+offsetz);
    glTexCoord2f(1, 1);
    glVertex3f(b->x2+offsetx, floorsizey, b->z1+offsetz);
    glTexCoord2f(0, 1);
    glVertex3f(b->x1+offsetx, floorsizey, b->z1+offsetz);
	
	
	glColor3f(1, 1, 1);
    glTexCoord2f(0, 0);
    glVertex3f(b->x2+offsetx, floorsizey+b->maxy, b->z1+offsetz);
   glTexCoord2f(1, 0);
    glVertex3f(b->x2+offsetx, floorsizey+b->maxy, b->z2+offsetz);
   glTexCoord2f(1, 1);
    glVertex3f(b->x2+offsetx, floorsizey, b->z2+offsetz);
    glTexCoord2f(0, 1);
    glVertex3f(b->x2+offsetx, floorsizey, b->z1+offsetz);
    
     glTexCoord2f(0, 0);
    glVertex3f(b->x2+offsetx, floorsizey+b->maxy, b->z2+offsetz);
    glTexCoord2f(1, 0);
    glVertex3f(b->x1+offsetx, floorsizey+b->maxy, b->z2+offsetz);
     glTexCoord2f(1, 1);
    glVertex3f(b->x1+offsetx, floorsizey, b->z2+offsetz);
     glTexCoord2f(0, 1);
    glVertex3f(b->x2+offsetx, floorsizey, b->z2+offsetz);
    
    
    glTexCoord2f(0, 0);
    glVertex3f(b->x1+offsetx, floorsizey+b->maxy, b->z2+offsetz);
   glTexCoord2f(1, 0);
    glVertex3f(b->x1+offsetx, floorsizey+b->maxy, b->z1+offsetz);
    glTexCoord2f(1, 1);
    glVertex3f(b->x1+offsetx, floorsizey, b->z1+offsetz);
    glTexCoord2f(0, 1);
    glVertex3f(b->x1+offsetx, floorsizey, b->z2+offsetz);

    /*
    glVertex3f(-floorsizex+30, floorsizey+15, floorsizez-20);
    glVertex3f(-floorsizex+30, floorsizey+15, floorsizez-60);
    glVertex3f(-floorsizex+20, floorsizey+15, floorsizez-60);
    glVertex3f(-floorsizex+20, floorsizey+15, floorsizez-20);
    
    
    //glColor3f(0, 0, 1);
    glVertex3f(-floorsizex, floorsizey, floorsizez-10);
    glVertex3f(-floorsizex+30, floorsizey, floorsizez-10);
    glVertex3f(-floorsizex+30, floorsizey+15, floorsizez-10);
    glVertex3f(-floorsizex, floorsizey+15, floorsizez-10);
    
    glVertex3f(-floorsizex+30, floorsizey, floorsizez-10);
    glVertex3f(-floorsizex+30, floorsizey, floorsizez-60);
    glVertex3f(-floorsizex+30, floorsizey+15, floorsizez-60);
    glVertex3f(-floorsizex+30, floorsizey+15, floorsizez-10);
    
    glVertex3f(-floorsizex+20, floorsizey, floorsizez-60);
    glVertex3f(-floorsizex+20, floorsizey, floorsizez-20);
    glVertex3f(-floorsizex+20, floorsizey+15, floorsizez-20);
    glVertex3f(-floorsizex+20, floorsizey+15, floorsizez-60);
    
    glVertex3f(-floorsizex+20, floorsizey, floorsizez-20);
    glVertex3f(-floorsizex, floorsizey, floorsizez-20);
    glVertex3f(-floorsizex, floorsizey+15, floorsizez-20);
    glVertex3f(-floorsizex+20, floorsizey+15, floorsizez-20);
    
    glVertex3f(-floorsizex+30, floorsizey, floorsizez-60);
    glVertex3f(-floorsizex+20, floorsizey, floorsizez-60);
    glVertex3f(-floorsizex+20, floorsizey+15, floorsizez-60);
    glVertex3f(-floorsizex+30, floorsizey+15, floorsizez-60);
    
    glVertex3f(-floorsizex, floorsizey, floorsizez-10);
    glVertex3f(-floorsizex, floorsizey, floorsizez-20);
    glVertex3f(-floorsizex, floorsizey+15, floorsizez-20);
    glVertex3f(-floorsizex, floorsizey+15, floorsizez-10);
    
   // glVertex3f(-floorsizex+40, floorsizey, floorsizez-10);
    
    glVertex3f(floorsizex,floorsizey+18 ,floorsizez);
    glVertex3f(floorsizex-20,floorsizey+18 ,floorsizez);
    glVertex3f(floorsizex-20,floorsizey+18 ,floorsizez-30);
    glVertex3f(floorsizex,floorsizey+18 ,floorsizez-30);
    
    
    
    glVertex3f(floorsizex,floorsizey+18 ,floorsizez);
    glVertex3f(floorsizex-20,floorsizey+18 ,floorsizez);
    glVertex3f(floorsizex-20,floorsizey ,floorsizez);
    glVertex3f(floorsizex,floorsizey,floorsizez);
    
    glVertex3f(floorsizex,floorsizey+18 ,floorsizez-30);
    glVertex3f(floorsizex,floorsizey+18 ,floorsizez);
    glVertex3f(floorsizex,floorsizey ,floorsizez);
    glVertex3f(floorsizex,floorsizey,floorsizez-30);
    
    glVertex3f(floorsizex-20,floorsizey+18 ,floorsizez-30);
    glVertex3f(floorsizex,floorsizey+18 ,floorsizez-30);
    glVertex3f(floorsizex,floorsizey ,floorsizez-30);
    glVertex3f(floorsizex-20,floorsizey,floorsizez-30);
    
    glVertex3f(floorsizex-20,floorsizey+18 ,floorsizez);
    glVertex3f(floorsizex-20,floorsizey+18 ,floorsizez-30);
    glVertex3f(floorsizex-20,floorsizey ,floorsizez-30);
    glVertex3f(floorsizex-20,floorsizey,floorsizez);
    
    
    glColor3f(1, 0, 0);
   // glVertex3f(-40-offsetx, -5,-40); // vertex 1
   // glVertex3f(-40-offsetx, -5,40); // vertex 2
   // glVertex3f(40-offsetx, -5,40); // vertex 3
   // glVertex3f(40-offsetx, -5,-40); // vertex 4
    
    glColor3f(1, 1, 1);*/
    
   
   
    
    
    
    glEnd();
    
   

    /*glPushMatrix();
    glTranslatef(0,-5,-10);//, <#GLfloat y#>, <#GLfloat z#>)
    
    glPopMatrix();*/
    
   }