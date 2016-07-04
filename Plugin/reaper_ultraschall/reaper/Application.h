////////////////////////////////////////////////////////////////////////////////
// 
// Copyright (c) 2014-2015 Ultraschall (http://ultraschall.fm)
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

#ifndef __ULTRASCHALL_REAPER_APPLICATION_H_INCL__
#define __ULTRASCHALL_REAPER_APPLICATION_H_INCL__

#include <Framework.h>
#include <IActivationService.h>
#include <IConfigurationService.h>
#include <ChapterMarker.h>
#include <TranscriptItem.h>

#include "CustomActionManager.h"
#include "CustomActionFactory.h"

namespace framework = ultraschall::framework;

namespace ultraschall {
namespace reaper {

class Application : public framework::IActivationService,
   public framework::IConfigurationService
{
public:
   virtual ~Application();

   static Application& Instance();

   ServiceStatus Start();
   void Stop();

   virtual ServiceStatus Configure();

   template<class CustomActionType> ServiceStatus RegisterCustomAction() const;
   static bool OnCustomAction(const int32_t id);

   template<class CustomActionType> void InvokeCustomAction() const;

   template<class CustomActionType> bool InvokeCustomActionAsync() const;

   static const size_t MAX_REAPER_STRING_BUFFER_SIZE = 4096;

   std::string GetExportPathName() const;
   std::string GetProjectPathName() const;
   std::string GetProjectFileName() const;
   std::string GetProjectFolderName() const;
   std::string GetProjectName() const;

   std::string TimestampToString(const double timestamp) const;
   double StringToTimestamp(const std::string& input) const;

   intptr_t GetCurrentProject() const;

   std::vector<framework::ChapterMarker> ChapterMarkers() const;
   int32_t SetChapterMarker(const framework::ChapterMarker& chapterMarker) const;
   bool DeleteChapterMarker(const framework::ChapterMarker& chapterMarker) const;
   void DeleteAllChapterMarkers() const;

private:
   Application();

   static bool HealthCheck();

   int Register(const char* name, void* pInfoStruct) const;

   static void CustomActionThreadProc(void* args);
};

typedef struct
{
   int uniqueSectionId; // 0/100=main/main alt, 32063=media explorer, 32060=midi editor, 32061=midi event list editor, 32062=midi inline editor, etc
   const char* idStr; // must be unique across all sections
   const char* name;
   void *extra; // reserved for future use
} custom_action_register_t;

template<class CustomActionType> ServiceStatus Application::RegisterCustomAction() const
{
   ServiceStatus status = SERVICE_FAILURE;

   const char* uniqueId = CustomActionType::UniqueId();
   if(uniqueId != 0)
   {
      CustomActionFactory& factory = CustomActionFactory::Instance();
      ICustomAction* pCustomAction = 0;
      status = factory.CreateCustomAction(uniqueId, pCustomAction);
      if(ServiceSucceeded(status) && (pCustomAction != 0))
      {
         const char* localisedName = pCustomAction->LocalizedName();
         if(localisedName != 0)
         {
            custom_action_register_t action = {0};
            action.idStr = uniqueId;
            action.name = localisedName;
            const int32_t id = Register("custom_action", (void*)&action);
            if(id != 0)
            {
               CustomActionManager& manager = CustomActionManager::Instance();
               status = manager.RegisterCustomAction(uniqueId, id, pCustomAction);
            }
         }
         else
         {
            status = SERVICE_INVALID_ARGUMENT;
         }

         framework::SafeRelease(pCustomAction);
      }
   }

   return status;
}

template<class CustomActionType> void Application::InvokeCustomAction() const
{
   reaper::CustomActionManager& manager = reaper::CustomActionManager::Instance();
   reaper::ICustomAction* pCustomAction = 0;
   ServiceStatus status = manager.LookupCustomAction(CustomActionType::UniqueId(), pCustomAction);
   if(ServiceSucceeded(status) && (pCustomAction != 0))
   {
      pCustomAction->Execute();
      framework::SafeRelease(pCustomAction);
   }
}

template<class CustomActionType> bool Application::InvokeCustomActionAsync() const
{
   bool result = false;

   reaper::CustomActionManager& manager = reaper::CustomActionManager::Instance();
   reaper::ICustomAction* pCustomAction = 0;
   ServiceStatus status = manager.LookupCustomAction(CustomActionType::UniqueId(), pCustomAction);
   if(ServiceSucceeded(status) && (pCustomAction != 0))
   {
#ifdef WIN32
      uintptr_t result = _beginthread(CustomActionThreadProc, 0, reinterpret_cast<void*>(pCustomAction));
      if(result != -1)
      {
         result = true;
      }
#else
      pCustomAction->Execute();
      result = true;
#endif

      framework::SafeRelease(pCustomAction);
   }

   return result;
}

}
}

#endif // #ifndef __ULTRASCHALL_REAPER_APPLICATION_H_INCL__
