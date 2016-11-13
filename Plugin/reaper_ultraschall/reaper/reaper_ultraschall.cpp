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

#include <TraceUtilities.h>
#include <ServiceStatus.h>

#include "Application.h"
#include "NotificationWindow.h"
#include "ReaperEntryPoints.h"
#include "InvalidEntryPointException.h"

#include "CustomActionManager.h"

#include "AboutAction.h"
#include "UpdateCheckAction.h"

#include "InsertChapterMarkersAction.h"
#include "SaveChapterMarkersAction.h"
#include "SaveChapterMarkersToProjectAction.h"

#include "SetChapterMarkerAction.h"
#include "SetEditMarkerAction.h"
#include "SetShownoteMarkerAction.h"
#include "SetHistoricalMarkerAction.h"
#include "UndoMarkerAction.h"

#include "ToggleChapterMarkersAction.h"
#include "ToggleEditMarkersAction.h"
#include "ToggleShownoteMarkersAction.h"

#include "NewProjectCommand.h"
#include "OpenProjectCommand.h"
#include "SaveProjectCommand.h"
#include "CloseProjectCommand.h"

namespace reaper = ultraschall::reaper;

extern "C"
{
	REAPER_PLUGIN_DLL_EXPORT int REAPER_PLUGIN_ENTRYPOINT(REAPER_PLUGIN_HINSTANCE instance, reaper_plugin_info_t *pPluginInfo)
	{
		reaper::Application& application = reaper::Application::Instance();

		if(pPluginInfo != 0)
		{
         Trace0(TRACE_LEVEL_INFO, "Ultraschall is starting-up...");

			static bool started = false;
			if(false == started)
			{
				try
				{
					reaper::ReaperEntryPoints::Setup(instance, pPluginInfo);

					if(ServiceSucceeded(application.Configure()))
					{
						if(ServiceSucceeded(application.Start()))
						{
                     application.RegisterCustomAction<reaper::AboutAction>();
                     Trace0(TRACE_LEVEL_INFO, "'AboutAction' activated.");

                     application.RegisterCustomAction<reaper::UpdateCheckAction>();
                     Trace0(TRACE_LEVEL_INFO, "'UpdateCheckAction' activated.");

							application.RegisterCustomAction<reaper::InsertChapterMarkersAction>();
                     Trace0(TRACE_LEVEL_INFO, "'InsertChapterMarkersAction' activated.");
                     application.RegisterCustomAction<reaper::SaveChapterMarkersAction>();
                     Trace0(TRACE_LEVEL_INFO, "'SaveChapterMarkersAction' activated.");
                     application.RegisterCustomAction<reaper::SaveChapterMarkersToProjectAction>();
                     Trace0(TRACE_LEVEL_INFO, "'SaveChapterMarkersToProjectAction' activated.");

                     application.RegisterCustomAction<reaper::SetChapterMarkerAction>();
                     Trace0(TRACE_LEVEL_INFO, "'SetChapterMarkerAction' activated.");
                     application.RegisterCustomAction<reaper::SetEditMarkerAction>();
                     Trace0(TRACE_LEVEL_INFO, "'SetEditMarkerAction' activated.");
                     application.RegisterCustomAction<reaper::SetShownoteMarkerAction>();
                     Trace0(TRACE_LEVEL_INFO, "'SetShownoteMarkerAction' activated.");

                     application.RegisterCustomAction<reaper::SetHistoricalMarkerAction>();
                     Trace0(TRACE_LEVEL_INFO, "'SetHistoricalChapterMarkerAction' activated.");
                     application.RegisterCustomAction<reaper::UndoMarkerAction>();
                     Trace0(TRACE_LEVEL_INFO, "'UndoMarkerAction' activated.");
                     
                     application.RegisterCustomAction<reaper::ToggleChapterMarkersAction>();
                     Trace0(TRACE_LEVEL_INFO, "'ToggleChapterMarkersAction' activated.");
                     application.RegisterCustomAction<reaper::ToggleEditMarkersAction>();
                     Trace0(TRACE_LEVEL_INFO, "'ToggleEditMarkersAction' activated.");
                     application.RegisterCustomAction<reaper::ToggleShownoteMarkersAction>();
                     Trace0(TRACE_LEVEL_INFO, "'ToggleShownoteMarkersAction' activated.");

                     application.RegisterCommand<reaper::NewProjectCommand>(40023);
                     Trace0(TRACE_LEVEL_INFO, "'NewProjectCommand' activated.");
                     application.RegisterCommand<reaper::OpenProjectCommand>(40025);
                     Trace0(TRACE_LEVEL_INFO, "'OpenProjectCommand' activated.");
                     application.RegisterCommand<reaper::SaveProjectCommand>(40026);
                     Trace0(TRACE_LEVEL_INFO, "'SaveProjectCommand' activated.");
                     application.RegisterCommand<reaper::CloseProjectCommand>(40860);
                     Trace0(TRACE_LEVEL_INFO, "'CloseProjectCommand' activated.");

							// run the update action on startup
                     Trace0(TRACE_LEVEL_INFO, "Running 'UpdateCheckAction'...");
							application.InvokeCustomAction<reaper::UpdateCheckAction>();
                     Trace0(TRACE_LEVEL_INFO, "'UpdateCheckAction' done.");
                  }
					}
				}
				catch(reaper::InvalidEntryPointException&)
				{
					std::string errorReason = "\
You are trying to load a version of REAPER that is not compatible to Ultraschall 2.";

					reaper::NotificationWindow::Show("Ultraschall failed to load!", errorReason, true);
					return 0;
				}

            Trace0(TRACE_LEVEL_INFO, "Ultraschall is running.");

				started = true;
			}

         Trace0(TRACE_LEVEL_INFO, "done.");
         
         return 1;
		}
		else
		{
         Trace0(TRACE_LEVEL_INFO, "Ultraschall is shutting down...");
         
         static bool stopped = false;
			if(false == stopped)
			{
				application.Stop();
				stopped = true;
			}

         Trace0(TRACE_LEVEL_INFO, "done.");
         
         return 0;
		}
	}
}

#ifdef WIN32
BOOL APIENTRY DllMain(HMODULE, ULONG ul_reason_for_call, LPVOID)
{
	switch(ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
#endif

