//Author:    Max Howell <max.howell@methylblue.com>, (C) 2004
//Copyright: See COPYING file that comes with this distribution

#ifndef FILELIGHTSUMMARY_H
#define FILELIGHTSUMMARY_H

#include <qwidget.h>

class DiskList;

namespace Filelight
{
   class SummaryWidget : public QWidget
   {
   Q_OBJECT
   public:
      SummaryWidget( QWidget *parent, const char *name );
     ~SummaryWidget();

   signals:
      void activated( const KURL& );

   private slots:
      void diskInformationReady();

   private:
      DiskList *m_disks;
   };
}

#endif
