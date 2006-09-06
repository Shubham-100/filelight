// Copyright 2003-6 Max Howell <max.howell@methylblue.com>
// Redistributable under the terms of the GNU General Public License

#ifndef FILELIGHT_PART_H
#define FILELIGHT_PART_H

#include <kparts/browserextension.h>
#include <kparts/statusbarextension.h>
#include <kparts/part.h>
#include <kurl.h>

class KAboutData;
using KParts::StatusBarExtension;
namespace RadialMap { class Widget; }
class Directory;


namespace Filelight
{
   class Part;

   class BrowserExtension : public KParts::BrowserExtension
   {
   public:
      explicit BrowserExtension( Part*, const char * = 0 );
   };


   class Part : public KParts::ReadOnlyPart
   {
      Q_OBJECT

   public:
      Part( QWidget *, const char *, QObject *, const char *, const QStringList& );

      virtual bool openFile() { return false; } //pure virtual in base class
      virtual bool closeURL();

      QString prettyURL() const { return m_url.protocol() == "file" ? m_url.path() : m_url.prettyURL(); }

      static KAboutData *createAboutData();

   public slots:
      virtual bool openURL( const KURL& );
      void configFilelight();
      void rescan();

   private slots:
      void postInit();
      void scanCompleted( Directory* );
      void mapChanged( const Directory* );

   private:
      KStatusBar *statusBar() { return m_statusbar->statusBar(); }

      BrowserExtension   *m_ext;
      StatusBarExtension *m_statusbar;
      RadialMap::Widget  *m_map;
      class ScanManager  *m_manager;

      bool m_started;

   private:
      bool start( const KURL& );

   private slots:
      void updateURL( const KURL & );
   };
}

#endif
