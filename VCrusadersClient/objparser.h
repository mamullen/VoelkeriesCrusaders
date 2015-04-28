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
		struct Relation {
			int vertex;
			int texture;
			int normal;
		};

		struct Mapping {
			Relation first;
			Relation second;
			Relation third;
		};

	public:
		ObjParser();
		ObjParser(char*);
		void ParseFile(char *filename, std::vector<Vector3>&, std::vector<Vector3>&, std::vector<Vector3>&, std::vector<Mapping>&);
		void DrawHelper(int, int, int);
		void Draw();

		std::vector<Vector3> v;
		std::vector<Vector3> t;
		std::vector<Vector3> n;
		std::vector<Mapping> m;

		std::vector<Mapping> *tmp_mapping_ptr = &m;

};


#endif