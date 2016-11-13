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

#include <TextFileReader.h>
#include <StringUtilities.h>
#include <ResourceManager.h>

#include "InsertChapterMarkersAction.h"
#include "CustomActionFactory.h"
#include "Marker.h"
#include "ProjectManager.h"
#include "FileManager.h"
#include "NotificationWindow.h"

namespace ultraschall { namespace reaper {

static DeclareCustomAction<InsertChapterMarkersAction> action;

InsertChapterMarkersAction::InsertChapterMarkersAction()
{
}

InsertChapterMarkersAction::~InsertChapterMarkersAction()
{
}

const char* InsertChapterMarkersAction::UniqueId()
{
   return "ULTRASCHALL_INSERT_CHAPTERS";
}

ServiceStatus InsertChapterMarkersAction::CreateCustomAction(ICustomAction*& pCustomAction)
{
   pCustomAction = new InsertChapterMarkersAction();
   PRECONDITION_RETURN(pCustomAction != 0, SERVICE_FAILURE);
   return SERVICE_SUCCESS;
}

const char* InsertChapterMarkersAction::LocalizedName() const
{
   return "ULTRASCHALL: Import chapter markers...";
}

ServiceStatus InsertChapterMarkersAction::Execute()
{
   ServiceStatus status = SERVICE_FAILURE;
   
   const std::string path = FileManager::BrowseForFiles("Import chapter markers...");
   PRECONDITION_RETURN(path.empty() == false, SERVICE_FAILURE);

   const ProjectManager& projectManager = ProjectManager::Instance();
   Project currentProject = projectManager.CurrentProject();
   std::vector<Marker> chapterMarkers;

   const std::vector<std::string> lines = framework::TextFileReader::ReadLines(path);
   for(const std::string& line : lines)
   {
      const std::vector<std::string> items = framework::split(line, ' ');
      if(items.size() > 0)
      {
         // TODO
         //const double timestamp = application.StringToTimestamp(items[0]);
         //std::string name;
         //if(items.size() > 1)
         //{
         //   name = items[1];
         //}
         //
         //for(size_t i = 2; i < items.size(); i++)
         //{
         //   name += " " + items[i];
         //}
         
         //chapterMarkers.push_back(framework::ChapterMarker(timestamp, name));
      }
   }
   
   size_t addedChapterMarkers = 0;
   for(size_t i = 0; i < chapterMarkers.size(); i++)
   {
      // TODO
      //const int32_t index = application.SetChapterMarker(chapterMarkers[i]);
      //if(index > -1)
      //{
      //   addedChapterMarkers++;
      //}
   }

   if(chapterMarkers.size() == addedChapterMarkers)
   {
      NotificationWindow::Show("The chapter markers have been added successfully.");
      status = SERVICE_SUCCESS;
   }
   else
   {
      NotificationWindow::Show("The chapter markers could not be added.");
   }
   
   return status;
}

}}

