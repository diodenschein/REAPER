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

#include <ResourceManager.h>

#include "SaveChapterMarkersToProjectAction.h"
#include "Application.h"
#include "ProjectManager.h"
#include "FileManager.h"
#include "NotificationWindow.h"

namespace ultraschall { namespace reaper {

static DeclareCustomAction<SaveChapterMarkersToProjectAction> action;

SaveChapterMarkersToProjectAction::SaveChapterMarkersToProjectAction()
{
   framework::ResourceManager& resourceManager = framework::ResourceManager::Instance();
   ServiceStatus status = resourceManager.RegisterLocalizedString(actionNameId_);
   if(ServiceSucceeded(status))
   {
      resourceManager.SetLocalizedString(actionNameId_, "en-EN", "ULTRASCHALL: Save chapter markers to project folder");
      resourceManager.SetLocalizedString(actionNameId_, "de-DE", "ULTRASCHALL: Kapitelmarken im Projektverzeichnis speichern");
   }

   status = resourceManager.RegisterLocalizedString(successMessageId_);
   if(ServiceSucceeded(status))
   {
      resourceManager.SetLocalizedString(successMessageId_, "en-EN", "The chapter markers have been saved successfully.");
      resourceManager.SetLocalizedString(successMessageId_, "de-DE", "Die Kapitelmarken wurden erfolgreich gespeichert.");
   }

   status = resourceManager.RegisterLocalizedString(failureMessageId_);
   if(ServiceSucceeded(status))
   {
      resourceManager.SetLocalizedString(failureMessageId_, "en-EN", "The chapter markers could not be saved.");
      resourceManager.SetLocalizedString(failureMessageId_, "de-DE", "Die Kapitelmarken konnten nicht gespeichert werden.");
   }

   status = resourceManager.RegisterLocalizedString(notFoundMessageId_);
   if(ServiceSucceeded(status))
   {
      resourceManager.SetLocalizedString(notFoundMessageId_, "en-EN", "No chapter markers have been found.");
      resourceManager.SetLocalizedString(notFoundMessageId_, "de-DE", "Es wurden keine Kapitelmarken gefunden.");
   }

   status = resourceManager.RegisterLocalizedString(noProjectNameMessageId_);
   if(ServiceSucceeded(status))
   {
      resourceManager.SetLocalizedString(noProjectNameMessageId_, "en-EN", "The project has no name yet. Please save the project and try again.");
      resourceManager.SetLocalizedString(noProjectNameMessageId_, "de-DE", "Das Projekt hat noch keinen Namen und muss zuerst gespeichert werden");
   }
}

SaveChapterMarkersToProjectAction::~SaveChapterMarkersToProjectAction()
{
   framework::ResourceManager& resourceManager = framework::ResourceManager::Instance();
   resourceManager.UnregisterLocalizedString(actionNameId_);
   resourceManager.UnregisterLocalizedString(successMessageId_);
   resourceManager.UnregisterLocalizedString(failureMessageId_);
   resourceManager.UnregisterLocalizedString(notFoundMessageId_);
   resourceManager.UnregisterLocalizedString(noProjectNameMessageId_);
}

const char* SaveChapterMarkersToProjectAction::UniqueId()
{
   return "ULTRASCHALL_SAVE_CHAPTERS_TO_PROJECT";
}

ServiceStatus SaveChapterMarkersToProjectAction::CreateCustomAction(ICustomAction*& pCustomAction)
{
   pCustomAction = new SaveChapterMarkersToProjectAction();
   PRECONDITION_RETURN(pCustomAction != 0, SERVICE_FAILURE);
   return SERVICE_SUCCESS;
}

const char* SaveChapterMarkersToProjectAction::LocalizedName() const
{
   framework::ResourceManager& resourceManager = framework::ResourceManager::Instance();
   return resourceManager.GetLocalizedString(actionNameId_);
}

ServiceStatus SaveChapterMarkersToProjectAction::Execute()
{
   ServiceStatus status = SERVICE_FAILURE;
   
   const ProjectManager& projectManager = ProjectManager::Instance();
   Project currentProject = projectManager.CurrentProject();
   const std::vector<Marker> chapterMarkers = currentProject.ChapterMarkers();
   if(chapterMarkers.empty() == false)
   {
      const std::string projectFolder = currentProject.FolderName();
      if(projectFolder.empty() == false)
      {
         const std::string projectName = currentProject.Name();
         if(projectName.empty() == false)
         {
            const std::string fullPath = FileManager::AppendPath(projectFolder, projectName + ".chapters.txt");
            std::ofstream output(fullPath, std::ios::out);
            for(size_t i = 0; i < chapterMarkers.size(); i++)
            {
               // TODO
               //const std::string timestamp = application.TimestampToString(chapterMarkers[i].Position());
               //const std::string entry = timestamp + " " + chapterMarkers[i].Name();
               //output << entry << std::endl;
            }

            output.close();

            status = SERVICE_SUCCESS;
            NotificationWindow::Show(successMessageId_);
         }
         else
         {
            NotificationWindow::Show(noProjectNameMessageId_);
         }
      }
      else
      {
         NotificationWindow::Show(noProjectNameMessageId_);
      }
   }
   else
   {
      NotificationWindow::Show(notFoundMessageId_);
   }
   
   return status;
}

}
}

