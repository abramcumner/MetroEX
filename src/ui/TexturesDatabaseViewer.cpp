#include <msclr/marshal_cppstd.h>
#include "TexturesDatabaseViewer.h"
#include "ui\MainForm.h"
#include "ui\NodeSorter.h"

using namespace msclr::interop;

static const int kImageIdxFile = 6;
static const int kImageIdxFolderClosed = 0;
static const int kImageIdxFolderOpen = 1;

namespace MetroEX {

    void TexturesDatabaseViewer::FillWithData() {
        if (this->mDataProvider == nullptr) {
            return;
        }

        System::Windows::Forms::Cursor::Current = System::Windows::Forms::Cursors::WaitCursor;

        this->dataTree->BeginUpdate();
        this->dataTree->Nodes->Clear();

        this->mOriginalRootNode = this->dataTree->Nodes->Add("content");
        this->mOriginalRootNode->ImageIndex = kImageIdxFolderClosed;
        this->mOriginalRootNode->SelectedImageIndex = kImageIdxFolderClosed;

        MyArray<MetroTextureInfo>* pool = this->mDataProvider->GetPool();
        for (size_t index = 0; index < pool->size(); index++) {
            MetroTextureInfo* texInfo = &pool->at(index);
            String^ name = marshal_as<String^>(texInfo->name);
            array<String^>^ parts = name->Split('\\');

            TreeNode^ parentNode = this->mOriginalRootNode;
            for (int i = 0; i < parts->Length - 1; i++) {
                TreeNode^ foundNode = FindNode(parentNode, parts[i]);

                if (foundNode == nullptr) {
                    parentNode = parentNode->Nodes->Add(parts[i]);
                    parentNode->ImageIndex = kImageIdxFolderClosed;
                    parentNode->SelectedImageIndex = kImageIdxFolderClosed;
                } else {
                    parentNode = foundNode;
                }
            }

            TreeNode^ node = parentNode->Nodes->Add(parts[parts->Length - 1]);
            node->Tag = index;
            node->ImageIndex = kImageIdxFile;
            node->SelectedImageIndex = kImageIdxFile;
        }

        NodeSorter^ sorter = gcnew NodeSorter();
        this->SortNodesRecursively(this->mOriginalRootNode, sorter);

        this->mOriginalRootNode->Expand();

        this->dataTree->EndUpdate();

        System::Windows::Forms::Cursor::Current = System::Windows::Forms::Cursors::Arrow;
    }

    TreeNode^ TexturesDatabaseViewer::FindNode(TreeNode^ parent, String^ text) {
        String^ term = text->ToUpper();

        for (int i = 0; i < parent->Nodes->Count; i++) {
            if (parent->Nodes[i]->Text->ToUpper() == term) {
                return parent->Nodes[i];
            }
        }

        return nullptr;
    }

    void TexturesDatabaseViewer::SortNodesRecursively(TreeNode^ parent, NodeSorter^ sorter)
    {
        array<TreeNode^>^ nodes = gcnew array<TreeNode^>(parent->Nodes->Count);
        parent->Nodes->CopyTo(nodes, 0);
        System::Array::Sort(nodes, sorter);
        parent->Nodes->Clear();
        parent->Nodes->AddRange(nodes);

        delete nodes;

        for (int i = 0; i < parent->Nodes->Count; i++) {
            if (parent->Nodes[i]->Nodes->Count > 0) {
                SortNodesRecursively(parent->Nodes[i], sorter);
            }
        }
    }

    void TexturesDatabaseViewer::filterTimer_Tick(System::Object^ sender, System::EventArgs^ e) {
        this->filterTimer->Stop();

        System::Windows::Forms::Cursor::Current = System::Windows::Forms::Cursors::WaitCursor;

        this->dataTree->BeginUpdate();
        this->dataTree->Nodes->Clear();

        if (String::IsNullOrWhiteSpace(this->filterText->Text)) {
            this->dataTree->Nodes->Add(this->mOriginalRootNode);
        } else {
            TreeNode^ root = safe_cast<TreeNode^>(this->mOriginalRootNode->Clone());
            this->FilterTreeView(root, this->filterText->Text);
            this->dataTree->Nodes->Add(root);
        }

        this->dataTree->Nodes[0]->ExpandAll();
        this->dataTree->EndUpdate();

        System::Windows::Forms::Cursor::Current = System::Windows::Forms::Cursors::Arrow;
    }

    void TexturesDatabaseViewer::FilterTreeView(TreeNode^ node, String^ text) {
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

    void TexturesDatabaseViewer::filterText_TextChanged(System::Object^ sender, System::EventArgs^ e) {
        if (this->mDataProvider != nullptr) {
            this->filterTimer->Stop();
            this->filterTimer->Start();
        }
    }

    void TexturesDatabaseViewer::dataTree_NodeMouseClick(System::Object^ sender, System::Windows::Forms::TreeNodeMouseClickEventArgs^ e) {
        if (e->Button != System::Windows::Forms::MouseButtons::Left || e->Node == nullptr || e->Node->Nodes->Count > 0) return;

        if (this->mPropertiesViewer == nullptr) {
            this->mPropertiesViewer = gcnew TexturePropertiesViewer();
        }

        size_t index = safe_cast<size_t>(e->Node->Tag);
        MetroTextureInfo* texInfo = &this->mDataProvider->GetPool()->at(index);
        String^ realPath = this->GetRealPath(index);

        this->mPropertiesViewer->SetTextureInfo(texInfo);
        this->mPropertiesViewer->SetRealPath(realPath);
        this->propertyGrid->SelectedObject = this->mPropertiesViewer;
    }

    void TexturesDatabaseViewer::dataTree_NodeMouseDoubleClick(System::Object^ sender, System::Windows::Forms::TreeNodeMouseClickEventArgs^ e) {
        if (e->Button != System::Windows::Forms::MouseButtons::Left || e->Node == nullptr || e->Node->Nodes->Count > 0) return;

        size_t index = safe_cast<size_t>(e->Node->Tag);
        String^ path = this->GetRealPath(index);

        this->mMainForm->ResetTreeView();
        if (!this->mMainForm->FindAndSelect("content\\textures\\" + path, this->mFileExtensions)) {
            this->mMainForm->ShowErrorMessage("Couldn't find texture!");
        }
    }

    String^ TexturesDatabaseViewer::GetRealPath(size_t index) {
        MetroTextureInfo* texInfo = &this->mDataProvider->GetPool()->at(index);
        const HashString& alias = this->mDataProvider->GetAlias(HashString(texInfo->name));

        if (alias.hash == 0) {
            return marshal_as<String^>(texInfo->name);
        } else {
            const MetroTextureInfo* realTexInfo = this->mDataProvider->GetInfoByName(alias);

            return marshal_as<String^>(realTexInfo->name);
        }
    }

    void TexturesDatabaseViewer::dataTree_AfterCollapse(System::Object^ sender, System::Windows::Forms::TreeViewEventArgs^ e) {
        e->Node->ImageIndex = kImageIdxFolderClosed;
        e->Node->SelectedImageIndex = kImageIdxFolderClosed;
    }

    void TexturesDatabaseViewer::dataTree_AfterExpand(System::Object^ sender, System::Windows::Forms::TreeViewEventArgs^ e) {
        e->Node->ImageIndex = kImageIdxFolderOpen;
        e->Node->SelectedImageIndex = kImageIdxFolderOpen;
    }

}
