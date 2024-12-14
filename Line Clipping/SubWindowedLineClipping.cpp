#include <freeglut.h>
#include <iostream>
#include <vector>


using namespace std;

struct Point {
    float x, y;
};

struct Color {
    float r, g, b;
};



// VARIABLES FOR COHENSUTHERLAND ALGORITHM
const int INSIDE = 0;
const int LEFT = 1;
const int RIGHT = 2;
const int BOTTOM = 4;
const int TOP = 8;

vector<Point> currentLine1;
vector<vector<Point>> lines1;
vector<Color> lineColors;

float xmin = -150, ymin = -100, xmax = 150, ymax = 100;

bool isClipped1 = false;
bool isDragging1 = false;
bool isDrawingMode1 = true;
bool isCheckedClipped1 = false;
bool isCheckedViewport1 = false;

Point dragStart1, dragEnd1;



const vector<Color> neonColors = {
    {0.0f, 1.0f, 1.0f},  // Neon Cyan
    {1.0f, 1.0f, 0.0f},  // Neon Yellow
    {0.0f, 1.0f, 0.0f},  // Neon Green
    {1.0f, 0.4f, 0.0f},  // Neon Orange
    {0.8f, 0.0f, 1.0f}   // Neon Purple
};








//VARIABLES FOR LIANG BARSKY ALGORITHM
float xMin = -150, yMin = -100, xMax = 150, yMax = 100;

float xClipped1, yClipped1, xClipped2, yClipped2;

vector<Point> currentLine2;
vector<vector<Point>> lines2;

bool isClipped2 = false;
bool isDragging2 = false;
bool isDrawingMode2 = true;
bool isCheckedClipped2 = false;
bool isCheckedViewport2 = false;

Point dragStart2, dragEnd2;

const vector<Color> darkerColors = {
    {0.7f, 0.0f, 0.0f},  // Dark Red
    {0.0f, 0.5f, 0.0f},  // Dark Green
    {0.0f, 0.0f, 0.7f},  // Dark Blue
    {0.5f, 0.0f, 0.5f},  // Dark Purple
    {0.7f, 0.3f, 0.0f}   // Dark Orange
};

vector<Color> lineColors2;



void renderBitmapString2(float x, float y, void* font, const char* string) {
    glColor3f(0.0, 0.0, 0.0);
    const char* c;
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void renderBitmapString(float x, float y, void* font, const char* string) {
    glColor3f(1.0, 1.0, 1.0);
    const char* c;
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}



int computeCode(float x, float y) {
    int code = INSIDE;

    if (x < xmin)
        code |= LEFT;
    else if (x > xmax)
        code |= RIGHT;
    if (y < ymin)
        code |= BOTTOM;
    else if (y > ymax)
        code |= TOP;

    return code;
}

bool cohenSutherlandAlgorithm(float& x1, float& y1, float& x2, float& y2) {

    int codeP1 = computeCode(x1, y1);
    int codeP2 = computeCode(x2, y2);

    bool accept = false;

    while (true) {

        if ((codeP1 == 0) && (codeP2 == 0)) // Trivial Accept Inside the Clipping Window
        {
            accept = true;
            break;
        }

        else if (codeP1 & codeP2) // Trivial Reject Outside the Clipping Window
        {
            break;
        }

        else // Partial Accept / Reject
        {

            int codeOutside = codeP1 ? codeP1 : codeP2;
            float x, y;

            if (codeOutside & TOP) {
                x = x1 + (x2 - x1) * (ymax - y1) / (y2 - y1);
                y = ymax;
            }

            else if (codeOutside & BOTTOM) {
                x = x1 + (x2 - x1) * (ymin - y1) / (y2 - y1);
                y = ymin;
            }

            else if (codeOutside & RIGHT) {
                y = y1 + (y2 - y1) * (xmax - x1) / (x2 - x1);
                x = xmax;
            }

            else if (codeOutside & LEFT) {
                y = y1 + (y2 - y1) * (xmin - x1) / (x2 - x1);
                x = xmin;
            }

            if (codeOutside == codeP1) {
                x1 = x;
                y1 = y;
                codeP1 = computeCode(x1, y1);
            }

            else {
                x2 = x;
                y2 = y;
                codeP2 = computeCode(x2, y2);
            }
        }
    }

    return accept;
}

bool liangBarskyClippingAlgorithm(float x1, float y1, float x2, float y2) {
    float p[4], q[4];

    float t1 = 0.0;
    float t2 = 1.0;

    float dx = x2 - x1, dy = y2 - y1;

    p[0] = -dx;
    q[0] = x1 - xMin;

    p[1] = dx;
    q[1] = xMax - x1;

    p[2] = -dy;
    q[2] = y1 - yMin;

    p[3] = dy;
    q[3] = yMax - y1;

    for (int i = 0; i < 4; i++) {

        if (p[i] == 0 && q[i] < 0) // Line is Parallel if this Condition is True 
        {
            return false;
        }

        float r = q[i] / p[i];

        if (p[i] < 0) // Line is Coming from Outside to Inside  
        {
            t1 = max(t1, r);
        }

        else if (p[i] > 0) // Line is going from Inside to Outside 
        {
            t2 = min(t2, r);
        }

        if (t1 > t2) // Discard this line as t1 can't be greater than t2  
        {
            return false;
        }

    }

    xClipped1 = x1 + t1 * dx;
    yClipped1 = y1 + t1 * dy;
    xClipped2 = x1 + t2 * dx;
    yClipped2 = y1 + t2 * dy;

    return true;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glFlush();
}



void leftSubDisplay() {
    glClear(GL_COLOR_BUFFER_BIT);


    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(xmin, ymin);
    glVertex2f(xmax, ymin);
    glVertex2f(xmax, ymax);
    glVertex2f(xmin, ymax);
    glEnd();


    for (size_t i = 0; i < lines1.size(); i++) {

        const auto& line = lines1[i];
        const auto& color = lineColors[i];

        if (line.size() == 2) {
            Point p1 = line[0];
            Point p2 = line[1];

            float x1 = p1.x;
            float y1 = p1.y;
            float x2 = p2.x;
            float y2 = p2.y;

            glColor3f(color.r, color.g, color.b);
            glLineWidth(3.0f);

            if (!isClipped1) {
                glBegin(GL_LINES);
                glVertex2f(x1, y1);
                glVertex2f(x2, y2);
                glEnd();
            }
            else {
                if (cohenSutherlandAlgorithm(x1, y1, x2, y2)) {
                    glBegin(GL_LINES);
                    glVertex2f(x1, y1);
                    glVertex2f(x2, y2);
                    glEnd();
                }
            }
        }
    }




    if (isDragging1 && !isDrawingMode1) {
        glColor3f(1.0f, 0.0f, 0.0f);
        glLineWidth(3.0f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(dragStart1.x, dragStart1.y);
        glVertex2f(dragEnd1.x, dragStart1.y);
        glVertex2f(dragEnd1.x, dragEnd1.y);
        glVertex2f(dragStart1.x, dragEnd1.y);
        glEnd();
    }

    if (!isCheckedClipped1) {
        glBegin(GL_QUADS);

        glColor3f(1.0, 0.0, 0.0);
        glVertex2f(102.0f, 195.0f);
        glVertex2f(107.0f, 195.0f);
        glVertex2f(107.0f, 190.0f);
        glVertex2f(102.0f, 190.0f);
        glEnd();
    }
    else {
        glBegin(GL_QUADS);

        glColor3f(0.0, 1.0, 0.0);
        glVertex2f(102.0f, 195.0f);
        glVertex2f(107.0f, 195.0f);
        glVertex2f(107.0f, 190.0f);
        glVertex2f(102.0f, 190.0f);
        glEnd();
    }


    if (!isCheckedViewport1) {
        glBegin(GL_QUADS);

        glColor3f(1.0, 0.0, 0.0);

        glVertex2f(102.0f, 185.0f);
        glVertex2f(107.0f, 185.0f);
        glVertex2f(107.0f, 180.0f);
        glVertex2f(102.0f, 180.0f);

        glEnd();
    }
    else {
        glBegin(GL_QUADS);

        glColor3f(0.0, 1.0, 0.0);
        glVertex2f(102.0f, 185.0f);
        glVertex2f(107.0f, 185.0f);
        glVertex2f(107.0f, 180.0f);
        glVertex2f(102.0f, 180.0f);

        glEnd();
    }




    renderBitmapString(110, 190, GLUT_BITMAP_HELVETICA_12, "Clipping (Press C + 1)");
    renderBitmapString(110, 180, GLUT_BITMAP_HELVETICA_12, "Resizing Viewport (Press R + 1)");
    renderBitmapString(110, 170, GLUT_BITMAP_HELVETICA_12, "Move Viewport:");
    renderBitmapString(110, 160, GLUT_BITMAP_HELVETICA_12, "Press W - Move Up");
    renderBitmapString(110, 150, GLUT_BITMAP_HELVETICA_12, "Press A - Move Left");
    renderBitmapString(110, 140, GLUT_BITMAP_HELVETICA_12, "Press S - Move Down");
    renderBitmapString(110, 130, GLUT_BITMAP_HELVETICA_12, "Press D - Move Right");


    renderBitmapString(-45.0f, 190.0f, GLUT_BITMAP_HELVETICA_18, "CohenSutherland Algorithm");
    renderBitmapString(60.0f, -190.0f, GLUT_BITMAP_HELVETICA_18, "Love Dewangan 500109339");


    glFlush();


}

void rightSubDisplay() {
    glClear(GL_COLOR_BUFFER_BIT);

    
    glColor3f(0.0, 0.0, 0.0);
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);

    glVertex2f(xMin, yMin);
    glVertex2f(xMax, yMin);
    glVertex2f(xMax, yMax);
    glVertex2f(xMin, yMax);


    glEnd();

    for (size_t i = 0; i < lines2.size(); i++) {
        const auto& line = lines2[i];
        const auto& color = lineColors2[i];

        if (line.size() == 2) {
            Point p1 = line[0];
            Point p2 = line[1];

            float x1 = p1.x;
            float y1 = p1.y;
            float x2 = p2.x;
            float y2 = p2.y;

            bool isClipped = liangBarskyClippingAlgorithm(x1, y1, x2, y2);

            if (!isClipped2 || (isClipped2 && isClipped)) {

                glColor3f(color.r, color.g, color.b);

                glLineWidth(3.0f);
                glBegin(GL_LINES);

                if (isClipped2) {
                    glVertex2f(xClipped1, yClipped1);
                    glVertex2f(xClipped2, yClipped2);
                }

                else {
                    
                    glVertex2f(x1, y1);
                    glVertex2f(x2, y2);
                }

                glEnd();
            }
        }
    }

    if (isDragging2 && !isDrawingMode2) {

        glColor3f(1.0f, 0.0f, 0.0f);
        glLineWidth(3.0f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(dragStart2.x, dragStart2.y);
        glVertex2f(dragEnd2.x, dragStart2.y);
        glVertex2f(dragEnd2.x, dragEnd2.y);
        glVertex2f(dragStart2.x, dragEnd2.y);
        glEnd();
    }

    
    if (!isCheckedClipped2) {

        glBegin(GL_QUADS);
        glColor3f(1.0, 0.0, 0.0);
        glVertex2f(102.0f, 195.0f);
        glVertex2f(107.0f, 195.0f);
        glVertex2f(107.0f, 190.0f);
        glVertex2f(102.0f, 190.0f);
        glEnd();
    }


    else {
        glBegin(GL_QUADS);
        glColor3f(0.0, 1.0, 0.0);
        glVertex2f(102.0f, 195.0f);
        glVertex2f(107.0f, 195.0f);
        glVertex2f(107.0f, 190.0f);
        glVertex2f(102.0f, 190.0f);
        glEnd();
    }

    if (!isCheckedViewport2) {

        glBegin(GL_QUADS);
        glColor3f(1.0, 0.0, 0.0);
        glVertex2f(102.0f, 185.0f);
        glVertex2f(107.0f, 185.0f);
        glVertex2f(107.0f, 180.0f);
        glVertex2f(102.0f, 180.0f);
        glEnd();
    }
    else {


        glBegin(GL_QUADS);
        glColor3f(0.0, 1.0, 0.0);
        glVertex2f(102.0f, 185.0f);
        glVertex2f(107.0f, 185.0f);
        glVertex2f(107.0f, 180.0f);
        glVertex2f(102.0f, 180.0f);
        glEnd();
    }

    renderBitmapString2(110, 190, GLUT_BITMAP_HELVETICA_12, "Clipping (Press C + 2)");
    renderBitmapString2(110, 180, GLUT_BITMAP_HELVETICA_12, "Resizing Viewport (Press R + 2)");
    renderBitmapString2(110, 170, GLUT_BITMAP_HELVETICA_12, "Move Viewport:");
    renderBitmapString2(110, 160, GLUT_BITMAP_HELVETICA_12, "Press ^ - Move Up");
    renderBitmapString2(110, 150, GLUT_BITMAP_HELVETICA_12, "Press < - Move Left");
    renderBitmapString2(110, 140, GLUT_BITMAP_HELVETICA_12, "Press > - Move Down");
    renderBitmapString2(110, 130, GLUT_BITMAP_HELVETICA_12, "Press v - Move Right");

    renderBitmapString2(-30.0f, 190.0f, GLUT_BITMAP_HELVETICA_18, "Liang Barsky Algorithm");
    renderBitmapString2(60.0f, -190.0f, GLUT_BITMAP_HELVETICA_18, "Love Dewangan 500109339");

    glFlush();
}



void mainWindowMouse(int button, int state, int x, int y) {

    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);
    float x_pos = (x / (float)width) * 400.0f - 200.0f;
    float y_pos = ((height - y) / (float)height) * 400.0f - 200.0f;

    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            if (isDrawingMode1) {
                currentLine1.push_back({ x_pos, y_pos });
                if (currentLine1.size() == 2) {
                    lines1.push_back(currentLine1);
                    lineColors.push_back(neonColors[lines1.size() % neonColors.size()]);
                    currentLine1.clear();
                }
            }
            else {
                dragStart1 = dragEnd1 = { x_pos, y_pos };
                isDragging1 = true;
            }
        }
        else if (state == GLUT_UP && !isDrawingMode1) {
            isDragging1 = false;

            xmin = min(dragStart1.x, dragEnd1.x);
            xmax = max(dragStart1.x, dragEnd1.x);
            ymin = min(dragStart1.y, dragEnd1.y);
            ymax = max(dragStart1.y, dragEnd1.y);
        }
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        currentLine1.clear();
        lines1.clear();
        lineColors.clear();
    }
    glutPostRedisplay();
}


void subWindowMouse(int button, int state, int x, int y) {

    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);
    float x_pos = (x / (float)width) * 400.0f - 200.0f;
    float y_pos = ((height - y) / (float)height) * 400.0f - 200.0f;

    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            if (isDrawingMode2) {
                currentLine2.push_back({ x_pos, y_pos });
                if (currentLine2.size() == 2) {
                    lines2.push_back(currentLine2);
                    lineColors2.push_back(darkerColors[lines2.size() % darkerColors.size()]);
                    currentLine2.clear();
                }
            }

            else {

                dragStart2 = dragEnd2 = { x_pos, y_pos };
                isDragging2 = true;
            }
        }
        else if (state == GLUT_UP && !isDrawingMode2) {
            isDragging2 = false;

            xMin = min(dragStart2.x, dragEnd2.x);
            xMax = max(dragStart2.x, dragEnd2.x);
            yMin = min(dragStart2.y, dragEnd2.y);
            yMax = max(dragStart2.y, dragEnd2.y);

        }
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {

        currentLine2.clear();
        lines2.clear();
        lineColors2.clear();
    }
    glutPostRedisplay();
}

void motion1(int x, int y) {
    if (isDragging1 && !isDrawingMode1) {

        int width = glutGet(GLUT_WINDOW_WIDTH);
        int height = glutGet(GLUT_WINDOW_HEIGHT);
        float x_pos = (x / (float)width) * 400.0f - 200.0f;
        float y_pos = ((height - y) / (float)height) * 400.0f - 200.0f;
        dragEnd1 = { x_pos, y_pos };
        glutPostRedisplay();
    }
}

void motion2(int x, int y) {

    if (isDragging2 && !isDrawingMode2) {

        int width = glutGet(GLUT_WINDOW_WIDTH);
        int height = glutGet(GLUT_WINDOW_HEIGHT);
        float x_pos = (x / (float)width) * 400.0f - 200.0f;
        float y_pos = ((height - y) / (float)height) * 400.0f - 200.0f;
        dragEnd2 = { x_pos, y_pos };
        glutPostRedisplay();
    }
}


char lastKey = '\0';

void keyboard1(unsigned char key, int x, int y) {

    if (lastKey == 'c' || lastKey == 'C') {
        if (key == '1') {
            isClipped1 = !isClipped1;
            isCheckedClipped1 = !isCheckedClipped1;
            glutPostRedisplay();
           
        }
        lastKey = '\0';  

    }
    else if (lastKey == 'r' || lastKey == 'R') {
        if (key == '1') {
            isDrawingMode1 = !isDrawingMode1;
            isCheckedViewport1 = !isCheckedViewport1;
            glutPostRedisplay();
            
        }

        lastKey = '\0';  

    }
    else if (key == 'w' || key == 'W') {
        
        ymax += 2;
        ymin += 2;

            glutPostRedisplay();

    }
    else if (key == 'a' || key == 'A') {

        xmax -= 2;
        xmin -= 2;

        glutPostRedisplay();

    }
    else if (key == 's' || key == 'S') {

        

        ymax -= 2;
        ymin -= 2;
        glutPostRedisplay();

    }
    else if (key == 'd' || key == 'D') {

        xmax += 2;
        xmin += 2;

        glutPostRedisplay();

    }
    else {
        lastKey = key;  

    }
}

void keyboard2_1(int key, int x, int y) {
    if (key == GLUT_KEY_UP) {

        yMax += 2;
        yMin += 2;

        glutPostRedisplay();

        }
    else if (key == GLUT_KEY_LEFT) {

            xMax -= 2;
            xMin -= 2;

            glutPostRedisplay();

            }
    else if (key == GLUT_KEY_DOWN) {



                yMax -= 2;
                yMin -= 2;
                glutPostRedisplay();

                }
    else if (key == GLUT_KEY_RIGHT) {

                    xMax += 2;
                    xMin += 2;

                    glutPostRedisplay();

                    }
    glutPostRedisplay();
}


void keyboard2(unsigned char key, int x, int y) {
    if (lastKey == 'c' || lastKey == 'C') {
        if (key == '2') {
            isClipped2 = !isClipped2;
            isCheckedClipped2 = !isCheckedClipped2;
            glutPostRedisplay();
        }


        lastKey = '\0';  

    }


    else if (lastKey == 'r' || lastKey == 'R') {
        if (key == '2') {
            isDrawingMode2 = !isDrawingMode2;
            isCheckedViewport2 = !isCheckedViewport2;
            glutPostRedisplay();
        }
        lastKey = '\0';  
    }


    


    else {
        lastKey = key;  
    }
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-200, 200, -200, 200);

    glMatrixMode(GL_MODELVIEW);
}

void init1() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-200, 200, -200, 200);
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-200, 200, -200, 200);
}

int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    // Set the display mode
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    // Set the window size
    glutInitWindowSize(1600, 800);
    // Set the window position
    glutInitWindowPosition(100, 100);
    // Create the window with a title
    int mainWindow = glutCreateWindow("Line Clipping");
    glutDisplayFunc(display);
    init();


    //glutSetWindow(mainWindow);
    int leftSubWindow = glutCreateSubWindow(mainWindow, 0, 0, 800, 800);
    //glutSetWindow(leftSubWindow);
    init1();
    
    glutDisplayFunc(leftSubDisplay);
    glutReshapeFunc(reshape);
    glutMouseFunc(mainWindowMouse);
    glutMotionFunc(motion1);
    glutKeyboardFunc(keyboard1);



    int rightSubWindow = glutCreateSubWindow(mainWindow, 800, 0, 800, 800);

    init();
    glutDisplayFunc(rightSubDisplay);
    glutReshapeFunc(reshape);
    glutMouseFunc(subWindowMouse);
    glutMotionFunc(motion2);
    glutKeyboardFunc(keyboard2);
    glutSpecialFunc(keyboard2_1);

    glutMainLoop();
    return 0;
}
