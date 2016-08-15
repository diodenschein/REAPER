////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2016 Ultraschall (http://ultraschall.fm)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <vector>
#include <fstream>

#include <cpr/cpr.h>

#ifndef _WIN32
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#endif

#include "ReaperVersionCheck.h"
#include "ThemeVersionCheck.h"
#include "HubVersionCheck.h"
#include "SoundboardVersionCheck.h"
#include "StudioLinkVersionCheck.h"
#include "PluginVersionCheck.h"
#include "SWSVersionCheck.h"
#include "AboutAction.h"
#include "NotificationWindow.h"
#include "FileManager.h"

namespace ultraschall {
namespace reaper {

static DeclareCustomAction<AboutAction> action;

AboutAction::AboutAction()
{
   framework::ResourceManager& resourceManager = framework::ResourceManager::Instance();
   ServiceStatus status = resourceManager.RegisterLocalizedString(actionNameId_);
   if(ServiceSucceeded(status))
   {
      resourceManager.SetLocalizedString(actionNameId_, "en-EN", "ULTRASCHALL: About Ultraschall...");
   }
}

AboutAction::~AboutAction()
{
   framework::ResourceManager& resourceManager = framework::ResourceManager::Instance();
   resourceManager.UnregisterLocalizedString(actionNameId_);
}

const char* AboutAction::UniqueId()
{
   return "ULTRASCHALL_ABOUT_ULTRASCHALL";
}

ServiceStatus AboutAction::CreateCustomAction(ICustomAction*& pCustomAction)
{
   pCustomAction = new AboutAction();
   PRECONDITION_RETURN(pCustomAction != 0, SERVICE_FAILURE);
   return SERVICE_SUCCESS;
}

const char* AboutAction::LocalizedName() const
{
   framework::ResourceManager& resourceManager = framework::ResourceManager::Instance();
   return resourceManager.GetLocalizedString(actionNameId_);
}

ServiceStatus AboutAction::Execute()
{
   const std::string pluginVersion = QueryPluginVersion();

   std::string message1 = "\
http://ultraschall.fm\r\n\r\n\
Copyright (c) 2016 Ralf Stockmann, Daniel Lindenfelser, Katrin Leinweber, Andreas Pieper, Artur Kordowski, Mich\u00E9l Knecht, Tim Pritlove, Heiko Panjas\r\n\r\n\
Ultraschall REAPER Extension " + pluginVersion + "\r\n";

   const std::string themeVersion = QueryThemeVersion();
   if(themeVersion.empty() == false)
   {
      message1 += "Ultraschall REAPER Theme " + themeVersion + "\r\n";
   }

#ifndef WIN32
   const std::string hubVersion = QueryHubVersion();
   if(hubVersion.empty() == false)
   {
      message1 += hubVersion + "\r\n";
   }
#endif // #ifndef WIN32

   const std::string soundboardVersion = QuerySoundboardVersion();
   if(soundboardVersion.empty() == false)
   {
      message1 += "Ultraschall Soundboard " + soundboardVersion + "\r\n";
   }

   const std::string studioLinkVersion = QueryStudioLinkVersion();
   if(studioLinkVersion.empty() == false)
   {
      message1 += "StudioLink Plug-in " + studioLinkVersion + "\r\n";
   }

   std::string message2 = "\
SWS REAPER Extension " + QuerySWSVersion() + "\r\n\
REAPER ";

   message2 += QueryRawReaperVersion();
   message2 += "\r\n";

   NotificationWindow::Show("About Ultraschall 2.3 \"Gropius\"...", message1 + message2);

   return SERVICE_SUCCESS;
}

}}
