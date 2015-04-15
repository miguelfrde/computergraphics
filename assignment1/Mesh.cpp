#include <cstdio>
#include <cstring>
#include "Mesh.h"

Mesh::Mesh(int nov, int nof) : nv(nov), nt(nof) {
    this->rotation = Vector(0, 0, 0);
    this->translation = Vector(0, 0, 0);
    this->scale = Vector(1, 1, 1);
}

Mesh::Mesh(int nv, int nt, float *vArr, int *tArr) : nv(nv), nt(nt) {
    this->vertices.resize(nv);
    this->vnorms.resize(nv);
    this->triangles.resize(nt);
    this->scale = Vector(1, 1, 1);
	
    // set mesh vertices
	for (int i = 0; i < nv; i++) {
        float x = vArr[i*3];
		float y = vArr[i*3+1];
		float z = vArr[i*3+2];
        this->vertices[i] = Vector(x, y, z); 
	}

	// set mesh triangles
	for (int i = 0; i < nt; i++) {
		this->triangles[i].vInds[0] = tArr[i*3];
		this->triangles[i].vInds[1] = tArr[i*3+1];
		this->triangles[i].vInds[2] = tArr[i*3+2];
	}

	// Assignment 1: 
	// Calculate and store suitable vertex normals for the mesh here.
	// Replace the code below that simply sets some arbitrary normal values	
	for (int i = 0; i < nv; i++) {
        this->vnorms[i] = Vector(0, 0, 0);
    }
    for (int i = 0; i < nt; i++) {
        Triangle t = this->triangles[i];
        for (int j = 0; j < 3; j++) {
            Vector ab = this->vertices[t.vInds[(j+1)%3]] - this->vertices[t.vInds[j]];
            Vector ac = this->vertices[t.vInds[(j+2)%3]] - this->vertices[t.vInds[j]];
            this->vnorms[t.vInds[j]] += ab.cross(ac).normalized();
        }
    }
    for (int i = 0; i < nv; i++) {
        this->vnorms[i] = this->vnorms[i].normalized();
    }

}

int Mesh::nvertices() {
    return this->nv;
}

int Mesh::ntriangles() {
    return this->nt;
}

void Mesh::setScale(Vector scale) {
    this->scale = scale;
}

void Mesh::setRotation(Vector rotation) {
    this->rotation = rotation;
}

void Mesh::setTranslation(Vector translation) {
    this->translation = translation;
}

Matrix Mesh::transformationMatrix() {
    return Matrix::translation(translation)
            * Matrix::rotation('x', rotation.x)
            * Matrix::rotation('y', rotation.y)
            * Matrix::rotation('z', rotation.z)
            * Matrix::scale(scale);
}

/*Mesh Mesh::load(string model_name) {
    vector<Vector> vertices;
    vector<Vector> vnorms;
    vector<Triangle> triangles;
    int r;
    char line_header[128];
    
    FILE * file = fopen(model_name.c_str(), "r");
    if (file == NULL) {
        // TODO: throw exception
    }
    
    while ((r = fscanf(file, "%s", line_header)) != EOF) {
        if (strcmp(line_header, "v") == 0) {
            Vector v;
            fscanf(file, "%f %f %f\n", &v.x, &v.y, &v.z);
            vertices.push_back(v);
        } else if (strcmp(line_header, "vt")) {
            // ignore for now
        } else if (strcmp(line_header, "vn")) {
            Vector v;
            fscanf(file, "%f %f %f\n", &v.x, &v.y, &v.z);
            vnorms.push_back(v);
        } else if (strcmp(line_header, "f")) {
            Triangle t;
            fscanf(file, "%d/%*d/%*d %*d/%*d/%*d %*d/%*d/%*d\n",
                    &t.vInds[0], &t.vInds[1], &t.vInds[2]);
            triangles.push_back(t);
        } else {
            // ignore for now
        }
    }

    if (vertices.size() != vnorms.size()) {
        // TODO: throw exception
    }

    Mesh m = Mesh(vertices.size(), triangles.size());
    m.vertices = vertices;
    m.triangles = triangles;
    m.vnorms = vnorms;
    return m;
}*/
