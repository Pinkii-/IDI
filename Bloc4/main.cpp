#include <pthread.h>

#include <iostream>
#include <vector>
#include <string>
#include <GL/gl.h>
#include <GL/freeglut.h>
#include "model/model.h"
#include <math.h>
#include <stdio.h>

#define help std::cout << "|------------------------------------|" << std::endl << "|Aiudame, io tengo muchos quereseres.|" << std::endl << "|Aiudame! porque mi casa esta súsia. |"<< std::endl << "|------------------------------------|" << std::endl << std::endl;

#define HOMER "model/homer.obj"
#define LEGOMAN "model/legoman.obj"
#define PHOMER "model/HomerProves.obj"
#define PATRICIO "model/Patricio.obj"
#define KPATRICIO "model/patricioKiller.obj"


enum Showing {
    sPoint, sLine, sFill, qttShowing
};

enum Models {
    homer, legoman, phomer, patricio, kpatricio, qttModels
};

enum CameraType {
    Ortho, Perspective, Fps ,qttCameraType
};

std::vector<Model> modelos;
std::vector<bool> loadedModels;

Model *loadModel(Models m) {
    if (not loadedModels[m]) {
        if (m == homer) modelos[m].load(HOMER);
        else if (m == legoman) modelos[m].load(LEGOMAN);
        else if (m == phomer)modelos[m].load(PHOMER);
        else if (m == patricio) modelos[m].load(PATRICIO);
        else if (m == kpatricio) modelos[m].load(KPATRICIO);
        loadedModels[m] = true;
    }
    return &modelos[m];
}

struct Color {
    GLfloat col[4];
    float r,g,b,a;
    Color(float r, float g, float b, float a) {
        col[0] = r;
        col[1] = g;
        col[2] = b;
        col[3] = a;
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
};

Vector3f operator + (Vector3f a , Vector3f b){Vector3f aux;aux.x = a.x + b.x;aux.y = a.y + b.y;aux.z = a.z + b.z;return aux;}
Vector3f operator - (Vector3f a , Vector3f b){Vector3f aux;aux.x = a.x - b.x;aux.y = a.y - b.y;aux.z = a.z - b.z;return aux;}
Vector3f operator * (Vector3f a , float b){Vector3f aux;aux.x = a.x * b;aux.y = a.y * b;aux.z = a.z * b;return aux;}

struct Caja {
    float minX, maxX, minY, maxY, minZ, maxZ;
    Caja() {}
    Caja(float x,float x2,float y,float y2,float z,float z2){
        minX = x2; maxX = x;
        minY = y2; maxY = y;
        minZ = z2; maxZ = z;
    }
    void createBox(Model m) {
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
    Vector3f findCenter() {
        Vector3f centro;
        centro.x = ((maxX-minX)/2)+minX;
        centro.y = ((maxY-minY)/2)+minY;
        centro.z = ((maxZ-minZ)/2)+minZ;
        return centro;
    }
};


struct Esfera {
    Vector3f centro;
    float radio;
};

void setMaterial(GLfloat* ambient,GLfloat* diffuse,GLfloat* specular,float shininess);

struct Bala {
    Vector3f pos;
    float dir;
    float s;
    Bala() {}
    ~Bala() {}
    Bala(Vector3f posIni,float dir) {
        this->dir = dir;
        pos = posIni;
        s = 5;
    }
    void update(float deltaTime) {
        pos = pos + Vector3f(sin(this->dir*M_PI/180),0,cos(this->dir*M_PI/180))*s*deltaTime;
    }
    void draw() {
        GLfloat c[4] = {0.8,0.8,0.8,0};
        GLfloat WHITE[4] = {1,1,1,1};
        setMaterial(c,c,WHITE,80);
        glutSolidCone(0.08,0.5,10,10);
    }
};

struct Muneco {
    Models name;
    Caja box;
    Model *modelo;
    Vector3f centro;
    Vector3f tamano;
    Vector3f posFinal;
    float scalated;
    Muneco () {}
    Muneco (Models name,float s,Vector3f pos) {
        scalated = s;
        changeModel(name);
        posFinal = Vector3f(pos.x,pos.y+tamano.y/2,pos.z);
    }
    void changeModel(Models name) {
        this->name = name;
        modelo = loadModel(name);
        box.createBox(*modelo);
        centro = box.findCenter();
        size();
    }
    void size() {
        float smax = std::max(box.maxX-box.minX,std::max(box.maxY-box.minY,box.maxZ-box.minZ));
        float xxx, yyy, zzz;
        xxx = scalated*(box.maxX-box.minX)/smax;
        yyy = scalated*(box.maxY-box.minY)/smax;
        zzz = scalated*(box.maxZ-box.minZ)/smax;
        tamano = Vector3f(xxx,yyy,zzz);
    }
};


int rWidth = 600;
int rHeight = 600;
int portrait = std::min(rWidth,rHeight);
std::pair<int,int> posMouse;
std::pair<bool,bool> mouse;
float r, g, b, a;
Showing showing;
CameraType cameraState;
const char* textCamera;
const char* textShow;
int rotateX,rotateY,rotateZ;
float dist; float zoom;
float monigote;
Muneco patricio1,patricio2;
float speed,direccionPatricio;
Esfera s;
bool sferaVisible,wallVisible,fps,ortho;
std::vector<Bala> balas;
bool lights,light0,light1,light2,normals;

GLfloat WHITE[] = {1,1,1,1};
GLfloat AWHITE[] = {0.8,0.8,0.8,0};
GLfloat BLACK[] = {0,0,0,0};
GLfloat GREEN[] = {0,1,0,0};
GLfloat ORANGE[] = {1,0.5,0.0,0};
GLfloat BLUE[] = {0.f,0.f,1.f,0};

int posLight0 = 0;
GLfloat posLight[5][2] = {{-5,-5},{-5,5},{5,5},{5,-5},{2.5,2.5}};




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

float distPtP(Vector3f a ,Vector3f b) {
    return pow(pow(a.x-b.x,2)+pow(a.y-b.y,2)+pow(a.z-b.z,2),1.0/2.0);
}

Esfera esferaContenidora() { //hardcodeado a ful
    Caja aux;
    aux.maxX = std::max(patricio1.posFinal.x+patricio1.tamano.x/2,5.f);
    aux.minX = std::min(patricio1.posFinal.x-patricio1.tamano.x/2,-5.f);
    aux.maxY = std::max(patricio1.posFinal.y+patricio1.tamano.y/2,0.f);
    aux.minY = std::min(patricio1.posFinal.y-patricio1.tamano.y/2,0.f);
    aux.maxZ = std::max(patricio1.posFinal.z+patricio1.tamano.z/2,5.f);
    aux.minZ = std::min(patricio1.posFinal.z-patricio1.tamano.z/2,-5.f);

    Vector3f centro = aux.findCenter();
    Esfera ret;
    ret.radio = distPtP(Vector3f(aux.maxX,aux.maxY,aux.maxZ),centro);
    ret.centro = centro;
    return ret;
}

void updateCameraType() {
    s = esferaContenidora();
    float r = s.radio;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (cameraState == Fps) {
        float fov = 60;
        if (float(rWidth)/rHeight < 1) fov = atan(tan(fov)/(float(rWidth)/rHeight));
        gluPerspective(2*fov*180/M_PI,float(rWidth)/rHeight,patricio1.tamano.z/2,r*2);
    }
    else {
        dist = r*2.5;
        if (cameraState == Ortho) {
            float wMin = std::min(rWidth,rHeight) * zoom;
            glOrtho(-r*(rWidth/wMin),r*(rWidth/wMin),
                    -r*(rHeight/wMin),r*(rHeight/wMin),
                    -r+dist,r+dist);
        }
        else {
            float fov = asin(r/dist)/zoom;
            if (float(rWidth)/rHeight < 1) fov = atan(tan(fov)/(float(rWidth)/rHeight));
            if (2*fov*180/M_PI > 180) fov = M_PI/(2*180)*180; // 2*fov*180/M_PI = 180
            gluPerspective(2*fov*180/M_PI,float(rWidth)/rHeight,dist-r,dist+r);
        }
    }
    glMatrixMode(GL_MODELVIEW);
}

void updateCameraPos() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if (fps) { //Omg! where you at?!11'!?! the killer is killing
        Vector3f director(sin(direccionPatricio*M_PI/180),0,cos(direccionPatricio*M_PI/180));
        Vector3f p(patricio1.posFinal.x,patricio1.posFinal.y+patricio1.tamano.y/4,patricio1.posFinal.z);
        gluLookAt(p.x+patricio1.tamano.x/3*director.x,p.y,p.z+patricio1.tamano.z/3*director.z,
                  p.x+director.x,p.y,p.z+director.z,
                  0,1,0);

    }
    else {
        glTranslated(0.,0.,-dist);
        glRotated(rotateZ,0,0,1);
        glRotated(rotateY,1,0,0);
        glRotated(rotateX,0,1,0);
        glTranslated(-s.centro.x,-s.centro.y,-s.centro.z);
    }
    glutPostRedisplay();
}

void updateCamera() {
    updateCameraType();
    updateCameraPos();
}

void changeCamera(CameraType s) {
    cameraState = s;
    if (s == Ortho) { ortho = true; fps = false;}
    else if (s == Perspective) {ortho = false; fps = false;}
    else fps = true;
    switch (s) {
    case Ortho:
       textCamera = "Camara Ortho";
        break;
    case Perspective:
        textCamera = "Camara Perstectiva";
        break;
    case Fps:
        textCamera = "First person shooter";
        break;
    default:
        break;
    }
    updateCamera();
    glutPostRedisplay();
}

void reset() {
    glEnable(GL_LIGHTING);
    lights = true;
    normals = true;

    light0 = false;
    glDisable(GL_LIGHT0);
    light1 = true;
    glEnable(GL_LIGHT1);
    light2 = false;
    glDisable(GL_LIGHT2);

    dist = s.radio*4;
    rotateX = rotateY = 45;
    rotateZ = 0;
    zoom = 1;
    changeCamera(Perspective);
}

void init() {
    glShadeModel(GL_SMOOTH);
    modelos = std::vector<Model>(qttModels);
    loadedModels = std::vector<bool>(qttModels,false);
    balas = std::vector<Bala>(0);
    patricio1 = Muneco(kpatricio,1,Vector3f(0,0,0));
    patricio2 = Muneco(patricio,1.5,Vector3f(2.5,0,2.5));
    speed = 0.1;
    direccionPatricio = -90;
    showing = sFill;
    textShow = "Fill";
    r = g = 1;
    b = a = 0.8;
    monigote = 0.5;
    sferaVisible = false;
    wallVisible = true;
    reset();
    changeCamera(Fps);
    updateCamera();
    glEnable(GL_NORMALIZE);
}

void setMaterial(GLfloat* ambient,GLfloat* diffuse,GLfloat* specular,float shininess) {
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,ambient);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diffuse);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,specular);
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shininess);
}

void setMaterialWithOutShine(GLfloat* color) {
    setMaterial(color,color,BLACK,128);
}

void setMaterialWithShine(GLfloat* color, float shine) {
    GLfloat aclarado[4];
    aclarado[0] = (color[0]+0.2)*1.5;
    aclarado[1] = (color[1]+0.2)*1.5;
    aclarado[2] = (color[2]+0.2)*1.5;
    aclarado[3] = (color[3]+0.2)*1.5;
    setMaterial(color,color,aclarado,shine);
}

void myBodyIsReady(float size) {
    glPushMatrix(); {// Body
        if (not lights) glColor3f(0,0.7,0);
        else setMaterialWithShine(WHITE,90);
        //    glTranslated(0.f,0.10f,0.f);
        glutSolidSphere(size,20.f,20.f);
    } glPopMatrix();
    glPushMatrix(); {// Head
        if (not lights) glColor3f(0,0,0.7);
        else setMaterialWithShine(WHITE,50);
        glTranslated(0.f,size*1.5,0.f);
        glutSolidSphere(size/2,20.f,20.f);
    } glPopMatrix();
    glPushMatrix(); { // The D
        if (not lights) glColor3f(0.7,0,0);
        else setMaterialWithShine(ORANGE,50);
        glTranslated(size/4,size*1.5,0.f);
        glRotated(90,0,1,0);
        glutSolidCone(size/4,size/2,20.f,20.f);
    } glPopMatrix();
}

void ejes() {
    glBegin(GL_LINES);if (not lights) glColor3f(1.0f, 0.0f, 0.0f);glVertex3f(0,0,0);glVertex3f(1,0,0);glEnd();
    glBegin(GL_LINES);if (not lights) glColor3f(0.0f, 1.0f, 0.0f);glVertex3f(0,0,0);glVertex3f(0,1,0);glEnd();
    glBegin(GL_LINES);if (not lights) glColor3f(0.0f, 0.0f, 1.0f);glVertex3f(0,0,0);glVertex3f(0,0,1);glEnd();
}


void daModelIsReal(Muneco m) {
    float smax = std::max(m.box.maxX-m.box.minX,std::max(m.box.maxY-m.box.minY,m.box.maxZ-m.box.minZ));
    glScaled(m.scalated/smax,m.scalated/smax,m.scalated/smax);
    glTranslated(-m.centro.x, -m.centro.y, -m.centro.z);

    float material = -1;
    glBegin(GL_TRIANGLES);

    for (int i = 0; i < m.modelo->faces().size(); ++i) {
        
        const Face &f = m.modelo->faces()[i];
        if (material != f.mat and lights) {
            setMaterial(Materials[f.mat].ambient,Materials[f.mat].diffuse,Materials[f.mat].specular,Materials[f.mat].shininess);
            material = f.mat;
        }
        else if (not lights) glColor4fv(Materials[f.mat].diffuse);

        const Vertex *v;

        if (m.modelo->normals().size() == 0 or not normals) glNormal3dv(f.normalC);
        for (int j = 0; j < 3; ++j) {
            v = &m.modelo->vertices()[f.v[j]];
            if (not m.modelo->normals().size() < 1 and normals) {
                const Normal *n = &m.modelo->normals()[f.n[j]];
                glNormal3dv(n);
            }
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

void writeOnWindow() {
    glPushMatrix();
        glLoadIdentity(); // Me fornico la modelview
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
            glLoadIdentity(); // Me fornico la projection
            glOrtho(-1,1,-1,1,-1,1);
        glMatrixMode(GL_MODELVIEW);

        //Pinto
        if (not lights) glColor3f(0,0,0);
        glRasterPos2f(-0.9,0.9);
        glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)(textCamera));

        if (not lights) glColor3f(0,0,0);
        glRasterPos2f(0.7,0.9);
        glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)(textShow));

        glMatrixMode(GL_PROJECTION);
        glPopMatrix(); // Deshago fornicacion de la projection
        glMatrixMode(GL_MODELVIEW);
    glPopMatrix(); // Deshago la fornicacion de la modelview
}

void drawMonigotes(){
    glPushMatrix(); // monigote 1
        glTranslated(2.5,monigote,-2.5);
        myBodyIsReady(monigote);
    glPopMatrix();
    glPushMatrix(); // monigote 2
        glTranslated(-2.5,monigote,2.5);
        myBodyIsReady(monigote);
    glPopMatrix();
    glPushMatrix(); // monigote 3
        glTranslated(-2.5,monigote,-2.5);
        myBodyIsReady(monigote);
    glPopMatrix();
}

void drawWall(Vector3f centro,Vector3f size) {
    glPushMatrix();
    glTranslated(centro.x,centro.y+size.y/2,centro.z);
    glScaled(size.x,size.y,size.z);
    glutSolidCube(1);
    glPopMatrix();
}

void drawLights() {
    if (light0) {
        glPushMatrix();
        GLfloat pos[4] = {posLight[posLight0][0],(posLight0==4?2.5:1.5),posLight[posLight0][1],1};
        glLightfv(GL_LIGHT0,GL_POSITION,pos);
        glLightfv(GL_LIGHT0,GL_DIFFUSE,ORANGE);
        glLightfv(GL_LIGHT0,GL_SPECULAR,ORANGE);
        glPopMatrix();
    }
    if (light1) {
        glPushMatrix();
        glLoadIdentity();
        GLfloat pos[4] = {0,3,0,1};
        glLightfv(GL_LIGHT1,GL_POSITION,pos);
        glLightfv(GL_LIGHT1,GL_DIFFUSE,WHITE);
        glLightfv(GL_LIGHT1,GL_SPECULAR,WHITE);
        glPopMatrix();
    }
    if (light2) {
        glPushMatrix();
        Vector3f director(sin(direccionPatricio*M_PI/180),0,cos(direccionPatricio*M_PI/180));
        Vector3f p(patricio1.posFinal.x,patricio1.posFinal.y+patricio1.tamano.y/4,patricio1.posFinal.z);
        GLfloat pos[4] = {p.x+patricio1.tamano.x/3*director.x,p.y,p.z+patricio1.tamano.z/3*director.z,1};
        glLightfv(GL_LIGHT2,GL_POSITION,pos);
        glLightfv(GL_LIGHT2,GL_DIFFUSE,WHITE);
        glLightfv(GL_LIGHT2,GL_SPECULAR,WHITE);
        glPopMatrix();
    }
}


void refresh(void) {
    glClearColor(r,g,b,a);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    drawLights();
    if (sferaVisible) {
    glPushMatrix();
        if (not lights) glColor3f(1,0,0);
        else {GLfloat color[4] = {1,0,0,0};setMaterialWithOutShine(color);}
        glTranslated(s.centro.x,s.centro.y,s.centro.z);
        glutWireSphere(s.radio,30,30);
    glPopMatrix(); }
    glPushMatrix();
        if (not lights) glColor3f(0.1,0.1,0.9);
        else setMaterialWithShine(BLUE,50);
        glNormal3d(0,1,0);
        drawGround(Vector3f(0,0,0), Vector3f(1,0,1), 10);
    glPopMatrix();
    ejes();
    if (wallVisible) {
        if (not lights) glColor3f(0.5,0.5,0.5);
        else {GLfloat color[4] = {0.1,0.8,0.1,0};setMaterialWithOutShine(color);}
        drawWall(Vector3f(0,0,-4.9),Vector3f(10,1.5,0.2));
        drawWall(Vector3f(1.5,0,2.5),Vector3f(0.2,1.5,4));
    }
    drawMonigotes();
    glPushMatrix(); //patricio serialKiller
        glTranslated(patricio1.posFinal.x,patricio1.posFinal.y,patricio1.posFinal.z);
        glRotated(direccionPatricio,0,1,0);
        daModelIsReal(patricio1);
    glPopMatrix();
    glPushMatrix(); //patricio 2
        glTranslated(patricio2.posFinal.x,patricio2.posFinal.y,patricio2.posFinal.z);
        daModelIsReal(patricio2);
    glPopMatrix();
    for (uint i = 0; i < balas.size();++i) {
        glPushMatrix();
        glTranslated(balas[i].pos.x,balas[i].pos.y,balas[i].pos.z);
        glRotated(balas[i].dir,0,1,0);
        glTranslated(0.25,0.2,0.32);
        balas[i].draw();
        glPopMatrix();
    }
    glPushMatrix();
        writeOnWindow();
    glPopMatrix();
    glutPostRedisplay();
    glutSwapBuffers();
}

void resize(int x, int y) {
    rWidth = x;
    rHeight = y;
    glViewport(0,0,x,y);
    updateCameraType();
}

void mouseClicked(int button, int pressed, int x, int y) {
    if (pressed == 0) {posMouse.first = x; posMouse.second = y;}
    if (button == 0 and pressed == 0) mouse.first = true;
    else if (button == 0 and pressed == 1) mouse.first = false;
    else if (button == 2 and pressed == 0) mouse.second = true;
    else if (button == 2 and pressed == 1) mouse.second = false;
}

void mouseMoved(int x, int y) {
    if (not fps) {
        if (mouse.first) {
            rotateX += posMouse.first - x;
            rotateY += posMouse.second - y;
            posMouse.first = x;
            posMouse.second = y;
            updateCameraPos();
        }
        else if (mouse.second) {
            zoom += float(y-posMouse.second)/rHeight;
            if (zoom < 0.01) zoom = 0.01;
            posMouse.second = y;
            updateCameraType();
        }
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
        std::cout << "You can see the state of the camera type on the up left corner." << std::endl;
        std::cout << "Press H to display the help(lolol)" << std::endl;
        std::cout << "Press R to reset the camera" << std::endl;
        std::cout << "Press P to switch between perspective and ortho camera" << std::endl;
        std::cout << "Press C to switch between first person camera and third person camera" << std::endl;
        std::cout << "Press WASD to move the Patricio" << std::endl;
        std::cout << "Press Z or X to increment/decrement the speed of the Patricio" << std::endl;
        std::cout << "Press V to make the walls visible/invisible" << std::endl;
        std::cout << "Press B to make the sfere of the scene visible/invisible" << std::endl;
        std::cout << "Press M to switch the model of the main character" << std::endl;
        std::cout << "Press O to change the polygon mode" << std::endl;
        std::cout << "Press Space to unleash the hidden and destructive power of the Killer Patricio" << std::endl;
        std::cout << std::endl;
        break;
    case 'w':
        patricio1.posFinal = patricio1.posFinal + Vector3f(sin(direccionPatricio*M_PI/180)*speed,0,cos(direccionPatricio*M_PI/180)*speed);
        updateCamera(); // Por si el patricio se ha salido de la esfera i ase iorar al ninio jisus
        glutPostRedisplay();
        break;
    case 's':
        patricio1.posFinal = patricio1.posFinal - Vector3f(sin(direccionPatricio*M_PI/180)*speed,0,cos(direccionPatricio*M_PI/180)*speed);
        updateCamera(); // Por si el patricio se ha salido de la esfera i ase iorar al ninio jisus
        glutPostRedisplay();
        break;
    case 'a':
        direccionPatricio += 2.5;
        if (fps) updateCamera();
        else glutPostRedisplay();
        break;
    case 'd':
        direccionPatricio -= 2.5;
        if (fps) updateCamera();
        else glutPostRedisplay();
        break;
    case 'z':
        speed += 0.03;
        break;
    case 'x':
        speed -= 0.03; if (speed < 0) speed = 0;
        break;
    case 'r':
        reset();
        glutPostRedisplay();
        break;
    case 'c':
        fps = not fps;
        if (fps) changeCamera(Fps);
        else changeCamera(ortho ? Ortho : Perspective);
        updateCamera();
        break;
    case 'p':
        if (cameraState != Fps) changeCamera(CameraType((cameraState+1)%(qttCameraType-1)));
        updateCameraType();
        break;
    case 'o':
        changeShowing(Showing((showing+1)%qttShowing));
        break;
    case ',':
        patricio1.changeModel(Models((patricio1.name+1)%qttModels));
        glutPostRedisplay();
        break;
    case 'v':
        wallVisible = not wallVisible;
        glutPostRedisplay();
        break;
    case 'b':
        sferaVisible = not sferaVisible;
        glutPostRedisplay();
        break;
    case ' ':
        balas.push_back(Bala(patricio1.posFinal,direccionPatricio));
        break;
    case 'i':
        lights = not lights;
        if (lights) glEnable(GL_LIGHTING);
        else glDisable(GL_LIGHTING);
        break;
    case 'n':
        normals = not normals;
        glutPostRedisplay();
        break;
    case 'm':
        posLight0 = (++posLight0)%5;
        break;
    case '0':
        light0 = not light0;
        if (light0) glEnable(GL_LIGHT0);
        else glDisable(GL_LIGHT0);
        break;
    case '1':
        light1 = not light1;
        if (light1) glEnable(GL_LIGHT1);
        else glDisable(GL_LIGHT1);
        break;
    case '2':
        light2 = not light2;
        if (light2) glEnable(GL_LIGHT2);
        else glDisable(GL_LIGHT2);
        break;
    default:
        break;
    }
}

float ttt,timebase=0;



void update() {
    ttt = glutGet(GLUT_ELAPSED_TIME);
    float deltaTime;
    deltaTime = (ttt-timebase)/1000;
//    sprintf(textShow,"%4.2f",1/deltaTime);
    timebase = ttt;

    for (uint i = 0; i < balas.size(); ++i)  {
        balas[i].update(deltaTime);
        if (distPtP(balas[i].pos,patricio1.posFinal) > s.radio*2) balas.erase(balas.begin()+i);
    }
}

int main(int argc, char **argv) {
    int basura = pthread_getconcurrency();

    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
    glutInitWindowSize(rHeight,rWidth);
    glutCreateWindow("Bloc 3");
    init();
    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(refresh);
    glutReshapeFunc(resize);
    glutMouseFunc(mouseClicked);
    glutMotionFunc(mouseMoved);
    glutKeyboardFunc(teclado);
    glutIdleFunc(update);
    glutMainLoop();
    return 0;
}


