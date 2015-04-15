////////////////////////////////////////
// objparser.h
// - author: Wesley Lau
// loads objs
////////////////////////////////////////

#ifndef OBJPAR_H_
#define OBJPAR_H_

#include "core.h"
#include "vector3.h"
#include <iostream>
#include <vector>

class ObjParser {
	private:
		struct info {
			Vector3 vertices;
			Vector3 colors;
			Vector3 normals;
		};

		struct vertNormRelation {
			int first;
			int second;
		};

		struct mapping {
			vertNormRelation first;
			vertNormRelation second;
			vertNormRelation third;
		};

	public:
		ObjParser();
		void ParseFile(char *filename, std::vector<info> &vec_info, std::vector<mapping> &vec_mapping);
		void Draw(std::vector<info>& vec_info, int vertexIndex, int normalIndex);
		void ParserDraw();

		std::vector<info> bunnyInfo;
		std::vector<mapping> bunnyMapping;

		std::vector<info> *tmp_info_ptr = &bunnyInfo;
		std::vector<mapping> *tmp_mapping_ptr = &bunnyMapping;

};


#endif