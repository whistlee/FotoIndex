/***************************************************************
 * Name:      FotoIndexMain.h
 * Purpose:   Defines Application Frame
 * Author:     ()
 * Created:   2019-05-19
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef FOTOINDEXMAIN_H
#define FOTOINDEXMAIN_H

//(*Headers(FotoIndexFrame)
#include <wx/button.h>
#include <wx/filepicker.h>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/statusbr.h>
#include <wx/dir.h>


class FotoIndexFrame: public wxFrame
{
    public:

        FotoIndexFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~FotoIndexFrame();

    private:

        //(*Handlers(FotoIndexFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnDirPickerCtrl1DirChanged(wxFileDirPickerEvent& event);
        void OnDirPickerCtrl1DirChanged1(wxFileDirPickerEvent& event);
        void OnDirPickerCtrl1DirChanged2(wxFileDirPickerEvent& event);
        void OnDirPickerCtrl1DirChanged3(wxFileDirPickerEvent& event);
        void OnsubmitClick(wxCommandEvent& event);
        void OnsourcePickerDirChanged(wxFileDirPickerEvent& event);
        void OntargetPickerDirChanged(wxFileDirPickerEvent& event);
        //*)

        //(*Identifiers(FotoIndexFrame)
        static const long ID_BUTTON1;
        static const long ID_STATICTEXT1;
        static const long ID_STATICTEXT2;
        static const long ID_DIRPICKERCTRL1;
        static const long ID_DIRPICKERCTRL2;
        static const long ID_PANEL1;
        static const long idMenuQuit;
        static const long idMenuAbout;
        //*)

        //(*Declarations(FotoIndexFrame)
        wxButton* submit;
        wxDirPickerCtrl* sourcePicker;
        wxDirPickerCtrl* targetPicker;
        wxPanel* Panel1;
        wxStaticText* sourceText;
        wxStaticText* targetText;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // FOTOINDEXMAIN_H
