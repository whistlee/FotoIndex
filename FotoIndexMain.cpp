/***************************************************************
 * Name:      FotoIndexMain.cpp
 * Purpose:   Code for Application Frame
 * Author:     ()
 * Created:   2019-05-19
 * Copyright:  ()
 * License:
 **************************************************************/

#include "FotoIndexMain.h"
#include <wx/msgdlg.h>
#include <sstream>
#include <string>
#include <wx/dcmemory.h>
#include <wx/textdlg.h>


 //(*InternalHeaders(FotoIndexFrame)
 #include <wx/font.h>
 #include <wx/intl.h>
 #include <wx/string.h>
 //*)

//helper functions
enum wxbuildinfoformat {
	short_f, long_f
};

wxString wxbuildinfo(wxbuildinfoformat format)
{
	wxString wxbuild(wxVERSION_STRING);

	if (format == long_f)
	{
#if defined(__WXMSW__)
		wxbuild << _T("-Windows");
#elif defined(__UNIX__)
		wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
		wxbuild << _T("-Unicode build");
#else
		wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
	}

	return wxbuild;
}

//(*IdInit(FotoIndexFrame)
const long FotoIndexFrame::ID_BUTTON1 = wxNewId();
const long FotoIndexFrame::ID_STATICTEXT1 = wxNewId();
const long FotoIndexFrame::ID_STATICTEXT2 = wxNewId();
const long FotoIndexFrame::ID_DIRPICKERCTRL1 = wxNewId();
const long FotoIndexFrame::ID_DIRPICKERCTRL2 = wxNewId();
const long FotoIndexFrame::ID_PANEL1 = wxNewId();
const long FotoIndexFrame::idMenuQuit = wxNewId();
const long FotoIndexFrame::idMenuAbout = wxNewId();
//*)

BEGIN_EVENT_TABLE(FotoIndexFrame, wxFrame)
//(*EventTable(FotoIndexFrame)
//*)
END_EVENT_TABLE()

FotoIndexFrame::FotoIndexFrame(wxWindow* parent, wxWindowID id)
{
	//(*Initialize(FotoIndexFrame)
	wxMenu* Menu1;
	wxMenu* Menu2;
	wxMenuBar* MenuBar1;
	wxMenuItem* MenuItem1;
	wxMenuItem* MenuItem2;

	Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
	SetClientSize(wxSize(267,191));
	SetMinSize(wxSize(344,240));
	SetMaxSize(wxSize(344,240));
	Panel1 = new wxPanel(this, ID_PANEL1, wxPoint(160,88), wxSize(344,240), wxSTATIC_BORDER|wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	Panel1->SetMinSize(wxSize(344,240));
	Panel1->SetMaxSize(wxSize(344,240));
	submit = new wxButton(Panel1, ID_BUTTON1, _("Submit"), wxPoint(104,112), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
	sourceText = new wxStaticText(Panel1, ID_STATICTEXT1, _("Source"), wxPoint(24,24), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	wxFont sourceTextFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	sourceText->SetFont(sourceTextFont);
	targetText = new wxStaticText(Panel1, ID_STATICTEXT2, _("Target"), wxPoint(24,72), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	wxFont targetTextFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,wxEmptyString,wxFONTENCODING_DEFAULT);
	targetText->SetFont(targetTextFont);
	sourcePicker = new wxDirPickerCtrl(Panel1, ID_DIRPICKERCTRL1, wxEmptyString, _("Choose source folder"), wxPoint(104,24), wxDefaultSize, wxDIRP_DIR_MUST_EXIST|wxDIRP_USE_TEXTCTRL, wxDefaultValidator, _T("ID_DIRPICKERCTRL1"));
	targetPicker = new wxDirPickerCtrl(Panel1, ID_DIRPICKERCTRL2, wxEmptyString, _("Choose destination folder"), wxPoint(104,72), wxDefaultSize, wxDIRP_DIR_MUST_EXIST|wxDIRP_USE_TEXTCTRL, wxDefaultValidator, _T("ID_DIRPICKERCTRL2"));
	MenuBar1 = new wxMenuBar();
	Menu1 = new wxMenu();
	MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
	Menu1->Append(MenuItem1);
	MenuBar1->Append(Menu1, _("&File"));
	Menu2 = new wxMenu();
	MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
	Menu2->Append(MenuItem2);
	MenuBar1->Append(Menu2, _("Help"));
	SetMenuBar(MenuBar1);

	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FotoIndexFrame::OnsubmitClick);
	Connect(ID_DIRPICKERCTRL1,wxEVT_COMMAND_DIRPICKER_CHANGED,(wxObjectEventFunction)&FotoIndexFrame::OnsourcePickerDirChanged);
	Connect(ID_DIRPICKERCTRL2,wxEVT_COMMAND_DIRPICKER_CHANGED,(wxObjectEventFunction)&FotoIndexFrame::OntargetPickerDirChanged);
	Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&FotoIndexFrame::OnQuit);
	Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&FotoIndexFrame::OnAbout);
	//*)
}


//COMPARE FUNCTION
static int CompareString(const wxString& first, const wxString& second)
{
	wxFileName f1 = first;
	wxFileName f2 = second;
	return f1.GetName().Cmp(f2.GetName()); // CmpNoCase if we dont want to consider capital letters
}
//returning pointer to array with all images paths
wxArrayString* fetchFileNames(const wxString sourcePath) {
	wxArrayString* filesArray = new wxArrayString(); //keeps all files paths
	size_t count = wxDir::GetAllFiles(sourcePath, filesArray);
	//SOROTWANKO
	filesArray->Sort(CompareString);
	return filesArray;
}

//creating path for single file
wxString createNewFilePath(const wxString& targetPath, const wxString& sourcePath, size_t counter) {
	std::stringstream sstm;
	sstm << targetPath << "\\" << counter << "_" "indexed_from-"<< sourcePath.AfterLast('\\') << ".bmp";
	return wxString(sstm.str());
}

//returning bitmap with specific text(filename)
wxBitmap createText(const wxFileName filename)
{

	wxBitmap txtBitmap = wxBitmap(150, 30);
	wxMemoryDC memdc;
	memdc.SelectObject(txtBitmap);
	memdc.SetBackgroundMode(wxPENSTYLE_SOLID);
	memdc.SetTextForeground(*wxWHITE);
	memdc.SetTextBackground(*wxBLACK);

	wxString txt = wxString(filename.GetName());
	wxSize center = memdc.GetMultiLineTextExtent(txt);
	if (center.GetWidth() > 150)
	{
		int chars = 0;
		int width = memdc.GetCharWidth();
		chars = (140) / width;
		int end = center.GetWidth() / width;
		txt = "~"+txt.Mid(end - chars);
		memdc.DrawText(txt, 0, 0);
	}
	else
	{
		int x = center.GetWidth() / 2;
		memdc.DrawText(txt, 75 - x, 0);
	}
	wxBitmap outBmp = memdc.GetAsBitmap();
	memdc.SelectObject(wxNullBitmap);
	return outBmp;
}

//returning bitmat with user comment for index page
wxBitmap addComment(const wxFileName filename,const wxString comment)
{
	wxBitmap txtBitmap = wxBitmap(800, 30);
	wxMemoryDC memdc;
	memdc.SelectObject(txtBitmap);
	memdc.SetBackgroundMode(wxPENSTYLE_SOLID);
	memdc.SetTextForeground(*wxRED);
	memdc.SetTextBackground(*wxBLACK);

	memdc.DrawText(comment, 10, 0);
	wxBitmap outBmp = memdc.GetAsBitmap();
	memdc.SelectObject(wxNullBitmap);
	return outBmp;
}

//messagebox with input for comment
wxString askUserForComment(int number){
    std::stringstream stream;
	stream << "Provide commentary for " << number << " page of index";
	const wxString comment = wxString(stream.str());
    wxTextEntryDialog* dialog = new wxTextEntryDialog(NULL, comment, wxT("Message"), wxT("my comment"));
	if (dialog->ShowModal() == wxID_OK)
        return  dialog->GetValue();
    return wxT("");

}

//checking all fetched files extensions and removing them if not image
bool checkExtension(wxString extension){
    wxArrayString supportedExtensions;
    supportedExtensions.Add(wxString("bmp"));
    supportedExtensions.Add(wxString("jpg"));
    supportedExtensions.Add(wxString("jpeg"));
    supportedExtensions.Add(wxString("png"));

    if(supportedExtensions.Index(extension.Lower()) == wxNOT_FOUND){
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////
//GENERALLY MAIN FUNCTION
//////////////////////////////////////////////////////////
void processPathArray(wxArrayString* pathArray, const wxString& sourcePath, const wxString& targetPath) {
	wxCoord posX = 5;
	wxCoord posY = 0;
	const int totalFiles = pathArray->GetCount();
	wxImage resultPic = wxImage(800, 600);

	wxFileName filename(targetPath);

	wxBitmap bComment;

	for (size_t i = 0; i < pathArray->GetCount(); ++i)
	{
		const wxString filePath = pathArray->Item(i);

		wxFileName filename(filePath);

		wxImage orginalPic = wxImage();
		orginalPic.LoadFile(filePath);
		wxImage scaledPic = orginalPic.Scale(150, 110);
		wxBitmap text = createText(filename);
		resultPic.Paste(scaledPic, posX, posY);
		resultPic.Paste(text.ConvertToImage(), posX, posY + 110);
		posX += 160;
		if (posX > 800) {
			posX = 5;
			posY += 140;
		}
		if ((i == 19) || (i>20 && ((i+1)%20) == 0)) {
            bComment = addComment(filename, askUserForComment(((i + 1) / 20)));
			resultPic.Paste(bComment.ConvertToImage(), 0, 570);
			resultPic.SaveFile(createNewFilePath(targetPath, sourcePath, ((i + 1) / 20)), wxBITMAP_TYPE_BMP);
			resultPic.Destroy();
			resultPic = wxImage(800, 600);
			posX = 5;
			posY = 0;
		};
	}
	if ((totalFiles % 20) != 0) {
        bComment = addComment(filename, askUserForComment((int)ceil(totalFiles / 20.0)));
		resultPic.Paste(bComment.ConvertToImage(), 0, 570);
		resultPic.SaveFile(createNewFilePath(targetPath, sourcePath, (int)ceil(totalFiles / 20.0)), wxBITMAP_TYPE_BMP);
	}
}

FotoIndexFrame::~FotoIndexFrame()
{
	//(*Destroy(FotoIndexFrame)
	//*)
}

void FotoIndexFrame::OnQuit(wxCommandEvent & event)
{
	Close();
}

void FotoIndexFrame::OnAbout(wxCommandEvent & event)
{
	wxString msg = wxbuildinfo(long_f);
	wxMessageBox(msg, _("Welcome to..."));
}
//////////////////////////////////////////////////////////
//THIS PART GOES AFTER SUBMIT CLICK
//////////////////////////////////////////////////////////
void FotoIndexFrame::OnsubmitClick(wxCommandEvent & event)
{
	const wxString sourcePath = sourcePicker->GetPath();
	const wxString targetPath = targetPicker->GetPath();
	const wxString emptyPath = wxString("");

	if (sourcePath == emptyPath || targetPath == emptyPath) {
		wxMessageBox("Select BOTH ditectories!");
	}
	else {
		wxArrayString* pathArray = fetchFileNames(sourcePath);//keeps all files paths

		//remove all non image files
		for (size_t i = 0; i < pathArray->GetCount(); ++i){
		    wxString filePath = pathArray->Item(i);
		    if(checkExtension(filePath.AfterLast('.')) == true)
            {
             pathArray->RemoveAt(i);
             i--;
            }
        }
        pathArray->Shrink();
		if (pathArray->GetCount() > 0) {
            std::stringstream sstm;
            sstm << "Found " << pathArray->GetCount() << " files!";
            wxMessageBox(wxString(sstm.str()));
			processPathArray(pathArray, sourcePath, targetPath);
		}
		else {
			wxMessageBox("Found 0 files!");
		}
	}
}

void FotoIndexFrame::OnsourcePickerDirChanged(wxFileDirPickerEvent & event)
{
}

void FotoIndexFrame::OntargetPickerDirChanged(wxFileDirPickerEvent & event)
{
}
