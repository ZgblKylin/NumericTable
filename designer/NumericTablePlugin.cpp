#include "NumericTablePlugin.h"
#include <NumericTable>

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(NumericTablePlugin, NumericTablePlugin)
#endif // QT_VERSION < 0x050000

NumericTablePlugin::NumericTablePlugin(QObject* parent)
    : QObject(parent)
{
    widgets.append(new NumericTablePluginInterface(this));
}

QList<QDesignerCustomWidgetInterface*> NumericTablePlugin::customWidgets() const
{
    return widgets;
}

NumericTablePluginInterface::NumericTablePluginInterface(QObject* parent)
    : QObject(parent)
{
}

QString NumericTablePluginInterface::name() const
{
    return QStringLiteral("NumericTable");
}

QString NumericTablePluginInterface::group() const
{
    return QString();
}

QString NumericTablePluginInterface::toolTip() const
{
    return QStringLiteral("Table view used to display huge amount of numeric values");
}

QString NumericTablePluginInterface::whatsThis() const
{
    return QStringLiteral("Table view used to display huge amount of numeric values");
}

QString NumericTablePluginInterface::includeFile() const
{
    return QStringLiteral("NumericTable");
}

QIcon NumericTablePluginInterface::icon() const
{
    return QIcon();
}

bool NumericTablePluginInterface::isContainer() const
{
    return false;
}

QWidget* NumericTablePluginInterface::createWidget(QWidget* parent)
{
    return new NumericTable(parent);
}

bool NumericTablePluginInterface::isInitialized() const
{
    return initialized;
}

void NumericTablePluginInterface::initialize(QDesignerFormEditorInterface* /*core*/)
{
    if (initialized) {
        return;
    }

    // Add extension registrations, etc. here

    initialized = true;
}
