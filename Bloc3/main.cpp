#include <pthread.h>

#include <iostream>
#include <vector>
#include <string>
#include <GL/gl.h>
#include <GL/freeglut.h>
#include "model/model.h"

#define help std::cout << "|------------------------------------|" << std::endl << "|Aiudame, io tengo muchos quereseres.|" << std::endl << "|Aiudame! porque mi casa esta súsia. |"<< std::endl << "|------------------------------------|" << std::endl << std::endl;

#define HOMER "model/homer.obj"
#define LEGOMAN "model/legoman.obj"
#define PHOMER "model/HomerProves.obj"


enum State {
    rotating, moving, none, qttState
};

enum Showing {
    sPoint, sLine, sFill, qttShowing
};

enum Models {
    homer, legoman, phomer, qttModels
};

struct Caja {
    float minX, maxX, minY, maxY, minZ, maxZ;
    Caja() {}
    Caja(float x,float x2,float y,float y2,float z,float z2){
        minX = x2; maxX = x;
        minY = y2; maxY = y;
        minZ = z2; maxZ = z;
    }
    Caja createBox(Model m) {
        minX = minY = minZ =999999999;
        maxX = maxY = maxZ = -99999999;
        int fSize = m.faces().size();
        for (int i = 0; i < fSize; ++i) {

            const Face &f = m.faces()[i];
            const Vertex *v;
            for (int j = 0; j < 3; ++j) {
                v = &m.vertices()[f.v[j]];

                if (v[0] > maxX) maxX = v[0];
                else if (v[0] < minX) minX = v[0];

                if (v[1] > maxY) maxY = v[1];
                else if (v[1] < minY) minY = v[1];

                if (v[2] > maxZ) maxZ = v[2];
                else if (v[2] < minZ) minZ = v[2];

            }
        }
    }
};

struct Vector3f {
    float x, y, z;
    Vector3f() {}
    Vector3f(float x,float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Vector3f findCenter(Caja b) {
        x = ((b.maxX-b.minX)/2)+b.minX;
        y = ((b.maxY-b.minY)/2)+b.minY;
        z = ((b.maxZ-b.minZ)/2)+b.minZ;
    }
};

struct Muneco {
    Models name;
    Caja box;
    Model modelo;
    Vector3f centro;
    Vector3f tamano;
    Vector3f posFinal;
    Muneco () {}
    Muneco (Models name) {
        changeModel(name);
        posFinal = Vector3f(-0.75,-0.39,0.75);
    }
    void changeModel(Models name) {
        this->name = name;
        if (name == homer) modelo.load(HOMER);
        else if (name == legoman)modelo.load(LEGOMAN);
        else modelo.load(PHOMER);
        box.createBox(modelo);
        centro.findCenter(box);
    }
};

int rWidth = 600;
int rHeight = 600;
int portrait = std::min(rWidth,rHeight);
std::pair<int,int> posMouse;
std::pair<bool,bool> mouse;
float r, g, b, a;
State state;
Showing showing;
char* textState;
char* textShow;
int rotateX,rotateY;
float monigote;
Muneco m;



void changeState(State s) {
    state = s;
    switch (s) {
    case rotating:
       textState = "Girando";
        break;
    case moving:
        textState = "Moviendose";
        break;
    case none:
        textState = "Pausado";
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

void changeShowing(Showing s) {   
    showing = s;
    switch (s) {
    case sPoint:
        textShow = "Points";glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
        break;
    case sLine:
        textShow = "Lines";glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        break;
    case sFill:
        textShow = "Fill";glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        break;
    default:
        break;
    }

    glutPostRedisplay();
}

void initCamara() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1,1,-1,1,-1,1);
    glMatrixMode(GL_MODELVIEW);
}

void init() {
    initCamara();
    m = Muneco(legoman);
    state = rotating;
    textState = "HELLO";
    showing = sFill;
    textShow = "Fill";
    r = g = 1;
    b = a = 0.8;
    monigote = 0.4;
    rotateX = rotateY = 0;
}

void myBodyIsReady(float size) {
    glPushMatrix(); {// Body
        glColor3f(0,1,0);
        //    glTranslated(0.f,0.10f,0.f);
        glutSolidSphere(size,20.f,20.f);
    } glPopMatrix();
    glPushMatrix(); {// Head
        glColor3f(0,0,1);
        glTranslated(0.f,size*1.5,0.f);
        glutSolidSphere(size/2,20.f,20.f);
    } glPopMatrix();
    glPushMatrix(); { // The D
        glColor3f(1,0,0);
        glTranslated(size/4,size*1.5,0.f);
        glRotated(90,0,1,0);
        glutSolidCone(size/4,size/2,20.f,20.f);
    } glPopMatrix();
}

void ejes() {
    glBegin(GL_LINES);glColor3f(1.0f, 0.0f, 0.0f);glVertex3f(0,0,0);glVertex3f(1,0,0);glEnd();
    glBegin(GL_LINES);glColor3f(0.0f, 1.0f, 0.0f);glVertex3f(0,0,0);glVertex3f(0,1,0);glEnd();
    glBegin(GL_LINES);glColor3f(0.0f, 0.0f, 1.0f);glVertex3f(0,0,0);glVertex3f(0,0,1);glEnd();
}

void daModelIsReal(Model mm, float size) {
    float xxx, yyy, zzz;
    float smax = std::max(m.box.maxX-m.box.minX,std::max(m.box.maxY-m.box.minY,m.box.maxZ-m.box.minZ));
    xxx = size*(m.box.maxX-m.box.minX)/smax;
    yyy = size*(m.box.maxY-m.box.minY)/smax;
    zzz = size*(m.box.maxZ-m.box.minZ)/smax;

    m.tamano = Vector3f(xxx,yyy,zzz);
    glScaled(xxx/(m.box.maxX-m.box.minX),yyy/(m.box.maxY-m.box.minY),zzz/(m.box.maxZ-m.box.minZ));
    glTranslated(-m.centro.x, -m.centro.y, -m.centro.z);

    glBegin(GL_TRIANGLES);

    for (int i = 0; i < mm.faces().size(); ++i) {

        const Face &f = mm.faces()[i];
        glColor4fv(Materials[f.mat].diffuse);
        const Vertex *v;
        for (int j = 0; j < 3; ++j) {
            v = &mm.vertices()[f.v[j]];
            glVertex3dv(v);
        }
    }
    glEnd();

}

void drawGround(Vector3f centro, Vector3f plano, float size) {
    float f = size/2;
    glTranslated(centro.x,centro.y,centro.z);
    glBegin(GL_POLYGON);
    if (plano.x == 0) {
        glVertex3f(0,-f,-f);
        glVertex3f(0,-f, f);
        glVertex3f(0, f, f);
        glVertex3f(0, f,-f);
    }
    else if (plano.y == 0) {
        glVertex3f(-f,0,-f);
        glVertex3f( f,0,-f);
        glVertex3f( f,0, f);
        glVertex3f(-f,0, f);
    }
    else {
        glVertex3f(-f,-f,0);
        glVertex3f( f,-f,0);
        glVertex3f( f, f,0);
        glVertex3f(-f, f,0);
    }
    glEnd();
}

void refresh(void) {
    glClearColor(r,g,b,a);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
glPushMatrix();
    glRotated(rotateY,1,0,0);
    glRotated(rotateX,0,1,0);
    glPushMatrix();
        glColor3f(1,1,0);
        drawGround(Vector3f(0,-0.4,0), Vector3f(1,0,1), 1.4);
    glPopMatrix();
    glPushMatrix();
        ejes();
    glPopMatrix();
    glPushMatrix();
        glTranslated(0,monigote-0.4,0);
        myBodyIsReady(monigote);
    glPopMatrix();
    glPushMatrix();
        glTranslated(m.posFinal.x+m.tamano.x/2.f,m.posFinal.y+m.tamano.y/2.f,m.posFinal.z-m.tamano.z/2.f);
        daModelIsReal(m.modelo, 0.5);
    glPopMatrix();
    glPopMatrix();
    glPushMatrix();
        glColor3f(0,0,0);
        glRasterPos2f(-0.9,0.9);
        glutBitmapString(GLUT_BITMAP_HELVETICA_18, textState);
    glPopMatrix();
    glPushMatrix();
        glColor3f(0,0,0);
        glRasterPos2f(0.7,0.9);
        glutBitmapString(GLUT_BITMAP_HELVETICA_18, textShow);
    glPopMatrix();
    glutPostRedisplay();
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
    if (button == 0 and pressed == 0) { mouse.first = true; posMouse.first = x; posMouse.second = y; }
    else if (button == 0 and pressed == 1) mouse.first = false;
}

void mouseMoved(int x, int y) {
    if (mouse.first and state == rotating) {
        if (posMouse.first > x) { rotateX += abs(posMouse.first - x);}
        else if (posMouse.first < x) { rotateX -= abs(posMouse.first - x); }

        if (posMouse.second > y) { rotateY += abs(posMouse.second - y);}
        else if (posMouse.second < y) { rotateY -= abs(posMouse.second - y); }

        posMouse.first = x;
        posMouse.second = y;
        glutPostRedisplay();
    }
    else if (mouse.first and state == moving) {
        m.posFinal.x += float(x-posMouse.first)/rWidth;
        m.posFinal.z += float(y-posMouse.second)/rHeight;


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
        std::cout << "Press T and click three time to declare a new moving" << std::endl;
        std::cout << std::endl;
        break;
    case 'f':
        if (state == rotating) changeState(moving);
        else if (state == moving) changeState(rotating);
        break;
    case 'c':
        if (state == moving) changeState(rotating);
        else if (state == rotating) changeState(moving);
        break;
    case 's':
        changeShowing((showing+1)%qttShowing);
        break;
    case 'a':
        m.changeModel((m.name+1)%qttModels);
        glutPostRedisplay();
        break;
    case 'z':
        monigote += 0.01;
        glutPostRedisplay();
        break;
    case 'x':
        monigote -= 0.01;
        glutPostRedisplay();
        break;
    default:
        break;
    }
}

int main(int argc, char **argv) {
    int basura = pthread_getconcurrency();

    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
    glutInitWindowSize(rHeight,rWidth);
    glutCreateWindow("Bloc 1");
    init();
    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(refresh);
    glutReshapeFunc(resize);
    glutMouseFunc(mouseClicked);
    glutMotionFunc(mouseMoved);
    glutKeyboardFunc(teclado);
    glutMainLoop();
    return 0;
}


