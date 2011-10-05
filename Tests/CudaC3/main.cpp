#include <cutil_inline.h>
#include <iostream>
#include <C3/Window.h>
#include <GL/glew.h>
#include <cuda_gl_interop.h>
#include <sstream>

// Pour lancer le kernel
extern "C" 
void runKernel(float4* pos, unsigned int mesh_width, unsigned int mesh_height, float time);


int main()
{
	int width = 800;
	int height = 600;

	int mesh_width = 256;
	int mesh_height = 256;

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
    gluPerspective(60.0, (GLfloat)width / (GLfloat) height, 0.1, 10.0);

	// VBO
	// *** Create
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// *** Initialize
	unsigned int size = mesh_width * mesh_height * 4 * sizeof(float);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// *** Register in CUDA
	cudaGraphicsResource *cuda_vbo_resource = NULL;
	cutilSafeCall(cudaGraphicsGLRegisterBuffer(&cuda_vbo_resource, vbo, cudaGraphicsMapFlagsWriteDiscard));


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
		float4 *dptr;
		cutilSafeCall(cudaGraphicsMapResources(1, &cuda_vbo_resource, 0));
		size_t num_bytes; 
		cutilSafeCall(cudaGraphicsResourceGetMappedPointer((void **)&dptr, &num_bytes, cuda_vbo_resource));
		// *** Run kernel
		runKernel(dptr, mesh_width, mesh_height,g_fAnim);
		cutilSafeCall( cutilDeviceSynchronize() );
		// *** Unmap
		cutilSafeCall(cudaGraphicsUnmapResources(1, &cuda_vbo_resource, 0));
		
		// OpenGL
		// *** Make some transformation
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0.0, 0.0, -3.0);
		glRotatef(0.0, 1.0, 0.0, 0.0);
		glRotatef(0.0, 0.0, 1.0, 0.0);
		// *** Render VBO
		// --- Bind
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexPointer(4, GL_FLOAT, 0, 0);
		// --- Draw
		glEnableClientState(GL_VERTEX_ARRAY);
		glColor3f(1.0, 0.0, 0.0);
		glDrawArrays(GL_POINTS, 0, mesh_width * mesh_height);
		glDisableClientState(GL_VERTEX_ARRAY);

		// Swap buffers
		OpenGLWin.Display();
	}

	// Liberation des ressources
	cudaGraphicsUnregisterResource(cuda_vbo_resource);
	
	glBindBuffer(1, vbo);
	glDeleteBuffers(1, &vbo);

	// Close device
	cutilDeviceReset();

	return 0;
}
