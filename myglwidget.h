#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H
#define NUM_LS 5

#include <QWidget>
#include <QOpenGLWidget>
#include <QVector3D>
#include <QKeyEvent>
#include <QDebug>
#include <QOpenGLFunctions_4_4_Core>
#include <QOpenGLShaderProgram>
#include "modelloader.h"
#include <QElapsedTimer>
#include "mySkybox.h"
#include <QtMath>
#include "model.h"

class MyGLWidget : public QOpenGLWidget, QOpenGLFunctions_4_4_Core{
    Q_OBJECT
    private:
        int m_FOV = 45;
        int m_Angle = 0;
        bool m_ProjectionMode;
        double m_Near = 2.0;
        double m_Far = 0.0;
        int m_RotationA;
        int m_RotationB;
        int m_RotationC;
        int m_RotationT;
        float counter2 = .2;
        bool location;

        GLfloat uAlpha;
        float TextureMod;
        GLuint m_tex;

        Model gimbal, sphere;

        QMatrix4x4 projecMat;
        QVector3D axisA, axisB, axisC;

        QMatrix4x4 uRotMatOuter;
        QMatrix4x4 uRotMatMiddle;
        QMatrix4x4 uRotMatInner;

        QMatrix4x4 cameraMat; //view
        QVector3D m_CameraPos;
        QVector2D m_CameraAngle;
        bool m_GimbalCam = false;

        ModelLoader loader;
        QElapsedTimer timer;
        bool animationActive = false;

        MySkybox skybox;

        double m_Aspect;

        QMatrix4x4 ballA;
        GLuint m_tex_ball;
        GLuint m_vbo_ball;
        GLuint m_vao_ball;
        GLuint m_ibo_ball;
        int counter;

        QOpenGLShaderProgram* mp_program;
        QOpenGLShaderProgram* mp_programC;
        QOpenGLShaderProgram* mp_program_light;
        GLuint m_vbo;
        GLuint m_vao;
        GLuint m_ibo;
        unsigned int uboLights;

        struct LightSource { // padding da trotz vec3 die Größe von vec4 reserviert wird
            QVector3D position;
            float pad0;
            QVector3D color;
            float pad1;
            float ka;
            float kd;
            float ks;
            float pad2;
            float constant;
            float linear;
            float quadratic;
            float pad3;
        };

        LightSource ls[NUM_LS];
        QVector3D lightPos1 = QVector3D(8.0, 0.0, 8.0) * 2;
        QVector3D lightPos2 = QVector3D(0.0, 8.0, 8.0) * 2;
        QVector3D lightPos3 = QVector3D(0.0, 0.0, 0.0) * 2;
        QVector3D lightPos4 = QVector3D(-8.0, 0.0, -8.0) * 2;
        QVector3D lightPos5 = QVector3D(2.0, -10.0, -1.0) * 2;
        QMatrix4x4 lightModel1;
        QMatrix4x4 lightModel2;
        QMatrix4x4 lightModel3;
        QMatrix4x4 lightModel4;
        QMatrix4x4 lightModel5;
        Model light;

        QVector3D oberflaechennormale,
                    lichtrichtung,
                    betrachterrichtung,
                    reflektionsrichtung;

        double iAmbient = 0.0, iDiffuse = 0.0, iSpecular = 0.0;

        QVector3D betrachterposition, lichtposition;

        int theShine;

    public:
        MyGLWidget(QWidget *parent);
        ~MyGLWidget();
        void keyPressEvent(QKeyEvent *event);
        int getAngle();
        int getFOV();

    public slots:
        void setFOV(int value);
        void setAngle(int value);
        void setProjectionMode();
        void setNear(double value);
        void setFar(double value);
        void setRotationA(int value);
        void setRotationB(int value);
        void setRotationC(int value);
        void setRotationT(int value);
        void moveTexture(int value);
        void setAnimation(bool value);
        void setGimbalCamera(bool gimbalCam);
        void updateProjMat();
        void updateCamera();

    signals:
        void farValueChanged(int value);
        void nearValueChanged(int value);


        // QOpenGLWidget interface
protected:
        void initializeGL();
        void resizeGL(int w, int h);
        void paintGL();
        void finalize();
};

#endif // MYGLWIDGET_H
