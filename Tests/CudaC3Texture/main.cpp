#include <cutil_inline.h>
#include <iostream>
#include <C3/Window.h>
#include <GL/glew.h>
#include <cuda_gl_interop.h>
#include <sstream>

// Pour lancer le kernel
extern "C" 
void runKernel(uchar4* data, unsigned int width, unsigned int height, float time);


int main()
{
	int width = 800;
	int height = 600;

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

	float g_fAnim = 0.f;
	int nbFrame = 0;
	float timeFPS = 0.f;
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

		// Lancer le calcul CUDA
		// *** map OpenGL buffer object for writing from CUDA
		uchar4 *d_pbo;
		cutilSafeCall(cudaGraphicsMapResources(1, &cuda_pbo_resource, 0));
		size_t num_bytes; 
		cutilSafeCall(cudaGraphicsResourceGetMappedPointer((void **)&d_pbo, &num_bytes, cuda_pbo_resource));
		// *** run kernel
		runKernel(d_pbo, width, height,g_fAnim);
		cutilSafeCall( cutilDeviceSynchronize() );
		// *** unmap
		cutilSafeCall(cudaGraphicsUnmapResources(1, &cuda_pbo_resource, 0));
		
		// download image from PBO to OpenGL texture
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);
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
	
	// OpenGL liberation des ressources
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);
	glDeleteBuffers(1, &pbo);
	glBindTexture(GL_TEXTURE_2D, texid);
	glDeleteTextures(1, &texid);

	// Close device
	cutilDeviceReset();

	return 0;
}
