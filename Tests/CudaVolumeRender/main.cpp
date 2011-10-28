#include <cutil_inline.h>
#include <iostream>
#include <C3/Window.h>
#include <GL/glew.h>
#include <cuda_gl_interop.h>
#include <sstream>

// CUDA Includes
#include <vector_types.h>
#include <vector_functions.h>
#include <driver_functions.h>

// Pour lancer le kernel
extern "C" {
	void loadInverseViewMatrix(float* dataMatrix, size_t sizeofMatrix);
	void runKernel(unsigned int *d_output, unsigned int imageW, unsigned int imageH, 
			   float density = 0.05f, float brightness = 1.0f, float transferOffset = 0.0f, float transferScale = 1.0f);
	void initCuda(void *h_volume, cudaExtent volumeSize);
    void freeCudaBuffers();
}

// Custom fuctions
void *loadRawFile(const std::string& path, size_t size);


int main()
{
	/////////////////////////////
	// CONSTANTES
	/////////////////////////////
	int width = 800;
	int height = 600;

	cudaExtent volumeSize = make_cudaExtent(32, 32, 32);
	const std::string path = "../Ressources/32fumee4.raw";

	// Creation du device
	cutilSafeCall( cudaGLSetGLDevice( cutGetMaxGflopsDeviceId() ) );

	// Creation d'une fenetre
	C3::Window OpenGLWin;
	OpenGLWin.Create(C3::WindowMode(width,height),"CudaC3");
	
	// Glew init
	GLenum err = glewInit();
	if(err != GLEW_OK)
		std::cout << "Error on GLEW initialization.\n";


	// Configuration OpenGL
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	// Texture creation
	// create texture for display
	GLuint texid;
    glGenTextures(1, &texid);
    glBindTexture(GL_TEXTURE_2D, texid);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

	// PBO Creation
	GLuint pbo;
	// OpenGL
	glGenBuffersARB(1, &pbo);
    glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pbo);
    glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, width*height*sizeof(uchar4), 0, GL_STREAM_DRAW_ARB);
    glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
	// Register
	cudaGraphicsResource *cuda_pbo_resource = NULL;
	cutilSafeCall(cudaGraphicsGLRegisterBuffer(&cuda_pbo_resource, pbo, 
					       cudaGraphicsMapFlagsWriteDiscard));

	// Load Volume
	// *** Read Texture 3D
	size_t size = volumeSize.width*volumeSize.height*volumeSize.depth*sizeof(unsigned char);
    void *h_volume = loadRawFile(path, size);
	// *** init cuda
	initCuda(h_volume,volumeSize);

	// Variables utilisee pour la boucle
	float g_fAnim = 0.f;
	int nbFrame = 0;
	float timeFPS = 0.f;
	float invViewMatrix[12];
	float3 viewRotation;
	float3 viewTranslation = make_float3(0.0, 0.0, -4.0f);
	bool mouseRotationClicked = false;

	// Rendering loop
	while(OpenGLWin.IsOpened())
	{
		// Events
		C3::Event event;
		while(OpenGLWin.PoolEvent(event))
		{
                        //std::cout << "Event !" << std::endl;
			if(event.Type == C3::Event::Closed)
			{
				std::cout << "Close ... " << std::endl;
				OpenGLWin.Close();
			}
			else if(event.Type == C3::Event::KeyPressed)
			{
				if(event.Key.Code == C3::Key::Escape)
				{
					std::cout << "Close ... " << std::endl;
					OpenGLWin.Close();
				}
			}
			else if(event.Type == C3::Event::MouseMoved)
			{
				if(mouseRotationClicked)
				{
					viewRotation.x += event.MouseMove.X / 500.f;
					viewRotation.y += event.MouseMove.Y / 500.0f;
				}
			}
			else if(event.Type == C3::Event::MouseButtonPressed)
			{
				if(event.MouseButton.Button == C3::Mouse::Left)
					mouseRotationClicked = true;
			}
			else if(event.Type == C3::Event::MouseButtonReleased)
			{
				if(event.MouseButton.Button == C3::Mouse::Left)
					mouseRotationClicked = false;
			}
		}
		// Mise a jour du temps
		g_fAnim += OpenGLWin.GetFrameTime() / 1000.f;
		timeFPS += OpenGLWin.GetFrameTime() / 1000.f;
		nbFrame++;
		if(timeFPS > 1.0f)
		{
			std::stringstream ss;
			ss << "CudaC3 [" << (int)ceil( nbFrame / timeFPS ) << " FPS]";
			OpenGLWin.SetTitle(ss.str());
			timeFPS = 0.f;
			nbFrame = 0;
		}
		
		// Draw the scene
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		// Recuperer la camera + inverse
		GLfloat modelView[16];
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			glLoadIdentity();
			glRotatef(-viewRotation.x, 1.0, 0.0, 0.0);
			glRotatef(-viewRotation.y, 0.0, 1.0, 0.0);
			glTranslatef(-viewTranslation.x, -viewTranslation.y, -viewTranslation.z);
		glGetFloatv(GL_MODELVIEW_MATRIX, modelView);
		glPopMatrix();

		invViewMatrix[0] = modelView[0]; invViewMatrix[1] = modelView[4]; invViewMatrix[2] = modelView[8]; invViewMatrix[3] = modelView[12];
		invViewMatrix[4] = modelView[1]; invViewMatrix[5] = modelView[5]; invViewMatrix[6] = modelView[9]; invViewMatrix[7] = modelView[13];
		invViewMatrix[8] = modelView[2]; invViewMatrix[9] = modelView[6]; invViewMatrix[10] = modelView[10]; invViewMatrix[11] = modelView[14];

		// Envoie de la matrix inverse a la carte graphique
		loadInverseViewMatrix(invViewMatrix, sizeof(float4)*3);

		// Lancer le calcul CUDA
		// *** map OpenGL buffer object for writing from CUDA
		unsigned int *d_pbo;
		cutilSafeCall(cudaGraphicsMapResources(1, &cuda_pbo_resource, 0));
		size_t num_bytes; 
		cutilSafeCall(cudaGraphicsResourceGetMappedPointer((void **)&d_pbo, &num_bytes, cuda_pbo_resource));
		// *** Clear buffer
		cutilSafeCall(cudaMemset(d_pbo, 0, width*height*4));
		// *** run kernel
		runKernel(d_pbo, width, height);
		cutilSafeCall( cutilDeviceSynchronize() );
		// *** unmap
		cutilSafeCall(cudaGraphicsUnmapResources(1, &cuda_pbo_resource, 0));
		
		// download image from PBO to OpenGL texture
		glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pbo);
		glBindTexture  (GL_TEXTURE_2D, texid);
		glPixelStorei  (GL_UNPACK_ALIGNMENT, 1);
		glTexSubImage2D(GL_TEXTURE_2D,
			0, 0, 0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, 0);
		glEnable(GL_TEXTURE_2D);

		// Render Tex with quad
		glDisable(GL_DEPTH_TEST);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f          , (GLfloat)1.f);  glVertex2f(-1.0f, -1.0f);
		glTexCoord2f((GLfloat)1.f, (GLfloat)1.f);  glVertex2f(1.0f, -1.0f);
		glTexCoord2f((GLfloat)1.f, 0.0f           );  glVertex2f(1.0f, 1.0f);
		glTexCoord2f(0.0f          , 0.0f           );  glVertex2f(-1.0f, 1.0f);
		glEnd();
		glDisable(GL_TEXTURE_2D);

		glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);

		// Swap buffers
		OpenGLWin.Display();
	}

	// Liberation des ressources
	cudaGraphicsUnregisterResource(cuda_pbo_resource);
	freeCudaBuffers(); // < Liberation sur la carte graphique

	// OpenGL liberation des ressources
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);
	glDeleteBuffers(1, &pbo);
	glBindTexture(GL_TEXTURE_2D, texid);
	glDeleteTextures(1, &texid);


	// Close device
	cutilDeviceReset();

	return 0;
}

void *loadRawFile(const std::string& filename, size_t size)
{
    FILE *fp = fopen(filename.c_str(), "rb");
    if (!fp) {
		std::cout << "Error opening file '" << filename << "'\n";
        return 0;
    }

    void *data = malloc(size);
    size_t read = fread(data, 1, size, fp);
    fclose(fp);

	std::cout << "Read '" << filename << "', " << read << " bytes\n";

    return data;
}
