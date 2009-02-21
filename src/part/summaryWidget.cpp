/***********************************************************************
* Copyright 2003-2004  Max Howell <max.howell@methylblue.com>
* Copyright 2008-2009  Martin Sandsmark <sandsmark@samfundet.no>
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation; either version 2 of
* the License or (at your option) version 3 or any later version
* accepted by the membership of KDE e.V. (or its successor approved
* by the membership of KDE e.V.), which shall act as a proxy
* defined in Section 14 of version 3 of the license.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
***********************************************************************/

#include "summaryWidget.h"

#include "Config.h"
#include "fileTree.h"
#include "radialMap/radialMap.h"
#include "radialMap/widget.h"
#include "summaryWidget.moc"

#include <KCursor>
#include <KDebug>
#include <KIconEffect> //MyRadialMap::mousePressEvent()
#include <KIconLoader>
#include <KIcon>
#include <KLocale>

#include <Solid/Device>
#include <Solid/StorageAccess>

#include <KDiskFreeSpaceInfo>

#include <QLabel>
#include <QLayout>
#include <QApplication>
#include <QByteArray>
#include <Q3ValueList>
#include <QMouseEvent>
#include <QTextOStream>


static Filelight::MapScheme oldScheme;


struct Disk
{
    QString mount;
    QString icon;

    int size;
    int used;
    int free; //NOTE used+avail != size (clustersize!)
};


struct DiskList : Q3ValueList<Disk>
{
    DiskList();
};


class MyRadialMap : public RadialMap::Widget
{
public:
    MyRadialMap(QWidget *parent)
            : RadialMap::Widget(parent)
    {
    }

    virtual void setCursor(const QCursor &c)
    {
        if (focusSegment() && focusSegment()->file()->name() == "Used")
            RadialMap::Widget::setCursor(c);
        else
            unsetCursor();
    }

    virtual void mousePressEvent(QMouseEvent *e)
    {
        const RadialMap::Segment *segment = focusSegment();

        //we will allow right clicks to the center circle
        if (segment == rootSegment())
            RadialMap::Widget::mousePressEvent(e);

        //and clicks to the used segment
        else if (segment && segment->file()->name() == "Used") {
            const QRect rect(e->x() - 20, e->y() - 20, 40, 40);
//            KIconEffect::visualActivate(this, rect); TODO: Re-enable
            emit activated(url());
        }
    }
};



SummaryWidget::SummaryWidget(QWidget *parent)
        : QWidget(parent)
{
    qApp->setOverrideCursor(Qt::WaitCursor);
    setLayout(new QGridLayout(this));
    createDiskMaps();
    qApp->restoreOverrideCursor();
}

SummaryWidget::~SummaryWidget()
{
    Config::scheme = oldScheme;
}

void SummaryWidget::createDiskMaps()
{
    DiskList disks;

    const QByteArray free = i18n("Free").toLocal8Bit();
    const QByteArray used = i18n("Used").toLocal8Bit();

    KIconLoader loader;

    oldScheme = Config::scheme;
    Config::scheme = (Filelight::MapScheme)2000;

    for (DiskList::ConstIterator it = disks.begin(), end = disks.end(); it != end; ++it)
    {
        Disk const &disk = *it;

        if (disk.free == 0 && disk.used == 0)
            continue;

        QVBoxLayout * lay = new QVBoxLayout(this);
        RadialMap::Widget *map = new MyRadialMap(this);

        QHBoxLayout* horizontalLayout = new QHBoxLayout(this);

        // Create the text and icon under the radialMap.
        QLabel *label = new QLabel(disk.mount, this);
        horizontalLayout->addWidget(label);
        QLabel *icon = new QLabel(this);
        icon->setPixmap(KIcon(disk.icon).pixmap(16,16));
        horizontalLayout->addWidget(icon);

        horizontalLayout->setAlignment(Qt::AlignCenter);
        lay->addWidget(map);
        lay->addItem(horizontalLayout);

        if (!(layout()->count() % 2)) {
            qobject_cast<QGridLayout*>(layout())->addItem(lay, 0, layout()->count() / 2);
        }
        else {
            qobject_cast<QGridLayout*>(layout())->addItem(lay, 1, (int) layout()->count() / 2);
        }

        Directory *tree = new Directory(disk.mount.toLocal8Bit());
        tree->append(free, disk.free);
        tree->append(used, disk.used);

        map->create(tree); //must be done when visible

        connect(map, SIGNAL(activated(const KUrl&)), SIGNAL(activated(const KUrl&)));
    }
}

DiskList::DiskList()
{
    const Solid::StorageAccess *partition;

    foreach (const Solid::Device &device, Solid::Device::listFromType(Solid::DeviceInterface::StorageAccess))
    { // Iterate over all the partitions available.

        if (!device.is<Solid::StorageAccess>()) continue; // It happens.

        partition = device.as<Solid::StorageAccess>();
        if (!partition->isAccessible()) continue;

        KDiskFreeSpaceInfo info = KDiskFreeSpaceInfo::freeSpaceInfo(partition->filePath());
        if (!info.isValid()) continue;

        Disk disk;
        disk.mount = partition->filePath();
        disk.icon = device.icon();
        disk.size = info.size() / 1024; // All sizes are in 1kb large blocks
        disk.free = info.available() / 1024;
        disk.used = info.used() / 1024;

        *this += disk;
    }
}
