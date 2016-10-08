# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif

# define __TOPBAR__

class TopBar {

private:

     //Top Bar Strings
    char baseStr[50] = "Comp 465 Warbird {f, t, u, d, v, x} : ";
    char fpsStr[15], viewStr[15] = " front view";
    char titleStr[100], timerStr[20] = " interval timer";

public:

    TopBar() {
        
    }

    void updateTitle() {
        strcpy(titleStr, baseStr);
        strcat(titleStr, viewStr);
        strcat(titleStr, fpsStr);
        // printf("title string = %s \n", titleStr);
        glutSetWindowTitle(titleStr);
    }

    void displayUpdate(int frameCount, double timeInterval) {

        sprintf(fpsStr, " fps %4d", (int)(frameCount / (timeInterval / 1000.0f)));
        updateTitle();

    }

    void handleKeyPress(unsigned char key) {

        switch (key) {
            case 'f': case 'F':  // front view
                strcpy(viewStr, " front view"); 
                break;
            case 't': case 'T':  // top view
                strcpy(viewStr, " top view"); 
                break;
            case 'u': case 'U':
                strcpy(viewStr, " unum cam"); 
                break;
            case 'd': case 'D':
                strcpy(viewStr, " duo Cam"); 
                break;
        }

    }

};  