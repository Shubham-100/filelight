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

#include "fileTree.h"

#include <KGlobal>
#include <KLocale>

//static definitions
const uint File::DENOMINATOR[4] = { 1<<0, 1<<10, 1<<20, 1<<30 };
static const char PREFIX[4]   = { 'K', 'M', 'G', 'T' };


QString
File::fullPath(const Folder *root /*= 0*/) const
{
    QString path;

    if (root == this)
        root = 0; //prevent returning empty string when there is something we could return

    for (const Folder *d = (Folder*)this; d != root && d; d = d->parent())
        path.prepend(d->name());

    return path;
}

QString
File::humanReadableSize(UnitPrefix key /*= mega*/) const //FIXME inline
{
    return humanReadableSize(m_size, key);
}

QString
File::humanReadableSize(FileSize size, UnitPrefix key /*= mega*/) //static
{
    Q_UNUSED( key );
    // TODO: LocalLister::scan() divides by 1024, to be able to use only 32bit for size
    // TODO: ensure again three significant figures
    return KGlobal::locale()->formatByteSize(1024*(quint64)size);

#if 0
    if (size == 0)
        return "0 B";

    QString s;
    double prettySize = (double)size / (double)DENOMINATOR[key];
    const KLocale &locale = *KGlobal::locale();

    if (prettySize >= 0.01)
    {
        //use three significant figures
        if (prettySize < 1)        s = locale.formatNumber(prettySize, 2);
        else if (prettySize < 100) s = locale.formatNumber(prettySize, 1);
        else                       s = locale.formatNumber(prettySize, 0);

        s += ' ';
        s += PREFIX[key];
        s += 'B';
    }

    if (prettySize < 0.1)
    {
        s += " (";
        s += locale.formatNumber(size / DENOMINATOR[key - 1], 0);
        s += ' ';
        s += PREFIX[key - 1];
        s += "B)";
    }

    return s;
#endif
}
