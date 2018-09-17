#ifndef NUMERICTABLEPLUGIN_H
#define NUMERICTABLEPLUGIN_H

#include <QtCore/QtGlobal>
#include <QtDesigner/QtDesigner>
#include <QtCore/QtPlugin>

#if QT_VERSION < 0x050700
#include <QtDesigner/QDesignerCustomWidgetInterface>
#else
#include <QtUiPlugin/QDesignerCustomWidgetInterface>
#endif

class NumericTablePlugin : public QObject, public QDesignerCustomWidgetCollectionInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetCollectionInterface")
#endif // QT_VERSION >= 0x050000

public:
    explicit NumericTablePlugin(QObject *parent = nullptr);

    virtual QList<QDesignerCustomWidgetInterface*> customWidgets() const;

private:
    QList<QDesignerCustomWidgetInterface*> widgets;
};

class NumericTablePluginInterface : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    explicit NumericTablePluginInterface(QObject* parent = nullptr);

    // QDesignerCustomWidgetInterface interface
    virtual QString name() const override;
    virtual QString group() const override;
    virtual QString toolTip() const override;
    virtual QString whatsThis() const override;
    virtual QString includeFile() const override;
    virtual QIcon icon() const override;
    virtual bool isContainer() const override;
    virtual QWidget* createWidget(QWidget* parent) override;
    virtual bool isInitialized() const override;
    virtual void initialize(QDesignerFormEditorInterface* core) override;

private:
    bool initialized = false;
};

#endif // NUMERICTABLEPLUGIN_H
