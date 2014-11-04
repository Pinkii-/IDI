#include <iostream>
#include <vector>
#include <string>
#include <GL/gl.h>
#include <GL/freeglut.h>

#define help std::cout << "|------------------------------------|" << std::endl << "|Aiudame, io tengo muchos quereseres.|" << std::endl << "|Aiudame! porque mi casa esta súsia. |"<< std::endl << "|------------------------------------|" << std::endl << std::endl;


enum State {
    backgroundColor, triangle, none, qttState
};

int rWidth = 600;
int rHeight = 600;
int portrait = std::min(rWidth,rHeight);
std::pair<int,int> posMouse;
std::pair<bool,bool> mouse;
float r, g, b, a;
State state;
char* textState;
char* fps;
std::vector<std::pair<float, float> > triangulo(3);

void changeState(State s) {
    state = s;
    switch (s) {
    case backgroundColor:
        textState = "Cambiando el color de fondo";
        break;
    case triangle:
        textState = "Eligiendo nuevo triangulo";
        break;
    case none:
        textState = "Pausado";
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

void init() {
    state = none;
    textState = "Pausado";
    r = g = 0.5;
    b = a = 1;
    triangulo[0].first = -1/2.f; triangulo[0].second = -1/3.f;
    triangulo[1].first = 1/2.f; triangulo[1].second = -1/3.f;
    triangulo[2].first = 0; triangulo[2].second = 2/3.f;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1,1,-1,1,-1,1);
    glMatrixMode(GL_MODELVIEW);
}

void refresh(void) {
    glClearColor(r,g,b,a);
    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();
    glRotated(180,0,1,0);
    glRotated(90,0,0,1);
    glutWireTeapot(0.5);
    glPopMatrix();

    glRasterPos2f(-0.9,0.9);
//    glColor3f(0,0,0);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, textState);
    glRasterPos2f(0.9,0.9);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, fps);

    glutSwapBuffers();
}

void resize(int x, int y) {
    rWidth = x;
    rHeight = y;

    float nX, nY;
    if (rWidth > rHeight) portrait= rHeight;
    else portrait = rWidth;

    nX = abs(rWidth-portrait)/2.f;
    nY = abs(rHeight-portrait)/2.f;

    glViewport(nX, nY, portrait, portrait);
}

void mouseClicked(int button, int pressed, int x, int y) {
    if (button == 0 and pressed == 0) {
        if (state == backgroundColor) { mouse.first = true; posMouse.first = x; posMouse.second = y; }


    }
    else if (button == 0 and pressed == 1) mouse.first = false;
}

void mouseMoved(int x, int y) {
    if (mouse.first and state == backgroundColor) {
        if (posMouse.first > x) { r += abs(posMouse.first - x)/float(portrait);}
        else if (posMouse.first < x) { r -= abs(posMouse.first - x)/float(portrait); }

        r < 0 ? r = 0 : r =(r > 1 ? 1 : r);

        if (posMouse.second > y) { g += abs(posMouse.second - y)/float(portrait);}
        else if (posMouse.second < y) { g -= abs(posMouse.second - y)/float(portrait); }

        g < 0 ? g = 0 : g =(g > 1 ? 1 : g);

        posMouse.first = x;
        posMouse.second = y;
        glutPostRedisplay();
    }
}

void teclado(unsigned char c, int x, int y) {
    (void)x;(void)y;
    switch (c) {
    case 27:
        glutExit();
        break;
    case 'h':
        help; //Nesisito aiuda amiho servesa bier
        std::cout << "You can see the state of the program on the up left corner." << std::endl;
        std::cout << "You cant jump from one state to another without passing for \"PAUSADO\"." << std::endl;
        std::cout << "Press F to change the background color" << std::endl;
        std::cout << "Press T and click three time to declare a new triangle" << std::endl;
        std::cout << "Press ESC to exit" << std::endl;
        std::cout << std::endl;
        break;
    case 'f':
        if (state == backgroundColor) changeState(none);
        else if (state == none) changeState(backgroundColor);
        break;
    default:
        break;
    }
}

int main(int argc, char **argv) {
    init();
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE |GLUT_RGBA);
    glutInitWindowSize(rHeight,rWidth);
    glutCreateWindow("Bloc 1");
    glutDisplayFunc(refresh);
    glutReshapeFunc(resize);
    glutMouseFunc(mouseClicked);
    glutMotionFunc(mouseMoved);
    glutKeyboardFunc(teclado);
    glutMainLoop();
    return 0;
}
