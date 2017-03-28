#include "stdafx.h"
#include "Model.h"
#include "Engine.h"
#include <sstream>
#include <fstream>


Model::Model()
{
	vertArr = 0;
	vertBuf = 0;
	vertCount = 0;
	amountOfObjects = 0;
	instanced = false;
}


Model::~Model()
{
}

bool Model::Buffer(std::string objFile)
{

	//2.1. Get Locs and UVs from File Data
	std::vector<vec3> locs;
	std::vector<vec2> uvs;
	std::vector<vec3> normals;
	std::vector<VertInd> vertInds;

	int vertNums = 0;
	int uvNums = 0;
	int normNums = 0;
	int faceNums = 0;

	float value1, value2, value3, value4;
	char slash;

	std::fstream modelLoader;

	modelLoader.open(objFile);
	std::string thisLineId;
	char currentString[256];
	bool isFileEmpty = false;

	if (modelLoader.is_open())
	{
		while (!modelLoader.eof())
		{
			modelLoader.getline(currentString, 256);
			std::stringstream ss (currentString);
			if(ss.rdbuf()->in_avail() != 0)ss >> thisLineId;
			
			if (thisLineId == "v")
			{
				vertNums++;
				ss >> value1;
				ss >> value2;
				ss >> value3;

				locs.push_back(vec3(value1, value2, value3));
			}

			if (thisLineId == "vt")
			{
				uvNums++;
				ss >> value1;
				ss >> value2;

				uvs.push_back(vec2(value1, value2));
			}

			if (thisLineId == "vn")
			{
				normNums++;
				ss >> value1;
				ss >> value2;
				ss >> value3;

				normals.push_back(vec3(value1, value2, value3));
			}

			if (thisLineId == "f")
			{
				int fvalue1, fvalue2, fvalue3;
				faceNums++;
				for (int i = 0; i < 3; i++)
				{
					ss >> fvalue1;
					ss >> slash;
					ss >> fvalue2;
					ss >> slash;
					ss >> fvalue3;

					fvalue1--;
					fvalue2--;
					fvalue3--;

					VertInd tmp = VertInd();
					tmp.locInd = fvalue1;
					tmp.normInd = fvalue3;
					tmp.uvInd = fvalue2;
					vertInds.push_back(tmp);
				}
			}
			
		}

		if (modelLoader.is_open()) modelLoader.close();

		std::cout << "Loaded " << vertNums << " vertices, " << uvNums << " UVs, " << normNums << " normals, " << faceNums << " faces." << std::endl;
	}

	//2.2. Connect the dots
	vertCount = vertInds.size();

	//2.3. Duplicate vertices into a single buffer
	std::vector<Vertex> vertBufData(vertCount);
	for (unsigned int i = 0; i < vertCount; i++)
	{
		vertBufData[i].loc = locs[vertInds[i].locInd];
		vertBufData[i].normal = normals[vertInds[i].normInd];
		vertBufData[i].uv = uvs[vertInds[i].uvInd];

	}

	//2.4. Generate a vertex array and vertex buffer
	glGenVertexArrays(1, &vertArr);
	glGenBuffers(1, &vertBuf);

	//2.5. Bind the vertex array/buffer
	glBindVertexArray(vertArr);
	glBindBuffer(GL_ARRAY_BUFFER, vertBuf);

	//2.6. Store data in the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertCount,
		&vertBufData[0], GL_STATIC_DRAW);

	//2.7. Describe the buffer layout
	glEnableVertexAttribArray(0); //attribute index - 0

	glVertexAttribPointer(0,	//attribute index - 0 in this case
		3,						//number of components - x,y,z
		GL_FLOAT,				//type of data
		GL_FALSE,				//should we normalize the data?
		sizeof(Vertex),			//stride (bytes between attributes
		0);						//offset to this attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)sizeof(vec3));

	glEnableVertexAttribArray(2); //attribute index - 2

	glVertexAttribPointer(2,	//attribute index - 0 in this case
		3,						//number of components - x,y,z
		GL_FLOAT,				//type of data
		GL_FALSE,				//should we normalize the data?
		sizeof(Vertex),			//stride (bytes between attributes
		(void*)(sizeof(vec3) + sizeof(vec2)));	//offset to this attribute

	glBindVertexArray(0);
	return true;
}

bool Model::BufferQuad()
{
	std::vector<vec3> locs;
	std::vector<vec2> uvs;
	std::vector<vec3> normals;
	std::vector<VertInd> vertInds;

	//2.1. Defining vertex data
	locs = {
		{ .9, .9, 0 },
		{ -.9, .9, 0 },
		{ -.9, -.9, 0 },
		{ .9, -.9, 0 } };

	uvs = {
		{ 1,1 },
		{ 0,1 },
		{ 0,0 },
		{ 1,0 } };

	//2.2. Connect the dots
	std::vector<unsigned int> locInds =
	{ 0, 1, 2, 0, 2, 3 };
	vertCount = locInds.size();

	//2.3. Duplicate vertices into a single buffer
	std::vector<Vertex> vertBufData(vertCount);
	for (unsigned int i = 0; i < vertCount; i++)
	{
		vertBufData[i].loc = locs[locInds[i]];
		vertBufData[i].uv = uvs[locInds[i]];
	}

	//2.4. Generate a vertex array and vertex buffer
	glGenVertexArrays(1, &vertArr);
	glGenBuffers(1, &vertBuf);

	//2.5. Bind the vertex array/buffer
	glBindVertexArray(vertArr);
	glBindBuffer(GL_ARRAY_BUFFER, vertBuf);

	//2.6. Store data in the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertCount,
		&vertBufData[0], GL_STATIC_DRAW);

	//2.7. Describe the buffer layout
	glEnableVertexAttribArray(0); //attribute index - 0

	glVertexAttribPointer(0,	//attribute index - 0 in this case
		3,						//number of components - x,y,z
		GL_FLOAT,				//type of data
		GL_FALSE,				//should we normalizr the data?
		sizeof(Vertex),			//stride (bytes between attributes
		0);						//offset to this attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)sizeof(vec3));

	//set up transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	return true;
}

void Model::Render()
{
	if (instanced)
	{
		glBindVertexArray(vertArr);
		glDrawArraysInstanced(GL_TRIANGLES, 0, vertCount, amountOfObjects);
		glBindVertexArray(0);
	}
	else
	{
		//bind array and draw
		glBindVertexArray(vertArr);
		glDrawArrays(GL_TRIANGLES, 0, vertCount);
		glBindVertexArray(0);
	}
}

void Model::InstanceModel(const glm::mat4* transformations, const GLuint amount)
{
	amountOfObjects = amount;
	instanced = true;

	// Vertex Buffer Object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &transformations[0], GL_STATIC_DRAW);

	glBindVertexArray(vertArr);

	// Vertex Attributes
	GLsizei vec4Size = sizeof(glm::vec4);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(vec4Size));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(2 * vec4Size));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(3 * vec4Size));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);
}

