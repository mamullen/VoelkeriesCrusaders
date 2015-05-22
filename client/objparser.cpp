#include "objparser.h"

ObjParser::ObjParser() {
}

ObjParser::ObjParser(char *filename) {
	ParseFile(filename, v, n, t, m);
	max = Vector3(0,0,0);
	min = Vector3(0,0,0);
}

void ObjParser::ParseFile(char *filename, std::vector<Vector3> &vertices, std::vector<Vector3> &normals, std::vector<Vector3> &textures, std::vector<Mapping> &mapping) {
	std::cout << "parse " << filename << std::endl;
	FILE *fp;
	float vertex1, vertex2, vertex3;
	float texture1, texture2;
	float normal1, normal2, normal3;

	int v1, v2, v3, v4;
	int t1, t2, t3, t4;
	int n1, n2, n3, n4;
	int c1=0, c2;
	
	fp = fopen(filename, "r"); // _CRT_SECURE_NO_DEPRECATE 
	if (fp == NULL) {
		std::cerr << "error loading file" << std::endl;
		exit(-1);
	}

	while (c1 != EOF) {
		c1 = fgetc(fp); c2 = fgetc(fp);
		if (c1 == 'v' && c2 == ' ') {
			fscanf(fp, "%f %f %f\n", &vertex1, &vertex2, &vertex3);
			vertices.push_back(Vector3(vertex1, vertex2, vertex3));
			if (vertex1 > max.x)
			{
				printf("%d\n\n", vertex1);
				max.x = vertex1;
			}
			if (vertex2 > max.y)
			{
				max.y = vertex2;
			}
			if (vertex3 > max.z)
			{
				max.z = vertex3;
			}

			if (vertex1 < min.x)
			{
				min.x = vertex1;
			}
			if (vertex2 < min.y)
			{
				min.y = vertex2;
			}
			if (vertex3 < min.z)
			{
				min.z = vertex3;
			}
		}
		else if (c1 == 'v' && c2 == 'n') {
			fscanf(fp, " %f %f %f\n", &normal1, &normal2, &normal3);
			normals.push_back(Vector3(normal1, normal2, normal3));
		}
		else if (c1 == 'v' && c2 == 't') {
			fscanf(fp, "%f %f\n", &texture1, &texture2);
			textures.push_back(Vector3(texture1, texture2, 0));
		}
		else if (c1 == 'f' && c2 == ' ') { // face
			fscanf(fp, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &v1, &t1, &n1, &v2, &t2, &n2, &v3, &t3, &n3, &v4, &t4, &n4);
			//printf("%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", v1, t1, n1, v2, t2, n2, v3, t3, n3, v4, t4, n4);
			Mapping map;
			Relation one;
			one.vertex = --v1;
			one.texture = --t1;
			one.normal = --n1;

			Relation two; 
			two.vertex = --v2;
			two.texture = --t2;
			two.normal = --n2;

			Relation three; 
			three.vertex = --v3;
			three.texture = --t3;
			three.normal = --n3;

			map.first = one;
			map.second = two;
			map.third = three;
			mapping.push_back(map);
		}
		else {
			while (c1 != '\n') {
				c1 = fgetc(fp);
				if (c1 == EOF)
					break;
			}
		}
	}
	std::cout << "done with " << filename << std::endl;


	glColor3f(1, 1, 1);



	glBegin(GL_QUADS);
	glVertex3f(max.x, max.y, max.z); // vertex 1
	glVertex3f(max.x, max.y, min.z); // vertex 2
	glVertex3f(min.x, max.y, min.z); // vertex 3
	glVertex3f(min.x, max.y, max.z); // vertex 4

	glVertex3f(max.x, max.y, max.z); // vertex 1
	glVertex3f(max.x, max.y, min.z); // vertex 2
	glVertex3f(max.x, min.y, min.z); // vertex 2
	glVertex3f(max.x, min.y, max.z); // vertex 1

	glEnd();
	
	fclose(fp);
}

void ObjParser::DrawHelper(int vertex, int texture, int normal)
{
	glColor3f(n.at(normal).x, n.at(normal).y, n.at(normal).z);
	glVertex3f(v.at(vertex).x, v.at(vertex).y, v.at(vertex).z);
	glNormal3f(n.at(normal).x, n.at(normal).y, n.at(normal).z);
}

void ObjParser::Draw() {
	glPushMatrix();
	glScalef(0.3, 0.3, 0.3);
	glTranslatef(0, 2, -2);
	glBegin(GL_TRIANGLES);

	for (std::vector<Mapping>::iterator it = m.begin(); it != m.end(); ++it) {
		Mapping tmp = *it;
		DrawHelper(tmp.first.vertex, tmp.first.texture, tmp.first.normal);
		DrawHelper(tmp.second.vertex, tmp.second.texture, tmp.second.normal);
		DrawHelper(tmp.third.vertex, tmp.third.texture, tmp.third.normal);
	}

	glEnd();
	glPopMatrix();
}