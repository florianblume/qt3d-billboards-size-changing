// Includes for framegraph
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DRender/QFrameGraphNode>
#include <Qt3DRender/QRenderSurfaceSelector>
#include <Qt3DRender/QViewport>
#include <Qt3DRender/QCameraSelector>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QCameraLens>
#include <Qt3DRender/QClearBuffers>
#include <Qt3DExtras/QForwardRenderer>

#include <Qt3DExtras/QFirstPersonCameraController>
#include <Qt3DInput/QInputSettings>

#include <Qt3DCore/QEntity>
#include <Qt3DExtras/QPlaneMesh>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DCore/QTransform>
#include <Qt3DRender/QMaterial>
#include <Qt3DRender/QParameter>
#include <Qt3DRender/QTexture>
#include <Qt3DRender/QTextureImage>
#include <Qt3DRender/QEffect>
#include <Qt3DRender/QTechnique>
#include <Qt3DRender/QRenderPass>
#include <Qt3DRender/QShaderProgram>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DRender/QGraphicsApiFilter>
#include <Qt3DExtras/QTextureMaterial>

#include <Qt3DRender/QAttribute>

#include <QSurfaceFormat>

#include <QVector3D>
#include <QColor>

#include <QGuiApplication>

#include "billboardgeometry.h"

#include <QOpenGLContext>

Qt3DExtras::QFirstPersonCameraController * cameraController;

int windowWidth = 1600;
int windowHeight = 800;

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    Qt3DCore::QEntity *root = new Qt3DCore::QEntity();

    cameraController = new Qt3DExtras::QFirstPersonCameraController(root);

    // Add plane
    Qt3DCore::QEntity *planeEntity = new Qt3DCore::QEntity(root);
    Qt3DExtras::QPlaneMesh *planeMesh = new Qt3DExtras::QPlaneMesh(planeEntity);
    planeMesh->setWidth(20);
    planeMesh->setHeight(20);
    Qt3DExtras::QPhongMaterial *planeMaterial = new Qt3DExtras::QPhongMaterial(planeEntity);
    planeMaterial->setAmbient(QColor(0, 0, 0.7 * 255, 0.1 * 255));
    planeEntity->addComponent(planeMesh);
    planeEntity->addComponent(planeMaterial);

    // Add sphere
    Qt3DCore::QEntity *sphereEntity = new Qt3DCore::QEntity(root);
    Qt3DExtras::QSphereMesh *sphereMesh = new Qt3DExtras::QSphereMesh(sphereEntity);
    Qt3DExtras::QPhongMaterial *sphereMaterial = new Qt3DExtras::QPhongMaterial(sphereEntity);
    sphereMaterial->setAmbient(Qt::red);
    Qt3DCore::QTransform *sphereTransform = new Qt3DCore::QTransform(sphereEntity);
    sphereTransform->setTranslation(QVector3D(0., 5., 0.));
    sphereEntity->addComponent(sphereMesh);
    sphereEntity->addComponent(sphereMaterial);
    sphereEntity->addComponent(sphereTransform);

    // Add cube
    Qt3DCore::QEntity *cubeEntity = new Qt3DCore::QEntity(root);
    Qt3DExtras::QCuboidMesh *cubeMesh = new Qt3DExtras::QCuboidMesh(cubeEntity);
    Qt3DExtras::QPhongMaterial *cubeMaterial = new Qt3DExtras::QPhongMaterial(cubeEntity);
    cubeMaterial->setAmbient(Qt::gray);
    Qt3DCore::QTransform *cubeTransform = new Qt3DCore::QTransform();
    cubeTransform->setTranslation(QVector3D(2., 2., 5.));
    cubeEntity->addComponent(cubeMesh);
    cubeEntity->addComponent(cubeMaterial);
    cubeEntity->addComponent(cubeTransform);

    // Add Billboard
    Qt3DCore::QEntity *billboardEntity = new Qt3DCore::QEntity(root);

    // Create billboard geometry
    QVector<QVector3D> pos;
    pos << QVector3D(1, 1, 0);
    pos << QVector3D(-1, 2, 4);
    pos << QVector3D(1, 1, 1);
    pos << QVector3D(0, 7, 2);
    pos << QVector3D(0, 10, 0);
    pos << QVector3D(3, 0, 3);
    pos << QVector3D(0, 3, 2);
    BillboardGeometry *billboardGeometry = new BillboardGeometry(billboardEntity);
    billboardGeometry->setPoints(pos);
    Qt3DRender::QGeometryRenderer *billboardRenderer = new Qt3DRender::QGeometryRenderer(billboardEntity);
    billboardRenderer->setGeometry(billboardGeometry);
    billboardRenderer->setInstanceCount(billboardGeometry->count());

    Qt3DCore::QTransform *billboardTransform = new Qt3DCore::QTransform(billboardEntity);
    billboardTransform->setTranslation(QVector3D(0., 4.5, 0.));
    billboardTransform->setRotationX(90);

    // Billboard material

    // Image of billboard material
    Qt3DRender::QMaterial *billboardMaterial = new Qt3DRender::QMaterial(billboardEntity);
    QMatrix4x4 inst = QMatrix4x4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
    Qt3DRender::QParameter *billboardParam1 = new Qt3DRender::QParameter(QStringLiteral("inst"), inst);
    Qt3DRender::QParameter *billboardParam2 = new Qt3DRender::QParameter(QStringLiteral("instNormal"), BillboardGeometry::normalMatrix((QMatrix4x4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1))));
    billboardMaterial->addParameter(billboardParam1);
    billboardMaterial->addParameter(billboardParam2);
    Qt3DRender::QTexture2D* texture = new Qt3DRender::QTexture2D();
    Qt3DRender::QTextureImage* textureImage = new Qt3DRender::QTextureImage(texture);
    textureImage->setSource(QUrl(QStringLiteral("qrc:/success-kid.png")));
    texture->addTextureImage(textureImage);
    Qt3DRender::QParameter* billboardParam3 = new Qt3DRender::QParameter(QStringLiteral("tex0"), texture);
    billboardMaterial->addParameter(billboardParam3);
    Qt3DRender::QParameter *cameraPosParameter = new Qt3DRender::QParameter(QStringLiteral("cameraPos"), QVector3D(0, 0, 0));
    Qt3DRender::QParameter *cameraUpParameter = new Qt3DRender::QParameter(QStringLiteral("cameraUp"), QVector3D(0, 0, 0));
    billboardMaterial->addParameter(cameraPosParameter);
    billboardMaterial->addParameter(cameraUpParameter);

    // Parameters of billboard material
    /*
    Qt3DRender::QParameter* billboardParam2 = new Qt3DRender::QParameter(QStringLiteral("WIN_SCALE"), QSize(1600, 800));
    Qt3DRender::QParameter* billboardParam3 = new Qt3DRender::QParameter(QStringLiteral("BB_SIZE"), QSize(100, 100));
    billboardMaterial->addParameter(billboardParam1);
    billboardMaterial->addParameter(billboardParam2);
    billboardMaterial->addParameter(billboardParam3);
    */

    // Effect of material
    Qt3DRender::QEffect* billboardEffect = new Qt3DRender::QEffect();
    Qt3DRender::QTechnique* billboardTechnique = new Qt3DRender::QTechnique();
    billboardTechnique->graphicsApiFilter()->setApi(Qt3DRender::QGraphicsApiFilter::OpenGL);
    billboardTechnique->graphicsApiFilter()->setProfile(Qt3DRender::QGraphicsApiFilter::CoreProfile);
    billboardTechnique->graphicsApiFilter()->setMajorVersion(3);
    billboardTechnique->graphicsApiFilter()->setMinorVersion(1);
    // You need the filter key because the QForwardRenderer employed as the default framegraph by the Qt3DWindow
    // extends QTechniqueFilter and filters for this key exactly. Without it, the material gets discarded.
    Qt3DRender::QFilterKey* filterKey = new Qt3DRender::QFilterKey(billboardMaterial);
    filterKey->setName(QStringLiteral("renderingStyle"));
    filterKey->setValue(QStringLiteral("forward"));
    billboardTechnique->addFilterKey(filterKey);
    Qt3DRender::QRenderPass* billboardRenderPass = new Qt3DRender::QRenderPass();
    Qt3DRender::QShaderProgram* billboardShaderProgram = new Qt3DRender::QShaderProgram();
    billboardShaderProgram->setVertexShaderCode(Qt3DRender::QShaderProgram::loadSource(QUrl("qrc:/shaders/billboards.vert")));
    //billboardShaderProgram->setGeometryShaderCode(Qt3DRender::QShaderProgram::loadSource(QUrl("qrc:/shaders/billboards.geom")));
    billboardShaderProgram->setFragmentShaderCode(Qt3DRender::QShaderProgram::loadSource(QUrl("qrc:/shaders/billboards.frag")));
    billboardRenderPass->setShaderProgram(billboardShaderProgram);
    billboardTechnique->addRenderPass(billboardRenderPass);
    billboardEffect->addTechnique(billboardTechnique);
    billboardMaterial->setEffect(billboardEffect);

    billboardEntity->addComponent(billboardRenderer);
    billboardEntity->addComponent(billboardMaterial);
    billboardEntity->addComponent(billboardTransform);

    Qt3DExtras::Qt3DWindow view;
    view.resize(windowWidth, windowHeight);
    Qt3DExtras::QForwardRenderer *renderer = (Qt3DExtras::QForwardRenderer *)view.activeFrameGraph();
    renderer->setClearColor("black");

    Qt3DRender::QCamera *camera = view.camera();
    QObject::connect(camera, &Qt3DRender::QCamera::upVectorChanged, [camera, cameraUpParameter](){cameraUpParameter->setValue(camera->upVector());});
    QObject::connect(camera, &Qt3DRender::QCamera::positionChanged, [camera, cameraPosParameter](){cameraPosParameter->setValue(camera->position());});
    camera->setProjectionType(Qt3DRender::QCameraLens::PerspectiveProjection);
    camera->setFieldOfView(45);
    // Cast to float to ensure float division
    camera->setAspectRatio(windowWidth / (float) windowHeight);
    camera->setNearPlane(0.1f);
    camera->setFarPlane(100.f);
    camera->setPosition(QVector3D(0., 10., 20.));
    camera->setViewCenter(QVector3D(0., 0., 0.));
    camera->setUpVector(QVector3D(0., 1., 0.));

    view.setRootEntity(root);
    cameraController->setCamera(camera);

    view.setTitle("Billboards");
    view.show();

    return app.exec();
}
