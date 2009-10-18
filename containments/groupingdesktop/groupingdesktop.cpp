/******************************************************************************
*                                   Container                                 *
*******************************************************************************
*                                                                             *
*        Copyright (C) 2009 Giulio Camuffo <giuliocamuffo@gmail.com>          *
*                                                                             *
*   This program is free software; you can redistribute it and/or modify      *
*   it under the terms of the GNU General Public License as published by      *
*   the Free Software Foundation; either version 2 of the License, or         *
*   (at your option) any later version.                                       *
*                                                                             *
*   This program is distributed in the hope that it will be useful,           *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of            *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
*   GNU General Public License for more details.                              *
*                                                                             *
*   You should have received a copy of the GNU General Public License along   *
*   with this program; if not, write to the Free Software Foundation, Inc.,   *
*   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA                *
*                                                                             *
******************************************************************************/

#include "groupingdesktop.h"

#include <QtGui/QGraphicsSceneContextMenuEvent>
#include <QAction>

#include <KDebug>

#include "abstractgroup.h"

K_EXPORT_PLASMA_APPLET(groupingdesktop, GroupingDesktop)

GroupingDesktop::GroupingDesktop(QObject* parent, const QVariantList& args)
               : Containment(parent, args)
{
    setContainmentType(Plasma::Containment::DesktopContainment);
}

GroupingDesktop::~GroupingDesktop()
{

}

void GroupingDesktop::init()
{
    Plasma::Containment::init();

    connect(this, SIGNAL(appletAdded(Plasma::Applet*, QPointF)),
            this, SLOT(layoutApplet(Plasma::Applet*, QPointF)));

    m_newGridLayout = new QAction(this);
    m_newGridLayout->setText(i18n("Add a new grid layout"));
    m_removeGroup = new QAction(this);
    m_removeGroup->setText(i18n("Remove this group"));
    connect (m_newGridLayout, SIGNAL(triggered()),
             this, SLOT(newGridLayoutClicked()));
    connect (m_removeGroup, SIGNAL(triggered()),
             this, SLOT(removeGroup()));

    addToolBoxAction(m_newGridLayout);
    newGridLayoutClicked();
}

void GroupingDesktop::newGridLayoutClicked()
{
    AbstractGroup *group = createGroup("gridlayout", 0);
    if (group) {
        group->setGeometry(20,20,400,400);

        emit configNeedsSaving();
    }
}

QList<QAction *> GroupingDesktop::contextualActions()
{
    QList<QAction *> list;
    list << m_newGridLayout << m_removeGroup;
    return list;
}

void GroupingDesktop::layoutApplet(Plasma::Applet *applet, const QPointF &pos)
{
    foreach (AbstractGroup *group, m_groups) {
        if (group->geometry().contains(pos)) {
            group->assignApplet(applet);
        }
    }
}

AbstractGroup *GroupingDesktop::createGroup(const QString &plugin, int id)
{
    Plasma::Applet *applet = Plasma::Applet::load(plugin, id);
    AbstractGroup *group = dynamic_cast<AbstractGroup *>(applet);

    if (!group) {
        delete applet;
        return 0;
    }

    group->setParent(this);
    group->setParentItem(this);
    group->init();
    group->updateConstraints(Plasma::StartupCompletedConstraint);
    group->flushPendingConstraintsEvents();
    group->updateConstraints(Plasma::AllConstraints);
    group->flushPendingConstraintsEvents();
    group->setZValue(-100000000); //maybe FIXME ?

    m_groups.insert(id, group);

    return group;
}

void GroupingDesktop::removeGroup()
{
    int id = m_removeGroup->data().toInt();

    m_groups.value(id)->destroy();
}

void GroupingDesktop::save(KConfigGroup &group) const
{
    if (!group.isValid()) {
        group = config();
    }

    Plasma::Containment::save(group);

    KConfigGroup groupsConfig(&group, "Groups");
    foreach (AbstractGroup *group, m_groups) {
        KConfigGroup groupConfig(&groupsConfig, QString::number(m_groups.key(group)));
        groupConfig.writeEntry("Plugin", group->pluginName());
        groupConfig.writeEntry("Geometry", group->geometry());
    }
}

void GroupingDesktop::saveContents(KConfigGroup& group) const
{
    Containment::saveContents(group);

    KConfigGroup appletsConfig(&group, "Applets");
    foreach (AbstractGroup *group, m_groups) {
        foreach (Plasma::Applet *applet, group->assignedApplets()) {
            KConfigGroup appletConfig(&appletsConfig, QString::number(applet->id()));
            KConfigGroup groupConfig(&appletConfig, "GroupInformation");
            groupConfig.writeEntry("Group", group->id());
            group->saveAppletLayoutInfo(applet, groupConfig);
        }
    }
}

void GroupingDesktop::restore(KConfigGroup& group)
{
    Containment::restore(group);

    KConfigGroup groupsConfig(&group, "Groups");
    foreach (QString groupId, groupsConfig.groupList()) {
        int id = groupId.toInt();
        KConfigGroup groupConfig(&groupsConfig, groupId);
        QRectF geometry = groupConfig.readEntry("Geometry", QRectF());
        QString plugin = groupConfig.readEntry("Plugin", QString());

        AbstractGroup *group = createGroup(plugin, id);
        group->setGeometry(geometry);
    }

    KConfigGroup appletsConfig(&group, "Applets");

    foreach (Applet *applet, applets()) {
        KConfigGroup appletConfig(&appletsConfig, QString::number(applet->id()));
        KConfigGroup groupConfig(&appletConfig, "GroupInformation");

        if (groupConfig.isValid()) {
            int groupId = groupConfig.readEntry("Group", -1);

            if (groupId != -1) {
                AbstractGroup *group = m_groups.value(groupId);
                if (group) {
                    kDebug()<<group->id();
                    group->assignApplet(applet, false);
                    group->restoreAppletLayoutInfo(applet, groupConfig);
                }
            }
        }
    }
}

void GroupingDesktop::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{

    m_removeGroup->setVisible(false);

    foreach (AbstractGroup *group, m_groups) {
        kDebug()<<group->geometry()<<event->pos();
        if (group->geometry().contains(event->pos())) {
            m_removeGroup->setVisible(true);
            m_removeGroup->setData(group->id());
            kDebug()<<"Group clicked";
        }
    }

    Plasma::Containment::contextMenuEvent(event);
}

void GroupingDesktop::onImmutabilityChanged(Plasma::ImmutabilityType immutability)
{
    bool movable = (immutability == Plasma::Mutable);
    foreach (AbstractGroup *group, m_groups) {
        group->setFlag(QGraphicsItem::ItemIsMovable, movable);
    }
}

#include "groupingdesktop.moc"