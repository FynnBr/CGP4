#include "myglwidget.h"

/*MyGLWidget::MyGLWidget()
{

}*/
MyGLWidget::MyGLWidget(QWidget *parent) : QOpenGLWidget(parent) {
    setFocusPolicy(Qt::StrongFocus);
}

void MyGLWidget::setFOV(int value) {
    m_FOV = value;
    theShine = value/100;
}

int MyGLWidget::getFOV() {
    return m_FOV;
}

void MyGLWidget::setAngle(int value) {
    m_Angle = value;
    updateProjMat();
}

int MyGLWidget::getAngle() {
    return m_Angle;
}

void MyGLWidget::setProjectionMode() {
    m_ProjectionMode = !m_ProjectionMode;
}

void MyGLWidget::setRotationA(int value) {
    uRotMatOuter.rotate(value - m_RotationA, axisA);
    uRotMatMiddle.rotate(value - m_RotationA, axisA);
    uRotMatInner.rotate(value - m_RotationA, axisA);

    axisB = uRotMatOuter.transposed() * QVector3D(1, 0, 0); //ursprüngliche RotAchse * RotEbene -> neue RotAchse
    axisC = uRotMatMiddle.transposed() * QVector3D(0, 1, 0);

    m_RotationA = value;
    iAmbient = (float)value/360;
    // mp_program->bind();
    // mp_program->setUniformValue(3,uRotMatOuter);
    // qInfo() << "uRotMatOuter" << uRotMatOuter << "\n uRotMatOuter Transposed " << uRotMatOuter.transposed() << "\n axisB" << axisB << "\n axisC" << axisC << "\n";
    // this->update();
}

void MyGLWidget::setRotationB(int value) {
    uRotMatMiddle.rotate(value - m_RotationB, axisB);
    uRotMatInner.rotate(value - m_RotationB, axisB);

    axisC = uRotMatMiddle.transposed() * QVector3D(0, 1, 0);

    m_RotationB = value;
    iDiffuse = (float)value/360;
    // mp_program->bind();
    // mp_program->setUniformValue(3,uRotMatMiddle);
    //this->update();
}

void MyGLWidget::setRotationC(int value) {
    uRotMatInner.rotate(value - m_RotationC, axisC);

    m_RotationC = value;
    iSpecular = (float)value/360;
    // mp_program->bind();
    // mp_program->setUniformValue(3,uRotMatInner);
    //this->update();
}

void MyGLWidget::setRotationT(int value){
    m_RotationT = value;
    mp_program->bind();
    mp_program->setUniformValue(1, float (m_RotationT/100.0));
}

void MyGLWidget::setNear(double value){
    m_Near = value;
    if(m_Near < m_Far && m_Near+1 == m_Far) {
        m_Far = m_Near + 2;
        emit farValueChanged(m_Far);
    } else if (m_Near == 1 && m_Far == 0) {
        m_Far = 3;
        emit farValueChanged(m_Far);
    } else if (m_Far < m_Near && m_Far+1 == m_Near) {
        m_Far = m_Near-2;
        emit farValueChanged(m_Far);
    } else if (m_Far == m_Near) {
        m_Far = m_Near+2;
        emit farValueChanged(m_Far);
    }
    updateProjMat();
}

void MyGLWidget::setFar(double value){
    m_Far = value;
    if(m_Far < m_Near && m_Far+1 == m_Near) {
        m_Near = m_Far + 2;
        emit nearValueChanged(m_Near);
    } else if (m_Far == 1 && m_Near == 0) {
        m_Near = 3;
        emit nearValueChanged(m_Near);
    } else if (m_Near < m_Far && m_Near+1 == m_Far) {
        m_Near = m_Far-2;
        emit nearValueChanged(m_Near);
    } else if (m_Far == m_Near) {
        m_Near = m_Far+2;
        emit nearValueChanged(m_Near);
    }
    updateProjMat();
}

void MyGLWidget::keyPressEvent(QKeyEvent *event) {
    if(event->key() == Qt::Key_W) {
        m_CameraPos.setY(m_CameraPos.y() + .02f);
        updateCamera();
    } else if(event->key() == Qt::Key_S) {
        m_CameraPos.setY(m_CameraPos.y() - .02f);
        updateCamera();
    } else if(event->key() == Qt::Key_A) {
        m_CameraPos.setX(m_CameraPos.x() - .02f);
        updateCamera();
    } else if(event->key() == Qt::Key_D) {
        m_CameraPos.setX(m_CameraPos.x() + .02f);
        updateCamera();
    } else if(event->key() == Qt::Key_Q) {
        m_CameraPos.setZ(m_CameraPos.z() - .02f);
        updateCamera();
    } else if(event->key() == Qt::Key_E) {
        m_CameraPos.setZ(m_CameraPos.z() + .02f);
        updateCamera();
    }else if(event->key() == Qt::Key_Left) {
        m_CameraAngle.setX(m_CameraAngle.x() - 20.0f);
        updateCamera();
    } else if(event->key() == Qt::Key_Right) {
        m_CameraAngle.setX(m_CameraAngle.x() + 20.0f);
        updateCamera();
    } else {
        QOpenGLWidget::keyPressEvent(event);
    }
}

void MyGLWidget::moveTexture(int value){
    TextureMod = (float)value/100;
    mp_program->setUniformValue(2, TextureMod);
    //this->update();
}

void MyGLWidget::setAnimation(bool value){
    animationActive = value;
    qInfo()<<"spin to win" << animationActive;
}

//void MyGLWidget::setGimbalCamera(bool value) {
//    m_GimbalCam = value;

//    if(m_GimbalCam) {

//    } else {
//        cameraMat = QMatrix4x4();
//    }
//}

void MyGLWidget::updateProjMat(){
    projecMat = QMatrix4x4();
    qInfo() << "Near: " << m_Near << " Far: " << m_Far << " Angle: " << m_Angle << "\n";
    qInfo() << "Width " << this->width() << " Height: " << this->height() << "\n";
    //projecMat.perspective(m_Angle, (double)this->width() / (double)this->height(),(float) m_Near, (float) m_Far);
    projecMat.perspective(qDegreesToRadians((double)m_Angle), m_Aspect, (float) m_Near, (float) m_Far);
    qInfo() << projecMat;
}

void MyGLWidget::setGimbalCamera(bool gimbalCam){
    m_GimbalCam = gimbalCam;
    if(m_GimbalCam){
        m_Near = 5.0;
        m_Far = 7.0;
        m_Angle = 180;
        setAngle(180);
        setNear(5.0);
        setFar(7.0);
    } else {
        m_Near = 2.0;
        m_Far = 0.0;
        m_Angle = 0;
        setNear(2.0);
        setFar(0.0);
        setAngle(0);
        cameraMat = QMatrix4x4();
    }
}

void MyGLWidget::updateCamera(){
    cameraMat = QMatrix4x4();
    /*cameraMat.lookAt(m_CameraPos, //wo Kamera ist
                    {0, 0, 0},    // wo die Kamera hinschaut
                    {0, 1, 0}     // wo oben ist
                     );*/

    cameraMat.translate(-m_CameraPos[1], -m_CameraPos[0], -m_CameraPos[2]);
    cameraMat.rotate(m_CameraAngle.x(), {0, 1, 0});
}

//destructor -->
MyGLWidget::~MyGLWidget(){
    finalize();
}

void MyGLWidget::initializeGL() {
    Q_ASSERT(initializeOpenGLFunctions());

    m_Aspect = this->width()/ this->height();

    glClearColor(0.0, 0.0, 0.0, 1.0);

    // projection matrix init
    projecMat.perspective(m_Angle, m_Aspect, m_Near, m_Far);

    // rotationsachsen init
    axisA = {0, 1, 0};
    axisB = {1, 0, 0};
    axisC = {0, 1, 0};

    // gimbal size
    uRotMatOuter.scale(0.9);
    uRotMatMiddle.scale(0.6);
    uRotMatInner.scale(0.4);

    //object init
    gimbal.initGL(":/objects/gimbal.obj");
    sphere.initGL(":/objects/sphere.obj");
    light.initGL(":/objects/sphere.obj");

    // Lichter initialisieren
    lightModel1.translate(lightPos1);
    lightModel2.translate(lightPos2);
    lightModel3.translate(lightPos3);
    lightModel4.translate(lightPos4);
    lightModel5.translate(lightPos5);

    // Gelbes Licht
    ls[0].position = lightPos1;
    ls[0].color = {1.0f, 1.0f, 0.0f};
    ls[0].ka = 0.8;
    ls[0].kd = 0.8;
    ls[0].ks = 1.0;
    ls[0].constant = 1.0;
    ls[0].linear = 0.22;
    ls[0].quadratic = 0.20;

    // Blaues Licht
    ls[1].position = lightPos1;
    ls[1].color = {0.0f, 0.0f, 1.0f};
    ls[1].ka = 0.8;
    ls[1].kd = 0.8;
    ls[1].ks = 1.0;
    ls[1].constant = 1.0;
    ls[1].linear = 0.22;
    ls[1].quadratic = 0.20;

    // Weißes Licht
    ls[2].position = lightPos3;
    ls[2].color = {.3f, .3f, .3f};
    ls[2].ka = 0.1;
    ls[2].kd = 0.1;
    ls[2].ks = 0.1;
    ls[2].constant = 1.0;
    ls[2].linear = 0.7;
    ls[2].quadratic = 1.8;

    // Lila Licht
    ls[3].position = lightPos4;
    ls[3].color = {0.65f, 0.3f, 1.0f};
    ls[3].ka = 0.8;
    ls[3].kd = 0.8;
    ls[3].ks = 1.0;
    ls[3].constant = .2;
    ls[3].linear = 0.22;
    ls[3].quadratic = 0.20;

    // Rotes Licht
    ls[4].position = lightPos5;
    ls[4].color = {1.0f, 0.0f, 0.0f};
    ls[4].ka = 0.8;
    ls[4].kd = 0.8;
    ls[4].ks = 1.0;
    ls[4].constant = 1.0;
    ls[4].linear = 0.22;
    ls[4].quadratic = 0.20;

    glGenBuffers(1, &uboLights);
    glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
    glBufferData(GL_UNIFORM_BUFFER, NUM_LS * sizeof(LightSource), nullptr, GL_STATIC_DRAW ); // Set Buer size, 64 Byte for each LS
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboLights);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    /*loader.loadObjectFromFile(":/objects/gimbal.obj");
    Q_ASSERT(loader.hasScene());

    GLfloat vbodata[loader.lengthOfVBO()];
    loader.genVBO(vbodata);

    GLuint ibodata[loader.lengthOfIndexArray()];
    loader.genIndexArray(ibodata);*/

    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glEnable(GL_CULL_FACE);

   /* glGenVertexArrays(1, &m_vao); //m_vao um elemente in m_vbo richtig zu unterteilen
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vbodata), vbodata, GL_STATIC_DRAW);

    glGenBuffers(1, &m_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ibodata), ibodata, GL_STATIC_DRAW);

    #define OFS(s,a) reinterpret_cast<void* const>(offsetof(s,a))

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), OFS(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), OFS(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), OFS(Vertex, texture));

    #undef OFS*/

    // Textur aus Datei in img laden
    QImage img;
    img.load(":/gimbal_wood.jpg");
    Q_ASSERT(!img.isNull());

    // Texturobjekt erstellen
    glGenTextures(1, &m_tex);
    glBindTexture(GL_TEXTURE_2D, m_tex);

    // Texturmodus auf Repeat stellen
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Mit Textur füllen
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, img.width(), img.height(),
    0, GL_BGRA, GL_UNSIGNED_BYTE, img.bits());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    skybox.initializeGL();

    mp_program = new QOpenGLShaderProgram();
    mp_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/default.vert");
    mp_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/default.frag");
    Q_ASSERT(mp_program->link());

    mp_program_light = new QOpenGLShaderProgram();
    mp_program_light->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/light.vert");
    mp_program_light->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/light.frag");
    Q_ASSERT(mp_program_light->link());
}

void MyGLWidget::resizeGL(int w, int h) {
    m_Aspect = (double)w/(double)h;
    updateProjMat();
}

void MyGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);

    skybox.render(projecMat.transposed(), cameraMat);

    glBindBuffer(GL_UNIFORM_BUFFER, uboLights); //Bind Buffer
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboLights);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ls), ls); // Übergebe LS an den Shader

    // glBindVertexArray(m_vao);
    glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, m_tex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.getHandle());

    // Color-Shaderprogramm Binden, Zeichnen und Releasen
    mp_program->bind();
    //mp_program->setUniformValue(mp_program->uniformLocation("uLightPosition"), lightPos);
    mp_program->setUniformValue(8, cameraMat);
    mp_program->setUniformValue(10, (float)iAmbient);
    mp_program->setUniformValue(11, (float)iDiffuse);
    mp_program->setUniformValue(12, (float)iSpecular);
    mp_program->setUniformValue(13, m_FOV);
    mp_program->setUniformValue(4, projecMat.transposed());
    mp_program->setUniformValue(5, cameraMat);
    mp_program->setUniformValue(3, uRotMatOuter.transposed());
    //glDrawElements(GL_TRIANGLES, loader.lengthOfIndexArray(), GL_UNSIGNED_INT, nullptr);
    // Gimbal-Material: Gold
    //QVector3D ambientMaterial = QVector3D(0.24725, 0.1995, 0.0745);
    //QVector3D diffuseMaterial = QVector3D(0.75164, 0.60648, 0.22648);
    //QVector3D specularMaterial = QVector3D(0.628281, 0.555802, 0.366065);
    QVector3D ambientMaterial = QVector3D(0.25, 0.25, 0.25);
    QVector3D diffuseMaterial = QVector3D(0.4, 0.4, 0.4);
    QVector3D specularMaterial = QVector3D(0.774597, 0.774597, 0.774597);
    float shininess = 0.6;
    int structLocation = 100;
    mp_program->setUniformValue(structLocation, ambientMaterial);
    mp_program->setUniformValue(structLocation + 1, diffuseMaterial);
    mp_program->setUniformValue(structLocation + 2, specularMaterial);
    mp_program->setUniformValue(structLocation + 3, shininess);
    gimbal.drawElements();
    mp_program->setUniformValue(3, uRotMatMiddle.transposed());
    //glDrawElements(GL_TRIANGLES, loader.lengthOfIndexArray(), GL_UNSIGNED_INT, nullptr);
    gimbal.drawElements();
    mp_program->setUniformValue(3, uRotMatOuter.transposed());
    gimbal.drawElements();
    ballA = uRotMatMiddle.transposed();
    ballA.scale(1/.45);
    ballA.rotate(++counter,{0,0,1});
    ballA.translate({0,.52,0});
    ballA.scale(0.03);
    ballA.rotate(counter * 3.5,{0,0,1});

    // Ball-Material: Schwarzer Gummi
    ambientMaterial = QVector3D(0.02, 0.02, 0.02);
    diffuseMaterial = QVector3D(0.01, 0.01, 0.01);
    specularMaterial = QVector3D(0.4, 0.4, 0.4);
    shininess = 0.078125;
    mp_program->setUniformValue(structLocation, ambientMaterial);
    mp_program->setUniformValue(structLocation + 1, diffuseMaterial);
    mp_program->setUniformValue(structLocation + 2, specularMaterial);
    mp_program->setUniformValue(structLocation + 3, shininess);
    mp_program->setUniformValue(3, ballA);
    sphere.drawElements();
    //glDrawElements(GL_TRIANGLES, loader.lengthOfIndexArray(), GL_UNSIGNED_INT, nullptr);
    mp_program->release();

    mp_program_light->bind();
    mp_program_light->setUniformValue(18, projecMat.transposed());
    mp_program_light->setUniformValue(19, cameraMat);

    // Licht 1 rendern
    mp_program_light->setUniformValue(20, lightModel1);
    mp_program_light->setUniformValue(22, ls[0].color);
    light.drawElements();

    // Licht 2 rendern
    mp_program_light->setUniformValue(20, lightModel2);
    mp_program_light->setUniformValue(22, ls[1].color);
    light.drawElements();

    // Licht 3 rendern
    mp_program_light->setUniformValue(20, lightModel3);
    mp_program_light->setUniformValue(22, ls[2].color);
    light.drawElements();

    // Licht 4 rendern
    mp_program_light->setUniformValue(20, lightModel4);
    mp_program_light->setUniformValue(22, ls[3].color);
    light.drawElements();

    // Licht 5 rendern
    mp_program_light->setUniformValue(20, lightModel5);
    mp_program_light->setUniformValue(22, ls[4].color);
    light.drawElements();

    // Licht 1 animieren
    lightModel1 = lightModel1.transposed();
    lightModel1.rotate(1.8, {0,1,0});
    lightModel1 = lightModel1.transposed();
    lightPos1.setX(lightModel1.row(0)[3]);
    lightPos1.setZ(lightModel1.row(2)[3]);

    // Licht 2 animieren
    lightModel2 = lightModel2.transposed();
    lightModel2.rotate(1.8, {1,0,0});
    lightModel2 = lightModel2.transposed();
    lightPos2.setX(lightModel2.row(0)[3]);
    lightPos2.setZ(lightModel2.row(2)[3]);

    // Licht 4 animieren
    lightModel4 = lightModel4.transposed();
    lightModel4.rotate(1.8, {0,1,0});
    lightModel4 = lightModel4.transposed();
    lightPos4.setX(lightModel4.row(0)[3]);
    lightPos4.setZ(lightModel4.row(2)[3]);

    // Licht 5 animieren
    lightModel5 = lightModel5.transposed();
    lightModel5.rotate(1.8, {0,1,0});
    lightModel5 = lightModel5.transposed();
    lightPos5.setX(lightModel5.row(0)[3]);
    lightPos5.setZ(lightModel5.row(2)[3]);

    // Schicke neue Position an Shader
    ls[0].position = lightPos1;
    ls[1].position = lightPos2;
    ls[2].position = lightPos3;
    ls[3].position = lightPos4;
    ls[4].position = lightPos5;
    mp_program_light->release();


  /*  // Color-Shaderprogramm binden, zeichnen und releasen
    mp_program->bind();
    // mp_program->setUniformValue(1, TextureMod);
    mp_program->setUniformValue(4, projecMat.transposed());
    mp_program->setUniformValue(5, cameraMat);
    mp_program->setUniformValue(3, uRotMatOuter.transposed());
    // glDrawElements(GL_TRIANGLES, loader.lengthOfIndexArray(), GL_UNSIGNED_INT, nullptr);
    gimbal.drawElements();

    mp_program->setUniformValue(3, uRotMatMiddle.transposed());
    // glDrawElements(GL_TRIANGLES, loader.lengthOfIndexArray(), GL_UNSIGNED_INT, nullptr);
    gimbal.drawElements();

    mp_program->setUniformValue(3, uRotMatInner.transposed());
    // glDrawElements(GL_TRIANGLES, loader.lengthOfIndexArray(), GL_UNSIGNED_INT, nullptr);
    gimbal.drawElements();

    //sphere movement
    ballA = uRotMatMiddle.transposed();
    ballA.scale(1/.65); // Ebene des Balls auf 1 bringen
    ballA.rotate(++counter,{0,0,1}); // Ebene drehen
    ballA.translate({0,0.75,0}); // Ball nach oben verschieben
    ballA.scale(0.05); // verkleinern des Balls
    ballA.rotate(counter * 3.5,{0,0,1}); // Ball um eigene Achse rotieren

    mp_program->setUniformValue(3, ballA);
    sphere.drawElements();
    //glDrawElements(GL_TRIANGLES, loader.lengthOfIndexArray(), GL_UNSIGNED_INT, nullptr);
    mp_program->release();

    // P4 uniform
//    layout (location = 8) uniform vec3 uViewPosition;
//    layout (location = 9) uniform vec3 uLightPosition;
//    layout (location = 10) uniform float uKa; // Ambient
//    layout(location = 11) uniform float uKd; //Diffuse
//    layout(location = 12) uniform float uKs; // Specular
//    layout(location = 13) uniform float uShininess;
    mp_program->setUniformValue(8, m_CameraPos);
    mp_program->setUniformValue(9, QVector3D(0.0, 0.0, 0.0));
    mp_program->setUniformValue(10, iAmbient);
    mp_program->setUniformValue(11, iDiffuse);
    mp_program->setUniformValue(12, iSpecular);
    mp_program->setUniformValue(13, theShine);
*/
    //Animation Rotation
    if(animationActive){
        setRotationA(m_RotationA + 1);
        setRotationB(m_RotationB + 2);
        setRotationC(m_RotationC + 3);
    }


    //set cam as inner gimbal
    if(m_GimbalCam){
        cameraMat = uRotMatInner;
        //cameraMat.transposed();
    }

    // glBindVertexArray(0);
    this->update();
}

void MyGLWidget::finalize() {
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ibo);
    glDeleteVertexArrays(1, &m_vao);
    glDeleteTextures(1, &m_tex);
    delete mp_program;
}

