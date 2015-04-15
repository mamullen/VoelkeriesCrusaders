#include "objparser.h"

ObjParser::ObjParser() {
	//ParseFile("dragon.obj", bunnyInfo, bunnyMapping);
}

void ObjParser::ParseFile(char *filename, std::vector<info> &vec_info, std::vector<mapping> &vec_mapping) {
	std::cout << "parse " << filename << std::endl;
	FILE *fp;
	float x, y, z;
	float r, g, b;
	float vn1, vn2, vn3;
	int v1, v2, v3, n1, n2, n3;
	int c1=0, c2;
	
	fp = fopen(filename, "r"); // _CRT_SECURE_NO_DEPRECATE 
	if (fp == NULL) {
		std::cerr << "error loading file" << std::endl;
		exit(-1);
	}

	while (c1 != EOF) {
		c1 = fgetc(fp); c2 = fgetc(fp);
		if (c1 == '#') {
			while (c1 != '\n') {
				c1 = fgetc(fp);
				if (c1 == EOF)
					break;
			}

		}
		else if (c1 == 'v' && c2 == 'n') {
			fscanf(fp, " %f %f %f\n", &vn1, &vn2, &vn3);
			//printf("vn: %f %f %f\n", vn1, vn2, vn3);

			c1 = fgetc(fp); c2 = fgetc(fp);
			if (c1 == 'v' && c2 == ' ') {
				fscanf(fp, "%f %f %f %f %f %f\n", &x, &y, &z, &r, &g, &b);
				//printf("v: %f %f %f %f %f %f\n", x, y, z, r, g, b);
			}

			info bI;
			bI.vertices = Vector3(x, y, z);
			bI.colors = Vector3(r, g, b);
			bI.normals = Vector3(vn1, vn2, vn3);
			vec_info.push_back(bI);
		}
		else if (c1 == 'f' && c2 == ' ') { // face
			fscanf(fp, "%d//%d %d//%d %d//%d\n", &v1, &n1, &v2, &n2, &v3, &n3);
			//printf("%d//%d %d//%d %d//%d\n", v1, n1, v2, n2, v3, n3);
			mapping map;
			vertNormRelation one;
			one.first = --v1;
			one.second = --n1;

			vertNormRelation two;
			two.first = --v2;
			two.second = --n2;

			vertNormRelation three;
			three.first = --v3;
			three.second = --n3;

			map.first = one;
			map.second = two;
			map.third = three;
			vec_mapping.push_back(map);
		}
	}
	std::cout << "done with " << filename << std::endl;

	fclose(fp);
}

void ObjParser::Draw(std::vector<info>& vec_info, int vertexIndex, int normalIndex)
{
	glNormal3f(vec_info.at(normalIndex).normals.x, vec_info.at(normalIndex).normals.y, vec_info.at(normalIndex).normals.z);
	glColor3f(vec_info.at(vertexIndex).colors.x, vec_info.at(vertexIndex).colors.y, vec_info.at(vertexIndex).colors.z);
	glVertex3f(vec_info.at(normalIndex).vertices.x, vec_info.at(normalIndex).vertices.y, vec_info.at(normalIndex).vertices.z);
}

void ObjParser::ParserDraw() {
	glBegin(GL_TRIANGLES);

	for (std::vector<mapping>::iterator it = tmp_mapping_ptr->begin(); it != tmp_mapping_ptr->end(); ++it) {
		mapping tmp = *it;
		Draw(*tmp_info_ptr, tmp.first.first, tmp.first.second);
		Draw(*tmp_info_ptr, tmp.second.first, tmp.second.second);
		Draw(*tmp_info_ptr, tmp.third.first, tmp.third.second);
	}

	glEnd();
}