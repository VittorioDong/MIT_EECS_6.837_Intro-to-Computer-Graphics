#include"ifs.h"
#include<random>
#include<iostream>

void IFS::read_description(const char* input_file){
// ========================================================
// ========================================================
// Some sample code you might like to use for
// parsing the IFS transformation files

// open the file
	FILE *input = fopen(input_file,"r");
	assert(input != NULL);

// read the number of transforms
	int num_transforms;
	fscanf(input,"%d",&num_transforms);

// < DO SOMETHING WITH num_transforms >
	this->num_transformations = num_transforms;
	this->transformations = new Matrix[this->num_transformations];
	this->probabilities = new float[this->num_transformations];

// read in the transforms
	for (int i = 0; i < num_transforms; i++) {
		float probability;
		fscanf (input,"%f",&probability);
		Matrix m;
		m.Read3x3(input);
		// < DO SOMETHING WITH probability and m >
		this->transformations[i] = m;
		this->probabilities[i] = probability;
	}
	// close the file
	fclose(input);

// ========================================================
// ========================================================
}

void IFS::render(Image& img, int num_points, int num_iters){
	vector<float> weights;
    for(int i = 0; i < this->num_transformations; i ++)
        weights.push_back(this->probabilities[i]);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<> d(weights.begin(), weights.end());

    for (int i = 0; i < num_points; i++) {
        Vec2f v = Vec2f((float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
        for (int k = 0; k < num_iters; k++) {         
            this->transformations[d(gen)].Transform(v);
        }
        img.SetPixel(v.x() * (img.Width() - 1), v.y() * (img.Height() - 1), Vec3f(0, 0, 0));
    }
}
