/*
 *   Copyright 2009-2010 by Giulio Camuffo <giuliocamuffo@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef FLOWGROUP_H
#define FLOWGROUP_H

#include "abstractgroup.h"

class QGraphicsLinearLayout;

class Spacer;

class FlowGroup : public AbstractGroup
{
    Q_OBJECT
    public:
        explicit FlowGroup(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);
        ~FlowGroup();

        QString pluginName() const;
        void restoreChildGroupInfo(QGraphicsWidget *child, const KConfigGroup &group);
        void saveChildGroupInfo(QGraphicsWidget *child, KConfigGroup group) const;
        bool showDropZone(const QPointF &pos);
        Handle *createHandleForChild(QGraphicsWidget *child);

        static QString prettyName();
        static QSet<Plasma::FormFactor> availableOnFormFactors();

    protected:
        void layoutChild(QGraphicsWidget *child, const QPointF &pos);

    private:
        QGraphicsLinearLayout *m_layout;
        Spacer *m_spacer;

        int m_spacerIndex;

};

#endif // FLOWGROUP_H
