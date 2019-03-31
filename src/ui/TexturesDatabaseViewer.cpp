#include <msclr/marshal_cppstd.h>
#include "TexturesDatabaseViewer.h"

using namespace msclr::interop;

namespace MetroEX {

    void TexturesDatabaseViewer::SetDataProvider(MyDict<HashString, MetroTextureInfo*>* data) {
        this->mDataProvider = data;
    }

    void TexturesDatabaseViewer::SetMainForm(MainForm^ form) {
        this->mMainForm = form;
    }

    void TexturesDatabaseViewer::FillWithData() {
        if (this->mDataProvider == nullptr) {
            return;
        }

        System::Windows::Forms::Cursor::Current = System::Windows::Forms::Cursors::WaitCursor;

        this->dataGridView->Rows->Clear();

        for (auto el = this->mDataProvider->begin(); el != this->mDataProvider->end(); ++el) {
            String^ name = marshal_as<String^>(el->second->name);

            if (!name->Contains(filterText->Text)) {
                continue;
            }

            this->dataGridView->Rows->Add(name, el->second->streamable);
        }

        System::Windows::Forms::Cursor::Current = System::Windows::Forms::Cursors::Arrow;
    }

    void TexturesDatabaseViewer::filterTimer_Tick(System::Object^ sender, System::EventArgs^ e) {
        this->filterTimer->Stop();

        this->FillWithData();
    }

    void TexturesDatabaseViewer::filterText_TextChanged(System::Object^ sender, System::EventArgs^ e) {
        if (this->mDataProvider != nullptr) {
            this->filterTimer->Stop();
            this->filterTimer->Start();
        }
    }

    void TexturesDatabaseViewer::dataGridView_MouseDoubleClick(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
        String^ name = dynamic_cast<String^>(this->dataGridView->CurrentRow->Cells[0]->Value);

        this->mMainForm->ResetTreeView();
        if (!this->mMainForm->FindAndSelect("content\\textures\\" + name, this->mFileExtensions)) {
            this->mMainForm->ShowErrorMessage("Couldn't find texture!");
        }
    }

}