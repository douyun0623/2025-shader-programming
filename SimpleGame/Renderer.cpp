#include "stdafx.h"
#include "Renderer.h"

Renderer::Renderer(int windowSizeX, int windowSizeY)
{
	Initialize(windowSizeX, windowSizeY);
}


Renderer::~Renderer()
{
}

void Renderer::Initialize(int windowSizeX, int windowSizeY)
{
	// Set window size
	m_WindowSizeX = windowSizeX;
	m_WindowSizeY = windowSizeY;

	// Compile all shader programs
	CompileAllShaderPrograms();

	// Create VBOs
	CreateVertexBufferObjects();

	// Create Grid Mesh
	CreateGridMesh(500, 500);

	// Create Particles
	GenerateParticles(2500);

	// Create Texture
	m_RGBTexture = CreatePngTexture("./rgb.png", GL_NEAREST);
	m_TextureTwice = CreatePngTexture("./rgb2.png", GL_NEAREST);

	m_0Texture = CreatePngTexture("./0.png", GL_NEAREST);
	m_1Texture = CreatePngTexture("./1.png", GL_NEAREST);
	m_2Texture = CreatePngTexture("./2.png", GL_NEAREST);
	m_3Texture = CreatePngTexture("./3.png", GL_NEAREST);
	m_4Texture = CreatePngTexture("./4.png", GL_NEAREST);
	m_5Texture = CreatePngTexture("./5.png", GL_NEAREST);
	m_6Texture = CreatePngTexture("./6.png", GL_NEAREST);
	m_7Texture = CreatePngTexture("./7.png", GL_NEAREST);
	m_8Texture = CreatePngTexture("./8.png", GL_NEAREST);
	m_9Texture = CreatePngTexture("./9.png", GL_NEAREST);
	m_numTexture = CreatePngTexture("./numbers.png", GL_NEAREST);
	m_ParticleTex = CreatePngTexture("./Particle.png", GL_NEAREST);

	CreateFBOs();

	// Fill Points
	int index = 0;
	for (int i = 0; i < 100; ++i)
	{
		float x = 2 * (float)rand() / (float)RAND_MAX - 1;
		float y = 2 * (float)rand() / (float)RAND_MAX - 1;
		float st = 10 * (float)rand() / (float)RAND_MAX;
		float lt = (float)rand() / (float)RAND_MAX;
		m_Points[index++] = x;
		m_Points[index++] = y;
		m_Points[index++] = st;
		m_Points[index++] = lt;
	}

	if (m_SolidRectShader > 0 && m_VBORect > 0)
	{
		m_Initialized = true;
	}
}

void Renderer::CompileAllShaderPrograms()
{
	//Load shaders
	m_SolidRectShader = CompileShaders(
		"./Shaders/SolidRect.vs",
		"./Shaders/SolidRect.fs");

	m_TestShader = CompileShaders(
		"./Shaders/Test.vs",
		"./Shaders/Test.fs");


	m_ParticleShader = CompileShaders(
		"./Shaders/particle.vs",
		"./Shaders/particle.fs");

	m_GridMeshShader = CompileShaders(
		"./Shaders/GridMesh.vs",
		"./Shaders/GridMesh.fs");

	m_FullScreenShader = CompileShaders(
		"./Shaders/FullScreen.vs",
		"./Shaders/FullScreen.fs");

	m_FSShader = CompileShaders(
		"./Shaders/FS.vs",
		"./Shaders/FS.fs");

	m_TexShader = CompileShaders(
		"./Shaders/Texture.vs",
		"./Shaders/Texture.fs");
}

void Renderer::DeleteAllShaderPrograms()
{
	glDeleteProgram(m_SolidRectShader);
	glDeleteProgram(m_TestShader);
	glDeleteProgram(m_ParticleShader);
	glDeleteProgram(m_GridMeshShader);
	glDeleteProgram(m_FullScreenShader);
	glDeleteProgram(m_FSShader);
	glDeleteProgram(m_TexShader);

	m_SolidRectShader = 0;
	m_TestShader = 0;
	m_ParticleShader = 0;
	m_GridMeshShader = 0;
	m_FullScreenShader = 0;
	m_FSShader = 0;
	m_TexShader = 0;
}

bool Renderer::IsInitialized()
{
	return m_Initialized;
}

void Renderer::ReloadAllShaderPrograms()
{
	DeleteAllShaderPrograms();
	CompileAllShaderPrograms();
}

void Renderer::CreateVertexBufferObjects()
{
	float rect[]
		=
	{
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, -1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, //Triangle1
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f,  1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, //Triangle2
	};

	glGenBuffers(1, &m_VBORect);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);
	
	// lecture2


	float temp = 0.5f;
	float size = 0.1f;
	float testPos[]
		=
	{
		(0.f - temp)* size, (0.f - temp)* size, 0.f, 0.5,
		(1.f - temp)* size, (0.f - temp)* size, 0.f, 0.5,
		(1.f - temp)* size, (1.f - temp)* size, 0.f, 0.5,
		(0.f - temp)* size, (0.f - temp)* size, 0.f, 0.5,
		(1.f - temp)* size, (1.f - temp)* size, 1.f, 0.5,
		(0.f - temp)* size, (1.f - temp)* size, 0.f, 0.5,	// Quard1

		(0.f - temp)* size, (0.f - temp)* size, 0.f, 1.f,
		(1.f - temp)* size, (0.f - temp)* size, 0.f, 1.f,
		(1.f - temp)* size, (1.f - temp)* size, 0.f, 1.f,
		(0.f - temp)* size, (0.f - temp)* size, 0.f, 1.f,
		(1.f - temp)* size, (1.f - temp)* size, 1.f, 1.f,
		(0.f - temp)* size, (1.f - temp)* size, 0.f, 1.f,	// Quard2
	};

	glGenBuffers(1, &m_VBOtestPos);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOtestPos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(testPos), testPos, GL_STATIC_DRAW);


	float testColor[]
		=
	{
		1.f, 0.f, 0.f, 1.f,
		0.f, 1.f, 0.f, 1.f,
		0.f, 0.f, 1.f, 1.f,	//Triangle1
		1.f, 0.f, 0.f, 1.f,
		0.f, 1.f, 0.f, 1.f,
		0.f, 0.f, 1.f, 1.f,	//Triangle2

		1.f, 0.f, 0.f, 1.f,
		0.f, 1.f, 0.f, 1.f,
		0.f, 0.f, 1.f, 1.f,	//Triangle1
		1.f, 0.f, 0.f, 1.f,
		0.f, 1.f, 0.f, 1.f,
		0.f, 0.f, 1.f, 1.f	//Triangle2
	};

	glGenBuffers(1, &m_VBOtestColor);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOtestColor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(testColor), testColor, GL_STATIC_DRAW);

	float fullRect[]
		=
	{
		-1.f , -1.f , 0.f, -1.f , 1.f , 0.f, 1.f , 1.f , 0.f,
		-1.f , -1.f , 0.f,  1.f , 1.f , 0.f, 1.f , -1.f , 0.f,
	};

	glGenBuffers(1, &m_VBOFullScreen);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOFullScreen);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fullRect), fullRect, GL_STATIC_DRAW);


	float FS[]
		=
	{
		-1.f , -1.f , 0.f, -1.f , 1.f , 0.f, 1.f , 1.f , 0.f,
		-1.f , -1.f , 0.f,  1.f , 1.f , 0.f, 1.f , -1.f , 0.f,
	};

	glGenBuffers(1, &m_VBOFS);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOFS);
	glBufferData(GL_ARRAY_BUFFER, sizeof(FS), FS, GL_STATIC_DRAW);


	// m_TexVBO
	float TexRect[]
		=
	{
		// Triangle A
		-1.f, -1.f, 0.f,  0.f, 1.f,   // bottom -> v = 1 (flipped)
		-1.f,  1.f, 0.f,  0.f, 0.f,   // top    -> v = 0
		 1.f,  1.f, 0.f,  1.f, 0.f,   // top    -> v = 0
		 // Triangle B
		 -1.f, -1.f, 0.f,  0.f, 1.f,
		  1.f,  1.f, 0.f,  1.f, 0.f,
		  1.f, -1.f, 0.f,  1.f, 1.f
	};

	glGenBuffers(1, &m_TexVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_TexVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TexRect), TexRect, GL_STATIC_DRAW);
}

void Renderer::CreateGridMesh(int x, int y)
{
	float basePosX = -1.0f;
	float basePosY = -1.0f;
	float targetPosX = 1.0f;
	float targetPosY = 1.0f;

	int pointCountX = x;
	int pointCountY = y;

	float width = targetPosX - basePosX;
	float height = targetPosY - basePosY;

	float* point = new float[pointCountX * pointCountY * 2];
	float* vertices = new float[(pointCountX - 1) * (pointCountY - 1) * 2 * 3 * 3];

	m_GridMeshVertexCount = (pointCountX - 1) * (pointCountY - 1) * 2 * 3;

	//Prepare points
	for (int x = 0; x < pointCountX; x++)
	{
		for (int y = 0; y < pointCountY; y++)
		{
			point[(y * pointCountX + x) * 2 + 0] = basePosX + width * (x / (float)(pointCountX - 1));
			point[(y * pointCountX + x) * 2 + 1] = basePosY + height * (y / (float)(pointCountY - 1));
		}
	}


	//Make triangles
	int vertIndex = 0;
	for (int x = 0; x < pointCountX - 1; x++)
	{
		for (int y = 0; y < pointCountY - 1; y++)
		{
			//Triangle part 1
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;

			//Triangle part 2
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;

		}

	}



	glGenBuffers(1, &m_GridMeshVBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_GridMeshVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (pointCountX - 1) * (pointCountY - 1) * 2 * 3 * 3, vertices, GL_STATIC_DRAW);


	delete[] point;
	delete[]vertices;
}

void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//쉐이더 오브젝트 생성
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];

	size_t slen = strlen(pShaderText);
	if (slen > INT_MAX) {
		// Handle error
	}
	GLint len = (GLint)slen;

	Lengths[0] = len;
	//쉐이더 코드를 쉐이더 오브젝트에 할당
	glShaderSource(ShaderObj, 1, p, Lengths);

	//할당된 쉐이더 코드를 컴파일
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj 가 성공적으로 컴파일 되었는지 확인
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL 의 shader log 데이터를 가져옴
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram 에 attach!!
	glAttachShader(ShaderProgram, ShaderObj);
	glDeleteShader(ShaderObj);
}

bool Renderer::ReadFile(char* filename, std::string *target)
{
	std::ifstream file(filename);
	if (file.fail())
	{
		std::cout << filename << " file loading failed.. \n";
		file.close();
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		target->append(line.c_str());
		target->append("\n");
	}
	return true;
}

GLuint Renderer::CompileShaders(char* filenameVS, char* filenameFS)
{
	GLuint ShaderProgram = glCreateProgram(); //빈 쉐이더 프로그램 생성

	if (ShaderProgram == 0) { //쉐이더 프로그램이 만들어졌는지 확인
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs 가 vs 안으로 로딩됨
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs 가 fs 안으로 로딩됨
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram 에 vs.c_str() 버텍스 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram 에 fs.c_str() 프레그먼트 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach 완료된 shaderProgram 을 링킹함
	glLinkProgram(ShaderProgram);

	//링크가 성공했는지 확인
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program 로그를 받아옴
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error linking shader program\n" << ErrorLog;
		return -1;
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error validating shader program\n" << ErrorLog;
		return -1;
	}

	glUseProgram(ShaderProgram);
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done.";

	return ShaderProgram;
}

void Renderer::DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a)
{
	float newX, newY;

	GetGLPosition(x, y, &newX, &newY);

	//Program select
	glUseProgram(m_SolidRectShader);

	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Trans"), newX, newY, 0, size);
	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Color"), r, g, b, a);

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawGridMesh()
{
	m_time += 0.0016f;

	//Program select
	int shader = m_GridMeshShader;
	glUseProgram(shader);

	int uTimeLoc = glGetUniformLocation(shader,
		"u_Time");
	glUniform1f(uTimeLoc, m_time);

	int uPointsLoc = glGetUniformLocation(shader,
		"u_Points");
	glUniform4fv(uPointsLoc, 100, m_Points);

	int attribPosition = glGetAttribLocation(
		shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);

	int uTextureLoc = glGetUniformLocation(shader, "u_Texture");
	glUniform1i(uTextureLoc, 0);

	glBindTexture(GL_TEXTURE_2D, m_TextureTwice);

	glBindBuffer(GL_ARRAY_BUFFER, m_GridMeshVBO);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, m_GridMeshVertexCount);
	// glDrawArrays(GL_LINE_STRIP, 0, m_GridMeshVertexCount);


	glDisableVertexAttribArray(attribPosition);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawFullScreenColor(float r, float g, float b, float a)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int shader = m_FullScreenShader;

	//Program select
	glUseProgram(shader);

	glUniform4f(glGetUniformLocation(shader, "u_Color"), r, g, b, a);

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOFullScreen);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, 
		sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDisable(GL_BLEND);
}

void Renderer::DrawFS()
{
	m_time += 0.00016f;

	int shader = m_FSShader;

	//Program select
	glUseProgram(shader);

	GLenum DrawBuffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, DrawBuffers);

	int uTimeLoc = glGetUniformLocation(shader,"u_Time");
	glUniform1f(uTimeLoc, m_time);

	int uTextureLoc = glGetUniformLocation(shader, "u_RGBTexture");
	glUniform1i(uTextureLoc, 11);

	int uDigitTextureLoc = glGetUniformLocation(shader, "u_DigitTexture");
	glUniform1i(uDigitTextureLoc, (int)floor(m_time)%10);

	int uNumTextureLoc = glGetUniformLocation(shader, "u_DigitTexture");
	glUniform1i(uNumTextureLoc, 10);


	glBindTexture(GL_TEXTURE_2D, m_TextureTwice);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_0Texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_1Texture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_2Texture);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_3Texture);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_4Texture);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, m_5Texture);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, m_6Texture);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, m_7Texture);
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, m_8Texture);
	glActiveTexture(GL_TEXTURE9);
	glBindTexture(GL_TEXTURE_2D, m_9Texture);
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, m_numTexture);

	glActiveTexture(GL_TEXTURE15);
	glBindTexture(GL_TEXTURE_2D, m_RGBTexture);
	glActiveTexture(GL_TEXTURE16);


	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOFS);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE,
		sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDisable(GL_BLEND);
}

void Renderer::DrawTexture(float x, float y, float sx, float sy, GLuint TexID, GLuint TexID1,
	GLuint method)
{
	// sx, sy --- 비율로 받냐, 절대 크기로 받냐... 선택임 ratio 로 받는걸로 하자

	int shader = m_TexShader;

	//Program select
	glUseProgram(shader);

	int uTex = glGetUniformLocation(shader, "u_TexID");
	glUniform1i(uTex, 0);
	int uTex1 = glGetUniformLocation(shader, "u_TexID1");
	glUniform1i(uTex1, 1);
	int uSize = glGetUniformLocation(shader, "u_Size");
	glUniform2f(uSize, sx, sy);
	int uTran = glGetUniformLocation(shader, "u_Tran");
	glUniform2f(uTran, x, y);
	int uTime = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uTime, m_time);
	int uMethod = glGetUniformLocation(shader, "u_Method");
	glUniform1i(uMethod, method);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TexID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TexID1);

	int aPosition = glGetAttribLocation(shader, "a_Position");
	int aTex = glGetAttribLocation(shader, "a_Tex");
	glEnableVertexAttribArray(aPosition);
	glEnableVertexAttribArray(aTex);

	glBindBuffer(GL_ARRAY_BUFFER, m_TexVBO);
	glVertexAttribPointer(aPosition, 3, GL_FLOAT, GL_FALSE,
		sizeof(float) * 5, 0);
	glVertexAttribPointer(aTex, 2, GL_FLOAT, GL_FALSE,
		sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(aPosition);
	glDisableVertexAttribArray(aTex);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawDebugTextures()
{
	DrawTexture(-0.5, -0.5, 0.5, 0.5, m_HDRRT0_1, 0, 0);
	DrawTexture( 0.5, -0.5, 0.5, 0.5, m_PingpongTexture[0], 0, 0);
}

void Renderer::DrawFBOs()
{
	//// Set FBO
	//glBindFramebuffer(GL_FRAMEBUFFER, m_FBO0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glViewport(0, 0, 512, 512);
	//// Draw 
	//DrawFS();


	// Set FBO
	glBindFramebuffer(GL_FRAMEBUFFER, m_HDRFBO0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, 512, 512);
	// Draw 
	DrawParticle();


	// RestoreFBO
	glViewport(0, 0, 512, 512);
	glBindFramebuffer(GL_FRAMEBUFFER, 0); //초기화 코드
}

void Renderer::DrawBloomParicle()
{
	// 1. Render particle effect(HDR texture)
	glBindFramebuffer(GL_FRAMEBUFFER, m_HDRFBO0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, 512, 512);
	// Draw 
	DrawParticle();

	// 2. Blur 
	glBindFramebuffer(GL_FRAMEBUFFER, m_PingpongFBO[0]);
	DrawTexture(0, 0, 1, 1, m_HDRRT0_1, 0, 1);

	for (int i = 0; i < 20; ++i) {
		glBindFramebuffer(GL_FRAMEBUFFER, m_PingpongFBO[1]);
		DrawTexture(0, 0, 1, 1, m_PingpongTexture[0], 0, 2);
		glBindFramebuffer(GL_FRAMEBUFFER, m_PingpongFBO[0]);
		DrawTexture(0, 0, 1, 1, m_PingpongTexture[1], 0, 1);
	}

	// RestoreFBO
	glViewport(0, 0, 512, 512);
	glBindFramebuffer(GL_FRAMEBUFFER, 0); //초기화 코드

	// 3. Normal Texture + Bloom Texture 
	DrawTexture(0, 0, 1, 1, m_HDRRT0_0, m_PingpongTexture[0], 3);
}

void Renderer::DrawTest()
{
	m_time += 0.00016f;

	//Program select
	glUseProgram(m_TestShader);

	int uTimeLoc = glGetUniformLocation(m_TestShader,
		"u_Time");
	glUniform1f(uTimeLoc, m_time);



	int aPosLoc = glGetAttribLocation(m_TestShader, "a_Position");
	int aValueLoc = glGetAttribLocation(m_TestShader, "a_Value");
	int aColorLoc = glGetAttribLocation(m_TestShader, "a_Color");

	glEnableVertexAttribArray(aPosLoc);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOtestPos);
	glVertexAttribPointer(aPosLoc, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);

	glEnableVertexAttribArray(aValueLoc);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOtestPos);
	glVertexAttribPointer(aValueLoc, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (GLvoid*)(sizeof(float)*3));

	glEnableVertexAttribArray(aColorLoc);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOtestColor);
	glVertexAttribPointer(aColorLoc, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);


	glDrawArrays(GL_TRIANGLES, 0, 12);

	glDisableVertexAttribArray(aPosLoc);
	glDisableVertexAttribArray(aValueLoc);
	glDisableVertexAttribArray(aColorLoc);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawParticle()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_time += 0.0016f;

	//Program select
	GLuint shader = m_ParticleShader;
	glUseProgram(shader);

	GLenum DrawBuffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, DrawBuffers);

	int uTimeLoc = glGetUniformLocation(shader,
		"u_Time");
	glUniform1f(uTimeLoc, m_time);

	int uForceLoc = glGetUniformLocation(shader,
		"u_Force");
	glUniform3f(uForceLoc, std::sin(m_time), 0, 0);

	int uTexLoc = glGetUniformLocation(shader,
		"u_Texture");
	glUniform1i(uTexLoc, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_ParticleTex);
	

	int aPosLoc = glGetAttribLocation(shader, "a_Position");
	int aValueLoc = glGetAttribLocation(shader, "a_Value");
	int aColorLoc = glGetAttribLocation(shader, "a_Color");
	int aSTimeLoc = glGetAttribLocation(shader, "a_STime");
	int aVelLoc = glGetAttribLocation(shader, "a_Vel");
	int aLifeTimeLoc = glGetAttribLocation(shader, "a_LifeTime");
	int aMassLoc = glGetAttribLocation(shader, "a_Mass");
	int aPeriodLoc = glGetAttribLocation(shader, "a_Period");
	int aTexLoc = glGetAttribLocation(shader, "a_Tex");

	int stride = 17;


	glBindBuffer(GL_ARRAY_BUFFER, m_VBOParticle);
	glEnableVertexAttribArray(aPosLoc);
	glEnableVertexAttribArray(aValueLoc);
	glEnableVertexAttribArray(aColorLoc);
	glEnableVertexAttribArray(aSTimeLoc);
	glEnableVertexAttribArray(aVelLoc);
	glEnableVertexAttribArray(aLifeTimeLoc);
	glEnableVertexAttribArray(aMassLoc);
	glEnableVertexAttribArray(aPeriodLoc);
	glEnableVertexAttribArray(aTexLoc);
	
	glVertexAttribPointer(aPosLoc, 3, GL_FLOAT, GL_FALSE, sizeof(float) * stride, 0);
	glVertexAttribPointer(aValueLoc, 1, GL_FLOAT, GL_FALSE, sizeof(float) * stride, (GLvoid*)(sizeof(float) * 3));
	glVertexAttribPointer(aColorLoc, 4, GL_FLOAT, GL_FALSE, sizeof(float) * stride, (GLvoid*)(sizeof(float) * 4));
	glVertexAttribPointer(aSTimeLoc, 1, GL_FLOAT, GL_FALSE, sizeof(float) * stride, (GLvoid*)(sizeof(float) * 8));
	glVertexAttribPointer(aVelLoc, 3, GL_FLOAT, GL_FALSE, sizeof(float) * stride, (GLvoid*)(sizeof(float) * 9));
	glVertexAttribPointer(aLifeTimeLoc, 1, GL_FLOAT, GL_FALSE, sizeof(float) * stride, (GLvoid*)(sizeof(float) * 12));
	glVertexAttribPointer(aMassLoc, 1, GL_FLOAT, GL_FALSE, sizeof(float) * stride, (GLvoid*)(sizeof(float) * 13));
	glVertexAttribPointer(aPeriodLoc, 1, GL_FLOAT, GL_FALSE, sizeof(float) * stride, (GLvoid*)(sizeof(float) * 14));
	glVertexAttribPointer(aTexLoc, 2, GL_FLOAT, GL_FALSE, sizeof(float) * stride, (GLvoid*)(sizeof(float) * 15));

	

	glDrawArrays(GL_TRIANGLES, 0, m_VBOParticleVertexCount);	// m_VBOParticleVertexCount

	glDisableVertexAttribArray(aPosLoc);
	glDisableVertexAttribArray(aValueLoc);
	glDisableVertexAttribArray(aColorLoc);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDisable(GL_BLEND);
}

void Renderer::GetGLPosition(float x, float y, float *newX, float *newY)
{
	*newX = x * 2.f / m_WindowSizeX;
	*newY = y * 2.f / m_WindowSizeY;
}

void Renderer::GenerateParticles(int numParticle)
{
	int floatCountPervertex = 3 + 1 + 4 + 1 + 3 + 1 + 1 + 1 + 2; // x, y, z, value, r, g, b, a, sTime, vx, vy, vz, lifeTime, mass, period
	int verticesCountPerParticle = 6;
	int floatCountPerParticle = floatCountPervertex * verticesCountPerParticle;
	int totalVerticesCount = numParticle * verticesCountPerParticle;
	int totalFloatCount = floatCountPervertex * totalVerticesCount;

	float* vertices = new float[totalFloatCount];

	for (int i = 0; i < numParticle; ++i) {
		float x, y, z, value, r, g, b, a, lifeTime;
		/*x = (float)rand() / (float)RAND_MAX * 2.f - 1.f;
		y = (float)rand() / (float)RAND_MAX * 2.f - 1.f;*/
		x = 0.f;
		y = 0.f;
		z = 0.f;
		value = (float)rand() / (float)RAND_MAX;
		r =  (float)rand() / (float)RAND_MAX;
		g =  (float)rand() / (float)RAND_MAX;
		b =  (float)rand() / (float)RAND_MAX;
		a =  (float)rand() / (float)RAND_MAX;
		float size;
		size = ((float)rand() / (float)RAND_MAX) * 0.05f;
		float sTime = ((float)rand() / (float)RAND_MAX) * 2.f;
		float vx, vy, vz;
		vx = 0; ((float)rand() / (float)RAND_MAX * 2.f - 1.f) * 1.0f;
		vy = 0; (((float)rand() / (float)RAND_MAX * 2.f - 1.f) + 1.5f) * 0.8f;
		vz = 0.f;
		lifeTime = (float)rand() / (float)RAND_MAX * 1.f;
		float mass = (float)rand() / (float)RAND_MAX + 1.f;
		float period = (float)rand() / (float)RAND_MAX + 1.f;

		int index = i * floatCountPervertex * verticesCountPerParticle;	

		vertices[index++] = x - size;		// v1
		vertices[index++] = y - size;
		vertices[index++] = z;
		vertices[index++] = value;
		vertices[index++] = r;
		vertices[index++] = g;
		vertices[index++] = b;
		vertices[index++] = a;
		vertices[index++] = sTime;
		vertices[index++] = vx;		
		vertices[index++] = vy;
		vertices[index++] = vz;
		vertices[index++] = lifeTime;
		vertices[index++] = mass;
		vertices[index++] = period;
		vertices[index++] = 0;
		vertices[index++] = 0;
		

		vertices[index++] = x + size;		// v2
		vertices[index++] = y - size;
		vertices[index++] = z;
		vertices[index++] = value;
		vertices[index++] = r;
		vertices[index++] = g;
		vertices[index++] = b;
		vertices[index++] = a;
		vertices[index++] = sTime;
		vertices[index++] = vx;
		vertices[index++] = vy;
		vertices[index++] = vz;
		vertices[index++] = lifeTime;
		vertices[index++] = mass;
		vertices[index++] = period;
		vertices[index++] = 1;
		vertices[index++] = 0;

		vertices[index++] = x + size;		// v3
		vertices[index++] = y + size;
		vertices[index++] = z;
		vertices[index++] = value;
		vertices[index++] = r;
		vertices[index++] = g;
		vertices[index++] = b;
		vertices[index++] = a;
		vertices[index++] = sTime;
		vertices[index++] = vx;
		vertices[index++] = vy;
		vertices[index++] = vz;
		vertices[index++] = lifeTime;
		vertices[index++] = mass;
		vertices[index++] = period;
		vertices[index++] = 1;
		vertices[index++] = 1;

		vertices[index++] = x - size;		// v4
		vertices[index++] = y + size;
		vertices[index++] = z;
		vertices[index++] = value;
		vertices[index++] = r;
		vertices[index++] = g;
		vertices[index++] = b;
		vertices[index++] = a;
		vertices[index++] = sTime;
		vertices[index++] = vx;
		vertices[index++] = vy;
		vertices[index++] = vz;
		vertices[index++] = lifeTime;
		vertices[index++] = mass;
		vertices[index++] = period;
		vertices[index++] = 0;
		vertices[index++] = 1;

		vertices[index++] = x - size;		// v5
		vertices[index++] = y - size;
		vertices[index++] = z;
		vertices[index++] = value;
		vertices[index++] = r;
		vertices[index++] = g;
		vertices[index++] = b;
		vertices[index++] = a;
		vertices[index++] = sTime;
		vertices[index++] = vx;
		vertices[index++] = vy;
		vertices[index++] = vz;
		vertices[index++] = lifeTime;
		vertices[index++] = mass;
		vertices[index++] = period;
		vertices[index++] = 0;
		vertices[index++] = 0;

		vertices[index++] = x + size;		// v6
		vertices[index++] = y + size;
		vertices[index++] = z;
		vertices[index++] = value;
		vertices[index++] = r;
		vertices[index++] = g;
		vertices[index++] = b;
		vertices[index++] = a;
		vertices[index++] = sTime;
		vertices[index++] = vx;
		vertices[index++] = vy;
		vertices[index++] = vz;
		vertices[index++] = lifeTime;
		vertices[index++] = mass;
		vertices[index++] = period;
		vertices[index++] = 1;
		vertices[index++] = 1;
	}

	glGenBuffers(1, &m_VBOParticle);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOParticle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * totalFloatCount, vertices, GL_STATIC_DRAW);

	delete [] vertices;

	m_VBOParticleVertexCount = totalVerticesCount;
}

GLuint Renderer::CreatePngTexture(char* filePath, GLuint samplingMethod)
{
	//Load Png
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, filePath);
	if (error != 0)
	{
		std::cout << "PNG image loading failed:" << filePath << std::endl;
		assert(0);
	}

	GLuint temp;
	glGenTextures(1, &temp);
	glBindTexture(GL_TEXTURE_2D, temp);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, &image[0]);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, samplingMethod);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, samplingMethod);

	return temp;
}

void Renderer::CreateFBOs()
{ 
	// Gen Color Buffer
	GLuint textureId; glGenTextures(1, &m_RT0_0);
	glBindTexture(GL_TEXTURE_2D, m_RT0_0);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	// Gen Color Buffer
	textureId; glGenTextures(1, &m_RT0_1);
	glBindTexture(GL_TEXTURE_2D, m_RT0_1);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	// Gen Depth Buffer
	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 512, 512);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	// 랜더 버퍼랑 댑스 버퍼의 차이?

	// Gen FBO
	glGenFramebuffers(1, &m_FBO0);

	// Attach to FBO 
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
		GL_TEXTURE_2D, m_RT0_0, 0);	// GL_COLOR_ATTACHMENT0 0 => location 0와 연관
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
		GL_TEXTURE_2D, m_RT0_1, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER, depthBuffer);


	// Check
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		assert(0);
	}



	// Gen Color Buffer
	glGenTextures(1, &m_RT1_0);
	glBindTexture(GL_TEXTURE_2D, m_RT1_0);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glGenTextures(1, &m_RT1_1);
	glBindTexture(GL_TEXTURE_2D, m_RT1_1);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	// Gen Depth Buffer
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 512, 512);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	// 랜더 버퍼랑 댑스 버퍼의 차이?

	// Gen FBO
	glGenFramebuffers(1, &m_FBO1);

	// Attach to FBO 
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO1);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_RT1_0, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_RT1_1, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);


	// Check
	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		assert(0);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);



	// Gen Color Buffer
	textureId; glGenTextures(1, &m_HDRRT0_0);
	glBindTexture(GL_TEXTURE_2D, m_HDRRT0_0);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	// Gen Color Buffer
	textureId; glGenTextures(1, &m_HDRRT0_1);
	glBindTexture(GL_TEXTURE_2D, m_HDRRT0_1);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	// Gen Depth Buffer
	depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 512, 512);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	// 랜더 버퍼랑 댑스 버퍼의 차이?

	// Gen FBO
	glGenFramebuffers(1, &m_HDRFBO0);

	// Attach to FBO 
	glBindFramebuffer(GL_FRAMEBUFFER, m_HDRFBO0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, m_HDRRT0_0, 0);	// GL_COLOR_ATTACHMENT0 0 => location 0와 연관
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
		GL_TEXTURE_2D, m_HDRRT0_1, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER, depthBuffer);


	// Check
	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		assert(0);
	}


	glGenFramebuffers(2, m_PingpongFBO);
	glGenTextures(2, m_PingpongTexture);
	for (int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_PingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, m_PingpongTexture[i]);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGBA16F, 512, 512, 0, GL_RGBA, GL_FLOAT, NULL
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_PingpongTexture[i], 0);
		status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "fbo creation failed" << std::endl;
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}


