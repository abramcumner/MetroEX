#include <msclr/marshal_cppstd.h>
#include "TreeViewFilter.h"
#include "mycommon.h"

using namespace System;
using namespace System::Collections;
using namespace msclr::interop;

namespace MetroEX {

    void TreeViewFilter::Initialize() {
        if (mOriginalRootNodes == nullptr) {
            mOriginalRootNodes = gcnew array<TreeNode^>(this->treeView->Nodes->Count);
        } else {
            Array::Resize(mOriginalRootNodes, this->treeView->Nodes->Count);
        }

        this->treeView->Nodes->CopyTo(mOriginalRootNodes, 0);
    }

    void TreeViewFilter::filterTimer_Tick(System::Object^ sender, System::EventArgs^ e) {
        this->filterTimer->Stop();

        System::Windows::Forms::Cursor::Current = System::Windows::Forms::Cursors::WaitCursor;

        this->treeView->BeginUpdate();
        this->treeView->Nodes->Clear();

        if (String::IsNullOrWhiteSpace(this->filterTextBox->Text)) {
            this->treeView->Nodes->AddRange(mOriginalRootNodes);
        }
        else {
            for (int i = 0; i < mOriginalRootNodes->Length; i++) {
                TreeNode^ root = safe_cast<TreeNode^>(mOriginalRootNodes[i]->Clone());
                this->FilterTreeView(root, this->filterTextBox->Text);
                this->treeView->Nodes->Add(root);
            }
        }

        this->treeView->Nodes[0]->ExpandAll();
        this->treeView->EndUpdate();

        System::Windows::Forms::Cursor::Current = System::Windows::Forms::Cursors::Arrow;
    }

    void TreeViewFilter::FilterTreeView(TreeNode^ node, String^ text) {
        System::Collections::Generic::List<TreeNode^>^ nodesToRemove = gcnew System::Collections::Generic::List<TreeNode^>();

        for (int i = 0; i < node->Nodes->Count; i++) {
            if (node->Nodes[i]->Nodes->Count > 0) {
                this->FilterTreeView(node->Nodes[i], text);

                if (node->Nodes[i]->Nodes->Count == 0) {
                    nodesToRemove->Add(node->Nodes[i]);
                }
            } else if (!node->Nodes[i]->Text->Contains(text)) {
                nodesToRemove->Add(node->Nodes[i]);
            }
        }

        for (int i = 0; i < nodesToRemove->Count; i++) {
            node->Nodes->Remove(nodesToRemove[i]);
        }

        delete nodesToRemove;
    }

    void TreeViewFilter::filterText_TextChanged(System::Object^ sender, System::EventArgs^ e) {
        if (mOriginalRootNodes != nullptr && mOriginalRootNodes->Length > 0) {
            this->filterTimer->Stop();
            this->filterTimer->Start();
        }
    }

}
