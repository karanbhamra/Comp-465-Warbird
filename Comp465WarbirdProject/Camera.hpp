# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif

# define __CAMERA__

class Camera {

private:

    bool shouldUpdateEye = false;
	int camCount = 0;
    //Camera Vectors:
    glm::vec3 eye;
    glm::vec3 at;
    glm::vec3 up;

    char * vertexShaderFile = "simpleVertex.glsl";
    char * fragmentShaderFile = "simpleFragment.glsl";
    GLuint MVP;  // Model View Projection matrix's handle

public:

    GLuint shaderProgram;

    Camera() {
        setupCamera();
        setupShaders();
        setToFront();
    }

    void setupCamera() {
        
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    }

    void setupShaders() {

        // load vertex shader and fragment shader and use them
        shaderProgram = loadShaders(vertexShaderFile, fragmentShaderFile);
        glUseProgram(shaderProgram);
        MVP = glGetUniformLocation(shaderProgram, "ModelViewProjection");

    }

    void setToFront() {
        shouldUpdateEye = false;
        eye = glm::vec3(0.0f, 10000.0f, 20000.0f);   // eye is 2000 "out of screen" from origin
        at = glm::vec3(0.0f, 0.0f, 0.0f);   // looking at origin
        up = glm::vec3(0.0f, 1.0f, 0.0f);   // camera'a up vector
    }

    void setToTop() {
        shouldUpdateEye = false;
        eye = glm::vec3(0.0f, 20000.0f, 0.0f);   // eye is 3000 up from origin
        at = glm::vec3(0.0f, 0.0f, 0.0f);   // looking at origin
        up = glm::vec3(0.0f, 0.0f, -1.0f);   // camera's up is looking towards -Z vector
    }

    void setToBottom() {
        shouldUpdateEye = false;
        eye = glm::vec3(10000.0f, 0.0f, 0.0f);   // eye is 1000 right from origin
        at = glm::vec3(0.0f, 0.0f, 0.0f);   // looking at origin
        up = glm::vec3(0.0f, 1.0f, 0.0f);   // camera'a up vector
    }

    void setUnumCam() {
        shouldUpdateEye = true;
        eye = glm::vec3(0.0f, 0.0f, -8000.0f);   // eye is 1000 right from origin
        at = glm::vec3(0.0f, 0.0f, 0.0f);   // looking at origin
        up = glm::vec3(0.0f, 1.0f, 0.0f);   // camera'a up vector
    }

    void setDuoCam() {
        shouldUpdateEye = true;
        eye = glm::vec3(0.0f, 0.0f, -8000.0f);   // eye is 1000 right from origin
        at = glm::vec3(0.0f, 0.0f, 0.0f);   // looking at origin
        up = glm::vec3(0.0f, 1.0f, 0.0f);   // camera'a up vector
    }
    void handleKeyPress(unsigned char key) {
		int numCams = 4;

        switch (key) {
            case 'f': case 'F':  // front view
                setToFront();
                break;
            case 't': case 'T':  // top view
                setToTop();
                break;
            case 'u': case 'U':
                setUnumCam();
                break;
            case 'd': case 'D':
                setDuoCam();
                break;

        }

    }

	glm::mat4 updateViewMatrix() {
        return glm::lookAt(eye, at, up);
    }

    glm::mat4 updateViewMatrix(glm::vec3 position) {
        return glm::lookAt(eye, position, up);
    }


};  