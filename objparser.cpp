#include "objparser.h"

ObjParser::ObjParser() {
	ParseFile("Object/REALDummy.obj", v, n, t, m);
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
			fscanf(fp, "%f %f %f\n", &v1, &v2, &v3);
			vertices.push_back(Vector3(v1, v2, v3));
		}
		else if (c1 == 'v' && c2 == 'n') {
			fscanf(fp, " %f %f %f\n", &n1, &n2, &n3);
			normals.push_back(Vector3(n1, n2, n3));
		}
		else if (c1 == 'v' && c2 == 't') {
			fscanf(fp, "%f %f\n", &t1, &t2);
			textures.push_back(Vector3(t1, t2, 0));
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

			Relation four;
			four.vertex = --v4;
			four.texture = --t4;
			four.normal = --n4;

			map.first = one;
			map.second = two;
			map.third = three;
			map.fourth = four;
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

	fclose(fp);
}

void ObjParser::Draw(int vertex, int texture, int normal)
{
	glColor3f(1, 1, 1);
	glVertex3f(v.at(vertex).x, v.at(vertex).y, v.at(vertex).z);
	glNormal3f(n.at(normal).x, n.at(normal).y, n.at(normal).z);
}

void ObjParser::ParserDraw() {
	glBegin(GL_QUADS);

	for (std::vector<Mapping>::iterator it = m.begin(); it != m.end(); ++it) {
		Mapping tmp = *it;
		Draw(tmp.first.vertex, tmp.first.texture, tmp.first.normal);
		Draw(tmp.second.vertex, tmp.second.texture, tmp.second.normal);
		Draw(tmp.third.vertex, tmp.third.texture, tmp.third.normal);
		Draw(tmp.fourth.vertex, tmp.fourth.texture, tmp.fourth.normal);
	}

	glEnd();
}