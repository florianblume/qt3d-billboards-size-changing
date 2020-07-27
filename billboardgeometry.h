#ifndef BILLBOARDGEOMETRY_H
#define BILLBOARDGEOMETRY_H

#include <Qt3DRender/QGeometry>
#include <Qt3DRender/QBuffer>
#include <Qt3DExtras/QPlaneGeometry>

#include <QVector3D>

#include <QMatrix4x4>

class BillboardGeometry : public Qt3DExtras::QPlaneGeometry
{
    Q_OBJECT

    Q_PROPERTY(int count READ count NOTIFY countChanged)

  public:
    BillboardGeometry( Qt3DCore::QNode *parent = nullptr );

    void setPoints( const QVector<QVector3D> &vertices );

    int count();

    Q_INVOKABLE static QMatrix4x4 normalMatrix(QMatrix4x4 mat);

  signals:
      void countChanged(int count);

  private:
    Qt3DRender::QAttribute *mPositionAttribute = nullptr;
    Qt3DRender::QBuffer *mInstanceBuffer = nullptr;
    int mInstanceCount = 0;
};

#endif // BILLBOARDGEOMETRY_H
